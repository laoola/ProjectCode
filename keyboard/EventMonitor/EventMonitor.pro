QT -= gui core

CONFIG += c++11 console
CONFIG -= app_bundle
DESTDIR = ../bin
TARGET = ghad

DEFINES += QT_DEPRECATED_WARNINGS


SOURCES += \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

LIBS += -lX11 -lXext -lXtst

