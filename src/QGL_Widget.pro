QT       += core gui
QT       += openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
include(QtGifImage/src/gifimage/qtgifimage.pri)

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    parser.c \
    facet.cpp \
    main.cpp \
    mainwindow.cpp \
    vertex.cpp \
    widget3d.cpp \
    carcass.c \
    get_next_line_bonus.c \
    get_next_line_utils_bonus.c \

HEADERS += \
    facet.h \
    mainwindow.h \
    vertex.h \
    widget3d.h \
    parser.h \
    carcass.h \
    get_next_line_bonus.h \

FORMS += \
    facet.ui \
    mainwindow.ui \
    vertex.ui

//LIBS += -lOpengl32


# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target
