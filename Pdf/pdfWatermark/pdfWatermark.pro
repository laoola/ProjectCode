QT -= gui core

CONFIG += c++11 console
CONFIG -= app_bundle
DEFINES += QT_DEPRECATED_WARNINGS
DESTDIR = ./../bin

SOURCES += \
        AddWatermark.cpp \
        Initialization.cpp \
        RegexSearch.cpp \
        RegexSetPattern.cpp \
        Utils.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    include/AddWatermark.h \
    include/Initialization.h \
    include/Pdfix.h \
    include/RegexSearch.h \
    include/RegexSetPattern.h \
    include/Utils.h

LIBS += -ldl -pthread