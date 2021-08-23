
#include "CheckIpInfo.h"
#include "PacketAnalyse.h"


CCheckIpInfo::CCheckIpInfo(void)
{
}

CCheckIpInfo::~CCheckIpInfo(void)
{
}

LONGLONG CCheckIpInfo::GenerateId( unsigned char Type, ULONG ulIp, USHORT usPort )
{
    LONGLONG llId     = 0;
    LONGLONG llTmpId  = 0;
    llTmpId		= Type;
    llId		= ( llId | ( llTmpId<<48) );
    llTmpId		= 0;
    llTmpId		= ulIp;
    llId		= ( llId | ( llTmpId<<16) );
    llId		= ( llId | usPort );
    return llId;
}

int  CDistiguishPkt::DistinguishPkt( WatchCallBack *lPCallBack , NET_STATION_INFO_ST &stInfo )
{   
    int			nRet	= 0;
    PktType		type	=NONE;
    CAnalyse	analyse;
    NET_STATION_INFO_ST st;
    memcpy( st.mac, lPCallBack->mac, 6*sizeof(char) );
    CEnthAnalyse* pEnthAnalyse = new CEnthAnalyse();
    if( NULL == pEnthAnalyse )
    {
        nRet = NONE;
        goto exit;
    }
    analyse.SetNext( pEnthAnalyse ); // 初始化

    if( analyse.AnalysePacket( lPCallBack->pData, st, type ) )
    {
        nRet	= (int)type;
        switch(type)
        {
        case TCP:
        {
            if( IsIpInWhiteList(st.ulIp ,m_WithInfoListTcp) )
            {
                nRet = NONE;
                goto exit;
            }
            stInfo	= st;
            break;
        }
        case UDP_S:
        {
            if( IsIpInWhiteList(st.ulRIp, m_WithInfoListUdp) )
            {
                nRet = NONE;
                goto exit;
            }
            st.ulRIp = st.ulRIp&0x00ffffff;
            st.Id		= CCheckIpInfo::GenerateId( st.Type, st.ulRIp, 0 );
            stInfo	= st;
            break;
        }
        case DHCP:
        {
            if( IsIpInWhiteList(st.ulRIp ,m_WithInfoListDhcp) )
            {
                nRet = NONE;
                goto exit;
            }
            stInfo	= st;
            break;
        }
        case ARP:
        {
            stInfo = st;
            break;
        }
        case NONE:
        {
            nRet = NONE;
            break;
        }
        }
    }
    else
    {
        nRet = NONE;
    }
    pEnthAnalyse->Release();
exit:
    return nRet;
}
BOOL CDistiguishPkt::IsIpInWhiteList(ULONG &ulRIp,std::vector< IP_RANGE_STRUCT > &whiteIpList)
{
    BOOL  bRet	= FALSE;
    ULONG CurIp = ntohl( ulRIp );
    pthread_mutex_lock(&m_hMutex);
    for( unsigned i = 0; i < whiteIpList.size(); i++ )
    {
        ULONG ulDIpStart = whiteIpList[i].start_ip;
        ULONG ulDIpEnd   = whiteIpList[i].end_ip;
        if(CurIp >= ulDIpStart && CurIp <= ulDIpEnd ) // 在白名单内返回NONE
        {
            bRet = TRUE;
            break;
        }
    }
    pthread_mutex_unlock(&m_hMutex);
    return bRet;
}

void CDistiguishPkt::SetTcpWhiteIP( std::vector< IP_RANGE_STRUCT > &whiteIpList )
{
    pthread_mutex_lock(&m_hMutex);
    m_WithInfoListTcp.clear();
    m_WithInfoListTcp = whiteIpList;
    m_isWiteListSet = TRUE;
    pthread_mutex_unlock(&m_hMutex);
}
void CDistiguishPkt::SetUdpWhiteIP( std::vector< IP_RANGE_STRUCT > &whiteIpList )
{
    pthread_mutex_lock(&m_hMutex);
    m_WithInfoListUdp.clear();
    m_WithInfoListUdp = whiteIpList;
    m_isWiteListSet = TRUE;
    pthread_mutex_unlock(&m_hMutex);
}
void CDistiguishPkt::SetDhcpWhiteIP( std::vector< IP_RANGE_STRUCT > &whiteIpList )
{
    pthread_mutex_lock(&m_hMutex);
    m_WithInfoListDhcp.clear();
    m_WithInfoListDhcp = whiteIpList;
    m_isWiteListSet = TRUE;
    pthread_mutex_unlock(&m_hMutex);
}
