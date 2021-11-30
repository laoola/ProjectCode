QT += core
QT -= gui

CONFIG += c++11

TARGET = LoadExit
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

LIBS                +=   -L. -L$$OUT_PWD \
                                -L$$(HOME)/vp/bin \
                                -L$$(HOME)/vp/bin/jjwsbin \
                                -ldl -lpthread \
                                -lcemsinclude -lblkid -ludev -lutil

DESTDIR = /opt/vrv/cems/

SOURCES += main.cpp
