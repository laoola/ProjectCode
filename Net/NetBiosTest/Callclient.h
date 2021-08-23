#ifndef CALLCLIENT_H
#define CALLCLIENT_H

#include "wintolinuxdef.h"

typedef std::vector<std::string> VECTOR_RESULT;

class MCallClient
{
public:
    MCallClient(void);
    ~MCallClient(void);

public:

protected:
    std::string  GetHostName();

protected:
    DWORD GetLocalUdpPort();
    std::string Read(std::string szKey);
    std::string ReadXml(std::string szElementName, std::string szKey, std::string szXml);

public:
    bool  Call(DWORD & dwBegin, DWORD & dwEnd);
    void  RecvUdp(SOCKET& sock);

public:
    void  GetResult(VECTOR_RESULT & vResult);

private:
    VECTOR_RESULT	m_vResult;

private:
    std::string m_szReply;
    std::string m_szCall;
};

void GetLocalTime(struct tm *ptm);

#endif // CALLCLIENT_H
