/**
 * @file pcap_interface.cpp
 * @author MiYanHui (miyanhui@vrvmail.com.cn)
 * @brief pcap回调函数的相关实现
 * @version 0.1
 * @date 2021-05-20
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "pcap_interface.h"
#include <string>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
//#include <log.h>
namespace mech
{
    namespace pcap
    {


        #define MAXBYTES2CAPTURE 2048

        /**
         * @brief 数据包处理的线程函数,每个线程对应一个网卡
         *
         * @param pParam
         * @return void*
         */
        static void * collect_packet_thread(void * pParam)
        {
            NetPacket* pcurrent = (NetPacket*)  pParam;
            return pcurrent->collect_packet_data();
        }


        /**
         * 收集当前机器上的网卡信息
         *
         * BUG: 收集动作早于某个网卡启动时,则该网卡的信息没有被获取到,
         * FIX: 可以考虑通过在pcap_open_live函数中,将第一个参数配置为any来实现网卡中途启停不影响
         */
        bool NetPacket::init_pcap()
        {
            pcap_if_t *device = NULL;
            struct bpf_program filter;
            char errbuf[PCAP_ERRBUF_SIZE]= "";

            ///查找网卡
            if(-1 == pcap_findalldevs(&device, errbuf))
            {
                std::cout << "pcap_findalldevs error"<<std::endl;
                return false ;
            }

            ///没找到， 就没有执行的必要。
            if (!device)
            {
                std::cout << "not find nic device"<<std::endl;
                return false ;
            }
            ///存放地址
            bpf_u_int32 netaddr = 0;
            ///存放掩码
            bpf_u_int32	mask = 0;

            std::string szfilter =  "";

            while(device)
            {
                ///获取网卡信息
                std::cout << "find nicname = " << device->name<<std::endl;
                if(pcap_lookupnet(device->name, &netaddr, &mask, errbuf) == -1)
                {
                    std::string error = "pcap_lookupnet error #";
                    error = error + errbuf ;
                    std::cout << error<<std::endl;
                    device = device->next ;
                    continue ;
                }

                ///回环地址跳过
                if(strcmp(device->name,"lo") == 0 ||
                        strcmp(device->name,"any") == 0)
                {
                    device = device->next ;
                    continue ;
                }

                std::cout << "获取网卡信息成功 name =" << device->name;

                struct nic_collect_thread item;
                item.pdescr = pcap_open_live(device->name, MAXBYTES2CAPTURE, 0, 512, errbuf);
                if(item.pdescr == NULL) {
                    std::cout << "打开网卡信息失败 name = " << device->name << ", err = " << errbuf<<std::endl;
                    device = device->next ;
                    continue ;
                }

                ///设置FILTER
                if(-1 == pcap_compile(item.pdescr, &filter, "arp", 0, mask)) {
                    std::cout << "设置过滤信息失败 name = " << device->name<<std::endl;
                    device = device->next ;
                    continue ;
                }
                pcap_setfilter(item.pdescr, &filter);

                item.name = device->name;
                ///申请包存放初始化空间
                if (m_nic_pack.find(item.name) == m_nic_pack.end())
                {
                    item.bruning = false;
                    m_nic_pack[item.name] = item;
                }
                else
                {
                    m_nic_pack[item.name].pdescr = item.pdescr;
                }

                device = device->next;
            }
            ///释放网卡信息内存空间
            pcap_freealldevs(device);

            std::cout << "pcap init finish"<<std::endl;
            return  true ;
        }

        /**
         * @brief 卸载pcap上的相关函数
         *
         * @return true 成功
         * @return false 失败
         */
        bool NetPacket::uninit_pcap()
        {
            m_nic_pack.clear();
            m_callback_function = NULL;
            return true;
        }

        /**
         *  给每一块网卡起一个线程。
         *  收集网卡的包数据
         */
        bool NetPacket::start_collect_packet_thread()
        {
            std::map<std::string, struct nic_collect_thread>::iterator iter;
            for (iter = m_nic_pack.begin(); iter != m_nic_pack.end(); iter++)
            {
                if (iter->second.bruning)
                {
                    std::cout << "nic = " << iter->first << " working"<<std::endl;
                    continue;
                }

                // 起线程 开始干活
                iter->second.bruning = true;
                int ret = pthread_create(&iter->second.trdid, NULL, collect_packet_thread, this);
                if (ret)
                {
                    std::cout << "pthread_create error = " << ret<<std::endl;
                    continue;
                }
                std::cout << "pthread_create success,nic name =" << iter->second.name << ", tid = " << iter->second.trdid<<std::endl;
            }
            return true;
        }

        /**
         * 停止网卡上的检测线程
         *
         */
        bool NetPacket::stop_collect_packet_thread()
        {
            std::map<std::string, struct nic_collect_thread>::iterator iter;
            for (iter = m_nic_pack.begin(); iter != m_nic_pack.end(); iter++)
            {
                if (iter->second.bruning)
                {
                    iter->second.bruning = false;
                }
                pthread_join(iter->second.trdid, NULL);
                iter->second.trdid = 0;
                //pdescr ??
            }
            std::cout << "stop_collect_packet_thread"<<std::endl;
            return true;
        }

        /**
         * 检测网卡数据包的线程
         */
        void* NetPacket::collect_packet_data()
        {
            pthread_t  tid = pthread_self();
            std::map<std::string, struct nic_collect_thread>::const_iterator iter;
            for (iter = m_nic_pack.begin(); iter != m_nic_pack.end(); iter++)
            {
                if (iter->second.trdid == tid)
                {
                    break;
                }
            }

            if (iter == m_nic_pack.end())
            {
                std::cout << "not found tid = " << tid << " (struct nic_collect_thread)"<<std::endl;
                return NULL;
            }


            struct pcap_pkthdr    *   pkt_header = NULL ;
            const  unsigned char  *   pkt_data   = NULL ;
            const struct nic_collect_thread& nic_thread = iter->second;
            while (nic_thread.bruning)
            {
                pkt_data = pcap_next(nic_thread.pdescr, pkt_header) ;
                if(NULL == pkt_data)
                {
                    std::cout << "nic name = " << nic_thread.name << ", pcap_next_ex return error " <<std::endl;
                    break;
                }
                ///进行过滤
                if (m_callback_function)
                    m_callback_function(pkt_header, pkt_data);
            }
            std::cout << "nic name = " << nic_thread.name << ", collect_packet_data thread tid = " << tid << " exit"<<std::endl;
            return NULL;
        }

    }

}
