QT -= core
QT -= gui

CONFIG += c++11
CONFIG -= app_bundle

SOURCES += \
        include/utils/istring.cpp \
        include/utils/vfile.cpp \
        main.cpp


unix:!macx: LIBS += -L$$PWD/libs/ -llog4cplus -ljsoncpp

INCLUDEPATH += $$PWD/include \
                $$PWD/utils

DEPENDPATH += $$PWD/libs

HEADERS += \
    include/utils/istring.h \
    include/utils/log.h \
    include/utils/log4.ipp \
    include/utils/vfile.h

#动态库链接失效  需创建软连接