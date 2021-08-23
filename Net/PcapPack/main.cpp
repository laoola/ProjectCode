/*
 * @Descripttion: 
 * @version: 
 * @Author: 主机安全部 (zhu_ji_an_quan_bu@vrv.com)
 * @Date: 2021-06-25 10:03:47
 * @LastEditTime: 2021-06-25 10:27:29
 */
#include "CDealArpOpt.h"

DWORD fn_CallBack(PARP_DISCOVERY_INFO pArpInfo, void* pThis)
{

    return 0;
}

int main(int argc, char *argv[])
{
    CDealArpOpt caq; //arp 发现

    caq.StartMonitor();

    return 0;
}
