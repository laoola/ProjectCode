#include "CPing.h"

#define IP_RECORD_ROUTE     0x7
#define ICMP_ECHO       	8			// ICMP回显请求报文的类型值为8
#define ICMP_ECHOREPLY      0			//ICMP回显应答报文的类型值为0
#define ICMP_MIN            8			// ICMP报文的最小长度是8字节(仅为首部)
#define DEF_PACKET_SIZE     32			// Default packet size
#define MAX_PACKET          0x10000		// ICMP报文的最大长度是0x10000
#define MAX_IP_HDR_SIZE     60			// IP报文首部的最大长度是60（包括选项）
#define ICMP_PACKET_COUNT	2			// ICMP 包数
#define SOCKET_SELECT_TIMEOUT 50		// select 超时

#define PING_SLEEP_TIME   3*1000

static int icmpcount = 0;

SOCKET sockRaw;
volatile int g_stop = 0;

CPing::CPing()
{
    Init();
}

CPing::~CPing()
{

}

void CPing::FillICMPData(char *icmp_data, int datasize)
{
    IcmpHeader *icmp_hdr = NULL;
    char        *datapart = NULL;

    icmp_hdr = (IcmpHeader*)icmp_data;
    icmp_hdr->i_type = ICMP_ECHO;			// ICMP报文类型为请求回显
    icmp_hdr->i_code = 0;					//代码为0
    icmp_hdr->i_id = getpid();  //获得进程号
    icmp_hdr->i_cksum = 0;					//校验和置零
    icmp_hdr->i_seq = 0;					//序列号置零

    datapart = icmp_data + sizeof(IcmpHeader);				//数据端的地址为ICMP报文地址加上ICMP的首部长度

    memset(datapart, 'E', datasize - sizeof(IcmpHeader));	//填充datapart中的所有字节为“E”，长度为ICMP报文数据段长度
}


USHORT CPing::checksum(USHORT *buffer, int size)
{
    unsigned long cksum = 0;

    while (size > 1)
    {
        //对IP头部中的每16bit进行二进制求和
        cksum += *buffer++;
        size -= sizeof(USHORT);
    }
    if (size)
    {
        cksum += *(UCHAR*)buffer;				//如果size剩1，则补全16位再加1次
    }
    cksum = (cksum >> 16) + (cksum & 0xffff);	//高16bit和低16bit相加
    cksum += (cksum >>16);						//可能有进位情况，高16bit和低16bit再加1次
    return (USHORT)(~cksum);					//将该16bit的值取反，存入校验和字段
}

void CPing::DecodeIPOptions(char *buf, int bytes)
{
    IpOptionHeader *ipopt = NULL;
    in_addr  inaddr;
    int i;
    hostent *host = NULL;

    ipopt = (IpOptionHeader *)(buf + 20);		//移动指针，指针指向选项头部

    printf("RR: ");
    for(i = 0; i < (ipopt->ptr / 4) - 1; i++)	// i<IP选项中记录的地址总数
    {
        inaddr.s_addr = ipopt->addr[i];	//第i个路由器地址
        if (i != 0)
        {
            printf(" ");  //输出空格
        }
        host = gethostbyaddr((char *)&inaddr, sizeof(inaddr), AF_INET);  //通过地址获得主机名赋给host

        if (host)  //转化成功
        {
            printf("(%-15s) %s\n", inet_ntoa(inaddr), host->h_name);  //打印IP地址及主机名
        }
        else  //转化失败
        {
            printf("(%-15s)\n", inet_ntoa(inaddr));  //打印IP地址
        }
    }
    return;
}

int CPing::DecodeICMPHeader(char *buf, int bytes, struct sockaddr_in *from)
{
    IpHeader *iphdr = NULL;
    IcmpHeader *icmphdr = NULL;
    unsigned short iphdrlen;
    unsigned long tick;

    iphdr = (IpHeader *)buf;		//获取IP报文首地址
    iphdrlen = iphdr->h_len * 4;	//因为h_len是32位word，要转换成bytes必须*4
    tick = GetSysRunTime(false);		//记录开机后的毫秒数

    if ((iphdrlen == MAX_IP_HDR_SIZE) && (!icmpcount))  //回复是一个IP报文
    {
        DecodeIPOptions(buf, bytes);					//解析IP选项
    }

    if (bytes < iphdrlen + ICMP_MIN)									//回复报文长度小于IP首部长度与ICMP报文最小长度之和
    {
        printf("Too few bytes from %s\n",inet_ntoa(from->sin_addr));	//打印提示信息
    }
    icmphdr = (IcmpHeader*)(buf + iphdrlen);							//ICMP首部的地址等于IP首部长加buf

    if (icmphdr->i_type != ICMP_ECHOREPLY)								//回复报文类型不是请求回显
    {
        printf("nonecho type %d recvd\n", icmphdr->i_type);				//打印提示信息
        return -1;
    }

    if (icmphdr->i_id != getpid())		//回复报文进程号是否匹配
    {
        printf("someone else's packet!\n");					//打印提示信息
        return -1;
    }
    std::string szDestIp = inet_ntoa(from->sin_addr);


//    printf("%d bytes from %s:", bytes, szDestIp.c_str());			//取出源地址
//    printf(" icmp_seq = %d. ", icmphdr->i_seq);						//取出序列号
//    printf(" time: %d ms", tick - icmphdr->timestamp);				//打印时间戳
//    printf("\n");


    m_mapPing.insert(MAP_PING::value_type(szDestIp, szDestIp));

    icmpcount++;
    return 0;
}

int CPing::Init()
{
    bRecordRoute = FALSE;
    lpdest = NULL;
    datasize = DEF_PACKET_SIZE;

    return 1;
}

void * SendPingProc(void * lparam)
{
    CPing* pMp = static_cast<CPing*>(lparam);
    if(pMp)
    {
        g_stop = 0;

        usleep(50);
        pMp->SendPing();
        usleep(PING_SLEEP_TIME);

        g_stop = 1;
    }

    return NULL;
}

void * RecvPingProc(void* lparam)
{
    CPing* pMp = static_cast<CPing*>(lparam);
    if(pMp)
    {
        pMp->RecvPing();
    }

    return NULL;
}

int	CPing::SendPing()
{
    char *icmp_data = NULL;
    USHORT seq_no = 0;
    struct sockaddr_in dest = {'\0'};
    struct hostent *hp = NULL;


    int iret = 0;

    memset(&dest, 0, sizeof(dest));
    for(unsigned long uip = m_dwBeginIp; uip <= m_dwEndIp; uip++ )
    {
        struct in_addr addr;
        addr.s_addr = htonl(uip);
        char* lpDestIp = inet_ntoa(addr);

        lpdest = lpDestIp;

        dest.sin_family = AF_INET;
        if ((dest.sin_addr.s_addr = inet_addr(lpdest)) == INADDR_NONE)  //判断是主机名还是IP地址
        {
            if ((hp = gethostbyname(lpdest)) != NULL)
            {
                memcpy(&(dest.sin_addr), hp->h_addr, hp->h_length);  //获取main的进程ID用于设置ICMP的标识符
                dest.sin_family = hp->h_addrtype;
            }
            else
            {
                iret = -1;
                goto END;
            }
        }

        //创建ICMP报文
        datasize = sizeof(IcmpHeader);
        icmp_data = (char *)malloc(MAX_PACKET); //分配发包内存
        if (!icmp_data)
        {
            iret = -1;
            goto END;
        }

        memset(icmp_data, 0, MAX_PACKET);		//填充icmp_data中的所有字节为“0”，长度为MAX_PACKET
        FillICMPData(icmp_data, datasize);		//初始化ICMP首部

        // 开始发送ICMP数据包
        int bwrote;

        //初始化ICMP首部
        ((IcmpHeader*)icmp_data)->i_cksum = 0;
        ((IcmpHeader*)icmp_data)->timestamp = GetSysRunTime(false);
        ((IcmpHeader*)icmp_data)->i_seq = seq_no++;
        ((IcmpHeader*)icmp_data)->i_cksum = checksum((USHORT*)icmp_data, datasize);

   //     printf("send = %s\n", lpDestIp);
        bwrote= sendto(sockRaw, icmp_data, datasize, 0,(struct sockaddr*)&dest, sizeof(dest));
        if (bwrote == SOCKET_ERROR)
        {

            printf("send timed out\n");

        }

        free(icmp_data);
    }

END:
    return iret;
}

int	CPing::RecvPing()
{
    mreceive(sockRaw);
    return 1;
}

int   CPing::PingFast(DWORD LowAddr, DWORD HighAddr)
{
    pthread_t hThread[2] = {0};

    m_dwBeginIp = LowAddr;
    m_dwEndIp = HighAddr;

    int bread = 0;
    int  ret = 0;
    unsigned int addr = 0;

    struct timeval timeo;
     // 设定TimeOut时间
     timeo.tv_sec = 10;
     timeo.tv_usec = 0;

    sockRaw = INVALID_SOCKET;
    IpOptionHeader ipopt = {'\0'};

    int iret = 0;

    sockRaw = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

    if (sockRaw == INVALID_SOCKET)
    {
        printf("create socket error: %s\n", strerror(errno));

        return 3;
    }
    if (bRecordRoute)  //记录路由
    {
        ZeroMemory(&ipopt, sizeof(ipopt));
        ipopt.code = IP_RECORD_ROUTE;
        ipopt.ptr = 4;
        ipopt.len = 39;

        ret = setsockopt(sockRaw, IPPROTO_IP, IP_OPTIONS,(char *)&ipopt, sizeof(ipopt));  //设置SOCKET当前属性
    }

    bread = setsockopt(sockRaw, SOL_SOCKET, SO_RCVTIMEO,(char*)&timeo, sizeof(timeo));
    if(bread == SOCKET_ERROR)
    {
        printf("set socketopt error: %s\n", strerror(errno));

        iret = -1;
        goto END;
    }

    bread = setsockopt(sockRaw, SOL_SOCKET, SO_SNDTIMEO,(char*)&timeo, sizeof(timeo));
    if (bread == SOCKET_ERROR)
    {
        iret = -1;
        goto END;
    }

    pthread_create(&hThread[0], 0, SendPingProc, this);
    pthread_create(&hThread[1], 0, RecvPingProc, this);

    pthread_join(hThread[0], NULL);
    pthread_join(hThread[1], NULL);

END:
    if (sockRaw != INVALID_SOCKET)
    {
        close(sockRaw);
    }

    printf("CPing size = %d\n", m_mapPing.size());

    return iret;
}

int	  CPing::PingResult(MAP_PING & CPing)
{
    m_mapPing.swap(CPing);

    return (int)CPing.size();
}

long CPing::GetSysRunTime(int type)
{
    struct timespec times = {0, 0};
    long time;

    clock_gettime(CLOCK_MONOTONIC, &times);

    if (1 == type)
    {
        time = times.tv_sec;
    }
    else
    {
        time = times.tv_sec * 1000 + times.tv_nsec / 1000000;
    }

    return time;
}

void  CPing::mreceive(SOCKET & sendSock)
{
    int len;
    int maxfds = (int)(sendSock + 1);
    int nfd  = 0;
    fd_set rset;
    FD_ZERO(&rset);
    struct sockaddr_in from_addr= {'\0'};
    int fromlen = sizeof(from_addr);

    char * recvbuf = NULL;
    recvbuf = (char*)malloc(MAX_PACKET);

    while(!g_stop)
    {
        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = SOCKET_SELECT_TIMEOUT *1000;

        FD_SET(sendSock, &rset);
        if ((nfd = select(maxfds, &rset, NULL, NULL, &timeout)) == -1)
        {
            printf("select error\n");
            continue;
        }
        if (nfd == 0)
        {
            printf("select timeout\n");
            continue;
        }
        if (FD_ISSET(sendSock, &rset))
        {
            if( (len = recvfrom(sendSock, recvbuf, MAX_PACKET, 0, (struct sockaddr *)&from_addr, (socklen_t *)&fromlen)) < 0)
            {
                printf("recv len = 0\n");
                continue;
            }

   //         printf("CPing Recv = %s\n", inet_ntoa(from_addr.sin_addr));
        }

        if(DecodeICMPHeader(recvbuf, len, &from_addr) == -1)
        {
            printf("Decode fail\n");
            continue;
        }
    }

    free(recvbuf);

    return ;
}
