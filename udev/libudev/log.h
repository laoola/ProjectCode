 #pragma once
 #include <iostream>

 namespace log
 {
    class log
    {
    public:
        log(const std::string& level) {std::cout << level;}
        ~log() {std::cout << std::endl;}
    public:
        template<typename T>
        const log& operator<<(const T& data) const
        {
            std::cout << data;
            return *this;
        }
    };
 } // namespace log

 #define LOG_DEBUG()    log::log("debug   : ")
 #define LOG_INFO()     log::log("info    : ")
 #define LOG_TRACE()    log::log("trace   : ")
 #define LOG_WARNING()  log::log("warning : ")
 #define LOG_ERROR()    log::log("error   : ")
 
