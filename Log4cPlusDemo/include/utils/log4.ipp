#pragma once
#include "vfile.h"
#include "json.h"
#include "istring.h"
#include <iostream>
#include <sstream>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/logger.h>
#include <log4cplus/clogger.h>
#include <log4cplus/configurator.h>
#include <log4cplus/helpers/stringhelper.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/consoleappender.h>

#ifdef LOG_MODULE
#   define MODULE_NAME LOG_MODULE
#else
#   define MODULE_NAME "" /// δ����ģ����,��Ĭ��Ϊroot
#endif

#define MYLOG4_LOG4CPLUS_MACRO_STR_BODY(logger, logEvent, logLevel, file, line, fun)    \
    LOG4CPLUS_SUPPRESS_DOWHILE_WARNING()                                            \
    do {                                                                            \
    log4cplus::Logger const & _l                                                    \
    = log4cplus::detail::macros_get_logger (logger);                                \
    if (LOG4CPLUS_MACRO_LOGLEVEL_PRED (                                             \
    _l.isEnabledFor (log4cplus::logLevel), logLevel)) {                             \
    log4cplus::detail::macro_forced_log (_l,                                        \
    log4cplus::logLevel, logEvent,                                                  \
    file, line, fun);                                                               \
        }                                                                           \
    } while(0)                                                                      \
    LOG4CPLUS_RESTORE_DOWHILE_WARNING()

#define MYLOG4_TRACE(logger, logEvent, file, line, fun)  MYLOG4_LOG4CPLUS_MACRO_STR_BODY(logger, logEvent, TRACE_LOG_LEVEL, file, line, fun)
#define MYLOG4_DEBUG(logger, logEvent, file, line, fun)  MYLOG4_LOG4CPLUS_MACRO_STR_BODY(logger, logEvent, DEBUG_LOG_LEVEL, file, line, fun)
#define MYLOG4_INFO(logger, logEvent, file, line, fun)  MYLOG4_LOG4CPLUS_MACRO_STR_BODY(logger, logEvent, INFO_LOG_LEVEL, file, line, fun)
#define MYLOG4_WARN(logger, logEvent, file, line, fun)  MYLOG4_LOG4CPLUS_MACRO_STR_BODY(logger, logEvent, WARN_LOG_LEVEL, file, line, fun)
#define MYLOG4_ERROR(logger, logEvent, file, line, fun)  MYLOG4_LOG4CPLUS_MACRO_STR_BODY(logger, logEvent, ERROR_LOG_LEVEL, file, line, fun)
#define MYLOG4_FATAL(logger, logEvent, file, line, fun)  MYLOG4_LOG4CPLUS_MACRO_STR_BODY(logger, logEvent, FATAL_LOG_LEVEL, file, line, fun)



namespace utils
{
/*    enum LogLevel
    {
        TRACE,
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL
    }; */

    struct LogMessage
    {
        LogMessage(const char* logger, const char* file, int line, int level, const char* fun=0)
            :m_file(file), m_line(line), m_fun(fun), m_level(level), m_logger(logger)
        {
        }

        ~LogMessage()
        {
            switch(m_level)
            {
            case L4CP_DEBUG_LOG_LEVEL:
                MYLOG4_DEBUG(m_logger, m_stream.str(), m_file, m_line, m_fun);
                break;
            case L4CP_INFO_LOG_LEVEL:
                MYLOG4_INFO(m_logger, m_stream.str(), m_file, m_line, m_fun);
                break;
            case L4CP_WARN_LOG_LEVEL:
                MYLOG4_WARN(m_logger, m_stream.str(), m_file, m_line, m_fun);
                break;
            case L4CP_ERROR_LOG_LEVEL:
                MYLOG4_ERROR(m_logger, m_stream.str(), m_file, m_line, m_fun); 
                break;
            case L4CP_FATAL_LOG_LEVEL:
                MYLOG4_FATAL(m_logger, m_stream.str(), m_file, m_line, m_fun);
                break;
            default:
                MYLOG4_TRACE(m_logger, m_stream.str(), m_file, m_line, m_fun); 
            }
        }

        inline std::stringstream& stream()
        {
            return m_stream;
        }
    private:
        const char* m_file;
        int m_line;
        const char* m_fun;
        int m_level;
        const char* m_logger;
        std::stringstream m_stream;
    };

    /// ��glog���޸Ķ���
    struct LogMessageVoidify 
    {
        LogMessageVoidify(){}
        // This has to be an operator with a precedence lower than << but
        // higher than ?:
        void operator&(std::ostream&) { }
    };



    /**
    * ������־ģ�����
    */
    struct LogConfig
    {
        std::string log_path;           // ��־�ļ�·��
        std::string config_path;        // ��־�����ļ�·��
        std::string pattern_format;     // ��ʽ�ַ���
        std::size_t max_file_size;      // �ļ�����С kb
        std::size_t max_backup_num;     // �����ļ�����
        int         min_level;          // ��С��־����
        bool        immediate_flush;    // �Ƿ�����ˢ��

        LogConfig() :
            log_path(""), config_path(""), pattern_format("%D.%D{%q} %t %-5p %m  |%l%n"),
            max_file_size(5*1024*1024), max_backup_num(19), min_level(L4CP_WARN_LOG_LEVEL), immediate_flush(true)
        {
        }
    };

    class ConfigFile
    {
    public:
        ConfigFile(std::string path) : m_path(path)
        {
        }

        bool load(struct LogConfig & conf)
        {
            Json::Reader reader;
            Json::Value root;

            std::fstream file;
            file.open(m_path.c_str());
            if (file.is_open() && reader.parse(file, root))
            {
                file.close();

                if (!root.isMember("MinLevel")
                 || !root.isMember("MaxFileSize")
                 || !root.isMember("MaxFileNumber")
                 || !root.isMember("Pattern")
                 || !root.isMember("Pattern1"))
                {
                    return false;
                }

                if (root["MinLevel"].isString())
                {
                    conf.min_level = level_by_string(root["MinLevel"].asString());
                }

                if (root["MaxFileSize"].isIntegral())
                {
                    conf.max_file_size = root["MaxFileSize"].asUInt64();
                }

                if (root["MaxFileNumber"].isIntegral())
                {
                    conf.max_backup_num = root["MaxFileNumber"].asUInt64();
                }

                logout_format_by_level (conf.min_level, root, conf.pattern_format);
                return true;
            }

            if (file.is_open())
            {
                file.close();
            }
            return false;
        }

    private:
        bool logout_format_by_level (int lvl, Json::Value& jvalue, std::string& strlof)
        {   
            if (L4CP_TRACE_LOG_LEVEL == lvl ||  
                    L4CP_DEBUG_LOG_LEVEL == lvl)
            {   
                if (jvalue["Pattern"].isString() && !jvalue["Pattern"].asString().empty())
                {   
                    strlof = jvalue["Pattern"].asString (); 
                }
            }
            else
            {
                if (jvalue["Pattern1"].isString() && !jvalue["Pattern1"].asString().empty())
                {
                    strlof = jvalue["Pattern1"].asString ();
                }
            }
            return true;
        }
        int level_by_string(std::string slvl)
        {
            int lvl = L4CP_WARN_LOG_LEVEL;
            IStr str(slvl);
            if (str.equal("TRACE"))
            {
                lvl = L4CP_TRACE_LOG_LEVEL;
            }
            else if (str.equal("DEBUG"))
            {
                lvl = L4CP_DEBUG_LOG_LEVEL;
            }
            else if (str.equal("INFO"))
            {
                lvl = L4CP_INFO_LOG_LEVEL;
            }
            else if (str.equal("WARN"))
            {
                lvl = L4CP_WARN_LOG_LEVEL;
            }
            else if (str.equal("ERROR"))
            {
                lvl = L4CP_ERROR_LOG_LEVEL;
            }
            else if (str.equal("FATAL"))
            {
                lvl = L4CP_FATAL_LOG_LEVEL;
            }
            return lvl;
        }

        std::string m_path;
    };

    inline void InitLog(struct LogConfig & conf)
    {
        /// ���ظ�������,��������ļ�����
        if( vfs::IsFileExist(conf.config_path) )
        {
            ConfigFile cf(conf.config_path);
            if (!cf.load(conf))
            {
                std::cout << "load the configure file of logger failed\n";
            }
        }

        log4cplus::Logger::getRoot().setLogLevel( conf.min_level );
        /// �ļ���־1:info���ϼ�����־
        log4cplus::SharedAppenderPtr append_file(
                new log4cplus::RollingFileAppender(
                    conf.log_path,
                    conf.max_file_size,
                    conf.max_backup_num,
                    conf.immediate_flush));
        append_file->setName(LOG4CPLUS_TEXT("default_file"));
        append_file->setLayout( std::auto_ptr<log4cplus::Layout>(
                    new log4cplus::PatternLayout( conf.pattern_format )) );
        log4cplus::Logger::getRoot().addAppender(append_file);
    }
}


#define LOG4( LOGGER, LEVEL ) utils::LogMessage(LOGGER, __FILE__,__LINE__, LEVEL).stream()
#define LOG4_IF( LOGGER, LEVEL, condition ) (condition) ? (void) 0 : utils::LogMessageVoidify() & LOG4(LOGGER, LEVEL)

#define LOG4M( LEVEL ) LOG4( MODULE_NAME, LEVEL )
#define LOG4M_IF( LEVEL, condition ) LOG4_IF( MODULE_NAME, LEVEL, condition )
