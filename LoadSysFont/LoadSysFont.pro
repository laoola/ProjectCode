QT += core
QT += gui

CONFIG += c++11

TARGET = LoadSysFont
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

QMAKE_LFLAGS=-Wl,-rpath=\'\$\$ORIGIN\':\'\$\$ORIGIN\'/qt/lib

SOURCES += main.cpp
