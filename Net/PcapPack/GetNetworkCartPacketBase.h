#pragma once

#include     "wintolinuxdef.h"

//回调参数
#pragma pack(push,1) 
typedef struct _WatchCallBack_ST_
{
    const unsigned char *	pData;		//数据
	unsigned int			len;		//长度
	char					mac[6];
	void *					pNext;		//保留字段
}WatchCallBack;


typedef struct _Packet_Filter_
{
	_Packet_Filter_()
	{
		bUseProtocol		= false;
		bUseSrcMac			= false;
		bUsedesMac			= false;
	}
	bool bUseProtocol;		//是否使用协议过滤
	bool bUseSrcMac;		//是否使用原地址过滤
	bool bUsedesMac;		//是否使用目标地址过滤
	char protocol[128];		//协议方式  如：IP ARP
	char srcMac[6];			//源MAC
	char desMac[6];			//目标MAC
}PACKET_FILTER ,*PPACKET_FILTER;
#pragma pack(pop) 
//回调函数
typedef int (* WATCHCALLBACKFUN)(int nPackLen, const  unsigned char* data);

//抓取数据包基础类
class GetNetworkCartPacketBase
{
public:
	//开启
	virtual BOOL   Start()=0;
	//关闭
	virtual BOOL   Stop()=0;

	//设置回调
	virtual BOOL  SetCallback(WATCHCALLBACKFUN pCallBack, PVOID pParentNext)=0;
	
	//删除回调
	virtual BOOL   RemoveCallback(WATCHCALLBACKFUN pCallBack)=0;

	//设置过滤,暂时不可用于2012年五月
    //virtual BOOL   SetFilter(LPCTSTR pVal)=0;

	//设置回调
	//pCallBack：回调函数指针
	//pParentNext：上下文指针
	//Proto：协议类型 如：IP ARP
	//返回发送的实际长度
	virtual BOOL  WINAPI SetCallbackEx(WATCHCALLBACKFUN pCallBack, PVOID pParentNext ,PPACKET_FILTER filer)=0;

	//发送数据包
	//srcMac：发送数据包使用的MAC， 
	//buf：发送的数据包
	//buflen：数据包的长度
	//返回发送的实际长度
	virtual int   WINAPI Sendpacket( char * srcMac, unsigned char *buf, int buflen )=0;

	virtual void   WINAPI Release( )=0;

};
