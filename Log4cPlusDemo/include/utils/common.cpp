#include "common.h"
#include <algorithm>
#include <cstring>
#include <cctype>
#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <regex.h>
#include <string.h>

namespace utils
{
    /************************************************************
    * Author:       LiJianbo
    * Date:         2018-05-28
    * Description:  字符串转时间，默认格式为："2018-05-28 14:01:55"
    ************************************************************/
    std::time_t MakeTime(std::string str, const std::string& fmt)
    {
        struct tm local;
        strptime(str.c_str(), fmt.c_str(), &local);
        return std::mktime(&local);
    }

    /************************************************************
    * Author:       LiJianbo
    * Date:         2018-05-28
    * Description:  格式化时间，默认格式为："2018-05-28 14:01:55"
    ************************************************************/
    std::string FormatTime(std::time_t tm, const std::string& fmt)
    {
        std::string str = "";
        struct tm *local = std::localtime(&tm);
        char tm_str[32] = {0};
        if (std::strftime(tm_str, sizeof(tm_str), fmt.c_str(), local) != 0)
        {
            str = std::string(tm_str);
        }
        return str;
    }

    /************************************************************
    * Author:       LiJianbo
    * Date:         2018-05-28
    * Description:  格式化时间，格式为："2018-05-28 14:01:55"
    ************************************************************/
    std::string FormatTime(const std::string& str_time)
    {
        return FormatTime(convert_as<std::time_t>(str_time));
    }
    /************************************************************
    * Author:       zsg
    * Date:         2020-04-23
    * Description:  获取指令执行状态
    ************************************************************/
    int SysCmd(const std::string& cmd)
    {
        pid_t status;
        status = system(cmd.c_str());
        if (-1 == status) 
        {     
            return -1;
        }
        else 
        {
            if (WIFEXITED(status))
            {        
                if (0 == WEXITSTATUS(status))
                {              
                    return 0;
                }
                else
                {
                    return WEXITSTATUS(status);
                }
            }
            else
            {
                return WEXITSTATUS(status);
            }
        }
        return 0;
    }
    int get_ip_from_url(const char *url, char *ip)
    {
        int ret = 0, len = 0;
        regex_t oregex;
        const size_t nmatch = 1;
        regmatch_t pm;
        char ipurl[128] = {0};
        const char * pregexstr = "(^(ftp|http|https)://)?((25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])\\.){3}(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])";

        if(url == NULL || ip == NULL)
        {
            return -1;
        }
        if ((ret = regcomp(&oregex, pregexstr, REG_EXTENDED)) == 0) 
        {
            if ((ret = regexec(&oregex, url, nmatch, &pm, 0)) == 0)
            {
                len =pm.rm_eo-pm.rm_so;
                memcpy(ipurl, &url[pm.rm_so],len);
                if(ipurl[0]>='a' && ipurl[0]<='z')
                {
                    strcpy(ip, strstr(ipurl, "//")+2);
                }
                else
                {
                    strcpy(ip, ipurl);
                }
            }
            regfree(&oregex);
        }
        return ret;
    }
    int get_ip_from_domain(std::string hostname, std::string &ip)
    {
        int ret = -1;        
        struct addrinfo hints;
        struct addrinfo *res, *res_p;

        memset(&hints, 0, sizeof(struct addrinfo));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_CANONNAME;
        hints.ai_protocol = 0;
 
        ret = getaddrinfo(hostname.c_str(), NULL, &hints, &res);
        if(ret != 0)
        {
            return ret;
        }
        ret = 1;
        for(res_p = res; res_p != NULL; res_p = res_p->ai_next)
        {
            char host[1024] = {0};
            ret = getnameinfo(res_p->ai_addr, res_p->ai_addrlen, host, sizeof(host), NULL, 0, NI_NUMERICHOST);
            if(ret == 0)
            {
                ip = host;
                break;
            }
        }
 
        freeaddrinfo(res);
        return ret;
        
    }
    int hostname_to_ip(std::string hostname, std::string &ip)
    {
        int ret = -1;
        long netlong;
        char ipaddr[64] = {0};

        if(hostname.empty())
        {
            return 1;
        }
        netlong = inet_addr(hostname.c_str());
        if(netlong != INADDR_NONE)
        {
            ip = hostname;
            return 0;
        }
        ret = get_ip_from_url(hostname.c_str(), ipaddr);
        if(ret == 0)
        {
            ip = ipaddr;
            return 0;
        }
        ret = get_ip_from_domain(hostname, ip);
        return ret;

    }
    int get_ostype_from_file(const char *filename, char *osname)
    {
        char line[1024];
        FILE *fp;
        int ret = -1;
        
        fp = fopen(filename,"r");
        if(fp == NULL)
            return -1;
        while (fgets(line, 1023, fp))
        {
            if(strstr(line, "Ubuntu"))
            {
                strcpy(osname, "ubuntu");
                ret = 0;
            }
            else if(strstr(line, "CentOS"))
            {
                strcpy(osname, "centos");
                ret = 0;
            }
            else if(strstr(line, "Red Hat"))
            {
                strcpy(osname, "redhat");
                ret = 0;
            }
            else if(strstr(line, "openSUSE"))
            {
                strcpy(osname, "opensuse");
                ret = 0;
            }
            else if(strstr(line, "SUSE"))
            {
                strcpy(osname, "suse");
                ret = 0;
            }
            else if(strstr(line, "Fedora"))
            {
                strcpy(osname, "fedora");
                ret = 0;
            }
            else if(strstr(line, "Debian"))
            {
                strcpy(osname, "debian");
                ret = 0;
            }
            else if (strstr (line, "Uniontech") || strstr (line, "UnionTech")||strstr(line,"uos"))//UOS分为专业版和个人版,专业版使用uos来区分
            {
                strcpy (osname, "uniontech");
                ret = 0;
            }
            else if(strstr(line, "Kylin"))
            {
                strcpy(osname, "kylin");
                ret = 0;
            }
            if(ret == 0)
            {
                break;
            }
        }
        if(fp)
        {
            fclose(fp);
            fp = NULL;
        }
        return ret;
    }
    int get_os_type(char *osname)
    {
        int ret = -1;
        if(osname == NULL)
            return ret;
        ret = get_ostype_from_file("/etc/os-release", osname);
        if(ret == 0)
            return ret;
        ret = get_ostype_from_file("/etc/redhat-release", osname);
        if(ret == 0)
            return ret;
        ret = get_ostype_from_file("/etc/lsb-release", osname);
        if(ret == 0)
            return ret;
        ret = get_ostype_from_file("/etc/issue", osname);
        if(ret == 0)
            return ret;
        ret = get_ostype_from_file("/etc/fedora-release", osname);
        return ret;
    }
}
