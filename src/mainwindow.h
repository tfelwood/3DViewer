#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QSettings>
#include <QTimer>

#include "qgifimage.h"
#include "vertex.h"
#include "facet.h"
#include "widget3d.h"
#include "ui_mainwindow.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionBackground_color_triggered();
    void on_openFileButton_clicked();
    void on_XRotateSlider_valueChanged(int value);
    void on_XRotateSpinBox_valueChanged(double arg1);
    void on_YRotateSlider_valueChanged(int value);
    void on_YRotateSpinBox_valueChanged(double arg1);
    void on_ZRotateSlider_valueChanged(int value);
    void on_ZRotateSpinBox_valueChanged(double arg1);
    void on_scaleSlider_valueChanged(int value);
    void on_scaleSpinBox_valueChanged(double arg1);
    void on_XTranslateSpinBox_valueChanged(double arg1);
    void on_XTranslateSlider_valueChanged(int value);
    void on_YTranslateSlider_valueChanged(int value);
    void on_YTranslateSpinBox_valueChanged(double arg1);
    void on_ZTranslateSpinBox_valueChanged(double arg1);
    void on_ZTranslateSlider_valueChanged(int value);
    void GifCreation();



    void on_actionVertices_triggered();

    void on_actionFacets_triggered();

    void on_actionUpload_triggered();

    void on_actionTo_default_settings_triggered();

    void on_actionClose_file_triggered();

    void on_saveImgButton_clicked();

    void on_saveGifButton_clicked();

    void on_actionParallel_triggered();

    void on_actionCentral_triggered();

    void on_actionSave_as_triggered();

    void on_actionSave_as_GIF_triggered();

private:

    void closeEvent (QCloseEvent *event);
    void writeSettings();
    void setConfiguration();


    Ui::MainWindow   *ui;

    QSettings       settings;
    QString         currentFile;
    QTimer          timer;
    QGifImage       *gifImage;
};
#endif // MAINWINDOW_H
