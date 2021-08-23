#ifndef PACKETANALYSE_H
#define PACKETANALYSE_H
#include "IPacketAnalyse.h"

// ��·��
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

// �����
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
// �����
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
#endif