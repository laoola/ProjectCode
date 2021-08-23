#pragma once

#include "wintolinuxdef.h"

struct Q_NETBIOSNS
{
	unsigned short int tid;
	unsigned short int flags;
	unsigned short int questions;
	unsigned short int answerRRS;
	unsigned short int authorityRRS;
	unsigned short int additionalRRS;
	unsigned char name[34];
	unsigned short int type;
	unsigned short int classe;
};

struct IP_RANGE
{
	ULONG first;
	ULONG second;
};


struct  DEV_INFO
{
	std::string szBoot;
	std::string szMac;
	std::string szIpAddress;
	std::string szDevId;
	std::string szGroupName;
	std::string szHostName;
	std::string szFireWall;
	std::string szAreaId;
	std::string szOrgId;
	std::string szRegAreaId;
	std::string szRegOrgId;
	time_t scantime;
};

typedef std::map<std::string, DEV_INFO> MAP_BIOS;

class MNetBios
{
public:
	MNetBios();
	~MNetBios();

private:
	MAP_BIOS	m_Bios;

private:
	IP_RANGE*	m_pIpRanage;

protected:
	void MakeQuestionPack(struct Q_NETBIOSNS& nbns);
	void PhrasePack(ULONG uip, char* buffer, DWORD dwLen);
	void Work();
	int  Check_IP(char * str);

public:
    static void* SendProc(void* lparam);

public:
	void MNetBiosScan(DWORD LowAddr, DWORD HighAddr);
	int  NetBiosResult(MAP_BIOS & bios);
};
