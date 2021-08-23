#pragma once
#include <map>
#include <vector>
#include "../Inc/GetNetworkCartPacketBase.h"
#include "../Inc/Type.h"
#include "wintolinuxdef.h"

#define PATH_MAINCONFING _T("EdpNtStChk.xml")

typedef std::vector< IP_RANGE_STRUCT > WhiteList;

//检测这个IP地址是否已经上报的类，如果没有上报就存储

class CCheckIpInfo
{
public:
	CCheckIpInfo(void);
	~CCheckIpInfo(void);
public:
	static LONGLONG GenerateId( unsigned char Type, ULONG ulIp, USHORT usPort );
};


#define MUTEX_NAME  _T( "Global\\FOR_VECTOR" )

//跟XML没洗
//检测数据包的类,通过解析数据包，检测数据包类型
class CDistiguishPkt
{
public:
	CDistiguishPkt()
	{
		m_isWiteListSet = FALSE;
        pthread_mutex_init(&m_hMutex, NULL);
	}
	virtual ~CDistiguishPkt(){}
public:
	int DistinguishPkt( WatchCallBack *lPCallBack , NET_STATION_INFO_ST &stInfo );
	VOID				SetTcpWhiteIP(std::vector< IP_RANGE_STRUCT > &whiteIpList );
	VOID				SetUdpWhiteIP(std::vector< IP_RANGE_STRUCT > &whiteIpList );
	VOID				SetDhcpWhiteIP(std::vector< IP_RANGE_STRUCT > &whiteIpList );

private:

	WhiteList	m_WithInfoListUdp;			// 白名单列表
	WhiteList	m_WithInfoListTcp;			// 白名单列表
	WhiteList	m_WithInfoListDhcp;			// 白名单列表
	BOOL		m_isWiteListSet;		// 是否设置了白名单
    pthread_mutex_t m_hMutex;			// 互斥量
	BOOL		IsIpInWhiteList		(ULONG &ulRIp ,std::vector< IP_RANGE_STRUCT > &whiteIpList);
};
