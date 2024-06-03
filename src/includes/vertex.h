#ifndef VERTEX_H
#define VERTEX_H

#include <QDialog>

#include "ui_vertex.h"

class Vertex : public QDialog {
  Q_OBJECT

 public:
  Vertex() = default;
  Vertex(QWidget *parent, int VType, double VSize, QColor color);
  ~Vertex();
  void reject() override;

 signals:
  void vertexColorChanged(QColor vertexColor);
  void vertexSizeChanged(double vertexThickness);
  void vertexTypeChanged(int vertexType);
  void vertexCancel();

 private slots:
  void on_colorButton_clicked();
  void on_pointSizeBox_valueChanged(double arg1);
  void on_pointTypeBox_currentIndexChanged(int index);
  void on_okButton_clicked();
  void on_cancelButton_clicked();

 private:
  void changeButtonColor(QColor newColor);

  Ui_Vertex *ui;
  bool mustBeRestored;
};

#endif
