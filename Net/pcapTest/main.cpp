#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <pcap.h>
#include <arpa/inet.h>

#define MAXBYTES2CAPTURE 2048
#define ARP_REQUEST     1
#define ARP_REPLY       2

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

int main(int argc, char **argv)
{
    int i = 0;
    bpf_u_int32 net = 0;
    bpf_u_int32 mask = 0;
    struct bpf_program filter; /*place to store the filter program*/
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;   /*interface handle*/
    struct pcap_pkthdr pkthdr; /**/
    const unsigned char *packet = NULL; /*received raw data*/
    arphdr_t *arpheader = NULL; /*point to arp header*/

    if (argc != 2) {
        printf("USAGE: arpsniffer <interface>\n");
        exit(1);
    }

    memset(errbuf, 0, PCAP_ERRBUF_SIZE);
    /*open network device for packet capture*/
    handle = pcap_open_live(argv[1], MAXBYTES2CAPTURE, 0, 512, errbuf);
    if (handle == NULL) {
        fprintf(stderr, "Couldn't open device %s: %s\n", argv[1], errbuf);
        exit(1);
    }

    /*look up device network addr and mask*/
    if (pcap_lookupnet(argv[1], &net, &mask, errbuf) == -1) {
        fprintf(stderr, "Couldn't get netmask for device %s: %s\n", argv[1], errbuf);
        exit(1);
    }

    /*complie the filter expression of filter program*/
    pcap_compile(handle, &filter, "arp", 0, mask);

    pcap_setfilter(handle, &filter);

    while(1) {
        /*Get one packet if null continue wait*/
        if ((packet = pcap_next(handle, &pkthdr)) == NULL) {
            continue;
        }typedef struct arphdr
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

        arpheader = (struct arphdr *)(packet + 14); /*Point to the ARP header*/
        printf("\n------------- ARP --------------\n");
        printf("Received Packet Size: %d bytes\n", pkthdr.len);
        printf("Hardware type: %s\n", (ntohs(arpheader->htype) == 1)?"Ethernet":"Unknown");
        printf("Protocol type: %s\n", (ntohs(arpheader->ptype) == 0x0800)?"IPv4":"Unknown");
        printf("Operation : %s\n", (ntohs(arpheader->oper) == ARP_REQUEST)?"ARP_REQUEST":"ARP_REPLY");

        /*If is Ethernet and IPv4 print packet contents*/
        if (ntohs(arpheader->htype) == 1 && ntohs(arpheader->ptype) == 0x0800) {
            printf("\nSoucre MAC:%02x:%02x:%02X:%02x:%02x:%02x\n",
                            arpheader->sha[0], arpheader->sha[1],
                            arpheader->sha[2], arpheader->sha[3],
                            arpheader->sha[4], arpheader->sha[5]);
            printf("Soucre IP:%d.%d.%d.%d\n",
                            arpheader->spa[0], arpheader->spa[1],
                            arpheader->spa[2], arpheader->spa[3]);
            printf("\nDestination MAC:%02x:%02x:%02X:%02x:%02x:%02x\n",
                            arpheader->tha[0], arpheader->tha[1],
                            arpheader->tha[2], arpheader->tha[3],
                            arpheader->tha[4], arpheader->tha[5]);
            printf("Destination IP:%d.%d.%d.%d\n",
                            arpheader->tpa[0], arpheader->tpa[1],
                            arpheader->tpa[2], arpheader->tpa[3]);
        }
    }
    return 0;
}

void *GetNetPackThread(void *)
{
    int i = 0;
    bpf_u_int32 net = 0;
    bpf_u_int32 mask = 0;
    struct bpf_program filter; /*place to store the filter program*/
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;   /*interface handle*/
    struct pcap_pkthdr pkthdr; /**/
    const unsigned char *packet = NULL; /*received raw data*/
    ARPHDR *arpheader = NULL; /*point to arp header*/

    memset(errbuf, 0, PCAP_ERRBUF_SIZE);
    /*open network device for packet capture*/
    handle = pcap_open_live("ens33", MAXBYTES2CAPTURE, 0, 512, errbuf);
    if (handle == NULL) {
        fprintf(stderr, "Couldn't open device ens33: %s\n", errbuf);
        exit(1);
    }

    /*look up device network addr and mask*/
    if (pcap_lookupnet("ens33", &net, &mask, errbuf) == -1) {
        fprintf(stderr, "Couldn't get netmask for device ens33: %s\n", errbuf);
        exit(1);
    }

    /*complie the filter expression of filter program*/
    pcap_compile(handle, &filter, "arp", 0, mask);

    pcap_setfilter(handle, &filter);

    while(1) {
        /*Get one packet if null continue wait*/
        if ((packet = pcap_next(handle, &pkthdr)) == NULL) {
            continue;
        }

        arpheader = (ARPHDR *)(packet + 14); /*Point to the ARP header*/
        printf("\n------------- ARP --------------\n");
        printf("Received Packet Size: %d bytes\n", pkthdr.len);
        printf("Hardware type: %s\n", (ntohs(arpheader->arp_hrd) == 1)?"Ethernet":"Unknown");
        printf("Protocol type: %s\n", (ntohs(arpheader->arp_pro) == 0x0800)?"IPv4":"Unknown");
        printf("Operation : %s\n", (ntohs(arpheader->arp_op) == ARP_REQUEST)?"ARP_REQUEST":"ARP_REPLY");

        /*If is Ethernet and IPv4 print packet contents*/
        if (ntohs(arpheader->arp_hrd) == 1 && ntohs(arpheader->arp_pro) == 0x0800) {
            printf("\nSoucre MAC:%02x:%02x:%02X:%02x:%02x:%02x\n",
                            arpheader->arp_sha[0], arpheader->arp_sha[1],
                            arpheader->arp_sha[2], arpheader->arp_sha[3],
                            arpheader->arp_sha[4], arpheader->arp_sha[5]);
            printf("Soucre IP:%d.%d.%d.%d\n",
                            arpheader->SourIP[0], arpheader->SourIP[1],
                            arpheader->SourIP[2], arpheader->SourIP[3]);
            printf("\nDestination MAC:%02x:%02x:%02X:%02x:%02x:%02x\n",
                            arpheader->arp_tha[0], arpheader->arp_tha[1],
                            arpheader->arp_tha[2], arpheader->arp_tha[3],
                            arpheader->arp_tha[4], arpheader->arp_tha[5]);
            printf("Destination IP:%d.%d.%d.%d\n",
                            arpheader->DestIP[0], arpheader->DestIP[1],
                            arpheader->DestIP[2], arpheader->DestIP[3]);
        }
    }

    return  NULL;
}