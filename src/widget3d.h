#ifndef WIDGET3D_H
#define WIDGET3D_H

#include <QString>
#include <QSettings>
#include <QOpenGLWidget>
#include <QtGui>
#include <QMouseEvent>
#include <QWheelEvent>
//#include "ui_widget3d.h"

extern "C" {
    #include "parser.h"
    #include "carcass.h"
}

#define VIEWER_DIST 2
#define TRANSLATE_DIVIDER 10
#define WHEEL_DELTA 0.00083333333 // 1/1200 = 0.00083333333 one turn of the mouse wheel equal to 120 (or -120)
#define MAX_SCALE 20
#define MIN_SCALE 0.01

//QT_BEGIN_NAMESPACE
//namespace Ui { class Widget3D; }
//QT_END_NAMESPACE

class Widget3D : public QOpenGLWidget
{
    Q_OBJECT
public:

    Widget3D( QWidget *parent);
 //   ~Widget3D();
    enum class projectionType{
        ORTHO,
        CENTRAL,
    };

    struct model{
        carcass_st  *crc;
        QColor      facetColor;
        QColor      vertexColor;
        double      facetThickness;
        double      vertexSize;
        int         facetType; //what's type?
        int         vertexType;  //what's type?
    };


    QColor          getBackgroundColor() const;
    void            setBackgroundColor(QColor color);

    //vertices changes

    QColor          getVertexColor() const;
    double          getVertexSize() const;
    int             getVertexType() const;

    void            setVertexColor(QColor color);
    void            setVertexSize(double value);
    void            setVertexType(int value);

    //facets changes

    QColor          getFacetColor() const;
    double          getFacetThickness() const;
    int             getFacetType() const;

    void            setFacetColor(QColor color);
    void            setFacetThickness(double value);
    void            setFacetType(int value);

    Widget3D::model const     *getModel() const; //need to refactor
    void            writeState(QSettings& settings) const;
    void            readState(const QSettings &settings, carcass_st *newCrc);

    void            changeProjection(projectionType type = projectionType::ORTHO, double scale = 1);

    // carcass changes

	void            scale(double value);
    void            translate(double value, unsigned start, double cf = 1);
    void            rotate(double angle, unsigned pivot);

    void            restoreVerticesState();
    void            saveVerticesState();
    void            restoreFacetsState();
    void            saveFacetsState();

    void            setCarcass(carcass_st *crc);
    void            clearScreen();
    void            restoreState();
    void            backToOrigin();
    void printCentre();

signals:
   void xRotValueChanged(double value);
   void yRotValueChanged(double value);
   void scaleWheelChanged(double value);

private:
    void initializeGL();
    void paintGL();
    void resizeGL( int w, int h );
    void drawVertices();
    void drawFacets();
    void rotate_priv(double angle, unsigned pivot);
    void centerShift(bool toCenter = true);
    void mousePressEvent(QMouseEvent* pe);
    void mouseMoveEvent(QMouseEvent* pe);
    void wheelEvent(QWheelEvent* pe);

private:
    QColor backgroundColor;
    projectionType  projection{};
    model currentModel{};
    model prevModel{};
    struct {
        float scale_value;
        double rotate_values[3];
        double translate_values[3];
        double centreCoords[3]{};
    } prevValues{};
    QPoint          ptrMousePosition{};
  //  Ui::Widget3D   *ui;


};

#endif // WIDGET3D_H

