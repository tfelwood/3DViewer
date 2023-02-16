#include <QColorDialog>
#include <QCloseEvent>

#include <iostream>

#include <QOpenGLWidget>
#include "vertex.h"

Vertex::Vertex(QWidget *parent, int VType, double VSize, QColor VColor):QDialog(parent),
    ui(new Ui_Vertex), mustBeRestored(true)
//    , color(VColor), thickness(VThickness), type(VType)
{
    ui->setupUi(this);
   // std::cout << GL_SMOOTH_POINT_SIZE_GRANULARITY << std::endl;
   // ui->pointSizeBox->setMaximum(GL_SMOOTH_POINT_SIZE_RANGE);
    QStringList sl;
    sl << "None" << "Circle" << "Square";
    ui->pointTypeBox->addItems(sl);
    changeButtonColor(VColor);
    ui->pointTypeBox->setCurrentIndex(VType);
    ui->pointSizeBox->setValue(VSize);
    update();
}

Vertex::~Vertex()
{
    delete ui;
}

void Vertex::reject()
{
    if(mustBeRestored)
        emit vertexCancel();
    QDialog::reject();
}

void Vertex::on_colorButton_clicked()
{
    QColor newColor = QColorDialog::getColor(ui->colorButton->palette().button().color());
  //  QColor newColor = QColorDialog::getColor(color, this);
    if (newColor.isValid())
    {
      //  color = newColor;
//          QTextStream out(stdout);
//            out << newColor.name() << "\n";
        changeButtonColor(newColor);
        emit vertexColorChanged(newColor);
    };
}


void Vertex::on_pointSizeBox_valueChanged(double arg1)
{
  //  thickness = arg1;
    emit vertexSizeChanged(arg1);
}

void Vertex::changeButtonColor(QColor newColor)
{
    QString colorName = newColor.name();
//    QPalette palette = ui->colorButton->palette();
//    palette.setColor(QPalette::Button, newColor);
//    palette.setColor(QPalette::Light, newColor.lighter());
//    palette.setColor(QPalette::Dark, newColor.darker());
//    ui->colorButton->setPalette(palette);


      ui->colorButton->setStyleSheet("background-color: " + colorName + ";");
      ui->colorButton->update();
}



void Vertex::on_pointTypeBox_currentIndexChanged(int index)
{
    //type = index;
    emit vertexTypeChanged(index);
}

void Vertex::on_okButton_clicked()
{
    mustBeRestored = false;
    close();
}


void Vertex::on_cancelButton_clicked()
{
 //
    close();
}

