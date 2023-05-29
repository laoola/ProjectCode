TEMPLATE = app
#CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += debug

DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += /usr/include/pango-1.0 \
                /usr/include/cairo \
                /usr/include/glib-2.0 \
                /usr/lib/x86_64-linux-gnu/glib-2.0/include \
                /usr/include/harfbuzz

LIBS += -lwayland-client -lcairo -lpango-1.0 -lpng -lm -lpthread


SOURCES += \
        main.cpp \
        os-compatibility.cpp \
        xdg-shell-protocols.c

HEADERS += \
    config.h \
    os-compatibility.h \
    xdg-shell-client-protocol.h \
    zalloc.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

