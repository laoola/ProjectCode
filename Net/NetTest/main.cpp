
#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <net/if.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

#include <error.h>
#include <string.h>
#include <string>

//typedef unsigned int       DWORD;

//struct NET_INFO
//{
//    std::string strIP;
//    std::string strMac;
//    std::string strbroadAddr;
//    std::string strMask;
//};



//void GetLocalNet()
//{
//    char hname[128];
//    struct hostent *hent;
//    int i;

//    gethostname(hname, sizeof(hname));

//    hent = gethostbyname(hname);

//    printf("hostname: %s\naddress list: ", hent->h_name);
//    for(i = 0; hent->h_addr_list[i]; i++)
//    {
//        printf("%s\t", inet_ntoa(*(struct in_addr*)(hent->h_addr_list[i])));
//        printf("\n");
//    }
//}

//void GetNetDevice()
//{
//    struct ifaddrs * ifAddrStruct=NULL;
//    void * tmpAddrPtr=NULL;

//    getifaddrs(&ifAddrStruct);

//    while (ifAddrStruct!=NULL)
//    {
//        if (ifAddrStruct->ifa_addr->sa_family==AF_INET)
//        { // check it is IP4
//            // is a valid IP4 Address
//            tmpAddrPtr=&((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;
//            char addressBuffer[INET_ADDRSTRLEN];
//            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
//            printf("%s IPv4 Address %s\n", ifAddrStruct->ifa_name, addressBuffer);
//        }
//        else if (ifAddrStruct->ifa_addr->sa_family==AF_INET6)
//        { // check it is IP6
//            // is a valid IP6 Address
//            tmpAddrPtr=&((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;
//            char addressBuffer[INET6_ADDRSTRLEN];
//            inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
//            printf("%s IPv6 Address %s\n", ifAddrStruct->ifa_name, addressBuffer);
//        }
//        ifAddrStruct=ifAddrStruct->ifa_next;
//    }
//}

//int GetIPMAC(NET_INFO &netInfo)
//{
//    char buf[8192] = {0};
//    struct ifconf ifc = {0};
//    struct ifreq *ifr = NULL;
//    int sck = 0;
//    int nInterfaces = 0;
//    int i = 0;
//    char ip[INET6_ADDRSTRLEN] = {0};
//    char macp[19];
//    char broadAddr[32] = {0};
//    char subnetMask[32] = {0};
//    struct ifreq *item;
//    struct sockaddr *addr;

//    /* Get a socket handle. */
//    sck = socket(PF_INET, SOCK_DGRAM, 0);
//    if(sck < 0)
//    {
//        perror("socket");
//        return 1;
//    }

//    /* Query available interfaces. */
//    ifc.ifc_len = sizeof(buf);
//    ifc.ifc_buf = buf;
//    if(ioctl(sck, SIOCGIFCONF, &ifc) < 0)
//    {
//        perror("ioctl(SIOCGIFCONF)");
//        return 1;
//    }

//    /* Iterate through the list of interfaces. */
//    ifr = ifc.ifc_req;
//    nInterfaces = ifc.ifc_len / sizeof(struct ifreq);

//    for(i = 0; i < nInterfaces; i++)
//    {
//        item = &ifr[i];

//        addr = &(item->ifr_addr);

//        printf("ndevice name: %s\n", item->ifr_name);
//        //ignore the interface that not up or not runing
//        if (ioctl(sck, SIOCGIFFLAGS, item))//设置网口标记
//        {
//            //  printf("ioctl: %s [%s:%d]n", strerror(errno), __FILE__, __LINE__);
//            close(sck);
//            return -1;
//        }

//        /* Get the IP address*/
//        if(ioctl(sck, SIOCGIFADDR, item) < 0)
//        {
//            perror("ioctl(OSIOCGIFADDR)");
//        }

//        if (inet_ntop(AF_INET, &(((struct sockaddr_in *)addr)->sin_addr), ip, sizeof ip) == NULL) //vracia adresu interf
//        {
//            perror("inet_ntop");
//            continue;
//        }
//        else
//        {
//            printf("device ipaddr: %s\n", ip);
//        }

//        /* Get the MAC address */
//        if(ioctl(sck, SIOCGIFHWADDR, item) < 0) {
//            perror("ioctl(SIOCGIFHWADDR)");
//            return 1;
//        }

//        /* display result */

//        sprintf(macp, " %02x:%02x:%02x:%02x:%02x:%02x",
//                (unsigned char)item->ifr_hwaddr.sa_data[0],
//                (unsigned char)item->ifr_hwaddr.sa_data[1],
//                (unsigned char)item->ifr_hwaddr.sa_data[2],
//                (unsigned char)item->ifr_hwaddr.sa_data[3],
//                (unsigned char)item->ifr_hwaddr.sa_data[4],
//                (unsigned char)item->ifr_hwaddr.sa_data[5]);

//        printf("device mac: %s\n", macp);

//        //get the broad address of this interface
//        if (!ioctl(sck, SIOCGIFBRDADDR, item))
//        {
//            snprintf(broadAddr, sizeof(broadAddr), "%s",
//                     (char *)inet_ntoa(((struct sockaddr_in *)&
//                                        (item->ifr_broadaddr))->sin_addr));
//            printf("device broadAddr: %s\n", broadAddr);
//        }
//        else
//        {
//        //    printf("ioctl: %s [%s:%d]n", strerror(errno), __FILE__, __LINE__);
//            close(sck);
//            return -1;
//        }

//        //get the subnet mask of this interface
//        if (!ioctl(sck, SIOCGIFNETMASK, item))
//        {
//            snprintf(subnetMask, sizeof(subnetMask), "%s",
//                     (char *)inet_ntoa(((struct sockaddr_in *)&
//                                        (item->ifr_netmask))->sin_addr));
//            printf("device subnetMask: %s\n", subnetMask);
//        }
//        else
//        {
//     //       printf("ioctl: %s [%s:%d]n", strerror(errno), __FILE__, __LINE__);
//            close(sck);
//            return -1;
//        }

//        netInfo.strIP = ip;
//        netInfo.strMac = macp;
//        netInfo.strbroadAddr = broadAddr;
//        netInfo.strMask = subnetMask;

//        if(netInfo.strIP == "172.0.0.0")
//        {
//            memset(&netInfo, 0, sizeof(NET_INFO));
//        }
//    }

//    return 0;
//}


//int GetIpRange(NET_INFO netInfo, DWORD &dwBegin, DWORD &dwEnd)
//{
//    int err = 0;
//    char ip[32] = {0};
//    u_int32_t netmask;
//    struct in_addr ipadd, subnet, broadcast;

//    netmask = inet_addr(netInfo.strMask.c_str());
//    if (!(err = inet_aton(netInfo.strIP.c_str(), &ipadd)))
//    {
//        fprintf(stderr, "IP type error!\n");
//        exit(2);
//    }

//    DWORD SrcAddr = 0;
//    DWORD LowAddr;
//    DWORD HighAddr;
//    LowAddr = ntohl((ipadd.s_addr & netmask)) + 1;
//    HighAddr = ntohl(ipadd.s_addr | ~netmask) - 1;

//    SrcAddr = ipadd.s_addr;

//    char *pC_MASK = "255.255.255.0";
//    unsigned long uMask = inet_addr(pC_MASK);

//    DWORD C_low = ntohl(ipadd.s_addr & uMask) + 1;
//    DWORD C_high = ntohl(ipadd.s_addr | ~uMask) - 1;

//    dwBegin = (C_low > LowAddr ? C_low : LowAddr);
//    dwEnd = (C_high < HighAddr ? C_high : HighAddr);

////    subnet.s_addr = ipadd.s_addr & netmask;
////    broadcast.s_addr = ~ (subnet.s_addr ^ netmask);
//    subnet.s_addr = ipadd.s_addr & uMask;
//    broadcast.s_addr = ipadd.s_addr | ~uMask;
////    dwBegin = subnet.s_addr;
////    dwEnd = broadcast.s_addr;

//    printf ("%s - ", inet_ntoa(subnet));
//    printf ("%s\n", inet_ntoa(broadcast));
//    return 0;
//}

//int main(int argc, char *argv[])
//{
//    GetLocalNet();
//    GetNetDevice();

//    NET_INFO netINfo;
//    DWORD dwBegin;
//    DWORD dwEnd;

//    GetIPMAC(netINfo);

//    printf("\n\n\n");

//    GetIpRange(netINfo, dwBegin, dwEnd);

//    printf("\n\n\n");

//    return 0;
//}

//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
//#include <stdint.h>
//#include <pcap.h>
//#include <arpa/inet.h>

//#define MAXBYTES2CAPTURE 2048
//#define ARP_REQUEST     1
//#define ARP_REPLY       2

//typedef struct arphdr
//{
//    unsigned short arp_hrd;		/*硬件类型*/
//    unsigned short arp_pro;		/*协议类型*/
//    unsigned char  arp_hln;		/*硬件地址长度*/
//    unsigned char  arp_pln;		/*协议地址长度*/
//    unsigned short arp_op;		/*ARP操作类型*/
//    unsigned char  arp_sha[6];	/*源MAC地址*/
//    unsigned char  SourIP[4];	/*源IP地址*/
//    unsigned char  arp_tha[6];	/*目标MAC地址*/
//    unsigned char  DestIP[4];	/*目标IP地址*/
//}ARPHDR,*PARPHDR;

//int main(int argc, char **argv)
//{
//    int i = 0;
//    bpf_u_int32 net = 0;
//    bpf_u_int32 mask = 0;
//    struct bpf_program filter; /*place to store the filter program*/
//    char errbuf[PCAP_ERRBUF_SIZE];
//    pcap_t *handle = NULL;   /*interface handle*/
//    struct pcap_pkthdr pkthdr; /**/
//    const unsigned char *packet = NULL; /*received raw data*/
//    ARPHDR *arpheader = NULL; /*point to arp header*/

//    if (argc != 2) {
//        printf("USAGE: arpsniffer <interface>\n");
//        exit(1);
//    }

//    memset(errbuf, 0, PCAP_ERRBUF_SIZE);
//    /*open network device for packet capture*/
//    handle = pcap_open_live(argv[1], MAXBYTES2CAPTURE, 0, 512, errbuf);
//    if (handle == NULL) {
//        fprintf(stderr, "Couldn't open device %s: %s\n", argv[1], errbuf);
//        exit(1);
//    }

//    /*look up device network addr and mask*/
//    if (pcap_lookupnet(argv[1], &net, &mask, errbuf) == -1) {
//        fprintf(stderr, "Couldn't get netmask for device %s: %s\n", argv[1], errbuf);
//        exit(1);
//    }

//    /*complie the filter expression of filter program*/
//    pcap_compile(handle, &filter, "arp", 0, mask);

//    pcap_setfilter(handle, &filter);

//    while(1) {
//        /*Get one packet if null continue wait*/
//        if ((packet = pcap_next(handle, &pkthdr)) == NULL) {
//            continue;
//        }

//        arpheader = (ARPHDR *)(packet + 14); /*Point to the ARP header*/
//        printf("\n------------- ARP --------------\n");
//        printf("Received Packet Size: %d bytes\n", pkthdr.len);
//        printf("Hardware type: %s\n", (ntohs(arpheader->arp_hrd) == 1)?"Ethernet":"Unknown");
//        printf("Protocol type: %s\n", (ntohs(arpheader->arp_pro) == 0x0800)?"IPv4":"Unknown");
//        printf("Operation : %s\n", (ntohs(arpheader->arp_op) == ARP_REQUEST)?"ARP_REQUEST":"ARP_REPLY");

//        /*If is Ethernet and IPv4 print packet contents*/
//        if (ntohs(arpheader->arp_hrd) == 1 && ntohs(arpheader->arp_pro) == 0x0800) {
//            printf("\nSoucre MAC:%02x:%02x:%02X:%02x:%02x:%02x\n",
//                            arpheader->arp_sha[0], arpheader->arp_sha[1],
//                            arpheader->arp_sha[2], arpheader->arp_sha[3],
//                            arpheader->arp_sha[4], arpheader->arp_sha[5]);
//            printf("Soucre IP:%d.%d.%d.%d\n",
//                            arpheader->SourIP[0], arpheader->SourIP[1],
//                            arpheader->SourIP[2], arpheader->SourIP[3]);
//            printf("\nDestination MAC:%02x:%02x:%02X:%02x:%02x:%02x\n",
//                            arpheader->arp_tha[0], arpheader->arp_tha[1],
//                            arpheader->arp_tha[2], arpheader->arp_tha[3],
//                            arpheader->arp_tha[4], arpheader->arp_tha[5]);
//            printf("Destination IP:%d.%d.%d.%d\n",
//                            arpheader->DestIP[0], arpheader->DestIP[1],
//                            arpheader->DestIP[2], arpheader->DestIP[3]);
//        }
//    }
//    return 0;
//}



#include<stdio.h>
#include<string.h>
#include<pcap.h>
#include<time.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netinet/if_ether.h>
#include<netinet/ip.h>
#include<netinet/udp.h>
#include<netinet/tcp.h>
#include<netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <pcap.h>

#define max 1024
/*
typedef u_int32_t int_addr_t;
struct in_addr
{
    int_addr_t s_addr;
};*/

int call(u_char *argument,const struct pcap_pkthdr* pack,const u_char *content)
{
    int m=0,n;
    const u_char *buf,*iphead;
    u_char *p;
    struct ether_header *ethernet;
    struct iphdr *ip;
    struct tcphdr *tcp;
    struct udphdr *udp;
    struct icmphdr *icmp;
    buf=content;
    printf("==================================================\n");
    printf("The Frame is \n");
    while(m< (pack->len))
    {
        printf("%02x",buf[m]);
        m=m+1;
        if(m%16==0)
            printf("\n");
        else
            printf(":");
    }
    printf("\n");
    printf("Grabbed packet of length %d\n",pack->len);
    printf("Recieved at ..... %s",ctime((const time_t*)&(pack->ts.tv_sec)));
    //  printf("Ethernet address length is %d\n",ETHER_HDR_LEN);

    ethernet=(struct ether_header *)content;
    p=ethernet->ether_dhost;
    n=ETHER_ADDR_LEN;
    printf("Dest MAC is:");
    do{
        printf("%02x:",*p++);
    }while(--n>0);
    printf("\n");
    p=ethernet->ether_shost;
    n=ETHER_ADDR_LEN;
    printf("Source MAC is:");
    do{
        printf("%02x:",*p++);
    }while(--n>0);
    printf("\n");

    if(ntohs(ethernet->ether_type)==ETHERTYPE_IP)
    {
        printf("It's a IP packet\n");
        ip=(struct iphdr*)(content+14);
        printf("IP Version:%d\n",ip->version);
        printf("TTL:%d\n",ip->ttl);
//        printf("Source address:%s\n",inet_ntoa(ip->saddr));
//        printf("Destination address:%s\n",inet_ntoa(ip->daddr));
        printf("Protocol:%d\n",ip->protocol);
        switch(ip->protocol)
        {
        case 6:
            printf("The Transport Layer Protocol is TCP\n");
            tcp=(struct tcphdr*)(content+14+20);
            printf("Source Port:%d\n",ntohs(tcp->source));
            printf("Destination Port:%d\n",ntohs(tcp->dest));
            printf("Sequence Number:%u\n",ntohl(tcp->ack_seq));
            break;
        case 17:
            printf("The Transport Layer Protocol is UDP\n");
            udp=(struct udphdr*)(content+14+20);
            printf("Source port:%d\n",ntohs(udp->source));
            printf("Destination port:%d\n",ntohs(udp->dest));
            break;
        case 1:
            printf("The Transport Layer Protocol is ICMP\n");
            icmp=(struct icmphdr*)(content+14+20);
            printf("ICMP Type:%d\n", icmp->type);
            switch(icmp->type)
            {
            case 8:
                printf("ICMP Echo Request Protocol\n");
                break;
            case 0:
                printf("ICMP Echo Reply Protocol\n");
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
        /*      if(*iphead==0x45)
        {
            printf("Source ip :%d.%d.%d.%d\n",iphead[12],iphead[13],iphead[14],iphead[15]);
            printf("Dest ip :%d.%d.%d.%d\n",iphead[16],iphead[17],iphead[18],iphead[19]);

        }*/
        //      tcp= (struct tcp_header*)(iphead);
        //      source_port = ntohs(tcp->tcp_source_port);
        //      dest_port = ntohs(tcp->tcp_destination_port);

    }
    else if(ntohs (ethernet->ether_type) == ETHERTYPE_ARP)
    {
        printf("This is ARP packet.\n");
        iphead=buf+14;
        if (*(iphead+2)==0x08)
        {
            printf("Source ip:\t %d.%d.%d.%d\n",iphead[14],iphead[15],iphead[16],iphead[17]);
            printf("Dest ip:\t %d.%d.%d.%d\n",iphead[24],iphead[25],iphead[26],iphead[27]);
            printf("ARP TYPE: %d (0:request;1:respond)\n",iphead[6]);

        }
    }
    return 0;
}
int main(int argc,char *argv[])
{
    if(argc!=2)
    {
        printf("%s <number>\n",argv[0]);
        return 0;
    }
    pcap_t *handle;
    pcap_if_t *alldev;
    pcap_if_t *p;
    char error[100];

    struct in_addr net_ip_addr;
    struct in_addr net_mask_addr;
    struct ether_header *ethernet;

    char *net_ip_string;
    char *net_mask_string;
    char *interface;
    u_int32_t net_ip;
    u_int32_t net_mask;

    struct pcap_pkthdr pack;
    const u_char *content;

    int i=0,num;
    if(pcap_findalldevs(&alldev,error)==-1)
    {
        printf("find all devices is error\n");
        return 0;
    }
    for(p=alldev;p;p=p->next)
    {
        printf("%d:%s\n",++i,p->name);
        if(p->description)
        {
            printf("%s\n",p->description);
        }
    }
    if(i==1)
        interface=p->name;
    else
    {
        printf("please input which interface you want to use\n");
        scanf("%d",&num);
        if(num<1||num>i)
        {
            printf("interface is unavillible\n");
            return 0;
        }
        for(p=alldev,i=1;i<=num;p=p->next,i++)
            interface=p->name;
    }
    /*
    if((interface=pcap_lookupdev(error))==NULL)
    {
        printf("%s\n",error);
        return 0;
    }*/
    if((handle=pcap_open_live(interface,max,1,0,error))==NULL)
    {
        printf("%s\n",error);
        return 0;
    }
    if(pcap_lookupnet(interface,&net_ip,&net_mask,error)==-1)
    {
        printf("%s\n",error);
        return 0;
    }
    printf("Interface is:%s\n",interface);
    net_ip_addr.s_addr=net_ip;
    net_ip_string=inet_ntoa(net_ip_addr);
    printf("The ip is:%s\n",net_ip_string);
    net_mask_addr.s_addr=net_mask;
    net_mask_string=inet_ntoa(net_mask_addr);
    printf("The mask is:%s\n",net_mask_string);
    pcap_loop(handle,atoi(argv[1]),call,NULL);
    pcap_freealldevs(alldev);
    return 1;
}
