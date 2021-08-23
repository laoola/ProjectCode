#ifndef CPROXYSCAN_H
#define CPROXYSCAN_H

#include "wintolinuxdef.h"
#include "CNetBios.h"
#include "Callclient.h"
#include "CPing.h"
#include "CArp.h"
#include "cems_net.h"
#include "ArpQuickDiscovery/CDealArpOpt.h"

class CProxyScan
{
public:
    CProxyScan();
    ~CProxyScan();
public:
    bool		ScanStart();
    void		ScanStop();

public:
    void		Work();
    void		Scan();

public:
    void		ScanClient(DWORD & dwBegin, DWORD & dwEnd);
    void		JsonToStruct(VECTOR_RESULT & result, MAP_BIOS& bios);
    bool		Filter(MAP_BIOS & t_bios, MAP_BIOS & t_change);
    bool		FilterEx(MAP_BIOS & t_bios, MAP_BIOS & t_change);
    std::string GetStrMd5(char* szSrc, DWORD dwLen);

    bool		ProcessUdp(std::string szUdp, CEMS_NET_HEAD* pHead);
    bool		UpScanInfo(MAP_BIOS & t_change);

    DWORD		GetScanTimePeriod();
    bool		GetDeviceInfo();

    void		FileterArp(PARP_DISCOVERY_INFO pArpInfo);
    BOOL        FileterNmap(std::wstring wszResult);

    bool		ReadCacheDevice(std::string& szProxyData);
    bool	    WriteCacheDevice(std::string szProxyData);

    std::string ToJson(MAP_BIOS	mapBios);
    bool		ToMap(std::string szJson, MAP_BIOS&	mapBios);

private:
    std::wstring m_dbPath;

private:
    DWORD		m_dwScanTimePeriod;
    std::string m_szOrgId;
    std::string m_szAreaId;

private:
    bool		m_bMainCtrl;
    ULONGLONG	m_TickCount;

private:
    MAP_BIOS			m_mapBios;

private:
    CArp			m_arp;
    MNetBios		m_mnb;
    CPing			m_ping;
    MCallClient		m_mcc;

protected:
    int  GetDeviceInfoByBus();

public:
    void GetScanDev(MAP_BIOS & bios);

private:
    DWORD	m_dwBegin;
    DWORD   m_dwEnd;
    NET_INFO m_NetInfo;
};

#endif // CPROXYSCAN_H
