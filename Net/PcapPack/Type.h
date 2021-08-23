#pragma once

//定义全局结构
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include "wintolinuxdef.h"

using namespace std;
typedef enum
{
	NONE	= 0,  //当为0的时候说明我们没有判别出来，也就是我们不关心的数据
	HTTP	= 1 ,
	EAMIL	= 2,
	FTP		= 3,
	DNS		= 4,
	DHCP	= 5,
	TCP		= 6,
	ARP		= 7,
	UDP_S		//违规外联
}PktType;

typedef struct _NET_STATION_INFO_ST
{  
	BYTE				SMac[6];			//原地址的mac地址
	BYTE				DMac[6];			//原地址的mac地址
	BYTE				mac[6];				//当前主机自己的MAC
	ULONG				ulRIp;				//ip地址 //源IP
	ULONG				ulIp;				//ip地址 //目的IP
	USHORT				usRPort;			//端口   //源端口
	USHORT				usPort;				//端口   //端口目的端口
	unsigned char		Type;				//类型 
	LONGLONG			Id;					//唯一ID，一个8字节值，由三部分组成 type(1)IP（4）port（2）
        string				strHosName;			//域名
	BOOL				bHaveCheck;			//检测标志
	time_t				checkTime;			//检查时间
}NET_STATION_INFO_ST;

typedef std::map<LONGLONG, NET_STATION_INFO_ST> MAP_NETSTATION_INFO;
typedef std::vector< NET_STATION_INFO_ST > VEC_STATION_INFO;


//////////////////////////////////////////////////////////////////////////
//IP段相关定义
typedef std::vector<string> CSTRING_VECTOR;

typedef struct _IP_RANGE
{
	unsigned long start_ip;	//开始地址
	unsigned long end_ip;	//结束地址
}IP_RANGE_STRUCT;

typedef struct _IP_MASK_ST_
{
        string	ip;
        string	mask;
}IP_MASK_ST, PIP_MASK_ST;

typedef std::vector<IP_MASK_ST> IPMASK_VECTOR;


typedef ULONG IPAddr;  
//////////////////////////////////////////////////////////////////////////

#define T_DLL_UP_INFO              1  //请求信息上报
#define T_DLL_REQUEST_DISCON	   2  //请求阻断
#define T_DLL_REQUEST_SHUTDOWN     3  //请求关机
#define T_DLL_PROMPT_INFO          4  //信息提示
#define T_DLL_WRITE_LOG			   5  //写日志
#define T_DLL_UP_RUNINFO		   6  //上报运行信息

//	恢复
#define KILL_NO				1

//	普通阻断
#define KILL_NORMAL			2

//	手工阻断
#define KILL_MANUAL			3

//	永久阻断模式
#define KILL_FOREVER		4

//	阻断模式(全阻断)
#define KILL_ALL_NOOTHER	5

//	阻断模式(带IP模式)
#define KILL_USER_IPADDR	6

//	恢复的阻断模式
#define KILL_RESTORE_6		7

//	阻断模式(带IP模式)
#define KILL_USER_IPADDR_2	8

//	恢复的阻断模式
#define KILL_RESTORE_8		9

//	阻断指定ip模式
#define KILL_SPECIFY_IP		10

//	阻断Modem
#define KILL_MODEM			0x10000000


#define POLICY_CLIENT_BORDERCHECK_NetCard		"POLICY-NETWORK-BOUNDARY-SCAN"
#define POLICY_CLIENT_BORDERCHECK_NetSegment	"POLICY-NETWORK-BOUNDARY-SCAN"
#define POLICY_CLIENT_BORDERCHECK_TCP			"POLICY-NETWORK-BOUNDARY-SCAN"
#define POLICY_CLIENT_BORDERCHECK_UDP			"POLICY-NETWORK-BOUNDARY-SCAN"
#define POLICY_CLIENT_BORDERCHECK_DHCP			"POLICY-NETWORK-BOUNDARY-SCAN"

#define KIND_BORDERCHECK_NETCARD				5201
#define KIND_BORDERCHECK_NETSEGMENT				5202
#define KIND_BORDERCHECK_TCP					5203
#define KIND_BORDERCHECK_DHCP					5205
#define KIND_BORDERCHECK_UDP					5204

struct AdapterInfo {
	string			deviceName;
	string			description;
	BYTE			clientMac[20];
	IPMASK_VECTOR	IpVec;
	CSTRING_VECTOR	IpGetWay;
        string			strGetway;
        string			strMask;
    int             nIndex;
};
