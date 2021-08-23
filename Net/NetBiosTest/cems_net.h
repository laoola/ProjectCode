/*
cems_net.h interface define file

create sunyi 15-04-13

*/
#pragma once

#include "wintolinuxdef.h"
using namespace  std;

////扫描服务////
#define CEMS_MAXCODE_SCAN							0x00FF0200	//
//00FF0200.00FF0001	TCP转发服务	客户端获取上传文件服务地址
//00FF0200.00FF0002	TCP转发服务	接收代理扫描结果(转给阻断服务和数据处理服务)
#define	CEMS_MINCODE_SCAN_TASK						0x00FF0002
#define CEMS_MINCODE_SCAN_BROADCAST					0x00FF000a

#define CEMS_MINCODE_REPORTDATA_VRVPRODUCT			0x00FF5007		//产品安装日志
#define CEMS_MINCODE_REPORTDATA_UP_VRVPRODUCT		0x00016002		//产品更新日志
//////////////////////////////////////////////////////////////////////////


#define CEMS_MAXCODE_APPROVAL						0x00FF1200	//审批服务udp通知客户端大功能号
#define CEMS_MINCODE_APPROVAL						0x00002002	//审批服务udp通知客户端小功能号



typedef struct _CEMS_UDP_HEAD
{
	DWORD dwVersion;//
	DWORD dwMaxCode;//0x00010100 0x00010400 0x00011200
	DWORD dwMinCode;//0x00000004 0x00000001
	char szMark[32];//
	DWORD dwMsgCode;//0x00000001

	DWORD dwSize;//数据包大小
	DWORD dwCount;//3
	DWORD dwIndex;//1,2,3
	BYTE data[20];// 
} CEMS_UDP_HEAD, * PCEMS_UDP_HEAD;

typedef struct _CEMS_UDP_PACK
{
	DWORD dwDataSize;//数据包大小
	WORD wIndex;//1,2,3
	string strData;//
} CEMS_UDP_PACK, * PCEMS_UDP_PACK;

typedef vector<CEMS_UDP_PACK> VECTOR_CEMS_UDP_PACK;

typedef struct _CEMS_NET_HEAD
{
	DWORD dwFlag;			//_edp
	DWORD dwVersion;		//0x01010101
	DWORD dwDataSize;		//数据包大小
	DWORD dwCrc;			//数据包的crc，数据包中包含自增msgcode，防止篡改发包

	BYTE szSessionId[16];	//GUID
	DWORD dwMsgCode;		//0x00000001自增，防止伪造发包

	DWORD dwMaxCode;		//数据包业务maxCode，0x00010100 0x00010400 0x00011200
	DWORD dwMinCode;		//数据包业务minCode，0x00000004 0x00000001

	WORD wHeadSize;			//sizeof(CEMS_TCP_HEAD)
	WORD wType;				//数据包是zip格式的
	WORD wCount;			//3
	WORD wIndex;			//1,2,3
	BYTE szAreaId[32];	//区域ID
} CEMS_NET_HEAD, * PCEMS_NET_HEAD;

