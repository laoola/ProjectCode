#include "CPackAnalyse.h"

// 解析链路层
bool CEnthAnalyse::Analyse( const unsigned char* pszPacketData, NET_STATION_INFO_ST& StationInfo, PktType& pktType )
{
    bool			bRet		= false;
    bool			bIsIPV4		= false;
    bool			bIsArp		= false;
    bool			bIsLocalMac = true;
    PIPHEADER		ipV4		= NULL;
    ipv6_header*	ipV6		= NULL;
    PARPHDR			Arp			= NULL;
    PETHHEADER		eth			= (PETHHEADER)pszPacketData;

    for( int i = 0; i < 6; i++ )
    {
        //对应UDP我们这里不关心我们发出的，只关心我们收到的
        StationInfo.SMac[i] = eth->ether_shost[i] ;
        StationInfo.DMac[i] = eth->ether_dhost[i] ;
    }

    cout<<ntohs(eth->ether_type)<<"  "<<ETH_ARP<<endl;

    if( ntohs( eth->ether_type) == ETH_IPv6 ) // 纯IPv6
    {
        ipV6 = (ipv6_header*)(pszPacketData+sizeof(ETHHEADER));
        // 暂时不做
        StationInfo.Type	= NONE;
        pktType				= NONE;
        bIsIPV4				= false;
    }
    else if( ntohs(eth->ether_type) == ETH_IP ) // IPV4
    {
        ipV4 = (PIPHEADER)(pszPacketData+sizeof(ETHHEADER)); // 计算到IP
        bIsIPV4 = true;
    }
    else if(ntohs(eth->ether_type) == ETH_ARP) // ARP
    {
        Arp = (PARPHDR)(pszPacketData + sizeof(ETHHEADER));
        bIsArp = true;
    }
    else // 未知包
    {
        StationInfo.Type	= NONE;
        pktType				= NONE;
       return false;
    }

    // 去除以太头进入下一层
    if( bIsIPV4 == true ) // IPV4
    {
        IAnalyse* pNextAnalyse = new CIPV4Analyse();
        SetNext( pNextAnalyse );
        bRet = AnalysePacket( (const unsigned char*)ipV4, StationInfo, pktType );
        pNextAnalyse->Release();
    }
    else if (bIsArp == true)
    {
        IAnalyse* pNextAnalyse = new CArpAnalyse();
        SetNext(pNextAnalyse);
        bRet = AnalysePacket((const unsigned char*)Arp, StationInfo, pktType);
        pNextAnalyse->Release();
    }
    else // IPV6
    {
        IAnalyse* pNextAnalyse = new CIPV6Analyse();
        SetNext( pNextAnalyse );
        bRet = AnalysePacket( (const unsigned char*)ipV6, StationInfo, pktType );
        m_pPktAnalyse->Release();
    }
    return bRet;
}

// 解析网络层IPv4
bool CIPV4Analyse::Analyse( const unsigned char* pszPacketData, NET_STATION_INFO_ST& StationInfo, PktType& pktType )
{
    // 解析IP头
    bool		bRet	= false;
    PIPHEADER	ipV4	= (PIPHEADER)pszPacketData;

    /*CString strRIp = "";
    CString strDIp = "";
    strRIp.Format("%d.%d.%d.%d",ipV4->ip_src[0],ipV4->ip_src[1],ipV4->ip_src[2],ipV4->ip_src[3]);
    strDIp.Format("%d.%d.%d.%d",ipV4->ip_dst[0],ipV4->ip_dst[1],ipV4->ip_dst[2],ipV4->ip_dst[3]);
    StationInfo.ulIp  = inet_addr( strDIp.GetBuffer() );
    StationInfo.ulRIp = inet_addr( strRIp.GetBuffer() );
    ULONG ulRIp = 0;*/
    memcpy( &StationInfo.ulIp, ipV4->ip_dst, 4);
    memcpy( &StationInfo.ulRIp, ipV4->ip_src, 4);

    if( ipV4->proto == 0x29 )
    {
        //双协议环境下IPv6 暂时不做
        StationInfo.Type	= NONE;
        pktType				= NONE;
        return false;
    }
    else if( ipV4->proto == 6 ) // TCP
    {
        PTCPHEADER	tcp;
        IAnalyse* pNextAnalyse	= new CTCPAnalyse();
        SetNext( pNextAnalyse );
        tcp						= (PTCPHEADER)(pszPacketData +(ipV4->ver_ihl&15)*4);
        bRet					= AnalysePacket( (const unsigned char*)tcp, StationInfo, pktType );
        m_pPktAnalyse->Release();
    }
    else if( ipV4->proto == 17 )// UDP
    {
        PUDPHEADER	udp;
        IAnalyse* pNextAnalyse	= new CUDPAnalyse();
        SetNext( pNextAnalyse );
        udp						= (PUDPHEADER)(pszPacketData +(ipV4->ver_ihl&15)*4);
        bRet					= AnalysePacket( (const unsigned char*)udp, StationInfo, pktType );
        m_pPktAnalyse->Release();
    }
    else
    {
        cout<<"other protocol."<<endl;
        // 其他服务暂不考虑
        StationInfo.Type	= NONE;
        pktType				= NONE;
        bRet				= false;
    }
    return bRet;
}

bool CArpAnalyse::Analyse(const unsigned char* pszPacketData, NET_STATION_INFO_ST& StationInfo, PktType& pktType )
{
    PARPHDR arp;

    bool bRet	= false;
    arp			= (PARPHDR)(pszPacketData);

    pktType				= ARP;
    StationInfo.Type	= ARP;

    memcpy( &StationInfo.ulRIp, arp->SourIP, 4);
    memcpy( &StationInfo.ulIp, arp->DestIP, 4);

    for (int i = 0; i < 6; i++)
    {
        StationInfo.SMac[i] = arp->arp_sha[i];
        StationInfo.DMac[i] = arp->arp_tha[i];
    }

    StationInfo.Id = CCheckIpInfo::GenerateId( StationInfo.Type, StationInfo.ulIp, 0 );

    bRet = true;
    return bRet;
}

// 解析网络层IPv6
bool CIPV6Analyse::Analyse( const unsigned char* pszPacketData, NET_STATION_INFO_ST& StationInfo, PktType& pktType )
{
    // 解析IP头
    bool	bRet = false;
    return	bRet;
}

// 传输层
bool CTCPAnalyse::Analyse( const unsigned char* pszPacketData, NET_STATION_INFO_ST& StationInfo, PktType& pktType )
{
    PTCPHEADER	tcp;
    bool bRet	= false;
    tcp			= (PTCPHEADER)(pszPacketData);
    //tcp数据包只关心我们自己发出的
    for( int i = 0; i < 6; i++ )
    {
       if( StationInfo.SMac[i]!= StationInfo.mac[i] )
       {
           bRet = false;
           return bRet;
       }
    }
    pktType				= TCP;
    StationInfo.Type	= TCP;
    StationInfo.usPort	= tcp->DestPort;
    StationInfo.usRPort = tcp->SourPort;
    StationInfo.Id		= CCheckIpInfo::GenerateId( StationInfo.Type, StationInfo.ulIp, 0 );
    //检测目的地址是否在白名单内
    bRet = true;
    return bRet;
}


bool CUDPAnalyse::Analyse( const unsigned char* pszPacketData, NET_STATION_INFO_ST& StationInfo, PktType& pktType )
{
    bool		bRet		= false;
    PUDPHEADER udp			= (PUDPHEADER)pszPacketData;
    bool		bIsLocalMac = true;

    //对应UDP我们这里不关心我们发出的，只关心我们收到的
    //for( int i = 0; i < 6; i++ )
    //{
    //	if( StationInfo.SMac[i]!= StationInfo.mac[i] )
    //	{
    //		bIsLocalMac = false;
    //		break;
    //	}
    //}

    //if ( bIsLocalMac == true )
    //{//如果是我们自己发出的UDP，放弃
    //	return bRet;
    //}

    // 其他数据包不关心
    StationInfo.usPort	= udp->dport;
    StationInfo.usRPort = udp->sport;
    if ( ntohs(StationInfo.usPort) == 68 &&ntohs(StationInfo.usRPort) == 67)
    {
        pktType				= DHCP;
        StationInfo.Type	= DHCP;
    }
    else
    {
        pktType				= UDP_S;
        StationInfo.Type	= UDP_S;
    }

    StationInfo.Id		= CCheckIpInfo::GenerateId( StationInfo.Type, StationInfo.ulRIp, StationInfo.usRPort );
    bRet = TRUE;

    return bRet;
}

