#ifndef CDBOPT_H
#define CDBOPT_H

#include "wintolinuxdef.h"
#include "MBC_DBUtil.h"

using namespace DBUtil;
using namespace std;


// 函数名称：SQLOpen, 打开指定数据库，获取数据库操作句柄
//HANDLE DB_SQLOpen(const char* pcszDbPath, LPVOID pParam);
typedef HANDLE (*PFN_DB_SQLOpen)(const char* pcszDbPath, LPVOID pParam);


// 函数名称：SQLClose, 关闭指定数据库，释放资源
//void DB_SQLClose(HANDLE hDbCtx);
typedef void (*PFN_DB_SQLClose)(HANDLE hDbCtx);


/* 函数名称：SQLExecute
* 功能    ：执行Sqlite更新语句
* 参数    ：hDbCtx    , 打开的数据库句柄
*           pcszSql   , 执行的Sql语句
*           pData     , 格式化参数，对应Sql语句中带的?
*           uDataCount, 格式化参数个数
*           uTimeOut  , 超时时间
* 返回值  ：成功0, 其他,失败 */
//LONG DB_SQLExecute(HANDLE hDbCtx, const char* pcszSql, MBC_DB_PARAM_TYPE_PTR pData, ULONG uDataCount, ULONG uTimeOut = MBC_DB_DEFAULT_TIMEOUT);
typedef LONG (*PFN_DB_SQLExecute)(HANDLE hDbCtx, const char* pcszSql, MBC_DB_PARAM_TYPE_PTR pData, ULONG uDataCount, ULONG uTimeOut);


/* 函数名称 : SQLRead
* 功能    : 执行Sqlite更新语句
* 参数    : hDbCtx    , 打开的数据库句柄
*           pcszSql   , 执行的Sql语句
*           pData     , 格式化参数，对应Sql语句中带的?
*           uDataCount, 格式化参数个数
*           ppDbRecord , 获取到的记录集合
*           uTimeOut  , 超时时间
* 返回值  ：成功0, 其他,失败 */
//LONG DB_SQLRead(HANDLE hDbCtx, const char* pcszSql, MBC_DB_PARAM_TYPE_PTR pData, ULONG uDataCount, IDBRecord* ppDbRecord, ULONG uTimeOut = MBC_DB_DEFAULT_TIMEOUT);
typedef LONG (*PFN_DB_SQLRead)(HANDLE hDbCtx, const char* pcszSql, MBC_DB_PARAM_TYPE_PTR pData, ULONG uDataCount, IDBRecord* ppDbRecord, ULONG uTimeOut);


// 分配记录集$$PWD/
//LONG DB_NewDbRecord(IDBRecord** ppDbRecord );
typedef LONG (*PFN_DB_NewDbRecord)(IDBRecord** ppDbRecord );


// 释放记录集
//LONG DB_FreeDbRecord(IDBRecord* pDbRecord );
typedef LONG (*PFN_DB_FreeDbRecord)(IDBRecord* pDbRecord );


class CDbOpt
{
public:
    CDbOpt();
    virtual ~CDbOpt();

    static CDbOpt *GetDbObj();

    bool initial();

    HANDLE DB_SQLOpen(const char* pcszDbPath, LPVOID pParam = NULL);
    void DB_SQLClose(HANDLE hDbCtx);
    LONG DB_SQLExecute(HANDLE hDbCtx, const char* pcszSql, MBC_DB_PARAM_TYPE_PTR pData, ULONG uDataCount, ULONG uTimeOut = MBC_DB_DEFAULT_TIMEOUT);
    LONG DB_SQLRead(HANDLE hDbCtx, const char* pcszSql, MBC_DB_PARAM_TYPE_PTR pData, ULONG uDataCount, IDBRecord* ppDbRecord, ULONG uTimeOut = MBC_DB_DEFAULT_TIMEOUT);
    LONG DB_NewDbRecord(IDBRecord** ppDbRecord );
    LONG DB_FreeDbRecord(IDBRecord* pDbRecord );

    std::string getErrorMsg();

private:
    PFN_DB_SQLOpen      m_pDbSqlOpen;
    PFN_DB_SQLClose     m_pDbSqlClose;
    PFN_DB_SQLExecute   m_pDbSqlExecute;
    PFN_DB_SQLRead      m_pDbSqlRead;
    PFN_DB_NewDbRecord  m_pNewDbRecord;
    PFN_DB_FreeDbRecord m_pFreeDbRecord;
    HMODULE m_hModDll;

    std::string m_strError;
};

std::string QueryConfigValue(const std::string& strConfFilePath, const std::string& strQueryKey);

#endif // CDBOPT_H
