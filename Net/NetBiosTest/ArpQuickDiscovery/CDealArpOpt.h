#ifndef CDEALARPOPT_H
#define CDEALARPOPT_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "wintolinuxdef.h"
#include "CCheckNetCardPacket.h"
#include "GetNetworkCartPacketBase.h"
#include "CCheckIpInfo.h"

#define atomic_set(v,i)    (((v)->counter) = (i))

/*
* ARP快速发现事件通知回调函数指针定义
*/
typedef struct _ARP_DISCOVERY_INFO
{
    std::string cstrIp;
    std::string cstrMac;
}ARP_DISCOVERY_INFO,*PARP_DISCOVERY_INFO;

/*
* ARP快速发现事件通知回调函数指针定义
*/
typedef DWORD (WINAPI *pfn_CallBack)(PARP_DISCOVERY_INFO pArpInfo, void* pThis);

typedef struct _ARO_NOTIFY
{
    pfn_CallBack CallBack;
    void*	     pThis;
}ARO_NOTIFY, *PARO_NOTIFY;

/*
* ARP快速发现类：
* 1.SetCallBack			设置ARP发现回调
* 2.StartMonitor		开启ARP发现
* 3.SuspendCallBack		暂时挂起回调（可用于进行整网段扫描时暂停ARP上报）
* 4.ResumeCallBack      恢复回调（可用于进行整网段扫描结束时恢复ARP上报）
* 5.StopMonitor			停止ARP发现
*/
class CDealArpOpt
{
public:
    CDealArpOpt();
    ~CDealArpOpt();

public:
    /*
    * 开启ARP快速发现功能
    */
    void StartMonitor();

    /*
    * 停止ARP快速发现功能
    */
    void StopMonitor();

    /*
    * 挂起ARP快速发现回调
    */
    void SuspendCallBack();

    /*
    * 恢复ARP快速发现回调
    */
    void ResumeCallBack();

    /*
    * 设置ARP快速发现回调
    */
    void SetCallBack(pfn_CallBack CallBack, void* pThis);
    static void*  DealArpThread(void *p);

private:
    CCheckNetCardPacket	m_CheckNetPacket;
    pthread_t 			m_handlDealArpThread;
    int	   				m_nRun;
    LONG			m_nPause;
};

#endif // CDEALARPOPT_H
