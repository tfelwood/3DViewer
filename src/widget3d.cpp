#define GL_SILENCE_DEPRECATION

#include <QOpenGLFunctions>
#include <QString>
#include <cmath>

#include "widget3d.h"

Widget3D::Widget3D(QWidget *parent) : QOpenGLWidget(parent), prevValues() {
  prevValues.scale_value = 1;
}

void Widget3D::setBackgroundColor(QColor color) {
  backgroundColor = color;
  update();
}

void Widget3D::setVertexColor(QColor color) {
  currentModel.vertexColor = color;
  update();
}

void Widget3D::setVertexSize(double value) {
  currentModel.vertexSize = value;
  update();
}

void Widget3D::setVertexType(int value) {
  currentModel.vertexType = value;
  update();
}

QColor Widget3D::getFacetColor() const { return currentModel.facetColor; }

double Widget3D::getFacetThickness() const {
  return currentModel.facetThickness;
}

int Widget3D::getFacetType() const { return currentModel.facetType; }

void Widget3D::setFacetColor(QColor color) {
  currentModel.facetColor = color;
  update();
}

void Widget3D::setFacetThickness(double value) {
  currentModel.facetThickness = value;
  update();
}

void Widget3D::setFacetType(int value) {
  currentModel.facetType = value;
  update();
}

const Widget3D::model *Widget3D::getModel() const { return &currentModel; }

Widget3D::~Widget3D() { freeCarcass(&currentModel.crc); }

void Widget3D::writeState(QSettings &settings) const {
  settings.setValue("BackgroundColor", backgroundColor);
  settings.setValue("VertexColor", currentModel.vertexColor);
  settings.setValue("FacetColor", currentModel.facetColor);
  settings.setValue("VertexSize", currentModel.vertexSize);
  settings.setValue("FacetThickness", currentModel.facetThickness);
  settings.setValue("VertexType", currentModel.vertexType);
  settings.setValue("FacetType", currentModel.facetType);
  settings.setValue("Projection", static_cast<bool>(projection));
  settings.setValue("ScaleValue", prevValues.scale_value);
  settings.setValue("XRotateValue", prevValues.rotate_values[0]);
  settings.setValue("YRotateValue", prevValues.rotate_values[1]);
  settings.setValue("ZRotateValue", prevValues.rotate_values[2]);
  settings.setValue("XTranslateValue", prevValues.translate_values[0]);
  settings.setValue("YTranslateValue", prevValues.translate_values[1]);
  settings.setValue("ZTranslateValue", prevValues.translate_values[2]);
  settings.setValue("PivotZX", prevValues.pivotZ[0]);
  settings.setValue("PivotZY", prevValues.pivotZ[1]);
  settings.setValue("PivotZZ", prevValues.pivotZ[2]);
  settings.setValue("PivotYX", prevValues.pivotY[0]);
  settings.setValue("PivotYY", prevValues.pivotY[1]);
  settings.setValue("PivotYZ", prevValues.pivotY[2]);
  settings.setValue("XCenterShiftValue", prevValues.centreCoords[0]);
  settings.setValue("YCenterShiftValue", prevValues.centreCoords[1]);
  settings.setValue(
      "ZCenterShiftValue",
      prevValues.centreCoords[2] +
          static_cast<int>(projection) *
              VIEWER_DIST);  // приводим координаты центра к орто-значению
}

void Widget3D::readState(const QSettings &settings, CarcassTdef *newCrc) {
  setBackgroundColor(
      settings.value("BackgroundColor", QColor(Qt::black)).value<QColor>());
  setVertexColor(
      settings.value("VertexColor", QColor(Qt::blue)).value<QColor>());
  setFacetColor(settings.value("FacetColor", QColor(Qt::red)).value<QColor>());
  setVertexSize(settings.value("VertexSize", 5).value<double>());
  setFacetThickness(settings.value("FacetThickness", 1).value<double>());
  setVertexType(settings.value("VertexType", 0).value<int>());
  setFacetType(settings.value("FacetType", 0).value<int>());
  prevValues.scale_value = settings.value("ScaleValue", 1).value<double>();
  prevValues.rotate_values[0] =
      settings.value("XRotateValue", 0).value<double>();
  prevValues.rotate_values[1] =
      settings.value("YRotateValue", 0).value<double>();
  prevValues.rotate_values[2] =
      settings.value("ZRotateValue", 0).value<double>();
  prevValues.translate_values[0] =
      settings.value("XTranslateValue", 0).value<double>();
  prevValues.translate_values[1] =
      settings.value("YTranslateValue", 0).value<double>();
  prevValues.translate_values[2] =
      settings.value("ZTranslateValue", 0).value<double>();
  prevValues.centreCoords[0] =
      settings.value("XCenterShiftValue", 0).value<double>();
  prevValues.centreCoords[1] =
      settings.value("YCenterShiftValue", 0).value<double>();
  prevValues.centreCoords[2] =
      settings.value("ZCenterShiftValue", 0).value<double>();
  prevValues.pivotZ[0] = settings.value("PivotZX", 0).value<double>();
  prevValues.pivotZ[1] = settings.value("PivotZY", 0).value<double>();
  prevValues.pivotZ[2] = settings.value("PivotZZ", 1).value<double>();
  prevValues.pivotY[0] = settings.value("PivotYX", 0).value<double>();
  prevValues.pivotY[1] = settings.value("PivotYY", 1).value<double>();
  prevValues.pivotY[2] = settings.value("PivotYZ", 0).value<double>();
  setCarcass(newCrc);
  restoreState();
  changeProjection(
      Widget3D::projectionType(settings.value("Projection", 0).value<bool>()));
  update();
}

void Widget3D::changeProjection(projectionType type) {
  if (type != projection) {
    makeCurrent();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (type == projectionType::ORTHO) {
      translateCoord(&currentModel.crc->vertexes, 2, +VIEWER_DIST);
      prevValues.centreCoords[2] += VIEWER_DIST;
      if (width() <= height())
        glOrtho(-1.5, 1.5, -1.5 * (GLfloat)height() / (GLfloat)width(),
                1.5 * (GLfloat)height() / (GLfloat)width(), -2, 2);
      else
        glOrtho(-1.5 * (GLfloat)width() / (GLfloat)height(),
                1.5 * (GLfloat)width() / (GLfloat)height(), -1.5, 1.5, -2, 2);
    } else {
      translateCoord(&currentModel.crc->vertexes, 2, -VIEWER_DIST);
      prevValues.centreCoords[2] -= VIEWER_DIST;
      if (width() <= height())
        glFrustum(-1, 1, -1 * (GLfloat)height() / (GLfloat)width(),
                  1 * (GLfloat)height() / (GLfloat)width(), 0.7, 40.0);
      else
        glFrustum(-1 * (GLfloat)width() / (GLfloat)height(),
                  1 * (GLfloat)width() / (GLfloat)height(), -1, 1, 0.7, 40.0);
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    projection = type;
    update();
  }
}

QColor Widget3D::getBackgroundColor() const { return backgroundColor; }

QColor Widget3D::getVertexColor() const { return currentModel.vertexColor; }

double Widget3D::getVertexSize() const { return currentModel.vertexSize; }

int Widget3D::getVertexType() const { return currentModel.vertexType; }

void Widget3D::initializeGL() {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnableClientState(GL_VERTEX_ARRAY);
}

void Widget3D::paintGL() {
  QOpenGLFunctions *fncs = QOpenGLContext::currentContext()->functions();

  fncs->glClearColor(backgroundColor.redF(), backgroundColor.greenF(),
                     backgroundColor.blueF(), backgroundColor.alphaF());
  fncs->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if (currentModel.crc) {
    drawVertices();
    drawFacets();
  }
}

void Widget3D::resizeGL(int w, int h) {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (projection == projectionType::ORTHO) {
    if (w <= h)
      glOrtho(-1.5, 1.5, -1.5 * (GLfloat)h / (GLfloat)w,
              1.5 * (GLfloat)h / (GLfloat)w, -2, 2);
    else
      glOrtho(-1.5 * (GLfloat)w / (GLfloat)h, 1.5 * (GLfloat)w / (GLfloat)h,
              -1.5, 1.5, -2, 2);
  } else {
    if (w <= h)
      glFrustum(-1, 1, -1 * (GLfloat)h / (GLfloat)w,
                1 * (GLfloat)h / (GLfloat)w, 0.7, 40);
    else
      glFrustum(-1 * (GLfloat)w / (GLfloat)h, 1 * (GLfloat)w / (GLfloat)h, -1,
                1, 0.7, 40);
  }
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void Widget3D::drawVertices() {
  glVertexPointer(3, GL_DOUBLE, 0, currentModel.crc->vertexes.array);
  if (currentModel.vertexType == 0) return;
  glColor4f(currentModel.vertexColor.redF(), currentModel.vertexColor.greenF(),
            currentModel.vertexColor.blueF(),
            currentModel.vertexColor.alphaF());
  glPointSize(GLfloat(currentModel.vertexSize));
  if (currentModel.vertexType == 1) {
    glEnable(GL_POINT_SMOOTH);
  } else {
    glDisable(GL_POINT_SMOOTH);
  }
  glDrawArrays(GL_POINTS, 0, currentModel.crc->vertexes.amount);
}

void Widget3D::drawFacets() {
  glColor4f(currentModel.facetColor.redF(), currentModel.facetColor.greenF(),
            currentModel.facetColor.blueF(), currentModel.facetColor.alphaF());
  glLineWidth(currentModel.facetThickness);
  if (currentModel.facetType == 1) {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0X0F0F);
  } else
    glDisable(GL_LINE_STIPPLE);

  glDrawElements(GL_LINES, currentModel.crc->facets.amount * 2, GL_UNSIGNED_INT,
                 currentModel.crc->facets.array);
}

void Widget3D::rotate_priv(double angle, unsigned int pivot) {
  static void (*rotateFuncts[3])(double *vertexes, unsigned size,
                                 double angle) = {&rotateX, &rotateY, &rotateZ};
  rotateFuncts[pivot](currentModel.crc->vertexes.array,
                      currentModel.crc->vertexes.amount * 3, angle);
  rotateFuncts[pivot](prevValues.pivotY, 3, angle);
  rotateFuncts[pivot](prevValues.pivotZ, 3, angle);
}

void Widget3D::centerShift(bool toCenter) {
  int cf = toCenter ? -1 : 1;
  for (unsigned i = 0; i < 3; ++i)
    translateCoord(&currentModel.crc->vertexes, i,
                   cf * prevValues.centreCoords[i]);
}

void Widget3D::restoreVerticesState() {
  setVertexColor(prevModel.vertexColor);
  setVertexSize(prevModel.vertexSize);
  setVertexType(prevModel.vertexType);
}

void Widget3D::saveVerticesState() {
  prevModel.vertexColor = currentModel.vertexColor;
  prevModel.vertexSize = currentModel.vertexSize;
  prevModel.vertexType = currentModel.vertexType;
}

void Widget3D::restoreFacetsState() {
  setFacetColor(prevModel.facetColor);
  setFacetThickness(prevModel.facetThickness);
  setFacetType(prevModel.facetType);
}

void Widget3D::saveFacetsState() {
  prevModel.facetColor = currentModel.facetColor;
  prevModel.facetThickness = currentModel.facetThickness;
  prevModel.facetType = currentModel.facetType;
}

void Widget3D::setCarcass(CarcassTdef *crc) {
  clearScreen();
  currentModel.crc = crc;
  update();
}

void Widget3D::clearScreen() {
  freeCarcass(&currentModel.crc);
  currentModel.crc = NULL;
  update();
}

void Widget3D::restoreState() {
  if (currentModel.crc) {
    restoreRotation();
    scaleCarcass(&currentModel.crc->vertexes, prevValues.scale_value);
    centerShift(false);
  }
}

void Widget3D::scale(double value) {
  if (value != prevValues.scale_value && currentModel.crc) {
    centerShift();
    scaleCarcass(&currentModel.crc->vertexes,
                 1.0 * value / prevValues.scale_value);
    centerShift(false);
    prevValues.scale_value = value;
    update();
  }
}

void Widget3D::translate(double value, unsigned start) {
  if (prevValues.translate_values[start] != value && currentModel.crc) {
    translateCoord(&currentModel.crc->vertexes, start,
                   (value - prevValues.translate_values[start]) / 10);
    prevValues.centreCoords[start] +=
        ((value - prevValues.translate_values[start]) / 10);
    prevValues.translate_values[start] = value;
    update();
  }
}

void Widget3D::rotate(double angle, unsigned pivot) {
  if (angle != prevValues.rotate_values[pivot] && currentModel.crc) {
    double rot_angle = (angle - prevValues.rotate_values[pivot]) * M_PI / 180;
    centerShift();
    rotate_priv(rot_angle, pivot);
    centerShift(false);
    prevValues.rotate_values[pivot] = angle;
    update();
  }
}

void Widget3D::backToOrigin() {
  restoreOrigin(&currentModel.crc->vertexes);
  memset(&prevValues, 0, sizeof(prevValues));
  prevValues.scale_value = 1;
  prevValues.pivotY[1] = 1;
  prevValues.pivotZ[2] = 1;
}

void Widget3D::mousePressEvent(QMouseEvent *pe) {
  ptrMousePosition = pe->pos();
}

void Widget3D::mouseMoveEvent(QMouseEvent *pe) {
  double angleX = prevValues.rotate_values[0];
  double angleY = prevValues.rotate_values[1];

  angleX +=
      180 * (GLfloat)(pe->position().y() - ptrMousePosition.y()) / height();
  angleY +=
      180 * (GLfloat)(pe->position().x() - ptrMousePosition.x()) / width();
  rotate(round(angleX > 180 ? 180 : angleX < -180 ? -180 : angleX), 0);
  rotate(round(angleY > 180 ? 180 : angleY < -180 ? -180 : angleY), 1);
  emit xRotValueChanged(angleX > 180 ? 180 : angleX < -180 ? -180 : angleX);
  emit yRotValueChanged(angleY > 180 ? 180 : angleY < -180 ? -180 : angleY);
  ptrMousePosition = pe->pos();
}

void Widget3D::wheelEvent(QWheelEvent *pe) {
  if ((pe->angleDelta().y()) > 0)
    scale(
        (prevValues.scale_value + (DELTA_WHEEL * pe->angleDelta().y())) >=
                MAX_SCALE
            ? MAX_SCALE
            : (prevValues.scale_value + (DELTA_WHEEL * pe->angleDelta().y())));
  else if ((pe->angleDelta().y()) < 0)
    scale(
        (prevValues.scale_value + (DELTA_WHEEL * pe->angleDelta().y())) <=
                MIN_SCALE
            ? MIN_SCALE
            : (prevValues.scale_value + (DELTA_WHEEL * pe->angleDelta().y())));

  emit scaleWheelChanged(prevValues.scale_value);
  update();
}

void Widget3D::restoreRotation() {
  if (currentModel.crc) {
    double defPivotY[3]{0, 1, 0};
    double defPivotZ[3]{0, 0, 1};
    double pivotYCopy[3]{prevValues.pivotY[0], prevValues.pivotY[1],
                         prevValues.pivotY[2]};
    double d = std::sqrt(prevValues.pivotZ[1] * prevValues.pivotZ[1] +
                         prevValues.pivotZ[2] * prevValues.pivotZ[2]);
    double *vertexes = currentModel.crc->vertexes.array;
    double tempY = pivotYCopy[1];
    pivotYCopy[1] = tempY * (prevValues.pivotZ[2] / d) +
                    pivotYCopy[2] * -(prevValues.pivotZ[1] / d);
    pivotYCopy[2] = tempY * (prevValues.pivotZ[1] / d) +
                    pivotYCopy[2] * (prevValues.pivotZ[2] / d);
    double tempX = pivotYCopy[0];
    pivotYCopy[0] = tempX * d + pivotYCopy[2] * -prevValues.pivotZ[0];
    pivotYCopy[2] = tempX * prevValues.pivotZ[0] + pivotYCopy[2] * d;
    // A = atan2(D.X * V.Y - D.Y * V.X, D.X * V.X + D.Y * V.Y)
    double rotAngle =
        atan2(pivotYCopy[0] * defPivotY[1] - pivotYCopy[1] * defPivotY[0],
              pivotYCopy[0] * defPivotY[0] + pivotYCopy[1] * defPivotY[1]);
    rotateZ(vertexes, currentModel.crc->vertexes.amount * 3, -rotAngle);
    rotateZ(pivotYCopy, 3, rotAngle);
    for (unsigned i = 1; i < currentModel.crc->vertexes.amount * 3; i += 3) {
      double tempX = vertexes[i - 1];
      vertexes[i - 1] = tempX * d + vertexes[i + 1] * prevValues.pivotZ[0];
      vertexes[i + 1] = tempX * -prevValues.pivotZ[0] + vertexes[i + 1] * d;
    }
    for (unsigned i = 0; i < currentModel.crc->vertexes.amount * 3; i += 3) {
      double tempY = vertexes[i + 1];
      vertexes[i + 1] = tempY * (prevValues.pivotZ[2] / d) +
                        vertexes[i + 2] * (prevValues.pivotZ[1] / d);
      vertexes[i + 2] = tempY * (-prevValues.pivotZ[1] / d) +
                        vertexes[i + 2] * (prevValues.pivotZ[2] / d);
    }
    update();
  }
}
