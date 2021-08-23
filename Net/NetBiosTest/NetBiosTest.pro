CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
#DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ArpQuickDiscovery/CCheckIpInfo.cpp \
    ArpQuickDiscovery/CCheckNetCardPacket.cpp \
    ArpQuickDiscovery/CDealArpOpt.cpp \
    ArpQuickDiscovery/CPackAnalyse.cpp \
    ArpQuickDiscovery/pcap_interface.cpp \
    CArp.cpp \
    CDbopt.cpp \
    CNetBios.cpp \
    CPing.cpp \
    CProxyscan.cpp \
    Callclient.cpp \
    Command.cpp \
    crc32.cpp \
    istring.cpp \
    jsoncpp.cpp \
    main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    ArpQuickDiscovery/CCheckIpInfo.h \
    ArpQuickDiscovery/CCheckNetCardPacket.h \
    ArpQuickDiscovery/CDealArpOpt.h \
    ArpQuickDiscovery/CPackAnalyse.h \
    ArpQuickDiscovery/GetNetworkCartPacketBase.h \
    ArpQuickDiscovery/IPacketAnalyse.h \
    ArpQuickDiscovery/PacketHeader.h \
    ArpQuickDiscovery/Type.h \
    ArpQuickDiscovery/pcap_interface.h \
    CArp.h \
    CDbopt.h \
    CNetBios.h \
    CPing.h \
    CProxyscan.h \
    Callclient.h \
    Command.h \
    EdpBaseDefine.h \
    IBase.h \
    IObject.h \
    MBC_DBUtil.h \
    cems_net.h \
    istring.h \
    json.h \
    json/json-forwards.h \
    json/json.h \
    wintolinuxdef.h

LIBS += -lutil -ldl -lpthread -lpcap

DISTFILES += \
