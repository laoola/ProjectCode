#include "CCheckNetCardPacket.h"
#include "pcap_interface.h"
#include "CArp.h"

#define MAXBYTES2CAPTURE 2048
#define ARP_REQUEST     1
#define ARP_REPLY       2


#define NETSACN_ID	1001
#define DhcpPolicyClassId	101
#define DhcpDebug_Level LOG_LEVEL_DEBUG_EX(DhcpPolicyClassId)

CDistiguishPkt		CCheckNetCardPacket::m_DistigushPkt;
pthread_mutex_t			g_cs;						// 同步
extern pthread_mutex_t	g_csDhcp;					// 同步
extern pthread_mutex_t	g_TcpCs;					// 同步
extern pthread_mutex_t	g_UDPCs;					// 同步
extern pthread_mutex_t	g_ARPCs;					// 同步

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
    if(NULL != m_pNetpacket)
    {
        delete m_pNetpacket;
        m_pNetpacket = NULL;
    }
}

bool CCheckNetCardPacket::Init(void)
{
    BOOL bRet = FALSE;

    pthread_mutex_init(&g_cs, NULL);
    m_pNetpacket = new mech::pcap::NetPacket(CallBackPkt);

    m_pNetpacket->init_pcap();
    m_pNetpacket->start_collect_packet_thread();

    bRet = TRUE;
    return bRet;
}

int CCheckNetCardPacket::CallBackPkt(int nPackLen, const  unsigned char* data)
{
    //分析数据包
    //判断数据包类型

    WatchCallBack watchpack;
    NET_INFO netinfo;
    CArp::GetIPMAC(netinfo);

    watchpack.len = nPackLen;
    watchpack.pData = data;

    for(int i = 0; i < 6; ++i)
    {
        watchpack.mac[i] = netinfo.byteMac[i];
    }

    int nRet = NONE;
    NET_STATION_INFO_ST st;
    int nType = m_DistigushPkt.DistinguishPkt(&watchpack ,st );
    pthread_mutex_lock(&g_cs);
    switch(nType)
    {
    case TCP:
        {
            pthread_mutex_lock(&g_TcpCs);
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
            pthread_mutex_unlock(&g_TcpCs);
            break;
        }
    case UDP_S:
        {
            if ( st.ulRIp==0)
            {
                break;
            }
            pthread_mutex_lock(&g_UDPCs);
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
            pthread_mutex_unlock(&g_UDPCs);
            break;
        }
    case DHCP:
        {
            pthread_mutex_lock(&g_csDhcp);
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
            pthread_mutex_unlock(&g_csDhcp);
            break;
        }
    case ARP:
        {
            pthread_mutex_lock(&g_ARPCs);

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

            pthread_mutex_lock(&g_ARPCs);
            break;
        }
    case NONE:
        {
            nRet = NONE;
            break;
        }
    }
    pthread_mutex_unlock(&g_cs);
    return 1;
}

void CCheckNetCardPacket::SetTcpWhiteIP(std::vector< IP_RANGE_STRUCT > &whiteIpList )
{
    m_DistigushPkt.SetTcpWhiteIP(whiteIpList);
}
void CCheckNetCardPacket::SetUdpWhiteIP(std::vector< IP_RANGE_STRUCT > &whiteIpList )
{
    m_DistigushPkt.SetUdpWhiteIP(whiteIpList);
}
void CCheckNetCardPacket::SetDhcpWhiteIP(std::vector< IP_RANGE_STRUCT > &whiteIpList )
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
        m_pNetpacket->stop_collect_packet_thread();
        m_pNetpacket->uninit_pcap();
    }
    m_nRun = 0;
}

