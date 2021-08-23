#pragma once
#include <map>
#include <vector>
#include "../Inc/GetNetworkCartPacketBase.h"
#include "../Inc/Type.h"
#include "wintolinuxdef.h"

#define PATH_MAINCONFING _T("EdpNtStChk.xml")

typedef std::vector< IP_RANGE_STRUCT > WhiteList;

//������IP��ַ�Ƿ��Ѿ��ϱ����࣬���û���ϱ��ʹ洢

class CCheckIpInfo
{
public:
	CCheckIpInfo(void);
	~CCheckIpInfo(void);
public:
	static LONGLONG GenerateId( unsigned char Type, ULONG ulIp, USHORT usPort );
};


#define MUTEX_NAME  _T( "Global\\FOR_VECTOR" )

//��XMLûϴ
//������ݰ�����,ͨ���������ݰ���������ݰ�����
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

	WhiteList	m_WithInfoListUdp;			// �������б�
	WhiteList	m_WithInfoListTcp;			// �������б�
	WhiteList	m_WithInfoListDhcp;			// �������б�
	BOOL		m_isWiteListSet;		// �Ƿ������˰�����
    pthread_mutex_t m_hMutex;			// ������
	BOOL		IsIpInWhiteList		(ULONG &ulRIp ,std::vector< IP_RANGE_STRUCT > &whiteIpList);
};
