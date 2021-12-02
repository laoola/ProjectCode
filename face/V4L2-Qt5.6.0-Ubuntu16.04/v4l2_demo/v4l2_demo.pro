#-------------------------------------------------
#
# Project created by QtCreator 2019-04-25T10:43:14
#
#-------------------------------------------------

QT       += core gui
QT       += multimediawidgets
QT       += xml
QT       += multimedia
QT       += network
QT       += widgets
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = v4l2_demo
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        widget.cpp \
    LPF_V4L2.c \
    majorimageprocessingthread.cpp

HEADERS += \
        widget.h \
    LPF_V4L2.h \
    majorimageprocessingthread.h

FORMS += \
        widget.ui

unix:!macx: LIBS += -L/usr/local/opencv3/lib/ \
                    -lopencv_calib3d -lopencv_core -lopencv_dnn -lopencv_features2d -lopencv_flann -lopencv_highgui -lopencv_imgcodecs \
                     -lopencv_imgproc -lopencv_ml -lopencv_objdetect -lopencv_photo \
                     -lopencv_shape -lopencv_stitching -lopencv_superres -lopencv_videoio -lopencv_video

INCLUDEPATH += /usr/local/opencv3/include
DEPENDPATH += /usr/local/opencv3/include
