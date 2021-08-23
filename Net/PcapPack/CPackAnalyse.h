#ifndef CPACKANALYSE_H
#define CPACKANALYSE_H

#include "IPacketAnalyse.h"

// 链路层
class CEnthAnalyse : public CAnalyse
{
public:
    CEnthAnalyse(){}
    CEnthAnalyse( IAnalyse* pPktAnalyse )
    {
        m_pPktAnalyse = pPktAnalyse;
    }
    virtual ~CEnthAnalyse(){}
public:
    virtual bool Analyse(const unsigned char* pszPacketData, NET_STATION_INFO_ST& StationInfo, PktType& pktType );
};

// 网络层
class CIPV4Analyse : public CAnalyse // IPV4
{
public:
    CIPV4Analyse(){}
    virtual ~CIPV4Analyse(){}
public:
    virtual bool Analyse(const unsigned char* pszPacketData, NET_STATION_INFO_ST& StationInfo, PktType& pktType );

};

class CIPV6Analyse : public CAnalyse // IPV6
{
public:
    CIPV6Analyse(){}
    virtual ~CIPV6Analyse(){}
public:
    virtual bool Analyse(const unsigned char* pszPacketData, NET_STATION_INFO_ST& StationInfo, PktType& pktType );
};

// ARP
class CArpAnalyse:public CAnalyse
{
public:
    CArpAnalyse(){}
    virtual ~CArpAnalyse(){}
public:
    virtual bool Analyse(const unsigned char* pszPacketData, NET_STATION_INFO_ST& StationInfo, PktType& pktType );
};
// 传输层
class CTCPAnalyse : public CAnalyse //TCP
{
public:
    CTCPAnalyse(){}
    virtual ~CTCPAnalyse(){}
public:
    virtual bool Analyse( const unsigned char* pszPacketData, NET_STATION_INFO_ST& StationInfo, PktType& pktType );
};

class CUDPAnalyse : public CAnalyse //UDP
{
public:
    CUDPAnalyse(){}
    virtual ~CUDPAnalyse(){}
public:
    virtual bool Analyse( const unsigned char* pszPacketData, NET_STATION_INFO_ST& StationInfo, PktType& pktType);
};

class CAppAnalyse : public CAnalyse
{
public:
    CAppAnalyse(){}
    virtual ~CAppAnalyse(){}
public:
    virtual bool Analyse( const unsigned char* pszPacketData, NET_STATION_INFO_ST& StationInfo, PktType& pktType );

};

#endif // CPACKANALYSE_H
