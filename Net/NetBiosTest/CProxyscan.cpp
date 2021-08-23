#include "CProxyscan.h"
#include "CDbopt.h"


#define		OPEN_WAIT_TIME			60*1000	  // 10 min

#define		FUN_CODE_SCANRESULT		2562
#define		SCAN_SERVER_FLAG        "ScanServerPacket"

#define     BOOT_TIMEOUT			300   //5min
#define CEMS_OK 0
#define CEMS_ERROR -1

CProxyScan::CProxyScan():m_dwBegin(0), m_dwEnd(0)
{
    GetDeviceInfo();
}

CProxyScan::~CProxyScan()
{

}

void *WorkProc(void *pParam)
{
    CProxyScan *pScan = static_cast<CProxyScan *>(pParam);

    if(pScan)
    {
        pScan->Work();
    }

    return NULL;
}

bool CProxyScan::ScanStart()
{
    pthread_t threadId;
    pthread_create(&threadId, NULL, WorkProc, this);

    pthread_join(threadId, 0);

    return true;
}

void CProxyScan::ScanStop()
{

}

DWORD  WINAPI fn_CallBack(PARP_DISCOVERY_INFO pArpInfo, void* pThis)
{
    CProxyScan* pScan = static_cast<CProxyScan*>(pThis);
    if(pScan)
    {
        pScan->FileterArp(pArpInfo);
    }
    return 0;
}

void CProxyScan::Work()
{
    CArp::GetIPMAC(m_NetInfo);
    CArp::GetIpRanage(m_NetInfo, m_dwBegin, m_dwEnd);

    CDealArpOpt caq; //arp 发现

    caq.SetCallBack(fn_CallBack, this);
    caq.StartMonitor();

    m_mapBios.clear();

    while(true)
    {
        Scan();

        sleep(15);
    }
}

void CProxyScan::Scan()
{
    DWORD LowAddr;
    DWORD HighAddr;

    DWORD dwTick = CPing::GetSysRunTime(false);

    if(CArp::GetIpRanage(m_NetInfo, LowAddr, HighAddr))
    {
        m_dwBegin = LowAddr;
        m_dwEnd = HighAddr;

        ScanClient(LowAddr, HighAddr);
    }

    printf("scan use %lu ms\n", CPing::GetSysRunTime(false) - dwTick);
}

void CProxyScan::ScanClient(DWORD &dwBegin, DWORD &dwEnd)
{
    //扫描注册
    VECTOR_RESULT	vResult;
    MAP_BIOS		mRegist;

    if(m_mcc.Call(dwBegin, dwEnd))
    {
        m_mcc.GetResult(vResult);

        JsonToStruct(vResult, mRegist);
    }

    //扫描非注册
    MAP_ARP		t_arp;
    MAP_BIOS	t_bios;
    MAP_PING	t_ping;

    m_mnb.MNetBiosScan(dwBegin, dwEnd);
    m_mnb.NetBiosResult(t_bios);

    //先清除 arp 缓存
    m_arp.MFlushArpCache();

    m_ping.PingFast(dwBegin, dwEnd);
    m_ping.PingResult(t_ping);

    m_arp.MSendArpByCache(dwBegin, dwEnd); //读取 arp 缓存
    m_arp.ArpResult(t_arp);

    MAP_ARP::iterator iterArp;
    iterArp = t_arp.begin();

    //判断防火墙状态  依据：arp有回包，ping不通。
    MAP_ARP Arp_FireWall;
    while(iterArp != t_arp.end())
    {
        BOOL bFind = FALSE;
        if(t_ping.find(iterArp->second) != t_ping.end())
        {
            bFind = TRUE;
        }

        if(!bFind)
        {
            Arp_FireWall.insert(MAP_ARP::value_type(iterArp->first, iterArp->second));
        }

        iterArp ++;
    }

    //合并 arp扫描、 nbt扫描结果
    iterArp = t_arp.begin();
    MAP_BIOS::iterator iterBios;
    while(iterArp != t_arp.end())
    {
        iterBios = t_bios.find(iterArp->first);
        if(iterBios == t_bios.end())
        {
            DEV_INFO d_i;
            d_i.szMac = iterArp->first;
            d_i.szIpAddress  = iterArp->second;
            t_bios.insert(MAP_BIOS::value_type(iterArp->first, d_i));
        }
        iterArp ++;
    }

    iterBios = t_bios.begin();
    MAP_BIOS::iterator itRegist;

    time_t tNow = time(NULL);

    while(iterBios != t_bios.end())
    {
        iterBios->second.szFireWall = "2"; //默认防火墙状态未知
        iterBios->second.szBoot = "1";	   //开机状态

        iterBios->second.szAreaId = m_szAreaId;    //本机所属组织机构id
        iterBios->second.szOrgId = m_szOrgId;      //本机所属区域id
        iterBios->second.szRegAreaId = m_szAreaId;
        iterBios->second.szRegOrgId = m_szOrgId;
        iterBios->second.scantime = tNow;

        //更新注册主机
        if((itRegist = mRegist.find(iterBios->first)) != mRegist.end())
        {
            iterBios->second = itRegist->second;
            iterBios->second.scantime = tNow;
        }

        //添加防火墙标记
        if(Arp_FireWall.find(iterBios->first) != Arp_FireWall.end())
        {
            iterBios->second.szFireWall = "1";
        }

        iterBios->second.szBoot = "1";
        iterBios ++;
    }

    MAP_BIOS t_change;

    if(Filter(t_bios, t_change))
    {
        if(t_change.size() > 0)
        {
            if(!UpScanInfo(t_change))
            {
                m_mapBios.clear();

                std::string szJson;
                if(ReadCacheDevice(szJson))
                {
                    printf("Filter->ReadCacheDevice suc\n");
                    ToMap(szJson, m_mapBios);
                }

                printf("上报结果失败\n");
            }
            else
            {
                std::string szJson;
                szJson = ToJson(m_mapBios);

                BOOL bWrite = WriteCacheDevice(szJson);
                if(!bWrite)
                {
                    printf("WriteCacheDevice fail");
                }
                printf("上报结果成功\n");
            }
        }
    }
}

void CProxyScan::JsonToStruct(VECTOR_RESULT &result, MAP_BIOS &bios)
{
    Json::Value  root;
    Json::Reader reader;

    for(size_t i = 0; i < result.size(); i++)
    {
        std::string szTemp = result.at(i);
        if(!reader.parse(szTemp, root))
        {
            continue;
        }

        DEV_INFO info;
        info.szBoot			= root["isOpened"].isNull() ? "1" : root["isOpened"].asString();
        info.szIpAddress	= root["ip"].isNull() ?  "" : root["ip"].asString();
        info.szMac			= root["mac"].isNull() ? "" : root["mac"].asString() ;
        info.szGroupName	= root["groupName"].isNull() ? "" :  root["groupName"].asString() ;
        info.szHostName		= root["hostName"].isNull() ? "" : root["hostName"].asString();
        info.szDevId		= root["devOnlyId"].isNull() ?  "" : root["devOnlyId"].asString();
        info.szRegOrgId		= root["orgId"].isNull() ?  "" : root["orgId"].asString();
        info.szRegAreaId	= root["areaId"].isNull() ? "" : root["areaId"].asString();
        info.szAreaId		= m_szAreaId;
        info.szOrgId		= m_szOrgId;
        info.szFireWall     = "2";

        /*MBC_DBG_OUTPUTA(CEMS_FUN_SCAN_OID, VDBG_LEVEL_DEBUG, "注册主机: ip = %s, mac = %s, devOnlyId = %s, regAreaId = %s, regOrgId = %s\n",
                    info.szIpAddress.c_str(),
                    info.szMac.c_str(),
                    info.szDevId.c_str(),
                    info.szRegAreaId.c_str(),
                    info.szRegOrgId.c_str());*/

        if(!info.szMac.empty())
        {
            bios.insert(MAP_BIOS::value_type(info.szMac, info));
        }
    }
}

bool CProxyScan::Filter(MAP_BIOS &t_bios, MAP_BIOS &t_change)
{
    if(m_mapBios.size() == 0)
    {
        m_mapBios = t_bios;
        t_change.swap(t_bios);
        return true;
    }

    MAP_BIOS::iterator itNew;
    MAP_BIOS::iterator itOld = m_mapBios.begin();

    while(itOld != m_mapBios.end())
    {
        std::string szSrc;
        itNew = t_bios.find(itOld->first);
        if(itNew !=  t_bios.end())
        {
            //设备变化
            if(itNew->second.szBoot.compare(itOld->second.szBoot) != 0 ||			//开机状态改变
                    itNew->second.szFireWall.compare(itOld->second.szFireWall) != 0)    //防火墙状态改变
            {
                itOld->second = itNew->second;
                t_change.insert(MAP_BIOS::value_type(itOld->first, itOld->second));
            }

        }
        else
        {
            time_t tNow = time(NULL);

            //关机设备
         //   printf("关机设备 : %s\n", itOld->second.szMac.c_str());
            if(itOld->second.szBoot.compare("0") != 0 &&
                    (tNow > (itOld->second.scantime + BOOT_TIMEOUT))
                    )
            {
                printf("now = %I64d, scan = %I64d, timeout = %d",
                             tNow, itOld->second.scantime, BOOT_TIMEOUT);

                itOld->second.szBoot = "0";
                t_change.insert(MAP_BIOS::value_type(itOld->first, itOld->second));
            }
        }
        itOld ++;
    }

    itNew = t_bios.begin();
    while(itNew != t_bios.end())
    {
        itOld = m_mapBios.find(itNew->first);
        if(itOld == m_mapBios.end())
        {
            //发现设备
         //   printf("发现设备 : %s\n", itNew->second.szMac.c_str());
            m_mapBios.insert(MAP_BIOS::value_type(itNew->first, itNew->second));
            t_change.insert(MAP_BIOS::value_type(itNew->first, itNew->second));
        }

        itNew++;
    }

    return (bool)t_change.size();
}

bool CProxyScan::FilterEx(MAP_BIOS &t_bios, MAP_BIOS &t_change)
{

}

string CProxyScan::GetStrMd5(char *szSrc, DWORD dwLen)
{

}

bool CProxyScan::ProcessUdp(string szUdp, CEMS_NET_HEAD *pHead)
{

}

bool CProxyScan::UpScanInfo(MAP_BIOS &t_change)
{
    string strReslut = ToJson(t_change);

    printf("json date: %s\n", strReslut.c_str());

//    for(MAP_BIOS::iterator iter = t_change.begin(); iter != t_change.end(); ++iter)
//    {
//        printf("mac %s\n", iter->second.szMac.c_str());
//        printf("ip %s\n", iter->second.szIpAddress.c_str());
//        printf("hostName %s\n", iter->second.szHostName.c_str());
//        printf("groupName %s\n", iter->second.szGroupName.c_str());
//        printf("devId %s\n", iter->second.szDevId.c_str());
//        printf("boot %s\n", iter->second.szBoot.c_str());
//        printf("fireWall %s\n", iter->second.szFireWall.c_str());
//        printf("areaId %s\n", iter->second.szAreaId.c_str());
//        printf("regAreaId %s\n", iter->second.szRegAreaId.c_str());
//        printf("orgId %s\n", iter->second.szOrgId.c_str());
//        printf("regOrgId %s\n\n", iter->second.szRegOrgId.c_str());
//    }

    printf("this scan up size %d\n", t_change.size());

    return true;
}

DWORD CProxyScan::GetScanTimePeriod()
{

}

bool CProxyScan::GetDeviceInfo()
{
    bool bRet = false;
    HANDLE hHandle = NULL;
    CDbOpt *pDbOpt = CDbOpt::GetDbObj();
    string strDbPath = QueryConfigValue("/etc/cems/vpengine.conf", "install path").c_str();

    if(strDbPath.empty())
        strDbPath = "../";

    strDbPath += "emsstore.dat";

    if(NULL != pDbOpt)
    {
        hHandle = pDbOpt->DB_SQLOpen(strDbPath.c_str(), NULL);

        if( NULL != hHandle )
        {
            IDBRecord* pDbRecord = NULL;
            CDbOpt *pDbOpt = CDbOpt::GetDbObj();

            if(NULL  == pDbOpt)
            {
                printf("load libemsstore.so Fail ");

                return false;
            }

            pDbOpt->DB_NewDbRecord(&pDbRecord);
            if( NULL != pDbRecord )
            {
                LONG lRet = pDbOpt->DB_SQLRead(hHandle, "select *from CEMS_DEVICE", NULL,0, pDbRecord, MBC_DB_DEFAULT_TIMEOUT) ;
                if( 0 == lRet )
                {
                    IDBItem* pItem = pDbRecord->GetFirstItem();
                    if( pItem )
                    {
                        MBC_DB_PARAM_TYPE dbValue;
                        string strOrgCode;
                        string strOrganizationId;

                        if( pItem->GetValue( "OrgCode", dbValue ) )
                        {
                            strOrgCode = dbValue.CharValue;
                        }

                        if( pItem->GetValue( "OrganizationId", dbValue ) )
                        {
                            strOrganizationId = dbValue.CharValue;
                        }

                        m_szAreaId = strOrgCode;
                        m_szOrgId = strOrganizationId;
                        bRet = true;
                    }
                }
                else
                {
                    printf("SQLDb Read error !");
                    bRet = false;
                }
            }
            else
            {
                printf("SqlDb NewDbRecord errot !");
                bRet = false;
            }

            if( NULL != pDbRecord )
            {
                pDbOpt->DB_FreeDbRecord(pDbRecord);
            }


            pDbOpt->DB_SQLClose( hHandle );
            hHandle = NULL;
        }
        else
        {
            printf("SQL DB open error : %s", strDbPath.c_str());
        }
    }

    return true;
}

void CProxyScan::FileterArp(PARP_DISCOVERY_INFO pArpInfo)
{
    MAP_BIOS t_change;
    MAP_BIOS t_bios;

    string szAip;
    string szAmac;

    szAip = pArpInfo->cstrIp;
    szAmac = pArpInfo->cstrMac;

    //  MBC_DBG_OUTPUTA(CEMS_MANAGER_OID, VDBG_LEVEL_DEBUG, "CALL BACK ip = %s, mac = %s, begin =%lu, end = %lu", (LPCSTR)szAip, (LPCSTR)szAmac, m_dwBegin, m_dwEnd);

    DWORD dwCurIp = ntohl(inet_addr(szAip.c_str()));

    if(dwCurIp > m_dwEnd || dwCurIp < m_dwBegin || m_dwBegin == 0 || m_dwEnd == 0)
    {
        ///      MBC_DBG_OUTPUTA(CEMS_MANAGER_OID, VDBG_LEVEL_DEBUG, "非本网段ip, 返回");
        return;
    }


    DEV_INFO dev;
    dev.szBoot = "1";
    dev.szMac = szAmac;
    dev.szIpAddress = szAip;
    dev.szAreaId = m_szAreaId;				//本机所属组织机构id
    dev.szOrgId = m_szOrgId;				//本机所属区域id
    dev.szRegAreaId = m_szAreaId;
    dev.szRegOrgId = m_szOrgId;
    dev.scantime = time(NULL);

    dev.szFireWall = "2";

    t_bios.insert(MAP_BIOS::value_type(szAmac, dev));

    if(FilterEx(t_bios, t_change))
    {
        if(t_change.size() > 0)
        {
            if(!UpScanInfo(t_change))
            {
                m_mapBios.clear();

                std::string szJson;
                if(ReadCacheDevice(szJson))
                {
                    // MBC_DBG_OUTPUTA(CEMS_FUN_SCAN_OID, VDBG_LEVEL_DEBUG,"FilterEx->ReadCacheDevice suc");
                    ToMap(szJson, m_mapBios);
                }
            }
            else
            {
                std::string szJson;
                szJson = ToJson(m_mapBios);

                BOOL bWrite = WriteCacheDevice(szJson);
                if(!bWrite)
                {
                    // MBC_DBG_OUTPUTA(CEMS_FUN_SCAN_OID, VDBG_LEVEL_DEBUG,"WriteCacheDevice fail");
                }
            }

            printf("this is get net card packet.\n");
        }
    }
}

BOOL CProxyScan::FileterNmap(wstring wszResult)
{

}

bool CProxyScan::ReadCacheDevice(string &szProxyData)
{
    return true;
}

bool CProxyScan::WriteCacheDevice(string szProxyData)
{

    return true;
}

string CProxyScan::ToJson(MAP_BIOS mapBios)
{
    std::string szJson;

    Json::Value root;
    Json::FastWriter writer;

    MAP_BIOS::iterator it = mapBios.begin();
    while(it != mapBios.end())
    {
        Json::Value jdev;
        jdev["mac"] = it->second.szMac;
        jdev["ip"] = it->second.szIpAddress;
        jdev["hostName"] = it->second.szHostName;
        jdev["groupName"] = it->second.szGroupName;
        jdev["devId"] = it->second.szDevId;
        jdev["boot"] = it->second.szBoot;
        jdev["fireWall"] = it->second.szFireWall;
        jdev["areaId"] = it->second.szAreaId;
        jdev["regAreaId"] = it->second.szRegAreaId;
        jdev["orgId"] = it->second.szOrgId;
        jdev["regOrgId"] = it->second.szRegOrgId;
        root.append(jdev);

        it ++;
    }

    szJson = writer.write(root);

    return szJson;
}

bool CProxyScan::ToMap(string szJson, MAP_BIOS &mapBios)
{

}

int CProxyScan::GetDeviceInfoByBus()
{

}
