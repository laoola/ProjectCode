#ifndef CCHECKNETCARDPACKET_H
#define CCHECKNETCARDPACKET_H

#pragma once

#include "GetNetworkCartPacketBase.h"
#include "CCheckIpInfo.h"
#include "pcap_interface.h"

class CCheckNetCardPacket
{
public:
    CCheckNetCardPacket(void);
    ~CCheckNetCardPacket(void);
public:
    static CDistiguishPkt		m_DistigushPkt;			//数据包解析类
public:
    void				StartCheck();
    void				StopCheck();
    bool				Init(void);

    static int	CallBackPkt(int nPackLen, const  unsigned char* data);

    void				SetTcpWhiteIP(std::vector< IP_RANGE_STRUCT > &whiteIpList );
    void				SetUdpWhiteIP(std::vector< IP_RANGE_STRUCT > &whiteIpList );
    void				SetDhcpWhiteIP(std::vector< IP_RANGE_STRUCT > &whiteIpList );
private:
    GetNetworkCartPacketBase *	m_GetNetPkt;
    mech::pcap::NetPacket *m_pNetpacket;
    int							m_nRun;					//运行标志
};


#endif // CCHECKNETCARDPACKET_H
