QT += gui core \
    widgets

CONFIG += c++11 console
CONFIG -= app_bundle
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_MESSAGELOGCONTEXT
DESTDIR = ../bin

QMAKE_LFLAGS=-Wl,-rpath=\'\$\$ORIGIN\'/./libs

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        mypdfcanvas.cpp \
        pdfutils.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += $$PWD/include

HEADERS += \
    include/poppler-qt5.h \
    mainwindow.h \
    mypdfcanvas.h \
    pdfutils.h

unix:!macx: LIBS += -L$$PWD/libs/ -lpoppler-qt5 -lpoppler

INCLUDEPATH += $$PWD/libs
DEPENDPATH += $$PWD/libs
