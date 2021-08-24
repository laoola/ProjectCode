#include "utils/log.h"

int main(int argc, char *argv[])
{

    // 初始化日志
    utils::LogConfig logconf;
    logconf.log_path = "./log/Logtest.log";
    logconf.config_path = "./logsets.conf";

    logconf.min_level   = L4CP_TRACE_LOG_LEVEL; // 版本稳定后再调为WARN

    utils::InitLog(logconf);

    LOG_ERROR()<<"this is Log4cplus test.";

    return 0;
}
