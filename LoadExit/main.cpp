#include <QCoreApplication>
#include "dlfcn.h"
#include <QDebug>

typedef bool (*PFN_AGENT_STOP)();

int main(int argc, char *argv[])
{

    bool bResult =false;
    std::string strDllPath = "/opt/vrv/cems/libvcemssrv.so";

    void *pDllHandle = dlopen(strDllPath.c_str(), RTLD_NOW);

    if(NULL == pDllHandle)
    {
        qDebug()<<"export libvcemssrv.so error :"<<dlerror();

        bResult = false;
    }

    PFN_AGENT_STOP pAgentStop = (PFN_AGENT_STOP)dlsym(pDllHandle, "AgentStop");

    if(NULL == pAgentStop)
    {
        qDebug()<<"export AgentStop error :"<<dlerror();

        bResult = false;
    }
    else
    {
        qDebug()<<"exit thread to AgentStop";
        bResult = pAgentStop();
    }

    return 0;
}
