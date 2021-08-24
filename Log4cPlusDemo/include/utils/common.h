#pragma once
#include "istring.h"
#include <map>
#include <set>
#include <ctime>

namespace utils
{
    template<typename t_dst, typename t_src>
    inline t_dst convert_as(const t_src & src)
    {
        t_dst dst;
        std::stringstream strm;
        strm << src;
        strm >> dst;
        return dst;
    }

    /// 格式化时间 2018-05-28 14:01:55
    std::time_t MakeTime(std::string str, const std::string& fmt = "%F %T");  // 字符串转时间
    std::string FormatTime(std::time_t tm, const std::string& fmt = "%F %T"); // 时间转字符串
    std::string FormatTime(const std::string& str_time); // 数字字符串转时间，采用默认格式


    /************************************************************
    * Author:       Yang Xiaoyu
    * Date:         2018-05-11
    ************************************************************/
    template<typename N, typename M>
    int map_difference(std::map<N, M> &cached, std::map<N, M> &updated, std::set<N> &keys)
    {
        typename std::map<N, M>::iterator cached_it, updated_it;
        cached_it = cached.begin();
        updated_it = updated.begin();
        while (cached_it != cached.end() && updated_it != updated.end())
        {
            if (cached_it->first < updated_it->first)
            {
                keys.insert(cached_it->first);
                ++cached_it;
            }
            else if (cached_it->first > updated_it->first)
            {
                ++updated_it;
            }
            else
            {
                ++cached_it; 
                ++updated_it;
            }
        }
        while(cached_it != cached.end())
        {
            keys.insert(cached_it->first);
            cached_it++;
        }
        return 0;
    }
    int SysCmd(const std::string& cmd); //系统指令执行
    int hostname_to_ip(std::string hostname, std::string &ip); //域名到ip的转换
    int get_os_type(char *osname);
}
