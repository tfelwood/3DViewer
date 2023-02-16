#include <QString>

#include <QOpenGLFunctions>

#include <iostream>

#include "widget3d.h"

Widget3D::Widget3D(QWidget *parent):QOpenGLWidget(parent), prevValues(){
    //ui->setupUi(this);
    prevValues.scale_value = 1;
}

void Widget3D::setBackgroundColor(QColor color){
    backgroundColor = color;
    update();
}

void Widget3D::setVertexColor(QColor color){
    currentModel.vertexColor = color;
    update();
}

void Widget3D::setVertexSize(double value){
    currentModel.vertexSize = value;
    update();
}

void Widget3D::setVertexType(int value){
    currentModel.vertexType = value;
    update();
}

QColor Widget3D::getFacetColor() const{
    return currentModel.facetColor;
}

double Widget3D::getFacetThickness() const{
    return currentModel.facetThickness;
}

int Widget3D::getFacetType() const{
    return currentModel.facetType;
}

void Widget3D::setFacetColor(QColor color){
    currentModel.facetColor = color;
    update();
}

void Widget3D::setFacetThickness(double value){
    currentModel.facetThickness = value;
    update();
}

void Widget3D::setFacetType(int value){
    currentModel.facetType = value;
    update();
}

const Widget3D::model *Widget3D::getModel() const{
    return &currentModel;
}

//Widget3D::~Widget3D(){
//    delete ui;
//}

void Widget3D::writeState(QSettings &settings) const
{
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
    //приводим координаты центра к орто-значению
    settings.setValue("XCenterShiftValue", prevValues.centreCoords[0]);
    settings.setValue("YCenterShiftValue", prevValues.centreCoords[1]);
    settings.setValue("ZCenterShiftValue", prevValues.centreCoords[2] + static_cast<int>(projection) * VIEWER_DIST);
}

void Widget3D::readState(const QSettings &settings, carcass_st *newCrc)
{
    setBackgroundColor(settings.value("BackgroundColor", QColor(Qt::black)).value<QColor>());
    setVertexColor(settings.value("VertexColor", QColor(Qt::blue)).value<QColor>());
    setFacetColor(settings.value("FacetColor", QColor(Qt::red)).value<QColor>());
    setVertexSize (settings.value("VertexSize", 5).value<double>());
    setFacetThickness(settings.value("FacetThickness", 1).value<double>());
    setVertexType(settings.value("VertexType", 0).value<int>());
    setFacetType(settings.value("FacetType", 0).value<int>());
    prevValues.scale_value = settings.value("ScaleValue", 1).value<double>();
    prevValues.rotate_values[0] = settings.value("XRotateValue", 0).value<int>();
    prevValues.rotate_values[1] = settings.value("YRotateValue", 0).value<int>();
    prevValues.rotate_values[2] = settings.value("ZRotateValue", 0).value<int>();
    prevValues.translate_values[0] = settings.value("XTranslateValue", 0).value<int>();
    prevValues.translate_values[1] = settings.value("YTranslateValue", 0).value<int>();
    prevValues.translate_values[2] = settings.value("ZTranslateValue", 0).value<int>();
    prevValues.centreCoords[0] = settings.value("XCenterShiftValue", 0).value<double>();
    prevValues.centreCoords[1] = settings.value("YCenterShiftValue", 0).value<double>();
    prevValues.centreCoords[2] = settings.value("ZCenterShiftValue", 0).value<double>();
    setCarcass(newCrc);
    restoreState();
    std::cout << prevValues.centreCoords[0] << " " << prevValues.centreCoords[1] << " " << prevValues.centreCoords[2] << std::endl;
    changeProjection(Widget3D::projectionType(settings.value("Projection", 0).value<bool>()));
    update();
    std::cout << prevValues.centreCoords[0] << " " << prevValues.centreCoords[1] << " " << prevValues.centreCoords[2] << std::endl;
}

void Widget3D::printCentre(){
    std::cout << prevValues.centreCoords[0] << " " << prevValues.centreCoords[1] << " " << prevValues.centreCoords[2] << std::endl;
}

void Widget3D::changeProjection(projectionType type, double scaleCf){
    if (type != projection){
        makeCurrent();
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        if (type == projectionType::ORTHO){
            translateCoord(&currentModel.crc->vertexes, 2, +VIEWER_DIST, scaleCf);
            prevValues.centreCoords[2] += VIEWER_DIST;
//            if (width() <= height())
//                glOrtho(-1, 1, -1 * (GLfloat)height()/(GLfloat)width(), 1 * (GLfloat)height()/(GLfloat)width(), -1, 1);
//            else
//                glOrtho(-1*(GLfloat)width()/(GLfloat)height(), 1*(GLfloat)width()/(GLfloat)height(), -1, 1, -1, 1);
            if (width() <= height())
                glOrtho(-1.5, 1.5, -1.5 * (GLfloat)height()/(GLfloat)width(), 1.5 * (GLfloat)height()/(GLfloat)width(), -1.5, 1.5);
            else
                glOrtho(-1.5*(GLfloat)width()/(GLfloat)height(), 1.5*(GLfloat)width()/(GLfloat)height(), -1.5, 1.5, -1.5, 1.5);
        }else{
            translateCoord(&currentModel.crc->vertexes, 2, -VIEWER_DIST, scaleCf);
            prevValues.centreCoords[2] -= VIEWER_DIST;
            if (width() <= height())
                glFrustum(-1, 1, -1 * (GLfloat)height()/(GLfloat)width(), 1 * (GLfloat)height()/(GLfloat)width(), 1, 40.0);
            else
                glFrustum(-1*(GLfloat)width()/(GLfloat)height(), 1*(GLfloat)width()/(GLfloat)height(), -1, 1, 1, 40.0);
        }
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        projection = type;
        update();
   }
}

QColor Widget3D::getBackgroundColor() const{
    return backgroundColor;
}

QColor Widget3D::getVertexColor() const{
    return currentModel.vertexColor;
}

double Widget3D::getVertexSize() const{
    return currentModel.vertexSize;
}

int Widget3D::getVertexType() const{
  return currentModel.vertexType;
}

void Widget3D::initializeGL(){
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnableClientState(GL_VERTEX_ARRAY);
}

void Widget3D::paintGL(){
    QOpenGLFunctions *fncs = QOpenGLContext::currentContext()->functions();

    fncs->glClearColor( backgroundColor.redF(), backgroundColor.greenF(), backgroundColor.blueF(), backgroundColor.alphaF());
    fncs->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(currentModel.crc){
        drawVertices();
        drawFacets();
    }
}

void Widget3D::resizeGL(int w, int h){
   // glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (projection == projectionType::ORTHO){
//        if (w <= h)

//          glOrtho(-1, 1, -1 * (GLfloat)h/(GLfloat)w, 1 * (GLfloat)h/(GLfloat)w, -1, 1);

//        else

//          glOrtho(-1*(GLfloat)w/(GLfloat)h, 1*(GLfloat)w/(GLfloat)h, -1, 1, -1, 1);
        if (w <= h)
            glOrtho(-1.5, 1.5, -1.5 * (GLfloat)h/(GLfloat)w, 1.5 * (GLfloat)h/(GLfloat)w, -1.5, 1.5);
        else
            glOrtho(-1.5 * (GLfloat)w/(GLfloat)h, 1.5*(GLfloat)w/(GLfloat)h, -1.5, 1.5, -1.5, 1.5);
    }
    else{
        if (w <= h)
            glFrustum(-1, 1, -1 * (GLfloat)h/(GLfloat)w, 1 * (GLfloat)h/(GLfloat)w, 1, 40);
        else
            glFrustum(-1*(GLfloat)w/(GLfloat)h, 1*(GLfloat)w/(GLfloat)h, -1, 1, 1, 40);
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Widget3D::drawVertices(){

        glVertexPointer(3, GL_DOUBLE, 0, currentModel.crc->vertexes.array);
        if (currentModel.vertexType == 0)
            return ;
        glColor4f(currentModel.vertexColor.redF(), currentModel.vertexColor.greenF(),
                   currentModel.vertexColor.blueF(),currentModel.vertexColor.alphaF());
        glPointSize(GLfloat(currentModel.vertexSize));
        if (currentModel.vertexType == 1) {
                //    glDisable(GL_POINT_SIZE);
                    glEnable(GL_POINT_SMOOTH);
        }else{
                glDisable(GL_POINT_SMOOTH);
             //   glEnable(GL_POINT_SIZE);
        }
        glDrawArrays(GL_POINTS, 0, currentModel.crc->vertexes.amount);

}

void Widget3D::drawFacets()
{
    glColor4f(currentModel.facetColor.redF(), currentModel.facetColor.greenF(),
               currentModel.facetColor.blueF(), currentModel.facetColor.alphaF());
    glLineWidth(currentModel.facetThickness);
    if (currentModel.facetType == 1){
         glEnable(GL_LINE_STIPPLE);
         glLineStipple(1, 0X000F);
    }else
        glDisable(GL_LINE_STIPPLE);

    glDrawElements(GL_LINES, currentModel.crc->facets.amount * 2, GL_UNSIGNED_INT,
                   currentModel.crc->facets.array);
}

void Widget3D::rotate_priv(double angle, unsigned int pivot)
{
    //std::cout << prevValues.rotate_values[pivot] << " " << angle << std::endl;

   static void (* rotateFuncts[3])(vertex_st *vertexes, double angle)
            = {&rotateX, &rotateY, &rotateZ};
   rotateFuncts[pivot](&currentModel.crc->vertexes, angle);
}

void Widget3D::centerShift(bool toCenter)
{
    int cf = toCenter ? -1 : 1;
    for(unsigned i = 0; i < 3; ++i)
     translateCoord(&currentModel.crc->vertexes, i, cf * prevValues.centreCoords[i], 1);
}

void Widget3D::restoreVerticesState(){
    setVertexColor(prevModel.vertexColor);
    setVertexSize(prevModel.vertexSize);
    setVertexType(prevModel.vertexType);
}

void Widget3D::saveVerticesState(){
    prevModel.vertexColor = currentModel.vertexColor;
    prevModel.vertexSize = currentModel.vertexSize;
    prevModel.vertexType = currentModel.vertexType;
}

void Widget3D::restoreFacetsState(){
    setFacetColor(prevModel.facetColor);
    setFacetThickness(prevModel.facetThickness);
    setFacetType(prevModel.facetType);
}

void Widget3D::saveFacetsState(){
    prevModel.facetColor = currentModel.facetColor;
    prevModel.facetThickness = currentModel.facetThickness;
    prevModel.facetType = currentModel.facetType;
}

void Widget3D::setCarcass(carcass_st *crc){
    clearScreen();
    currentModel.crc = crc;
//    restoreState();
    update();
}

void Widget3D::clearScreen(){
    freeCarcass(currentModel.crc);
    currentModel.crc = NULL;
    update();
}

void Widget3D::restoreState(){
    for(unsigned i = 0; i < 3; ++i)
        rotate_priv(prevValues.rotate_values[i] * M_PI / 180, i);
    scaleCarcass(&currentModel.crc->vertexes, prevValues.scale_value);
    printCentre();
    centerShift(false);
}

void Widget3D::scale(double value){
    if (value != prevValues.scale_value){
        centerShift();
        scaleCarcass(&currentModel.crc->vertexes, 1.0 * value / prevValues.scale_value);
        centerShift(false);
        prevValues.scale_value = value;
        update();
    }
}

void Widget3D::translate(double value, unsigned start, double cf){
    if (prevValues.translate_values[start] != value){
       translateCoord(&currentModel.crc->vertexes, start, (value - prevValues.translate_values[start]) / 10, cf);
       prevValues.centreCoords[start] += ((value - prevValues.translate_values[start]) / 10) * cf;
       prevValues.translate_values[start] = value;
       update();
       printCentre();
    }
}

void Widget3D::rotate(double angle, unsigned pivot){
    if (angle != prevValues.rotate_values[pivot]){
        double rot_angle = (angle - prevValues.rotate_values[pivot]) * M_PI / 180;
        //std::cout << prevValues.centreCoords[0] << " " << prevValues.centreCoords[1] << " " << prevValues.centreCoords[2] << std::endl;
        //std::cout << prevValues.rotate_values[pivot] << " " << angle << std::endl;
        centerShift();
        rotate_priv(rot_angle, pivot);
        centerShift(false);
        prevValues.rotate_values[pivot] = angle;
        update();
   }
}

void Widget3D::backToOrigin(){
    restoreOrigin(&currentModel.crc->vertexes);

    memset(&prevValues, 0, sizeof (prevValues));
    prevValues.scale_value = 1;
}


void Widget3D::mousePressEvent(QMouseEvent* pe){
   ptrMousePosition = pe->pos();
}

void Widget3D::mouseMoveEvent(QMouseEvent* pe){
    double angleX;
    double angleY;

    angleX = prevValues.rotate_values[0];
    angleY = prevValues.rotate_values[1];

    //angleX += 180*(GLfloat)(pe->y()-ptrMousePosition.y())/height();
    //angleY += 180*(GLfloat)(pe->x()-ptrMousePosition.x())/width();

    angleX += 180*(GLfloat)(pe->position().y()-ptrMousePosition.y())/height();
    angleY += 180*(GLfloat)(pe->position().x()-ptrMousePosition.x())/width();

    rotate(angleX, 0);
    rotate(angleY, 1);
    emit xRotValueChanged(angleX);
    emit yRotValueChanged(angleY);

//    rotateFuncts0/57.2958);
//    prevValues.rotate_values[0] = angleX;
//    rotateFuncts1/57.2958);
//    prevValues.rotate_values[1] = angleY;
    //parent->YTranslateSlider->setValue(angleY);

    //xRot += 180/nSca*(GLfloat)(pe->y()-ptrMousePosition.y())/height();
    //zRot += 180/nSca*(GLfloat)(pe->x()-ptrMousePosition.x())/width();

   ptrMousePosition = pe->pos();

 //  update(); // обновление изображения
}

void Widget3D::wheelEvent(QWheelEvent* pe)
{
    //std::cout<<pe->angleDelta().y()<<" wheel"<<std::endl;
   if ((pe->angleDelta().y()) > 0)
       scale((prevValues.scale_value + (WHEEL_DELTA * pe->angleDelta().y())) >= MAX_SCALE ? MAX_SCALE
                                   : (prevValues.scale_value + (WHEEL_DELTA * pe->angleDelta().y())));
   else if ((pe->angleDelta().y()) < 0)
       scale((prevValues.scale_value + (WHEEL_DELTA * pe->angleDelta().y())) <= MIN_SCALE ? MIN_SCALE
                                   : (prevValues.scale_value + (WHEEL_DELTA * pe->angleDelta().y())));

   emit scaleWheelChanged(prevValues.scale_value);
   update();
}


//        double x[]{0.254821, 0.020945, 0.984178, 0.098506,
//                 0.010185, -0.004600, -0.140128, -0.064383, -1.000000, -0.195623, -0.004361, -0.955469, -0.254821, 0.038080, -0.907278, 0.030985, 0.052662, 0.027863, 0.178971, 0.064383, 1.000000};
//       glVertexPointer(3, GL_DOUBLE, 0, x);
//        glFuncs->glDrawArrays(GL_POINTS, 0,8);
