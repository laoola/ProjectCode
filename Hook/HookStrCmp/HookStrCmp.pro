#-------------------------------------------------
#
# Project created by QtCreator 2021-05-20T10:22:15
#
#-------------------------------------------------

QT       -= core gui

TARGET = HookStrCmp
TEMPLATE = lib
CONFIG += shared plugin

DEFINES += HOOKSTRCMP_LIBRARY

LIBS += -ldl

SOURCES += hookstrcmp.cpp

HEADERS += hookstrcmp.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
