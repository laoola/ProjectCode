QT += gui core widgets

CONFIG += c++11 console
CONFIG -= app_bundle
DESTDIR = /opt/apps/cn.com.vrv.cems/files/cems/esa
#DESTDIR = $$(HOME)/vp/bin/esa
#DESTDIR = ../bin
TARGET = vrvscreenhot

QMAKE_LFLAGS += -g -Wl,-rpath=. -fvisibility=hidden -Wl,-z,now -Wl,-z,noexecstack -fPIC -pie -fPIE -fstack-protector-all -ftrapv -s
QMAKE_CFLAGS +=-g -fvisibility=hidden -Wl,-z,now -Wl,-z,noexecstack -fPIC -pie -fPIE -fstack-protector-all -ftrapv -s
QMAKE_CXXFLAGS +=-g -fvisibility=hidden -Wl,-z,now -Wl,-z,noexecstack -fPIC -pie -fPIE -fstack-protector-all -ftrapv -s
DEFINES += QT_DEPRECATED_WARNINGS
QMAKE_LFLAGS=-Wl,-rpath=\'\$\$ORIGIN\'/../:\'\$\$ORIGIN\'/../qt/lib


SOURCES += \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

LIBS += -lX11 -lXext -lXtst

