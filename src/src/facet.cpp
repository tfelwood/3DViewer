#include <QCloseEvent>
#include <QColorDialog>

#include "facet.h"

Facet::Facet(QWidget *parent, int FType, double FThickness, QColor FColor)
    : QDialog(parent),
      ui(new Ui_Facet),
      mustBeRestored(true)

{
  ui->setupUi(this);
  QStringList sl;
  sl << "Solid"
     << "Dashed";
  ui->typeBox->addItems(sl);
  changeButtonColor(FColor);
  ui->typeBox->setCurrentIndex(FType);
  ui->thicknessBox->setValue(FThickness);
  update();
}

Facet::~Facet() { delete ui; }

void Facet::reject() {
  if (mustBeRestored) emit facetCancel();
  QDialog::reject();
}

void Facet::on_colorButton_clicked() {
  QColor newColor =
      QColorDialog::getColor(ui->colorButton->palette().button().color());
  if (newColor.isValid()) {
    changeButtonColor(newColor);
    emit facetColorChanged(newColor);
  };
}

void Facet::on_thicknessBox_valueChanged(double arg1) {
  emit facetThicknessChanged(arg1);
}

void Facet::on_typeBox_currentIndexChanged(int index) {
  emit facetTypeChanged(index);
}

void Facet::on_okButton_clicked() {
  mustBeRestored = false;
  close();
}

void Facet::on_cancelButton_clicked() { close(); }

void Facet::changeButtonColor(QColor newColor) {
  QString colorName = newColor.name();
  ui->colorButton->setStyleSheet("background-color: " + colorName + ";");
  ui->colorButton->update();
}
