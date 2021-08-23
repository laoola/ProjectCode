QT -= gui

CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        CArp.cpp \
        CCheckIpInfo.cpp \
        CCheckNetCardPacket.cpp \
        CDealArpOpt.cpp \
        CPackAnalyse.cpp \
        main.cpp \
        pcap_interface.cpp


LIBS += -lpcap -lpthread

HEADERS += \
    CArp.h \
    CCheckIpInfo.h \
    CCheckNetCardPacket.h \
    CDealArpOpt.h \
    CPackAnalyse.h \
    GetNetworkCartPacketBase.h \
    IPacketAnalyse.h \
    PacketHeader.h \
    Type.h \
    pcap_interface.h \
    wintolinuxdef.h

DISTFILES +=
