#include <QColorDialog>
#include <QFileDialog>
#include <QMessageBox>

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      settings(QSettings("21EduSoft", "3DViewer")),
      timer(QTimer(this)),
      gifImage(nullptr),
      ui(new Ui::MainWindow) {
  ui->setupUi(this);
  connect(&timer, SIGNAL(timeout()), this, SLOT(GifCreation()));
  ui->actionCentral->setCheckable(true);
  ui->actionParallel->setCheckable(true);
  blockActions();
  connect(ui->screenWidget, &Widget3D::xRotValueChanged, ui->XRotateSpinBox,
          &QDoubleSpinBox::setValue);
  connect(ui->screenWidget, &Widget3D::yRotValueChanged, ui->YRotateSpinBox,
          &QDoubleSpinBox::setValue);
  connect(ui->screenWidget, &Widget3D::scaleWheelChanged, ui->scaleSpinBox,
          &QDoubleSpinBox::setValue);
  setConfiguration();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_actionBackground_color_triggered() {
  QColor color = QColorDialog::getColor(ui->screenWidget->getBackgroundColor(),
                                        this, "Choose colour",
                                        QColorDialog::DontUseNativeDialog);
  if (color.isValid()) ui->screenWidget->setBackgroundColor(color);
}

void MainWindow::on_openFileButton_clicked() {
  QString file = QFileDialog::getOpenFileName(
      this, "Choose object file", QDir::currentPath(), "*.obj", nullptr,
      QFileDialog::DontUseNativeDialog);
  if (!file.isNull()) {
    on_actionClose_file_triggered();
    CarcassTdef *crcPtr = fileProcessing(file);
    ui->screenWidget->setCarcass(crcPtr);
    ui->screenWidget->restoreState();
    if (!currentFile.isNull() && !currentFile.isEmpty()) unblockActions();
  }
}

void MainWindow::on_XRotateSlider_valueChanged(int value) {
  ui->XRotateSpinBox->setValue(value);
}

void MainWindow::on_XRotateSpinBox_valueChanged(double arg1) {
  ui->screenWidget->rotate(arg1, 0);
  ui->XRotateSlider->setValue(arg1);
}

void MainWindow::on_YRotateSlider_valueChanged(int value) {
  ui->YRotateSpinBox->setValue(value);
}

void MainWindow::on_YRotateSpinBox_valueChanged(double arg1) {
  ui->screenWidget->rotate(arg1, 1);
  ui->YRotateSlider->setValue(arg1);
}

void MainWindow::on_ZRotateSlider_valueChanged(int value) {
  ui->ZRotateSpinBox->setValue(value);
}

void MainWindow::on_ZRotateSpinBox_valueChanged(double arg1) {
  ui->screenWidget->rotate(arg1, 2);
  ui->ZRotateSlider->setValue(arg1);
}

void MainWindow::on_scaleSlider_valueChanged(int value) {
  ui->scaleSpinBox->setValue(value <= 10 ? value / 10.0 : value - 9);
}

void MainWindow::on_scaleSpinBox_valueChanged(double arg1) {
  ui->scaleSlider->blockSignals(true);
  ui->screenWidget->scale(arg1);
  ui->scaleSlider->setValue(round(arg1 <= 1 ? arg1 * 10 : 9 + arg1));
  ui->scaleSlider->blockSignals(false);
}

void MainWindow::on_XTranslateSpinBox_valueChanged(double arg1) {
  ui->screenWidget->translate(arg1, 0);
  ui->XTranslateSlider->setValue(arg1);
}

void MainWindow::on_XTranslateSlider_valueChanged(int value) {
  ui->XTranslateSpinBox->setValue(value);
}

void MainWindow::on_YTranslateSlider_valueChanged(int value) {
  ui->YTranslateSpinBox->setValue(value);
}

void MainWindow::on_YTranslateSpinBox_valueChanged(double arg1) {
  ui->screenWidget->translate(arg1, 1);
  ui->YTranslateSlider->setValue(arg1);
}

void MainWindow::on_ZTranslateSpinBox_valueChanged(double arg1) {
  ui->screenWidget->translate(arg1, 2);
  ui->ZTranslateSlider->setValue(arg1);
}

void MainWindow::on_ZTranslateSlider_valueChanged(int value) {
  ui->ZTranslateSpinBox->setValue(value);
}

void MainWindow::GifCreation() {
  if (gifImage->frameCount() < 50)
    gifImage->addFrame(ui->screenWidget->grabFramebuffer().scaled(
        640, 480, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
  else {
    timer.stop();
    QString ExpectedName = currentFile;
    ExpectedName.resize(ExpectedName.size() - 4);
    QString file = QFileDialog::getSaveFileName(
        this, "Save as...", ExpectedName, "GIF (*.gif)", nullptr,
        QFileDialog::DontUseNativeDialog);
    if (!file.isNull()) gifImage->save(file + QString(".gif"));
    delete gifImage;
    gifImage = nullptr;
    ui->saveGifButton->setEnabled(true);
  }
}

void MainWindow::MainWindow::closeEvent(QCloseEvent *event) {
  writeSettings();
  QWidget::closeEvent(event);
}

void MainWindow::writeSettings() {
  settings.setValue("File", currentFile);
  ui->screenWidget->writeState(settings);
}

void MainWindow::setConfiguration() {
  ui->fileInfoLabel->setText("");
  QString filename = settings.value("File", "").toString();
  CarcassTdef *crcPtr = fileProcessing(filename);
  if (crcPtr) unblockActions();
  ui->screenWidget->readState(settings, crcPtr);
  ui->XRotateSlider->setValue(
      settings.value("XRotateValue", 0).value<double>());
  ui->YRotateSlider->setValue(
      settings.value("YRotateValue", 0).value<double>());
  ui->ZRotateSlider->setValue(
      settings.value("ZRotateValue", 0).value<double>());
  ui->XTranslateSlider->setValue(
      settings.value("XTranslateValue", 0).value<double>());
  ui->YTranslateSlider->setValue(
      settings.value("YTranslateValue", 0).value<double>());
  ui->ZTranslateSlider->setValue(
      settings.value("ZTranslateValue", 0).value<double>());
  ui->scaleSpinBox->setValue(settings.value("ScaleValue", 1).value<double>());
  if (settings.value("Projection", 0).value<bool>() == true)
    ui->actionCentral->setChecked(true);
  else
    ui->actionParallel->setChecked(true);
}

void MainWindow::showErrorMessage(ParserErrorsEnum error) {
  if (error != kSuccess) {
    switch (error) {
      case kFileOpenError:
        QMessageBox::critical(
            this, "3DViewer",
            "An error occurred while opening or reading the file."
            " Check file access rights ( ls -l ) and path.  Try again.");
        break;
      case kFileContentError:
        QMessageBox::critical(
            this, "3DViewer",
            "An error occurred while processing the file: invalid file."
            "Check the contents of the file and try again.");
        break;
      case kMallocError:
        QMessageBox::critical(
            this, "3DViewer",
            "An error occurred while processing the file: malloc error.");
        break;
      default:
        break;
    }
  }
}

CarcassTdef *MainWindow::fileProcessing(QString filename) {
  CarcassTdef *newCrc = nullptr;

  if (!filename.isNull() && !filename.isEmpty()) {
    ui->fileInfoLabel->setText("");
    currentFile = nullptr;
    ParserErrorsEnum error =
        parseCarcass(filename.toLocal8Bit().data(), &newCrc);
    showErrorMessage(error);
    if (error == kSuccess) {
      currentFile = filename;
      ui->fileInfoLabel->setText(
          "Filename: " + filename +
          "\tVertices: " + QString::number(newCrc->vertexes.amount) +
          "\tFacets: " + QString::number(newCrc->facets.amount));
    }
  }
  return newCrc;
}

void MainWindow::blockActions() {
  ui->screenWidget->setEnabled(false);
  ui->screenWidget->update();
  ui->XRotateSlider->setEnabled(false);
  ui->YRotateSlider->setEnabled(false);
  ui->ZRotateSlider->setEnabled(false);
  ui->XRotateSpinBox->setEnabled(false);
  ui->YRotateSpinBox->setEnabled(false);
  ui->ZRotateSpinBox->setEnabled(false);
  ui->XTranslateSlider->setEnabled(false);
  ui->YTranslateSlider->setEnabled(false);
  ui->ZTranslateSlider->setEnabled(false);
  ui->XTranslateSpinBox->setEnabled(false);
  ui->YTranslateSpinBox->setEnabled(false);
  ui->ZTranslateSpinBox->setEnabled(false);
  ui->scaleSlider->setEnabled(false);
  ui->scaleSpinBox->setEnabled(false);
  ui->saveImgButton->setEnabled(false);
  ui->saveGifButton->setEnabled(false);
  ui->actionSave_as->setEnabled(false);
  ui->actionSave_as_GIF->setEnabled(false);
  ui->actionClose_file->setEnabled(false);
  this->update();
}

void MainWindow::unblockActions() {
  ui->screenWidget->setEnabled(true);
  ui->screenWidget->update();
  ui->XRotateSlider->setEnabled(true);
  ui->YRotateSlider->setEnabled(true);
  ui->ZRotateSlider->setEnabled(true);
  ui->XRotateSpinBox->setEnabled(true);
  ui->YRotateSpinBox->setEnabled(true);
  ui->ZRotateSpinBox->setEnabled(true);
  ui->XTranslateSlider->setEnabled(true);
  ui->YTranslateSlider->setEnabled(true);
  ui->ZTranslateSlider->setEnabled(true);
  ui->XTranslateSpinBox->setEnabled(true);
  ui->YTranslateSpinBox->setEnabled(true);
  ui->ZTranslateSpinBox->setEnabled(true);
  ui->scaleSlider->setEnabled(true);
  ui->scaleSpinBox->setEnabled(true);
  ui->saveImgButton->setEnabled(true);
  ui->saveGifButton->setEnabled(true);
  ui->actionSave_as->setEnabled(true);
  ui->actionSave_as_GIF->setEnabled(true);
  ui->actionClose_file->setEnabled(true);
  this->update();
}

void MainWindow::on_actionVertices_triggered() {
  const Widget3D::model *tmp = ui->screenWidget->getModel();
  ui->screenWidget->saveVerticesState();
  Vertex vtx(this, tmp->vertexType, tmp->vertexSize, tmp->vertexColor);
  connect(&vtx, &Vertex::vertexColorChanged, ui->screenWidget,
          &Widget3D::setVertexColor);
  connect(&vtx, &Vertex::vertexSizeChanged, ui->screenWidget,
          &Widget3D::setVertexSize);
  connect(&vtx, &Vertex::vertexTypeChanged, ui->screenWidget,
          &Widget3D::setVertexType);
  connect(&vtx, &Vertex::vertexCancel, ui->screenWidget,
          &Widget3D::restoreVerticesState);
  vtx.exec();
}

void MainWindow::on_actionFacets_triggered() {
  const Widget3D::model *tmp = ui->screenWidget->getModel();
  ui->screenWidget->saveFacetsState();
  Facet fct(this, tmp->facetType, tmp->facetThickness, tmp->facetColor);
  connect(&fct, &Facet::facetColorChanged, ui->screenWidget,
          &Widget3D::setFacetColor);
  connect(&fct, &Facet::facetThicknessChanged, ui->screenWidget,
          &Widget3D::setFacetThickness);
  connect(&fct, &Facet::facetTypeChanged, ui->screenWidget,
          &Widget3D::setFacetType);
  connect(&fct, &Facet::facetCancel, ui->screenWidget,
          &Widget3D::restoreFacetsState);
  fct.exec();
}

void MainWindow::on_actionUpload_triggered() { on_openFileButton_clicked(); }

void MainWindow::on_actionTo_default_settings_triggered() {
  on_actionParallel_triggered();
  ui->screenWidget->backToOrigin();
  ui->screenWidget->setBackgroundColor(Qt::black);
  ui->screenWidget->setVertexColor(Qt::blue);
  ui->screenWidget->setFacetColor(Qt::red);
  ui->screenWidget->setVertexSize(5);
  ui->screenWidget->setFacetThickness(1);
  ui->screenWidget->setVertexType(0);
  ui->screenWidget->setFacetType(0);
  ui->XRotateSlider->setValue(0);
  ui->YRotateSlider->setValue(0);
  ui->ZRotateSlider->setValue(0);
  ui->XTranslateSlider->setValue(0);
  ui->YTranslateSlider->setValue(0);
  ui->ZTranslateSlider->setValue(0);
  ui->scaleSpinBox->setValue(1);
  update();
}

void MainWindow::on_actionClose_file_triggered() {
  ui->fileInfoLabel->setText("");
  ui->screenWidget->clearScreen();
  currentFile = nullptr;
  blockActions();
}

void MainWindow::on_saveImgButton_clicked() {
  QString ExpectedName = currentFile;
  QString sfx;
  ExpectedName.resize(ExpectedName.size() - 4);
  QString file = QFileDialog::getSaveFileName(
      this, "Save as...", ExpectedName, "JPEG (*.jpg *.jpeg);; BMP (*.bmp)",
      &sfx,
      QFileDialog::DontUseNativeDialog | QFileDialog::DontConfirmOverwrite);
  if (!file.isNull()) {
    if (sfx == "JPEG (*.jpg *.jpeg)")
      ui->screenWidget->grab().save(file + ".jpeg");
    else
      ui->screenWidget->grab().save(file + ".bmp");
  }
}

void MainWindow::on_saveGifButton_clicked() {
  ui->saveGifButton->setEnabled(false);
  gifImage = new QGifImage(QSize(640, 480));
  gifImage->setDefaultDelay(100);
  timer.start(100);
}

void MainWindow::on_actionParallel_triggered() {
  ui->actionCentral->setChecked(false);
  ui->screenWidget->changeProjection(Widget3D::projectionType::ORTHO);
  ui->actionParallel->setChecked(true);
}

void MainWindow::on_actionCentral_triggered() {
  ui->actionParallel->setChecked(false);
  ui->screenWidget->changeProjection(Widget3D::projectionType::CENTRAL);
  ui->actionCentral->setChecked(true);
}

void MainWindow::on_actionSave_as_triggered() { on_saveImgButton_clicked(); }

void MainWindow::on_actionSave_as_GIF_triggered() {
  on_saveGifButton_clicked();
}
