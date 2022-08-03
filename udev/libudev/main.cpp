#include "log.h"
#include "CUsbMonitor.h"

int main(int argc, char* argv[])
{
    CUsbMg contorl;
    contorl.OpenAudit();
    CUsbMonitor monitor(&contorl);
    monitor.InitMonitor();
    monitor.WaitEvent();
    return 0;
}