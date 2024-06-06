#include <QCloseEvent>
#include <QColorDialog>
#include <QOpenGLWidget>

#include "vertex.h"

Vertex::Vertex(QWidget *parent, int VType, double VSize, QColor VColor)
    : QDialog(parent), ui(new Ui_Vertex), mustBeRestored(true) {
  ui->setupUi(this);
  QStringList sl;
  sl << "None"
     << "Circle"
     << "Square";
  ui->pointTypeBox->addItems(sl);
  changeButtonColor(VColor);
  ui->pointTypeBox->setCurrentIndex(VType);
  ui->pointSizeBox->setValue(VSize);
  update();
}

Vertex::~Vertex() { delete ui; }

void Vertex::reject() {
  if (mustBeRestored) emit vertexCancel();
  QDialog::reject();
}

void Vertex::on_colorButton_clicked() {
  QColor newColor =
      QColorDialog::getColor(ui->colorButton->palette().button().color());
  if (newColor.isValid()) {
    changeButtonColor(newColor);
    emit vertexColorChanged(newColor);
  };
}

void Vertex::on_pointSizeBox_valueChanged(double arg1) {
  emit vertexSizeChanged(arg1);
}

void Vertex::changeButtonColor(QColor newColor) {
  QString colorName = newColor.name();
  ui->colorButton->setStyleSheet("background-color: " + colorName + ";");
  ui->colorButton->update();
}

void Vertex::on_pointTypeBox_currentIndexChanged(int index) {
  emit vertexTypeChanged(index);
}

void Vertex::on_okButton_clicked() {
  mustBeRestored = false;
  close();
}

void Vertex::on_cancelButton_clicked() { close(); }
