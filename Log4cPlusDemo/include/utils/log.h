#pragma once

#include "log4.ipp"


#define LOG_ERROR()       LOG4M(L4CP_ERROR_LOG_LEVEL)
#define LOG_WARNING()     LOG4M(L4CP_WARN_LOG_LEVEL)
#define LOG_INFO()        LOG4M(L4CP_INFO_LOG_LEVEL)
#define LOG_DEBUG()       LOG4M(L4CP_DEBUG_LOG_LEVEL)
#define LOG_TRACE()       LOG4M(L4CP_TRACE_LOG_LEVEL)

#define LOG_ERROR_IF(condition)       LOG4M_IF(utils::ERROR, !condition)
#define LOG_WARNING_IF(condition)     LOG4M_IF(utils::WARN, !condition)
#define LOG_INFO_IF(condition)        LOG4M_IF(utils::INFO, !condition)
#define LOG_DEBUG_IF(condition)       LOG4M_IF(utils::DEBUG, !condition)
#define LOG_TRACE_IF(condition)       LOG4M_IF(utils::TRACE, !condition)
