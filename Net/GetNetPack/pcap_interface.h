/**
 * @file pcap_interface.h
 * @author MiYanHui (miyanhui@vrvmail.com.cn)
 * @brief pcap回调的相关类
 * @version 0.1
 * @date 2021-05-20
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once 
#include <iostream>
#include <map>
#include <pcap/pcap.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>
#include <netinet/tcp.h>
#include "Inc/GetNetworkCartPacketBase.h"

namespace mech
{
    namespace pcap
    {

        typedef int (* pfunc_check_callback)(struct pcap_pkthdr* , const  unsigned char* );
        
        
        /*pcap分发的数据*/
        struct pcap_packet_data
        {
            unsigned char* ptkhead;
            unsigned char* pdata;
            pcap_packet_data() : ptkhead(NULL), pdata(NULL) {}
        };
        

        /**
         *  收集网卡数据包
         */
        struct nic_collect_thread
        {
            std::string         name;//网卡名称
            pcap_t*             pdescr ; //pcap 库
            pthread_t           trdid;//监控线程线程id
            volatile     bool   bruning;//是否在运行的标识
            nic_collect_thread(): pdescr(NULL), trdid(0), bruning(false)
            {};
            nic_collect_thread(const struct nic_collect_thread& obj): 
            pdescr(obj.pdescr), trdid(obj.trdid), bruning(obj.bruning)//, check_list(obj.check_list)
            {
                
            }
        };
        
        class NetPacket
        {
        public:
            NetPacket(pfunc_check_callback callback) :  m_callback_function(callback) {};
            bool init_pcap();
            bool uninit_pcap();
            bool start_collect_packet_thread();
            bool stop_collect_packet_thread();
            void* collect_packet_data();
        private:
            pfunc_check_callback m_callback_function; //回调函数
            std::map<std::string, struct nic_collect_thread> m_nic_pack;
        };
    }
}
