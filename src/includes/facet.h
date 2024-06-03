#ifndef FACET_H
#define FACET_H

#include <QDialog>

#include "ui_facet.h"

class Facet : public QDialog {
  Q_OBJECT
 public:
  Facet() = default;
  Facet(QWidget *parent, int FType, double FThickness, QColor FColor);
  ~Facet();

  void reject() override;

 signals:
  void facetColorChanged(QColor vertexColor);
  void facetThicknessChanged(double vertexThickness);
  void facetTypeChanged(int vertexType);
  void facetCancel();

 private slots:
  void on_colorButton_clicked();
  void on_thicknessBox_valueChanged(double arg1);
  void on_typeBox_currentIndexChanged(int index);
  void on_okButton_clicked();
  void on_cancelButton_clicked();

 private:
  void changeButtonColor(QColor newColor);

  Ui_Facet *ui;
  bool mustBeRestored;
};

#endif
