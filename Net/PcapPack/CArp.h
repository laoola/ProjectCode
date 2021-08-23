#ifndef CARP_H
#define CARP_H

#include "wintolinuxdef.h"
//#include <iprtrmib.h>

struct ItemWork
{
    PVOID   param;
    ULONG	uIpNet;
};

struct _ARP_CACHE
{
    std::string StrIpaddr;
    std::string StrMacaddr;
};

struct NET_INFO
{
    std::string strIP;
    std::string strMac;
    std::string strbroadAddr;
    std::string strMask;
    BYTE byteMac[6];
};

typedef std::map<std::string, std::string>  MAP_ARP;
typedef std::map<DWORD, std::string> MAP_MAC;

class CArp
{
public:
    CArp();
    ~CArp();

public:
  //  static DWORD WINAPI AsySendArp( __in LPVOID lpParameter);

public:
    static void GetCurrentIpAddress(std::string & szCurrentIp) ;
    static int GetIPMAC(NET_INFO &netInfo);
    static bool GetIpRanage(NET_INFO netInfo, DWORD &dwBegin, DWORD &dwEnd);

public:
    static long g_ItemCount;
    static long g_ScanCount;

public:
    static HANDLE g_hCompleteEvent;

public:
    MAP_ARP		m_arp;

public:
    int  ArpResult(MAP_ARP & arp);

public:
    int  GetMacTable(MAP_MAC & map_mac);
};

#endif // CARP_H
