#ifndef IPACKET_ANALYSE_H
#define IPACKET_ANALYSE_H
#include "PacketHeader.h"
#include "CheckIpInfo.h"
#include "wintolinuxdef.h"

// 数据包解析接口

//////////////////////////////////////////////////////////////////////////
// 接口
class IAnalyse
{
public:
	IAnalyse(){}
	virtual ~IAnalyse(){}
public:
    virtual bool Analyse( const unsigned char* pszPacketData, NET_STATION_INFO_ST& StationInfo, PktType& pktType ) = 0;
    virtual void Release( ) = 0;
};

class CAnalyse : public IAnalyse
{
public:
	CAnalyse(){}
	virtual ~CAnalyse(){}
public:
	virtual void Release()
	{
		delete this;
	}
	virtual void SetNext( IAnalyse* pPktAnalyse )
	{   
		m_pPktAnalyse = pPktAnalyse;
	}
	virtual bool AnalysePacket( const unsigned char* pszPacketData, NET_STATION_INFO_ST& StationInfo, PktType& pktType )
	{   
		bool bRet = false;
		bRet = m_pPktAnalyse->Analyse( pszPacketData, StationInfo, pktType );
		return bRet;
	}
	virtual bool Analyse( const unsigned char* pszPacketData, NET_STATION_INFO_ST& StationInfo, PktType& pktType )
	{
		return false;
	}
protected:
	IAnalyse* m_pPktAnalyse; // 下一个状态
};
//////////////////////////////////////////////////////////////////////////

#endif
