#include "screenrecord.h"

#include <QApplication>

#include <dirent.h>
#include <sys/resource.h>

void open_core_dump()
{
    struct rlimit  limit;
    if(getrlimit(RLIMIT_CORE,&limit) == 0)
    {
        struct rlimit  limitnew;
        limitnew.rlim_cur = limitnew.rlim_max = RLIM_INFINITY;
        if(setrlimit(RLIMIT_CORE,&limitnew) != 0)
        {
            limitnew.rlim_cur = limitnew.rlim_max = limit.rlim_max;
            setrlimit(RLIMIT_CORE,&limitnew);
        }
    }
}

int main(int argc, char *argv[])
{
    open_core_dump();
    if(3 != argc)
    {
        printf("input : Time filepath \n");

        return 0;
    }
    else
    {
        printf("Time :%s file :%s\n",
               argv[1], argv[2]);
    }

    QApplication a(argc, argv);
    ScreenRecord w;

    w.SetConf(argv[1], argv[2]);
    w.record();

    return a.exec();
}
