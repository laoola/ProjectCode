#include "Callclient.h"
#include "cems_net.h"

#define  CEMS_CLIENT_UDP		22116
#define  BUFFER_SIZE			1024*5

//#define  MAX_UDP_CODE	0x10100
//#define  MIN_UDP_CODE   0x4

using namespace std;

MCallClient::MCallClient(void)
{
}

MCallClient::~MCallClient(void)
{
}

std::string MCallClient::Read(std::string szKey)
{
    BOOL bRet = FALSE;
    std::string szValue;

    return szValue;
}

std::string MCallClient::ReadXml(std::string szElementName, std::string szKey, std::string szXml)
{
    std::string szReadValue;


    return szReadValue;
}

DWORD MCallClient::GetLocalUdpPort()
{
    DWORD dwPort = CEMS_CLIENT_UDP;

    std::string szXml = Read("NetConfig");
    if(!szXml.empty())
    {
        dwPort = atoi(ReadXml("udpServer", "Port", szXml).c_str());
        if(dwPort == 0)
        {
            dwPort = CEMS_CLIENT_UDP;
        }
    }

    return dwPort;
}


std::string MCallClient::GetHostName()
{
    char name[MAX_PATH] = {0};
    int iLen = MAX_PATH;

    std::string szName;
    if(gethostname(name, iLen) == 0)
    {
        szName = name;
    }

    return szName;
}

extern  unsigned int crc32(unsigned int crc, const void *buf, unsigned int size);

//udpP2p
bool  MCallClient::Call(DWORD & dwBegin, DWORD & dwEnd)
{
    m_vResult.clear();

    SOCKET SendSocket;

    sockaddr_in toAddr;

    string szHeader;

    SendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if(INVALID_SOCKET == SendSocket)
    {
        return false;
    }

    toAddr.sin_family = AF_INET;
    //emsstore.dat 读取CEMS_TRAY;netconfig udp port
    toAddr.sin_port = htons(GetLocalUdpPort());

    struct timeval tv_out = {2, 0};

    //recvfrom 设置超时
    setsockopt(SendSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv_out, sizeof(tv_out));

    //组包
    Json::Value root;
    Json::FastWriter writer;

    char name[MAX_PATH] = {0};
    hostent *phostinfo;

    std::string szCurrentIp;
    if( gethostname ( name, sizeof(name)) == 0)
    {
        if((phostinfo = gethostbyname(name)))
        {
            szCurrentIp = inet_ntoa(*(struct in_addr *)phostinfo->h_addr_list);
        }
    }

    root["ip"]	= szCurrentIp;

    std::string szQueryJson = writer.write(root);

    _CEMS_NET_HEAD header = {0};

    DWORD dwJsonSize = (DWORD)szQueryJson.size();
    DWORD dwCrc = 0xffffffff;

    header.dwFlag = 'pde_';
    header.dwVersion = (1);
    header.dwDataSize = dwJsonSize;
    header.dwCrc = crc32(dwCrc, (void*)szQueryJson.c_str(), szQueryJson.size());

    //header.szSessionId = {0};
    header.dwMsgCode = (1);

    header.dwMaxCode = ((unsigned long)CEMS_MAXCODE_SCAN);
    header.dwMinCode = ((unsigned long)CEMS_MINCODE_SCAN_TASK);

    header.wHeadSize = sizeof(header);
    header.wType = (0);
    header.wCount = (1);
    header.wIndex = (1);

    DWORD dwSize = sizeof(header) + dwJsonSize;

    char * pbuffer = new char[dwSize];

    memcpy(pbuffer, &header, sizeof(header));
    memcpy(pbuffer + sizeof(header), szQueryJson.c_str(),szQueryJson.size());

    DWORD dwSendLen = sizeof(header) + dwJsonSize;

    for(ULONG uip = dwBegin; uip <= dwEnd; uip ++)
    {
        toAddr.sin_addr.s_addr = htonl(uip);
        int sent = sendto(SendSocket, pbuffer, dwSendLen, 0, (const struct sockaddr*) &toAddr, sizeof(toAddr));

        if(sent == SOCKET_ERROR)
        {
            printf("sendto fail.\n");
        }
    }

    delete []pbuffer;

    RecvUdp(SendSocket);
    close(SendSocket);
    return true;
}

void  MCallClient::RecvUdp(SOCKET& sock)
{
    sockaddr_in fromAddr;
    int ReceiveAddrSize = sizeof(fromAddr);
    static int nRecvConut = 0;

    while(1)
    {
        int ret;
        fd_set fst;
        FD_ZERO(&fst);
        FD_SET(sock, &fst);

        struct timeval tm_out = {4, 0};
        ret = select((int)(sock + 1), &fst, NULL, NULL, &tm_out);

        if(ret == 0 || ret == -1)
        {
            break;
        }

        char RecvBuf[BUFFER_SIZE] = {0};

        ret = recvfrom(sock, RecvBuf, BUFFER_SIZE, 0, (sockaddr*)&fromAddr, (socklen_t*)&ReceiveAddrSize);

        if(ret != SOCKET_ERROR)
        {
            _CEMS_NET_HEAD* pHead = (_CEMS_NET_HEAD*)RecvBuf;

            std::string szReceive;
            DWORD dwSize = (pHead->dwDataSize);
            szReceive.append((char*)(RecvBuf + pHead->wHeadSize), dwSize);

            m_vResult.push_back(szReceive);

            char ip[INET6_ADDRSTRLEN] = {0};
            if (inet_ntop(AF_INET, &(((struct sockaddr_in *)&fromAddr)->sin_addr), ip, sizeof ip) == NULL) //vracia adresu interf
            {
                perror("inet_ntop");
                continue;
            }
            else
            {
//                printf("respon client ipaddr: %s\n", ip);
            }

            struct tm st;

            GetLocalTime(&st);
//            printf("receive from client : %s  time : %04d-%02d-%02d %02d:%02d:%02d \n", szReceive.c_str(),
//                   st.tm_year, st.tm_mon, st.tm_mday, st.tm_hour, st.tm_min, st.tm_sec);
            nRecvConut++;
        }
    }
    printf("recv %d client response.\n", nRecvConut);
}

void  MCallClient::GetResult(VECTOR_RESULT & vResult)
{
    m_vResult.swap(vResult);
}


void GetLocalTime(struct tm *ptm)
{
    time_t curTime = time(NULL);
    tm * loc = localtime(&curTime);
    *ptm = *loc;
    ptm->tm_mon++;
    ptm->tm_year += 1900;
}
