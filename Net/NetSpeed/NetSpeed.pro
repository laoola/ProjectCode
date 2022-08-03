QT -= gui core

CONFIG += c++11 console
CONFIG -= app_bundle
DEFINES += QT_DEPRECATED_WARNINGS

DESTDIR=../bin

SOURCES += \
        main.cpp \
        net_device.cpp \
        utility.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    net_device.hpp \
    utility.hpp
