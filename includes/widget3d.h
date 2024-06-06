#ifndef WIDGET3D_H
#define WIDGET3D_H

#include <QMouseEvent>
#include <QOpenGLWidget>
#include <QSettings>
#include <QString>
#include <QWheelEvent>
#include <QtGui>

extern "C" {
#include "carcass.h"
#include "parser.h"
}

#define VIEWER_DIST 2
#define TRANSLATE_DIVIDER 10
#define DELTA_WHEEL \
  0.00083333333  // 1/1200 = 0.00083333333 one turn of the mouse wheel equal to
                 // 120 (or -120)
#define MAX_SCALE 20
#define MIN_SCALE 0.01

class Widget3D : public QOpenGLWidget {
  Q_OBJECT
 public:
  Widget3D(QWidget *parent);
  ~Widget3D();
  enum class projectionType {
    ORTHO,
    CENTRAL,
  };

  struct model {
    CarcassTdef *crc;
    QColor facetColor;
    QColor vertexColor;
    double facetThickness;
    double vertexSize;
    int facetType;
    int vertexType;
  };

  QColor getBackgroundColor() const;
  void setBackgroundColor(QColor color);

  // vertices changes

  QColor getVertexColor() const;
  double getVertexSize() const;
  int getVertexType() const;

  void setVertexColor(QColor color);
  void setVertexSize(double value);
  void setVertexType(int value);

  // facets changes

  QColor getFacetColor() const;
  double getFacetThickness() const;
  int getFacetType() const;

  void setFacetColor(QColor color);
  void setFacetThickness(double value);
  void setFacetType(int value);

  Widget3D::model const *getModel() const;
  void writeState(QSettings &settings) const;
  void readState(const QSettings &settings, CarcassTdef *newCrc);

  void changeProjection(projectionType type = projectionType::ORTHO);

  // carcass changes

  void scale(double value);
  void translate(double value, unsigned start);
  void rotate(double angle, unsigned pivot);

  void restoreVerticesState();
  void saveVerticesState();
  void restoreFacetsState();
  void saveFacetsState();

  void setCarcass(CarcassTdef *crc);
  void clearScreen();
  void restoreState();
  void backToOrigin();

 signals:
  void xRotValueChanged(double value);
  void yRotValueChanged(double value);
  void scaleWheelChanged(double value);

 private:
  void initializeGL();
  void paintGL();
  void resizeGL(int w, int h);
  void drawVertices();
  void drawFacets();
  void rotate_priv(double angle, unsigned pivot);
  void centerShift(bool toCenter = true);
  void mousePressEvent(QMouseEvent *pe);
  void mouseMoveEvent(QMouseEvent *pe);
  void wheelEvent(QWheelEvent *pe);
  void restoreRotation();

 private:
  QColor backgroundColor;
  projectionType projection{};
  model currentModel{};
  model prevModel{};
  struct {
    float scale_value;
    double rotate_values[3];
    double translate_values[3];
    double centreCoords[3]{};
    double pivotZ[3];
    double pivotY[3];
  } prevValues{};
  QPoint ptrMousePosition{};
};

#endif
