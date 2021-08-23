#ifndef PING_H
#define PING_H

#include "wintolinuxdef.h"

// IP 首部结构
typedef struct _iphdr
{
    unsigned int    h_len:4;			// 4位首部长度
    unsigned int    version:4;			// IP版本号
    unsigned char   tos;				// 8位服务类型TOS
    unsigned short  total_len;			// 16位总长度（字节）
    unsigned short  ident;				// 16位标识
    unsigned short  frag_and_flags;		// 3位标志位
    unsigned char   ttl;				// 8位生存时间 TTL
    unsigned char   proto;				// 8位协议
    unsigned short  checksum;			// 16位IP首部校验和
    unsigned int    sourceIP;			// 32位源IP地址
    unsigned int    destIP;				// 32位目的IP地址
} IpHeader;


// ICMP 首部结构
typedef struct _icmphdr
{
    BYTE    i_type;						// 8位类型
    BYTE    i_code;						// 8位代码
    USHORT  i_cksum;					// 16位校验和
    USHORT  i_id;						// ID标识
    USHORT  i_seq;						// 报文序列号
    ULONG   timestamp;					// 时间戳
} IcmpHeader;


// IP 选项首部 - use with socket option IP_OPTIONS
typedef struct _ipoptionhdr
{
    unsigned char code;					// 1位类型
    unsigned char len;					// 1位长度
    unsigned char ptr;					// 1位指针
    unsigned long addr[9];				// IP地址表
} IpOptionHeader;

typedef std::map<std::string, std::string> MAP_PING;

class CPing
{
public:
    CPing();
    ~CPing();

private:
    MAP_PING m_mapPing;

protected:
    /*  Function: FillICMPData
    *  Description:
    * 初始化ICMP首部
    */

    void FillICMPData(char *icmp_data, int datasize);

    /* Function: checksum
    * Description:
    * 计算ICMP首部校验和
    */

    USHORT checksum(USHORT *buffer, int size);

    /* Function: DecodeIPOptions
    * 解析IP选项
    * Description:
    * 如果有IP选项首部的话，找到IP首部中的选项输出路由记录选项
    */
    void DecodeIPOptions(char *buf, int bytes);

    /* Function: DecodeICMPHeader
    *  解析ICMP首部
    *  Description:
    *  若回复是一个IP报文，必须解析IP首部来放置ICMP数据
    */
    int DecodeICMPHeader(char *buf, int bytes, struct sockaddr_in *from);

    /*初始化*/
    int Init();
    int	work(DWORD LowAddr, DWORD HighAddr);

public:

    static long  GetSysRunTime(int type);

    int   StartPing(char* lpDestIp);
    int   SendPing();
    int   RecvPing();
    int   PingFast(DWORD LowAddr, DWORD HighAddr);
    int	  PingResult(MAP_PING & CPing);

    void  mreceive(SOCKET & sendSock);

private:
    BOOL bRecordRoute;  //是否记录路由
    int  datasize;		//数据长度
    char *lpdest;		//目的IP地址指针

private:
    DWORD m_dwBeginIp;
    DWORD m_dwEndIp;
};

#endif // PING_H
