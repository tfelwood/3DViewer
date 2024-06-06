QT       += core gui
QT       += openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += warn_on

QMAKE_CC = gcc
QMAKE_CFLAGS += -Wall -Werror -Wextra -std=c11
QMAKE_CXXFLAGS += -Wall -Werror -Wextra

INCLUDEPATH = includes/

DESTDIR     = bin
MOC_DIR     = moc
OBJECTS_DIR = obj
#RCC_DIR     = qrc
UI_DIR      = uic

SOURCES += $$files(src/*.cpp) $$files(src/*.c)
HEADERS += $$files(includes/*.h)
FORMS   += $$files(forms/*.ui)



include(QtGifImage/src/gifimage/qtgifimage.pri)

win32{
LIBS += -lOpengl32
}
# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target
