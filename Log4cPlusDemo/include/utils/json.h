#pragma once
#include <json/json.h>
#include <exception>

namespace utils
{
    namespace json
    {
        typedef Json::Value json_t;

        inline std::string ToString(json_t& value)
        {
            try
            {
                if (value.empty())
                {
                    return std::string();
                }
                return Json::FastWriter().write(value);
            }
            catch (std::exception& e)
            {
            }
            catch (...)
            {
            }
            return std::string();
        }

        inline json_t ToJson(std::string str)
        {
            try
            {
                json_t value;
                if (!Json::Reader().parse(str, value))
                {
                    value.clear();
                }
                return value;
            }
            catch (std::exception& e)
            {
            }
            catch (...)
            {
            }
            return json_t::null;
        }
    }
}
