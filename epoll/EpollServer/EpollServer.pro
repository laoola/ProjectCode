QT -= core
QT -= gui

CONFIG += c++11

TARGET = EpollServer
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app
DESTDIR = ../output

SOURCES += main.cpp
