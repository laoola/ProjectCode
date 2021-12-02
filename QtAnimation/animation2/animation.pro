#-------------------------------------------------
#
# Project created by QtCreator 2016-04-06T09:36:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = animation
TEMPLATE = app
INCLUDEPATH += . animation


SOURCES += main.cpp\
    animation/abstractanimation.cpp \
    animation/animationcenter.cpp \
    animation/animationcover.cpp \
    animation/animationfly.cpp \
    animation/animationsharp.cpp \
    animation/animationslide.cpp \
    animation/animationwidget.cpp \
    mainwindow.cpp

HEADERS  += \
    animation/abstractanimation.h \
    animation/animation.h \
    animation/animationcenter.h \
    animation/animationcover.h \
    animation/animationfly.h \
    animation/animationsharp.h \
    animation/animationslide.h \
    animation/animationwidget.h \
    mainwindow.h

RESOURCES += \
    Res.qrc
