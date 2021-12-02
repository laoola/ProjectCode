#-------------------------------------------------
#
# Project created by QtCreator 2017-04-19T12:43:18
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FaceRecognition
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    camerathread.cpp \
    camerautils.cpp \
    imageutils.cpp \
    faceutils.cpp

HEADERS  += mainwindow.h \
    camerathread.h \
    camerautils.h \
    imageutils.h \
    faceutils.h

FORMS    += mainwindow.ui

INCLUDEPATH +=  /usr/local/opencv3/include \

LIBS    +=   -L. -L/usr/local/opencv3/lib

#include($(QT_DIR)/config/x8.config)
