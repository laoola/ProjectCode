// TestNetBios.cpp : Defines the entry point for the console application.
//
//*********************************************************************************//
// 询问包结构:
// Transaction ID 两字节（16位） 0x00 0x00
// Flags 两字节（16位） 0x00 0x10
// Questions 两字节（16位） 0x00 0x01
// AnswerRRs 两字节（16位） 0x00 0x00
// AuthorityRRs 两字节（16位） 0x00 0x00
// AdditionalRRs 两字节（16位） 0x00 0x00
// Name:array [1..34] 0x20 0x43 0x4B 0x41(30个) 0x00 ;
// Type:NBSTAT 两字节 0x00 0x21
// Class:INET 两字节（16位）0x00 0x01
//------------------------------------------------------------------------
// “UDP－NetBIOS－NS”应答包的结构及主要字段一览表
// 序号字段名长度
// 1 Transaction ID 两字节（16位）
// 2 Flags 两字节（16位）
// 3 Questions 两字节（16位）
// 4 AnswerRRs 两字节（16位）
// 5 AuthorityRRs 两字节（16位）
// 6 AdditionalRRs 两字节（16位）
// 7 Name<Workstation/Redirector> 34字节（272位）
// 8 Type:NBSTAT 两字节（16位）
// 9 Class:INET 两字节（16位）
// 10 Time To Live 四字节（32位）
// 11 Length 两字节（16位）
// 12 Number of name 一个字节（8位）
// NetBIOS Name Info 18×Number Of Name字节
// Unit ID 6字节（48位)
//**************************************************************************************//

#include "CNetBios.h"


using namespace std;

#define  BUFFER_SIZE    1024*50
#define  NBNS_PORT      137

typedef map<DWORD, DWORD> mapFLAG;


MNetBios::MNetBios() : m_pIpRanage(NULL)
{

}

MNetBios::~MNetBios()
{

}

void MNetBios::MakeQuestionPack(struct Q_NETBIOSNS& nbns)
{
	nbns.tid = 0x0000;
	nbns.flags = 0x0000;
	nbns.questions = 0x0100;
	nbns.answerRRS = 0x0000;
	nbns.authorityRRS = 0x0000;
	nbns.additionalRRS = 0x0000;
	nbns.name[0] = 0x20;
	nbns.name[1] = 0x43;
	nbns.name[2] = 0x4b;

	int j = 0;
	for(j = 3; j < 34; j++)
		nbns.name[j] = 0x41;

	nbns.name[33] = 0x00;
	nbns.type = 0x2100;
	nbns.classe = 0x0100;
}

void MNetBios::PhrasePack(ULONG uip, char* buffer, DWORD dwLen)
{
	if(!uip)
	{
		return;
	}

	string szip, szgroup, szhostname, szmac;

	struct sockaddr_in toAddr = {0};
	toAddr.sin_addr.s_addr = htonl(uip);
	szip = inet_ntoa(toAddr.sin_addr);

	USHORT NumberOfNames = 0;
	memcpy(&NumberOfNames, buffer + 56, 1);

	int i = 0;
	char NetbiosName[17] = {0};

	memcpy(NetbiosName, buffer + 57, 15);

	string flags;
	for(int ic = 0; ic < NumberOfNames; ic++)
	{
		if(!szhostname.empty() && !szgroup.empty())
		{
			break;
		}

		BYTE ch;
        memcpy(&ch, buffer + 57 + ic * 18 + 16, 1);

		char NetbiosName[17] ={0};
		memcpy(NetbiosName, buffer + 57 + ic*18, 15);

		UINT u = (UINT)ch;
		if((u & 0x80) == 0)			//unique name
		{
			szhostname = NetbiosName;
		}
		else	//group name
		{
			szgroup = NetbiosName;
		}
	}

	USHORT mac[6] = {0};
	for(i = 0; i < 6; i++)
	{
		char buff[10] = {0};
		memcpy(&mac[i], buffer + 57 + NumberOfNames*18 + i, 1);

		if(i != 5)
            snprintf(buff, 10, "%02X-", mac[i]);
		else
            snprintf(buff, 10, "%02X", mac[i]);

		szmac += buff;
	}

	DEV_INFO devinfo;
	devinfo.szIpAddress = szip;
	devinfo.szGroupName = szgroup;
	devinfo.szHostName = szhostname;
	devinfo.szMac = szmac;


	char buffervalue[MAX_PATH] = {0};
    snprintf(buffervalue, MAX_PATH, "IP: %s, HostName: %s, GroupName: %s, MAC: %s.", szip.c_str(), szhostname.c_str(), szgroup.c_str(), szmac.c_str());
//	printf("%s\n", buffervalue);


	if(!szmac.empty())
	{
		m_Bios.insert(MAP_BIOS::value_type(szmac, devinfo));
	}

}

#define  IP_SEND_COUNT       1000;

void  MNetBios::Work()
{
	if(m_pIpRanage == NULL)
	{
		return;
	}

	SOCKET scanSocket;

	ULONG first = m_pIpRanage->first;
	ULONG second = m_pIpRanage->second;

	struct timeval tv_out = {2, 0};

	scanSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	//recvfrom ÉèÖÃ³¬Ê±
	setsockopt(scanSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv_out, sizeof(tv_out));

	int count = second - first + 1;

	int left  = count % IP_SEND_COUNT;
	int loops = count / IP_SEND_COUNT;


	if(left > 0)
	{
		loops += 1;
	}

	struct Q_NETBIOSNS nbns;
	MakeQuestionPack(nbns);

	for(int i = 0; i < loops ; i++)
	{
		ULONG ipSet = first + i * IP_SEND_COUNT;
		ULONG ipEnd = ipSet + IP_SEND_COUNT;

		if(i == loops - 1 && left > 0)
		{
			ipEnd = ipSet + left - 1;
		}

		for( ; ipSet <= ipEnd ; ipSet++)
		{
			char sendbuf[50] = {0};

			struct sockaddr_in toAddr = {0};
			toAddr.sin_family = AF_INET;
			toAddr.sin_addr.s_addr = htonl(ipSet);
			toAddr.sin_port = htons(NBNS_PORT);

			sendto(scanSocket, (const char*)&nbns, sizeof(nbns), 0, (struct sockaddr *)&toAddr, sizeof(toAddr) );

		}

		while(1)
		{
			int ret;
			fd_set fst;
			FD_ZERO(&fst);
			FD_SET(scanSocket, &fst);

			struct timeval tm_out = {0, 50 * 1000};
			ret = select((int)(scanSocket + 1), &fst, NULL, NULL, &tm_out);

			if(ret == 0 || ret == -1)
			{
				break;
			}

			char RecvBuf[256] = {0};
			struct sockaddr_in addrfrom = {0};
            socklen_t size = sizeof(addrfrom);
            ret = recvfrom(scanSocket, RecvBuf, 256, 0, (sockaddr*)&addrfrom, &size);
                    //(scanSocket, RecvBuf, 256, 0, (sockaddr*)&addrfrom, &size);

			if(ret > 0)
			{
				PhrasePack(ntohl(addrfrom.sin_addr.s_addr), RecvBuf, (DWORD)strlen(RecvBuf));
			}
		}	
	}

    printf("NetBios size = %lu\n", m_Bios.size());

	delete	m_pIpRanage;
	m_pIpRanage = NULL;
}

void * MNetBios::SendProc(void* lparam)
{
	MNetBios * pmn = static_cast<MNetBios*>(lparam);
	pmn->Work();
    return NULL;
}

int Check_IP(char * str)
{
	int dot_count = 0;
	int num_count = 0;

	while( (*str) != '\0' )
	{
		if((*str) != '.')
		{
			if( (*str) <= '9' && (*str) >='0')
			{
				num_count = num_count*10+(int)(*str)-'0';

				if( num_count <0 || num_count >255 )
					return 0;
			}
			else
				return 0;

		}
		else
		{
			if( num_count == 0 && dot_count == 0 )
				return 0;
			dot_count++;
			if( num_count < 0 || num_count > 255 )
				return 0;
			num_count = 0;
		}
		str++;
	}
	if( dot_count != 3)
		return 0;
	return 1;
}

void  MNetBios::MNetBiosScan(DWORD LowAddr, DWORD HighAddr)
{
	m_Bios.clear();

    pthread_t hThread;

	m_pIpRanage = new IP_RANGE;

	m_pIpRanage->first = LowAddr;
	m_pIpRanage->second = HighAddr;

	struct in_addr addr;
	addr.s_addr = htonl(m_pIpRanage->first);

    pthread_create(&hThread, 0, SendProc, this);

    pthread_join(hThread, NULL);
}

int  MNetBios::NetBiosResult(MAP_BIOS & bios)
{
	m_Bios.swap(bios);

	return (int)bios.size();
}



