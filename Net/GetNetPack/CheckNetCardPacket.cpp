#include "pcap_interface.h"
#include "CheckNetCardPacket.h"
#include "./Inc/Critical.h"

#define NETSACN_ID	1001
#define DhcpPolicyClassId	101
#define DhcpDebug_Level LOG_LEVEL_DEBUG_EX(DhcpPolicyClassId)

CDistiguishPkt		CCheckNetCardPacket::m_DistigushPkt;
CCritical			g_cs;						// 同步
extern CCritical	g_csDhcp;					// 同步
extern CCritical	g_TcpCs;					// 同步
extern CCritical	g_UDPCs;					// 同步
extern CCritical	g_ARPCs;					// 同步

MAP_NETSTATION_INFO g_TmpUdpInfo;				//UCP的临时数据表
MAP_NETSTATION_INFO g_TmTcpNetInfo;				//TCP的临时数据表
MAP_NETSTATION_INFO	g_TmDhcpNetInfo;			//DHCP的临时数据表
MAP_NETSTATION_INFO g_TmpArpInfo;				// ARP的临时数据表

extern MAP_NETSTATION_INFO g_CheckUdpInfo;		//已经检测过的UDP的数据表
extern MAP_NETSTATION_INFO g_CheckTcpNetInfo;	//已经检测过的TCP的数据表
extern MAP_NETSTATION_INFO g_CheckDhcpNetInfo;	//DHCP的临时数据表
extern MAP_NETSTATION_INFO g_CheckArpInfo;		// 已经检测过的ARP的数据表

CCheckNetCardPacket::CCheckNetCardPacket(void)
{
	m_nRun = 0;
}

CCheckNetCardPacket::~CCheckNetCardPacket(void)
{
}

BOOL CCheckNetCardPacket::Init(void)
{
	BOOL bRet = FALSE;

    g_cs.Init();
    mech::pcap::NetPacket *pNetpacket = new mech::pcap::NetPacket(CallBackPkt);

    pNetpacket->init_pcap();
    pNetpacket->start_collect_packet_thread();

	bRet = TRUE;
	return bRet;
}

int CCheckNetCardPacket::CallBackPkt(struct pcap_pkthdr* pkt, const  unsigned char* data)
{
	//分析数据包
	//判断数据包类型

    WatchCallBack watchpack;

    watchpack.len = pkt->len;
    watchpack.pData = data;
//    memcpy(watchpack.mac, )

	int nRet = NONE;
	NET_STATION_INFO_ST st;
    int nType = m_DistigushPkt.DistinguishPkt(&watchpack ,st );
	g_cs.Enter();
	switch(nType)
	{
	case TCP:
		{
			g_TcpCs.Enter();
			if (g_TmTcpNetInfo.find( st.Id ) == g_TmTcpNetInfo.end() ) // 没有找到
			{
				g_TmTcpNetInfo.insert( std::make_pair( st.Id, st ) );
				nRet	= (int)nType;
			}
			else
			{
				// 找到了返回空类型
				nRet = NONE;
			}
			g_TcpCs.Leave();
			break;
		}
	case UDP_S:
		{
			if ( st.ulRIp==0)
			{
				break;
			}
			g_UDPCs.Enter();
			if (g_TmpUdpInfo.find( st.Id ) == g_TmpUdpInfo.end() ) // 没有找到
			{
				g_TmpUdpInfo.insert( std::make_pair( st.Id, st ) );
				nRet	= (int)nType;
			}
			else
			{
				// 找到了返回空类型
				nRet = NONE;
			}
			g_UDPCs.Leave();
			break;
		}
	case DHCP:
		{
			g_csDhcp.Enter();
			if (g_TmDhcpNetInfo.find( st.Id ) == g_TmDhcpNetInfo.end() ) 
			{
				// 没有找到
				g_TmDhcpNetInfo.insert( std::make_pair( st.Id, st ) );
				nRet	= (int)nType;
			}
			else
			{
				// 找到了返回空类型
				nRet = NONE;
			}
			g_csDhcp.Leave();
			break;
		}
	case ARP:
		{
			g_ARPCs.Enter();

			if (g_TmpArpInfo.find(st.Id) == g_TmpArpInfo.end()) 
			{
				// 没有找到
				g_TmpArpInfo.insert(std::make_pair(st.Id, st));
				nRet = (int)nType;
			}
			else
			{
				nRet = NONE;
			}

			g_ARPCs.Leave();
			break;
		}
	case NONE:
		{
			nRet = NONE;
			break;
		}
	}
	g_cs.Leave();
	return 1;
}

VOID CCheckNetCardPacket::SetTcpWhiteIP(std::vector< IP_RANGE_STRUCT > &whiteIpList )
{
	m_DistigushPkt.SetTcpWhiteIP(whiteIpList);
}
VOID CCheckNetCardPacket::SetUdpWhiteIP(std::vector< IP_RANGE_STRUCT > &whiteIpList )
{
	m_DistigushPkt.SetUdpWhiteIP(whiteIpList);
}
VOID CCheckNetCardPacket::SetDhcpWhiteIP(std::vector< IP_RANGE_STRUCT > &whiteIpList )
{
	m_DistigushPkt.SetDhcpWhiteIP(whiteIpList);
}

void CCheckNetCardPacket::StartCheck()
{
	if ( m_nRun == 0)
	{
		m_nRun = Init();
	}
}

void CCheckNetCardPacket::StopCheck()
{
	
	if ( m_GetNetPkt!=NULL && m_nRun==1)
	{
        m_GetNetPkt->RemoveCallback(CallBackPkt);
		m_GetNetPkt->Stop();
		m_GetNetPkt = NULL;
	}
	m_nRun = 0;
	g_cs.Delete();
}

BOOL CCheckNetCardPacket::ScanOS(NET_STATION_INFO_ST &netInfo)
{
	ULONG ulCheckIp = netInfo.ulRIp;

	if ( netInfo.Type==TCP)
	{
		ulCheckIp = netInfo.ulIp;
	}
	{
		struct	in_addr addr1;
		memcpy(&addr1, &ulCheckIp, 4);
        std::string strIp_a	= inet_ntoa(addr1);
        std::string strIp;
		strIp = strIp_a;
        //WtLog(NETSACN_ID ,LOG_LEVEL_DEBUG,strIp.GetBuffer());
        //strIp.ReleaseBuffer();
	}
	
    //CScanOS		ScanOS;
	
	ulCheckIp = ulCheckIp & 0x00ffffff;
	ULONG ulmask = 0x00ffffff;
    for( BYTE x=1; x<5 ;++x)
	{
		
		ULONG ulTmp = x;
		ulTmp <<=24;

		ULONG ulIp = ulCheckIp | ulTmp;
		struct	in_addr addr1;
		memcpy(&addr1, &ulIp, 4);
        std::string strIp	= inet_ntoa(addr1);

//		ScanOS.OSScan(strIp.GetBuffer(), NULL );
	}

	return FALSE;
}
