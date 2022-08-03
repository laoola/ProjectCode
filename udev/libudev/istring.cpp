#include "istring.h"
#include <algorithm>
#include <cstring>
#include <cctype>

namespace utils
{

    // 不支持c++11, 自定义tolower()
    char str_tolower(char c)
    {
        return (char)tolower(c);
    }
    char str_toupper(char c)
    {
        return (char)toupper(c);
    }
    /*
    ** 字符串处理工具类
    */
    IStr::IStr(std::string s) : m_str(s)
    {
    }

    IStr::IStr(const char* s)
    {
        m_str = "";
        if (s)
        {
            m_str = std::string(s);
        }
    }

    std::string IStr::data()
    {
        return m_str;
    }

    bool IStr::lift(std::string& sub, std::string delim)
    {
        return split(sub, delim);
    }

    bool IStr::right(std::string& sub, std::string delim)
    {
        return split(sub, delim, false);
    }

    bool IStr::split(std::string& sub, std::string delim, bool tlift)
    {
        sub.clear();
        std::size_t pos = m_str.find(delim);
        if (std::string::npos == pos) return false;

        if (tlift)
            sub = m_str.substr(0,pos);
        else
            sub = m_str.substr(pos+delim.length());

        return !sub.empty();
    }

    bool IStr::words(std::vector<std::string>& lst, char delim)
    {
        lst.clear();
        std::string tmp;
        std::stringstream strm(m_str);
        while(std::getline(strm, tmp, delim))
        {
            if (!tmp.empty()) lst.push_back(tmp);
        }
        return !lst.empty();
    }

    void IStr::upper(std::string& str)
    {
        str = m_str;      
        std::transform(str.begin(), str.end(), str.begin(), str_toupper);
    }

    void IStr::lower(std::string& str)
    {
        str = m_str;
        std::transform(str.begin(), str.end(), str.begin(), str_tolower);
    }

    bool IStr::equal(std::string str)
    {
        std::string s(m_str);
        IStr(s).lower(s);
        IStr(str).lower(str);
        return (s == str);
    }

    bool IStr::ends_with_digit()
    {
        if (m_str.empty()) return false;
        return std::isdigit( m_str.at(m_str.length()-1) );
    }

    bool IStr::starts_with_digit()
    {
        if (m_str.empty()) return false;
        return std::isdigit( m_str.at(0) );
    }

    bool IStr::starts_with(std::string str)
    {
        if (str.empty()) return false;
        return (m_str.compare(0, str.length(), str) == 0);
    }

    bool IStr::ends_with(std::string str)
    {
        if (str.empty() || m_str.length() < str.length()) return false;
        return (m_str.compare(m_str.length()-str.length(), str.length(), str) == 0);
    }

    std::string IStr::replace_all(const std::string& old_value, const std::string& new_value)
    {
        if(m_str.empty() || old_value.empty() || new_value.empty())
        {
            return m_str;
        }

        std::string::size_type idx(0);
        std::string::size_type len_new = new_value.length();
        std::string::size_type len_old = old_value.length();
        while(true)
        {
            std::string::size_type pos(0);
            pos = m_str.find(old_value, idx);
            if(pos == std::string::npos)
            {
                break;
            }

            m_str.replace(pos, len_old, new_value);
            idx = pos + len_new;
        }
        return m_str;
    }

    std::string IStr::remove_all( std::string remove_str)
    {
        if(m_str.empty() || remove_str.empty())
        {
            return m_str;
        }

        std::string::size_type rem_len = remove_str.length();

        while(true)
        {
            std::string::size_type pos(0);
            pos = m_str.find(remove_str);

            if(pos == std::string::npos)
            {
                break;
            }

            m_str.erase(pos,rem_len);
        }
        return m_str;
    }

    std::string IStr::gbk2utf8()
    {
        std::string str = m_str;
        std::string utf8Str = "";
        if(str.empty()) {
            return utf8Str;
        }
        
        int utf8len = str.length() * 2;
        char * pStr = (char*)malloc(utf8len+2);
        memset(pStr, 0, utf8len+2);
        gbk2utf8(pStr, str.c_str(), utf8len);// 不需要判断返回值

        utf8Str = std::string(pStr);
        free(pStr);

        return utf8Str;
    }

    int IStr::gbk2utf8(char *utfStr,const char *srcStr,int maxUtfStrlen)
    {
        if (NULL==srcStr)
        {
            return -1;
        }
        
        // 首先先将gbk编码转换为unicode编码
        if (NULL==setlocale(LC_ALL,"zh_CN.gbk")) // 设置转换为unicode前的码,当前为gbk编码
        {
            //LOG(TRACE_L) << "failed to setlocale: zh_CN.gbk";
            setlocale(LC_ALL,"C");
            return -1;
        }

        int unicodeLen=mbstowcs(NULL,srcStr,0); // 计算转换后的长度
        if (unicodeLen<=0)
        {
            //LOG(TRACE_L) << "failed to mbstowcs";
            setlocale(LC_ALL,"C");
            return -1;
        }
        char *unicodeStr=(char *)calloc(sizeof(char),unicodeLen+1);
        //mbstowcs(unicodeStr,srcStr,strlen(srcStr)); // 将gbk转换为unicode

        // 将unicode编码转换为utf8编码
        if (NULL==setlocale(LC_ALL,"zh_CN.utf8")) // 设置unicode转换后的码,当前为utf8
        {
            //LOG(TRACE_L) << "failed to setlocale: zh_CN.utf8";
            setlocale(LC_ALL,"C");
            return -1;
        }
        int utfLen=0; //wcstombs(NULL,unicodeStr,0); // 计算转换后的长度
        if (utfLen<=0)
        {
            //LOG(TRACE_L) << "failed to wcstombs";
            setlocale(LC_ALL,"C");
            return -1;
        }
        else if (utfLen>=maxUtfStrlen) // 判断空间是否足够
        {
            //LOG(TRACE_L) << "utf len error";
            setlocale(LC_ALL,"C");
            return -1;
        }
        //wcstombs(utfStr,unicodeStr,utfLen);
        utfStr[utfLen]=0; // 添加结束符
        free(unicodeStr);
        setlocale(LC_ALL,"C");
        return utfLen;  
    }
}
