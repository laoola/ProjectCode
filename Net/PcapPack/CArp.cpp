#include "CArp.h"

#define IT_TIME 3
DWORD SrcAddr = 0;

typedef std::map<ULONG, std::pair<std::string, std::string>> MAP_ARP_TABLE;
typedef std::vector<std::string> VECTORY_SUBKEY;

CArp::CArp()
{

}

CArp::~CArp()
{

}

void CArp::GetCurrentIpAddress(std::string &szCurrentIp)
{
    NET_INFO netInfo;

    GetIPMAC(netInfo);
    szCurrentIp = netInfo.strIP;
}

int CArp::GetIPMAC(NET_INFO &netInfo)
{
    char buf[8192] = {0};
    struct ifconf ifc = {0};
    struct ifreq *ifr = NULL;
    int sck = 0;
    int nInterfaces = 0;
    int i = 0;
    char ip[INET6_ADDRSTRLEN] = {0};
    char macp[19];
    char broadAddr[32] = {0};
    char subnetMask[32] = {0};
    struct ifreq *item;
    struct sockaddr *addr;

    /* Get a socket handle. */
    sck = socket(PF_INET, SOCK_DGRAM, 0);
    if(sck < 0)
    {
        perror("socket");
        return 1;
    }

    /* Query available interfaces. */
    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = buf;
    if(ioctl(sck, SIOCGIFCONF, &ifc) < 0)
    {
        perror("ioctl(SIOCGIFCONF)");
        return 1;
    }

    /* Iterate through the list of interfaces. */
    ifr = ifc.ifc_req;
    nInterfaces = ifc.ifc_len / sizeof(struct ifreq);

    for(i = 0; i < nInterfaces; i++)
    {
        item = &ifr[i];

        addr = &(item->ifr_addr);

 //       printf("ndevice name: %s\n", item->ifr_name);
        //ignore the interface that not up or not runing
        if (ioctl(sck, SIOCGIFFLAGS, item))//设置网口标记
        {
            //  printf("ioctl: %s [%s:%d]n", strerror(errno), __FILE__, __LINE__);
            close(sck);
            return -1;
        }

        /* Get the IP address*/
        if(ioctl(sck, SIOCGIFADDR, item) < 0)
        {
            perror("ioctl(OSIOCGIFADDR)");
        }

        if (inet_ntop(AF_INET, &(((struct sockaddr_in *)addr)->sin_addr), ip, sizeof ip) == NULL) //vracia adresu interf
        {
            perror("inet_ntop");
            continue;
        }
        else
        {
 //           printf("device ipaddr: %s\n", ip);
        }

        /* Get the MAC address */
        if(ioctl(sck, SIOCGIFHWADDR, item) < 0) {
            perror("ioctl(SIOCGIFHWADDR)");
            return 1;
        }

        /* display result */

        sprintf(macp, " %02x:%02x:%02x:%02x:%02x:%02x",
                (unsigned char)item->ifr_hwaddr.sa_data[0],
                (unsigned char)item->ifr_hwaddr.sa_data[1],
                (unsigned char)item->ifr_hwaddr.sa_data[2],
                (unsigned char)item->ifr_hwaddr.sa_data[3],
                (unsigned char)item->ifr_hwaddr.sa_data[4],
                (unsigned char)item->ifr_hwaddr.sa_data[5]);

        for(int i = 0; i < 6; ++i)
        {
            netInfo.byteMac[i] = item->ifr_hwaddr.sa_data[i];
        }

    //   printf("device mac: %s\n", macp);

        //get the broad address of this interface
        if (!ioctl(sck, SIOCGIFBRDADDR, item))
        {
            snprintf(broadAddr, sizeof(broadAddr), "%s",
                     (char *)inet_ntoa(((struct sockaddr_in *)&
                                        (item->ifr_broadaddr))->sin_addr));
     //       printf("device broadAddr: %s\n", broadAddr);
        }
        else
        {
            //    printf("ioctl: %s [%s:%d]n", strerror(errno), __FILE__, __LINE__);
            close(sck);
            return -1;
        }

        //get the subnet mask of this interface
        if (!ioctl(sck, SIOCGIFNETMASK, item))
        {
            snprintf(subnetMask, sizeof(subnetMask), "%s",
                     (char *)inet_ntoa(((struct sockaddr_in *)&
                                        (item->ifr_netmask))->sin_addr));
   //         printf("device subnetMask: %s\n", subnetMask);
        }
        else
        {
            //       printf("ioctl: %s [%s:%d]n", strerror(errno), __FILE__, __LINE__);
            close(sck);
            return -1;
        }

        netInfo.strIP = ip;
        netInfo.strMac = macp;
        netInfo.strbroadAddr = broadAddr;
        netInfo.strMask = subnetMask;

        if(netInfo.strIP == "172.0.0.0")
        {
            memset(&netInfo, 0, sizeof(NET_INFO));
        }
    }

    return 0;
}

bool CArp::GetIpRanage(NET_INFO netInfo, DWORD &dwBegin, DWORD &dwEnd)
{
    int err = 0;
    char ip[32] = {0};
    u_int32_t netmask;
    struct in_addr ipadd, subnet, broadcast;

    netmask = inet_addr(netInfo.strMask.c_str());
    if (!(err = inet_aton(netInfo.strIP.c_str(), &ipadd)))
    {
        fprintf(stderr, "IP type error!\n");
        exit(2);
    }

    DWORD SrcAddr = 0;
    DWORD LowAddr;
    DWORD HighAddr;
    LowAddr = ntohl((ipadd.s_addr & netmask)) + 1;
    HighAddr = ntohl(ipadd.s_addr | ~netmask) - 1;

    SrcAddr = ipadd.s_addr;

    char *pC_MASK = "255.255.255.0";
    unsigned long uMask = inet_addr(pC_MASK);

    DWORD C_low = ntohl(ipadd.s_addr & uMask) + 1;
    DWORD C_high = ntohl(ipadd.s_addr | ~uMask) - 1;

    dwBegin = (C_low > LowAddr ? C_low : LowAddr);
    dwEnd = (C_high < HighAddr ? C_high : HighAddr);

    //    subnet.s_addr = ipadd.s_addr & netmask;
    //    broadcast.s_addr = ~ (subnet.s_addr ^ netmask);
    subnet.s_addr = ipadd.s_addr & uMask;
    broadcast.s_addr = ipadd.s_addr | ~uMask;
    //    dwBegin = subnet.s_addr;
    //    dwEnd = broadcast.s_addr;

    //    printf ("%s - ", inet_ntoa(subnet));
    //    printf ("%s\n", inet_ntoa(broadcast));
    return true;
}



int CArp::ArpResult(MAP_ARP &arp)
{
    m_arp.swap(arp);

    return (int)arp.size();
}

