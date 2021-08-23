
#include "./Inc/Critical.h"
#include "CheckNetCardPacket.h"

CCritical	g_csDhcp;
CCritical	g_TcpCs;
CCritical	g_UDPCs;
CCritical	g_ARPCs;


int main(int argc, char *argv[])
{
    CCheckNetCardPacket checkNetPack;

    checkNetPack.StartCheck();

    return 0;
}
