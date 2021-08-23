#pragma once
#include "./Inc/GetNetworkCartPacketBase.h"
#include "./AnalysisPkt/CheckIpInfo.h"

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
	BOOL				Init(void);

    static int	CallBackPkt(struct pcap_pkthdr* pkt, const  unsigned char* data);

	VOID				SetTcpWhiteIP(std::vector< IP_RANGE_STRUCT > &whiteIpList );
	VOID				SetUdpWhiteIP(std::vector< IP_RANGE_STRUCT > &whiteIpList );
	VOID				SetDhcpWhiteIP(std::vector< IP_RANGE_STRUCT > &whiteIpList );

	BOOL				ScanOS(NET_STATION_INFO_ST &netInfo);
private:
	HANDLE m_hCheck;
	HANDLE m_hTcpCheck;
	HANDLE m_hDhcpCheck;
	HANDLE m_hUdpCheck;
	GetNetworkCartPacketBase *	m_GetNetPkt;
	
	HINSTANCE					m_hInstance;
	int							m_nRun;					//运行标志
};
