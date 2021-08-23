#include "CDbopt.h"
//#include "../publicfun.h"


CDbOpt *CDbOpt::GetDbObj()
{
    static CDbOpt *pDbOpt = NULL;

    if(NULL == pDbOpt)
    {
        static CDbOpt DbOpt;

        pDbOpt = &DbOpt;
    }

    return pDbOpt;
}

CDbOpt::CDbOpt():m_pDbSqlOpen(NULL), m_pDbSqlClose(NULL), m_pDbSqlExecute(NULL),
    m_pDbSqlRead(NULL), m_pNewDbRecord(NULL), m_pFreeDbRecord(NULL),  m_hModDll(NULL)
{
    initial();
}

CDbOpt::~CDbOpt()
{
    if(NULL != m_hModDll)
    {
        dlclose(m_hModDll);
        m_hModDll = NULL;
    }
}

bool CDbOpt::initial()
{
    std::string strInstallPath = QueryConfigValue("/etc/cems/vpengine.conf", "install path");
    if(strInstallPath.empty())
    {
        strInstallPath = "../";
    }
    std::string strDllPath = strInstallPath + "libvcemsstore.so";

    //加载动态库
    if (NULL == m_hModDll)
        m_hModDll = dlopen(strDllPath.c_str(), RTLD_LAZY);

    //导出接口
    if(NULL != m_hModDll)
    {
        m_pDbSqlOpen = (PFN_DB_SQLOpen)dlsym(m_hModDll, "DB_SQLOpen" );
        if (NULL == m_pDbSqlOpen)
        {
            m_strError = "CDbOpt===>DB_SQLOpen export failed.";
            return false;
        }

        m_pDbSqlClose = (PFN_DB_SQLClose)dlsym(m_hModDll, "DB_SQLClose");
        if (NULL == m_pDbSqlClose)
        {
            m_strError = "CDbOpt===>DB_SQLClose export failed.";
            return false;
        }

        m_pDbSqlRead = (PFN_DB_SQLRead)dlsym(m_hModDll, "DB_SQLRead");
        if (NULL == m_pDbSqlRead)
        {
            m_strError = "CDbOpt===>DB_SQLRead export failed.";
            return false;
        }

        m_pDbSqlExecute = (PFN_DB_SQLExecute)dlsym(m_hModDll, "DB_SQLExecute");
        if (NULL == m_pDbSqlExecute)
        {
            m_strError = "CDbOpt===>DB_SQLExecute export failed.";
            return false;
        }

        m_pNewDbRecord = (PFN_DB_NewDbRecord)dlsym(m_hModDll, "DB_NewDbRecord");
        if (NULL == m_pNewDbRecord)
        {
            m_strError = "CDbOpt===>DB_NewDbRecord export failed.";
            return false;
        }

        m_pFreeDbRecord = (PFN_DB_FreeDbRecord)dlsym(m_hModDll, "DB_FreeDbRecord");
        if (NULL == m_pFreeDbRecord)
        {
            m_strError = "CDbOpt===>DB_FreeDbRecord export failed.";
            return false;
        }

        if(NULL != m_pDbSqlOpen && NULL != m_pDbSqlClose && NULL != m_pDbSqlRead
                && NULL != m_pDbSqlExecute && NULL != m_pNewDbRecord && NULL != m_pFreeDbRecord)
        {
            return true;
        }
    }
    else
    {
        m_strError = "CDbOpt===>libvcemsstore.so load failed.";
    }

    return false;
}

//打开数据库文件
HANDLE CDbOpt::DB_SQLOpen(const char *pcszDbPath, LPVOID pParam)
{
    HANDLE hDb = NULL;
    if( NULL != m_pDbSqlOpen )
    {
        hDb = m_pDbSqlOpen( pcszDbPath, pParam );
    }

    return hDb;
}

//关闭数据库文件
void CDbOpt::DB_SQLClose(HANDLE hDbCtx)
{
    if(NULL != m_pDbSqlClose)
    {
        m_pDbSqlClose(hDbCtx);
    }
}

//执行SQL语句
LONG CDbOpt::DB_SQLExecute(HANDLE hDbCtx, const char *pcszSql, MBC_DB_PARAM_TYPE_PTR pData, ULONG uDataCount, ULONG uTimeOut)
{
    LONG iRet = -1;

    if(NULL != m_pDbSqlExecute)
    {
        iRet = m_pDbSqlExecute(hDbCtx, pcszSql, pData, uDataCount, uTimeOut);
    }

    return iRet;
}

//执行Read语句
LONG CDbOpt::DB_SQLRead(HANDLE hDbCtx, const char *pcszSql, MBC_DB_PARAM_TYPE_PTR pData, ULONG uDataCount, IDBRecord *ppDbRecord, ULONG uTimeOut)
{
    LONG iRet = -1;

    if(NULL != m_pDbSqlRead)
    {
        iRet = m_pDbSqlRead(hDbCtx, pcszSql, pData, uDataCount, ppDbRecord, uTimeOut);
    }

    return iRet;
}

//申请记录缓存
LONG CDbOpt::DB_NewDbRecord(IDBRecord **ppDbRecord)
{
    LONG iRet = -1;

    if(NULL != m_pNewDbRecord)
    {
        iRet = m_pNewDbRecord(ppDbRecord);
    }

    return iRet;
}

//释放记录缓存
LONG CDbOpt::DB_FreeDbRecord(IDBRecord *pDbRecord)
{
    LONG iRet = -1;

    if(NULL != m_pFreeDbRecord)
    {
        iRet = m_pFreeDbRecord(pDbRecord);
    }

    return iRet;
}

std::string CDbOpt::getErrorMsg()
{
    return m_strError;
}

void TrimStringA(std::string& str, bool bAll)
{
    std::string::size_type pos = 0;
    if (bAll)
    {
        while(str.npos != (pos = str.find(" ")))
        {
            str = str.replace(pos, 1, "");
            pos = 0;//位置重置
        }
    }
    else
    {
        if (str.npos != (pos = str.find_first_not_of(" ")))
        {
            str = str.substr(str.find_first_not_of(" "));
        }

        str = str.substr(0, str.find_last_not_of(" ")+1);
    }
}

std::string QueryConfigValue(const std::string& strConfFilePath, const std::string& strQueryKey)
{
    std::string strReturnValue = "";

    ifstream configfile(strConfFilePath.c_str());
    if(configfile)
    {
        std::string strLine = "";
        while(getline(configfile, strLine))
        {
            //不同的编译器处理不一样，如果读到了换行符，主动处理
            size_t nPosRN = strLine.find('\n');
            if (nPosRN!=std::string::npos)
            {
                strLine.erase(nPosRN, 1);
            }
            nPosRN = strLine.find('\r');
            if (nPosRN != std::string::npos)
            {
                strLine.erase(nPosRN, 1);
            }

            std::string::size_type equalsymbol_pos = 0;

            std::string strKey = "";
            std::string strValue = "";

            if(strLine.npos != (equalsymbol_pos = strLine.find("=")))
            {
                strKey = strLine.substr(0, equalsymbol_pos);
                strValue = strLine.substr(equalsymbol_pos+1, strLine.size()-1);
                TrimStringA(strKey, false);
                TrimStringA(strValue, false);
                if (0 == strcmp(strKey.c_str(), strQueryKey.c_str()))
                {
                    strReturnValue = strValue;
                    break;
                }
            }
        }
        configfile.close();
        configfile.clear();
    }

    return strReturnValue;
}
