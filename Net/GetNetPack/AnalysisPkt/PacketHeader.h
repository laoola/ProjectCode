#ifndef PACKETSTRUCT_H
#define PACKETSTRUCT_H

#include "wintolinuxdef.h"

// 本程序用到的数据结构及其定义
#define ETH_ARP 0x0806
#define ARP_REQUEST 0x0001
#define ARP_REPLY 0x0002
#define ARP_HARDWARE 0x0001
#define ETH_IP 0x0800
#define ETH_IPv6 0x86dd
#define MAXSTRINGSIZE 256
#define MAXENTRY 1024
#define IP_TCP 6
#define IP_UDP 17

typedef unsigned char   u_char;
typedef unsigned short  u_short;
typedef unsigned int    u_int;
typedef unsigned long   u_long;

struct EtherAddr 
{
	unsigned char AddrByte[6];
};
//以太帧头结构体
typedef struct ether_header 
{
	unsigned char ether_dhost[6];
	unsigned char ether_shost[6];
	unsigned short ether_type;
}ETHHEADER,*PETHHEADER;

struct _ip_addr 
{
	unsigned char AddrByte[4];
};
//IPv4包头结构体
typedef struct ip_header 
{
	unsigned char ver_ihl;  //Version (4 bits) + Internet header length (4 bits)
	unsigned char tos;     //Type of service
	unsigned short tlen;    //Total length
	unsigned short identification;   //Identification
	unsigned short flags_fo;       //Flags (3 bits) + Fragment offset (13 bits)
	unsigned char ttl;            //Time to live
	unsigned char proto;          //Protocol
	unsigned short crc;           //Header checksum
	u_char ip_src[4];            //Source address
	u_char ip_dst[4];            //Destination address
}IPHEADER,*PIPHEADER;

//IPv6包结构体
typedef struct ipv6_header {
	u_char ver_tf;
	u_char traffic;
	u_short label;
	u_short length;
	u_char next_header;
	u_char limits;
	u_char Srcv6[16];
	u_char Destv6[16];
}*PIPv6_HEADER;

//TCP包头结构体
typedef struct tcp_header 
{
	WORD SourPort;    //源端口号　　
	WORD DestPort;    //目的端口号
	DWORD SeqNo;    //序号
	DWORD AckNo;    //确认序号
	BYTE HLen;       //首部长度（保留位）
	BYTE Flag;        //标识（保留位）
	WORD Window;    //窗口大小
	WORD ChkSum;    //校验和
	WORD UrgPtr;     //紧急指针
}TCPHEADER,*PTCPHEADER;

//UDP包头结构体
typedef struct udp_header 
{
	u_short sport;          //源端口号
	u_short dport;          //目的端口号
	u_short len;            //数据报长度
	u_short crc;            //校验和
}UDPHEADER,*PUDPHEADER;


typedef struct arphdr
{
	unsigned short arp_hrd;		/*硬件类型*/
	unsigned short arp_pro;		/*协议类型*/
	unsigned char  arp_hln;		/*硬件地址长度*/
	unsigned char  arp_pln;		/*协议地址长度*/
	unsigned short arp_op;		/*ARP操作类型*/
	unsigned char  arp_sha[6];	/*源MAC地址*/
	unsigned char  SourIP[4];	/*源IP地址*/
	unsigned char  arp_tha[6];	/*目标MAC地址*/
	unsigned char  DestIP[4];	/*目标IP地址*/
}ARPHDR,*PARPHDR;

struct ARPPacket 
{
	WORD Type;						/*硬件类型*/
	WORD Proto;						/*协议类型*/
	BYTE HardAddrLen;				/*硬件地址长度*/
	BYTE AddrLen;					/*协议地址长度*/
	WORD Oper;						/*ARP操作类型*/
	struct EtherAddr SourEther;		/*发送者的硬件地址*/
	struct _ip_addr SourIP;			/*发送者的协议地址*/
	struct EtherAddr DestEther;		/*目标的硬件地址*/
	struct _ip_addr DestIP;			/*目标的协议地址*/
};

#endif
