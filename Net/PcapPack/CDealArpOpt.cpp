#include "CDealArpOpt.h"
#include "CArp.h"
#include "pthread.h"

pthread_mutex_t	g_csDhcp;
pthread_mutex_t	g_TcpCs;
pthread_mutex_t	g_UDPCs;
pthread_mutex_t	g_ARPCs;


MAP_NETSTATION_INFO	g_CheckArpInfo;				// 已经检测过的ARP的数据表
MAP_NETSTATION_INFO g_CheckUdpInfo;				// 已经检测过的UDP的数据表
MAP_NETSTATION_INFO g_CheckTcpNetInfo;			// 已经检测过的TCP的数据表
MAP_NETSTATION_INFO g_CheckDhcpNetInfo;			// DHCP的临时数据表
extern MAP_NETSTATION_INFO g_TmpArpInfo;		// ARP的临时数据表

typedef std::map<string, ARP_DISCOVERY_INFO> MAP_ARP_INFO;
MAP_ARP_INFO		g_CacheArpInfo;				// ARP缓存
ARO_NOTIFY			g_pfnCallBack = {0};

CDealArpOpt::CDealArpOpt(void)
{
    pthread_mutex_init(&g_csDhcp, NULL);
    pthread_mutex_init(&g_TcpCs, NULL);
    pthread_mutex_init(&g_UDPCs, NULL);
    pthread_mutex_init(&g_ARPCs, NULL);

    m_nRun = 0;
    m_nPause = 1;
}

CDealArpOpt::~CDealArpOpt(void)
{

}

void CDealArpOpt::StopMonitor()
{
    m_CheckNetPacket.StopCheck();

    if (m_nRun == 1)
    {
        m_nRun = 0;
        m_nPause = 1;

        if (m_handlDealArpThread)
        {
            pthread_cancel(m_handlDealArpThread);
        }
    }
}

void CDealArpOpt::StartMonitor()
{
    if (1 != m_nRun)
    {
        m_CheckNetPacket.StartCheck();

        unsigned thid1;
        m_nRun = 1;
        m_nPause = 0;
        pthread_create(&m_handlDealArpThread, NULL, DealArpThread, this);
        pthread_join(m_handlDealArpThread, NULL);

//        if(NULL != m_CheckNetPacket.m_pNetpacket)
//        {
//            pthread_t threadTmp = m_CheckNetPacket.m_pNetpacket->m_nic_pack.begin()->second.trdid;

//            pthread_join(threadTmp, NULL);
//        }

    }
}


void CDealArpOpt::SuspendCallBack()
{
    if (m_nPause != 1)
    {
       // InterlockedExchange(&m_nPause, 1);
    }
}

void CDealArpOpt::ResumeCallBack()
{
    if (m_nPause != 0)
    {
     //   InterlockedExchange(&m_nPause, 0);
    }
}

void CDealArpOpt::SetCallBack(pfn_CallBack CallBack, void* pThis)
{
    g_pfnCallBack.CallBack = CallBack;
    g_pfnCallBack.pThis    = pThis;
}

string GetIpFromULong(unsigned long uIp)
{
    in_addr addr;

    memcpy(&addr, &uIp, sizeof(uIp));

    string strIp = inet_ntoa(addr);

    return strIp;
}

BOOL GetMac(IPAddr IpAddr, BYTE *MacAddr)
{
    char macAddr[13]={0};

    ULONG   Mac[2];
    ULONG   MacLen;

    memset (Mac, 0xff, sizeof (Mac));
    MacLen = 6;

    NET_INFO netInfo;
    CArp::GetIPMAC(netInfo);

    IpAddr = ntohl(inet_addr(netInfo.strIP.c_str()));

    for(int i = 0; i < 6; ++i)
    {
        MacAddr[i] = netInfo.byteMac[i];
    }

    return true;
}

void* CDealArpOpt::DealArpThread(void *p)
{
    CDealArpOpt * delPol = (CDealArpOpt*)p;

    while (1 == delPol->m_nRun)
    {
        for(int i=0; i<5; ++i )
        {
            if (1 != delPol->m_nRun)
            {
                goto exitDhcpThread;
            }
            usleep(200);
        }

        pthread_mutex_lock(&g_ARPCs);
        if (g_TmpArpInfo.size() == 0)
        {
            pthread_mutex_unlock(&g_ARPCs);
            continue;
        }

        for (MAP_NETSTATION_INFO::iterator iterMap = g_TmpArpInfo.begin(); iterMap!=g_TmpArpInfo.end() ;++iterMap )
        {
            if (1 != delPol->m_nRun)
            {
                pthread_mutex_unlock(&g_ARPCs);
                goto exitDhcpThread;
            }

            NET_STATION_INFO_ST NetInfo;
            if (g_CheckArpInfo.find(iterMap->first) == g_CheckArpInfo.end())
            {
                //没有找到，说明这是一个新的包，需要处理
                NetInfo = iterMap->second;
                NetInfo.bHaveCheck = FALSE;
                g_CheckArpInfo.insert(std::make_pair(iterMap->first, NetInfo));
            }
        }

        g_TmpArpInfo.clear();
        pthread_mutex_unlock(&g_ARPCs);

        // 数据处理
        for (MAP_NETSTATION_INFO::iterator iterMap = g_CheckArpInfo.begin(); iterMap!=g_CheckArpInfo.end();)
        {
            if ( delPol->m_nRun!=1 )
            {
                goto exitDhcpThread;
            }

            NET_STATION_INFO_ST &NetInfo= iterMap->second;

            if ( NetInfo.bHaveCheck == FALSE)
            {
                // 将该数据包标记为探测过
                NetInfo.bHaveCheck = TRUE;

                if (0 == delPol->m_nPause)
                {
                    std::string ip   = "";
                    std::string Rip  = "";
                    char szSMac[32] = "";
                    char szDMac[32] = "";

                    // source
                    Rip = GetIpFromULong(NetInfo.ulRIp);
                    snprintf(szSMac, 32, "%02X-%02X-%02X-%02X-%02X-%02X",
                        NetInfo.SMac[0],
                        NetInfo.SMac[1],
                        NetInfo.SMac[2],
                        NetInfo.SMac[3],
                        NetInfo.SMac[4],
                        NetInfo.SMac[5]);
                    if (Rip.length() >= 7 && Rip.compare("0.0.0.0") != 0)
                    {

                        if (strlen(szSMac)   != 0	&&
                            strncmp(szSMac,"00-00-00-00-00-00", 18) == 0)
                        {
                            BYTE SMac[6] = {0};
                            BOOL bRet = GetMac(NetInfo.ulRIp, SMac);
                            if (bRet)
                            {
                                snprintf(szSMac, 32, "%02X-%02X-%02X-%02X-%02X-%02X",
                                    SMac[0],
                                    SMac[1],
                                    SMac[2],
                                    SMac[3],
                                    SMac[4],
                                    SMac[5]);
                            }
                        }

                        if (strncmp(szSMac,"00-00-00-00-00-00", 18) != 0)
                        {
                            ARP_DISCOVERY_INFO arpInfo = {0};
                            arpInfo.cstrIp  = Rip.c_str();
                            arpInfo.cstrMac = szSMac;
                            g_pfnCallBack.CallBack(&arpInfo, g_pfnCallBack.pThis);
                        }
                    }

                    // Destination
                    ip  = GetIpFromULong(NetInfo.ulIp);
                    snprintf(szDMac, 32, "%02X-%02X-%02X-%02X-%02X-%02X",
                        NetInfo.DMac[0],
                        NetInfo.DMac[1],
                        NetInfo.DMac[2],
                        NetInfo.DMac[3],
                        NetInfo.DMac[4],
                        NetInfo.DMac[5]);
                    if (ip.length() >= 7 && ip.compare("0.0.0.0") != 0)
                    {
                        if (strlen(szDMac)   != 0	&&
                                strncmp(szDMac,"00-00-00-00-00-00", 18) == 0)
                        {
                            BYTE DMac[6] = {0};
                            BOOL bRet = GetMac(NetInfo.ulIp, DMac);
                            if (bRet)
                            {
                                snprintf(szDMac, 32, "%02X-%02X-%02X-%02X-%02X-%02X",
                                    DMac[0],
                                    DMac[1],
                                    DMac[2],
                                    DMac[3],
                                    DMac[4],
                                    DMac[5]);
                            }
                        }

                        if (strncmp(szDMac,"00-00-00-00-00-00", 18) != 0)
                        {
                            ARP_DISCOVERY_INFO arpInfo = {0};
                            arpInfo.cstrIp  = ip.c_str();
                            arpInfo.cstrMac = szDMac;
                            g_pfnCallBack.CallBack(&arpInfo, g_pfnCallBack.pThis);

                        }
                    }

                }

                time(&NetInfo.checkTime);
            }
            else
            {
                //超时检测
                time_t tNow;
                time(&tNow);
                if (difftime(tNow, NetInfo.checkTime) > 30*1) // 30秒监测一次
                {
                    iterMap = g_CheckArpInfo.erase(iterMap);
                    continue;
                }
            }
            ++iterMap;
        }
    }

exitDhcpThread:
    return NULL;
}

