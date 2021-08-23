#ifndef PACKETSTRUCT_H
#define PACKETSTRUCT_H

#include "wintolinuxdef.h"

// �������õ������ݽṹ���䶨��
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
//��̫֡ͷ�ṹ��
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
//IPv4��ͷ�ṹ��
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

//IPv6���ṹ��
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

//TCP��ͷ�ṹ��
typedef struct tcp_header 
{
	WORD SourPort;    //Դ�˿ںš���
	WORD DestPort;    //Ŀ�Ķ˿ں�
	DWORD SeqNo;    //���
	DWORD AckNo;    //ȷ�����
	BYTE HLen;       //�ײ����ȣ�����λ��
	BYTE Flag;        //��ʶ������λ��
	WORD Window;    //���ڴ�С
	WORD ChkSum;    //У���
	WORD UrgPtr;     //����ָ��
}TCPHEADER,*PTCPHEADER;

//UDP��ͷ�ṹ��
typedef struct udp_header 
{
	u_short sport;          //Դ�˿ں�
	u_short dport;          //Ŀ�Ķ˿ں�
	u_short len;            //���ݱ�����
	u_short crc;            //У���
}UDPHEADER,*PUDPHEADER;


typedef struct arphdr
{
	unsigned short arp_hrd;		/*Ӳ������*/
	unsigned short arp_pro;		/*Э������*/
	unsigned char  arp_hln;		/*Ӳ����ַ����*/
	unsigned char  arp_pln;		/*Э���ַ����*/
	unsigned short arp_op;		/*ARP��������*/
	unsigned char  arp_sha[6];	/*ԴMAC��ַ*/
	unsigned char  SourIP[4];	/*ԴIP��ַ*/
	unsigned char  arp_tha[6];	/*Ŀ��MAC��ַ*/
	unsigned char  DestIP[4];	/*Ŀ��IP��ַ*/
}ARPHDR,*PARPHDR;

struct ARPPacket 
{
	WORD Type;						/*Ӳ������*/
	WORD Proto;						/*Э������*/
	BYTE HardAddrLen;				/*Ӳ����ַ����*/
	BYTE AddrLen;					/*Э���ַ����*/
	WORD Oper;						/*ARP��������*/
	struct EtherAddr SourEther;		/*�����ߵ�Ӳ����ַ*/
	struct _ip_addr SourIP;			/*�����ߵ�Э���ַ*/
	struct EtherAddr DestEther;		/*Ŀ���Ӳ����ַ*/
	struct _ip_addr DestIP;			/*Ŀ���Э���ַ*/
};

#endif
