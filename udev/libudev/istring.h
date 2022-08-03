#pragma once
#ifndef __ISTRING_H__
#define __ISTRING_H__
#include <vector>
#include <string>
#include <sstream>

namespace utils
{
    class IStr
    {
    public:
        IStr(std::string s);
        IStr(const char* s);
        bool ends_with_digit();
        bool starts_with_digit();
        bool starts_with(std::string str);
        bool ends_with(std::string str);
        bool equal(std::string str); // 不区分大小写比较
        bool words(std::vector<std::string>& lst, char delim = ' '); // 默认空格分割
        bool lift(std::string& sub, std::string delim);
        bool right(std::string& sub, std::string delim);
        void upper(std::string& str);
        void lower(std::string& str);
        std::string replace_all(const std::string& old_value, const std::string& new_value);
        std::string remove_all( std::string remove_str);
        std::string data();
        std::string gbk2utf8();
    private:
        int gbk2utf8(char *utfStr,const char *srcStr,int maxUtfStrlen);
        bool split(std::string& sub, std::string delim, bool tlift = true);
        std::string m_str;
    };
}
#endif
