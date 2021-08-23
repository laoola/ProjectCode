#pragma once

#ifndef __MBC_DBUTIL_H__
#define __MBC_DBUTIL_H__
#include "IBase.h"
/*
* 数据库管理接口，所有传入传出字符型参数应为utf-8格式
*/

namespace DBUtil {
#define LIB_VER "20170105" //库文件版本，只有版本相等才能调用

#ifndef SQLITE_INTEGER
    #define SQLITE_INTEGER  1
#endif

#ifndef SQLITE_FLOAT
	#define SQLITE_FLOAT    2
#endif

#ifndef SQLITE3_TEXT
	#define SQLITE3_TEXT    3
#endif

#ifndef SQLITE_BLOB
	#define SQLITE_BLOB     4
#endif

#ifndef SQLITE_NULL
	#define SQLITE_NULL     5
#endif

#ifndef INT64
#define INT64 __int64_t
#endif

#ifndef DOUBLE
#define DOUBLE double
#endif

#define MBC_DB_DEFAULT_TIMEOUT  5000

    typedef struct _MBC_DB_PARAM_TYPE
    {
        ULONG       SqliteType ; //
        ULONG       DataSize ;   // only for blob type
        union
        {
            INT64   Int64Value ;
            DOUBLE  DoubleValue ;
            LPCSTR   CharValue ;
            PVOID   BlobValue ;
        } ;
    } MBC_DB_PARAM_TYPE, * MBC_DB_PARAM_TYPE_PTR;


	//缓存记录定义
	class IDBItem
	{
	public:
                virtual const char* GetVersion() = 0;
	public:
                virtual BOOL AddValue( const char* pszName, MBC_DB_PARAM_TYPE& dbVaue  ) = 0;
                virtual BOOL GetValue( const char* pszName, MBC_DB_PARAM_TYPE& dbValue ) = 0;
	};

	//缓存记录集定义
	class IDBRecord
	{
	public:
                virtual int GetRecordCount() = 0;
	public:
                virtual IDBItem* GetFirstItem() = 0;
                virtual IDBItem* GetNextItem () = 0;
	};
    
	//策略表项定义
    class IDBPolicyItem : public IDBItem
	{
	public:
            virtual INT64 GetPoicyItemId()      = 0;
            virtual DWORD GetPolicyCRC()        = 0;
            virtual const char* GetPolicyId()   = 0;
            virtual const char* GetPolicyData() = 0;
            virtual const char* GetPolicyGUID() = 0;
            virtual const char*   GetPolicyState()      = 0;
            virtual const char*   GetExecResult()      = 0;
            virtual const char*   GetExecTimeStamp()      = 0;
            virtual const char*   GetTimeStamp()      = 0;
        public:
            virtual void SetPolicyCRC ( DWORD& dwCRC )              = 0;
            virtual void SetPolicyId  ( const char* pszPolicyId   ) = 0;
            virtual void SetPolicyData( const char* pszPolicyData ) = 0;
            virtual void SetPolicyGUID( const char* pszPolicyGUID ) = 0;
            virtual void SetPolicyState( const char* pszState )     = 0;
            virtual void SetExecResult(const char* pszResult)      = 0;
            virtual void SetExecTimeStamp(const char* timeStamp)      = 0;
            virtual void SetTimeStamp(const char* timeStamp)      = 0;
	};
	//////////////////////////////////////////////////////////////////////////
    
	//策略记录集定义
    class IDBPolicyRecord
    {
    public:
        virtual int GetRecordCount() = 0;
    public:
        virtual IDBPolicyItem* GetFirstItem() = 0;
        virtual IDBPolicyItem* GetNextItem () = 0;
    };
	//////////////////////////////////////////////////////////////////////////

    //设备表表项
	class IDbDeviceItem : public IDBItem
	{
	public:
                virtual INT64 GetDeviceItemId()        = 0;
                virtual const char* GetDeviceGUID()    = 0;
                virtual int   GetState()               = 0;
                virtual const char* GetClientVersion() = 0;
                virtual const char* GetInstallTime  () = 0;
                virtual const char* GetUnInstallTime() = 0;
                virtual const char* GetDeviceId     () = 0;
                virtual const char* GetUserId       () = 0;
                virtual const char* GetOrganizationId() = 0;
                virtual const char* GetIp            () = 0;
                virtual int  GetIpType               () = 0;
                virtual const char* GetIpNumber      () = 0;
                virtual const char* GetMac           () = 0;
                virtual const char* GetName          () = 0;
                virtual const char* GetDescription   () = 0;
                virtual int GetOSId                  () = 0;
        virtual int GetDeviceTypeId          () = 0;
                virtual int GetSListId               () = 0;
                virtual const char* GetOrgCode       () = 0;
                virtual const char* GetPolicyList    () = 0;
	public:
                virtual void SetDeviceGUID( const char* pszGUID )    = 0;
                virtual void SetState( int nState )                  = 0;
                virtual void SetClientVersion( const char* pszClientVersion ) = 0;
                virtual void SetInstallTime  ( const char* pszInstallTime   ) = 0;
                virtual void SetUnInstallTime( const char* pszUnInstallTime ) = 0;
                virtual void SetDeviceId     ( const char* pszDeviceId )      = 0;
                virtual void SetUserId       ( const char* pszUserId )        = 0;
                virtual void SetOrganizationId( const char* pszOrgId )        = 0;
                virtual void SetIp            ( const char* pszId )           = 0;
                virtual void SetIpType               ( int nIpType )          = 0;
                virtual void SetIpNumber      ( const char* pszIpNumber)      = 0;
                virtual void SetMac           ( const char* pszMac )          = 0;
                virtual void SetName          ( const char* pszName )         = 0;
                virtual void SetDescription   ( const char* pszDescription )  = 0;

                virtual void SetOSId                 ( int nOsId )       = 0;
                virtual void SetDeviceTypeId         ( int nDevId )      = 0;
                virtual void SetSListId               ( int nSListId )   = 0;
                virtual void SetOrgCode       ( const char* pszOrgCode ) = 0;
                virtual void SetPolicyList    ( const char* pszPolicyList ) = 0;
	};
	//////////////////////////////////////////////////////////////////////////

	//设备记录集定义
	class IDBDeviceRecord
	{
	public:
                virtual int GetRecordCount() = 0;
	public:
                virtual IDbDeviceItem* GetFirstItem() = 0;
                virtual IDbDeviceItem* GetNextItem () = 0;
	};
	//////////////////////////////////////////////////////////////////////////


    //注册服务器表项定义
	class IDBRegSrvItem : public IDBItem
	{
	public:
                virtual INT64 GetItemId() = 0;
                virtual int   GetSrvId () = 0;
                virtual const char* GetRegURL() = 0;
                virtual const char* GetReqURL() = 0;
                virtual int   GetPrefer      () = 0;
	public:
                virtual void SetSrvId( int nSrvId ) = 0;
                virtual void SetRegURL( const char* pszRegURL ) = 0;
                virtual void SetReqURL( const char* pszReqURL ) = 0;
                virtual void SetPrefer( int nPrefer )           = 0;
	};

	//注册服务器记录集定义
	class IDBRegSrvRecord 
	{
	public:
                virtual int GetRecordCount() = 0;
	public:
                virtual IDBRegSrvItem* GetFirstItem() = 0;
                virtual IDBRegSrvItem* GetNextItem () = 0;
	};
	//////////////////////////////////////////////////////////////////////////
    
    //备用服务器表项定义
	class IDBSrvListItem : public IDBItem
	{
	public:
                virtual INT64 GetItemId()        = 0;
                virtual int GetSrvId ()          = 0;
                virtual const char* GetType()    = 0;
                virtual const char* GetOrgKey()  = 0;
                virtual const char* GetOrgCode() = 0;
                virtual const char* GetName   () = 0;
                virtual const char* GetIp     () = 0;
                virtual int GetPort           () = 0;
                virtual const char* GetProtocol() = 0;
                virtual const char* GetProject() = 0;
                virtual const char* GetMainDir() = 0;
                virtual int GetPrefer () = 0;
	public:
                virtual void SetSrvId ( int nSrvId ) = 0;
                virtual void SetType  ( const char* pszType )   = 0;
                virtual void SetOrgKey( const char* pszOrgKey ) = 0;
                virtual void SetOrgCode( const char* pszOrgCode)= 0;
                virtual void SetName  ( const char* pszName )   = 0;
                virtual void SetIp    ( const char* pszId )     = 0;
                virtual void SetPort  ( int nPort )             = 0;
                virtual void SetProtocol( const char* pszProtocol ) = 0;
                virtual void SetProject( const char* pszProject )   = 0;
                virtual void SetMainDir( const char* pszMainDir )   = 0;
                virtual void SetPrefer ( int nPrefer )              = 0;
	};
	//////////////////////////////////////////////////////////////////////////

	//备用服务器记录集定义
	class IDBSrvListRecord
	{
	public:
                virtual int GetRecordCount() = 0;
	public:
                virtual IDBSrvListItem* GetFirstItem() = 0;
                virtual IDBSrvListItem* GetNextItem () = 0;
	};
	//////////////////////////////////////////////////////////////////////////

	//终端配置信息表项定义
	class IDbLocalInfoItem : public IDBItem
	{
	public:
                virtual INT64 GetItemId() = 0;
                virtual const char* GetUserId () = 0;
                virtual const char* GetAccount() = 0;
                virtual const char* GetType   () = 0;
                virtual const char* GetData   () = 0;
	public:
                virtual void SetUserId ( const char* pszUserId ) = 0;
                virtual void SetAccount( const char* pszAccount) = 0;
                virtual void SetType   ( const char* pszType )   = 0;
                virtual void SetData   ( const char* pszData )   = 0;
	};
	//////////////////////////////////////////////////////////////////////////

	//终端配置记录集定义
	class IDBLocalInfoRecord
	{
	public:
                virtual int GetRecordCount() = 0;
	public:
                virtual IDbLocalInfoItem* GetFirstItem() = 0;
                virtual IDbLocalInfoItem* GetNextItem () = 0;
	};
	//////////////////////////////////////////////////////////////////////////

	//用户表项定义
	class IDBUserItem : public IDBItem
	{
	public:
                virtual INT64 GetItemId() = 0;
                virtual const char* GetGUID() = 0;
                virtual int   GetType      () = 0;
                virtual const char* GetUserId() = 0;
                virtual const char* GetOrganizationId() = 0;
                virtual const char* GetName          () = 0;
                virtual const char* GetOrgName       () = 0;
                virtual const char* GetIdCardNo      () = 0;
                virtual const char* GetWorkId        () = 0;
                virtual const char* GetPhone         () = 0;
                virtual const char* GetTel           () = 0;
                virtual const char* GetEmail         () = 0;
                virtual const char* GetBirthday      () = 0;
                virtual const char* GetLocation      () = 0;
                virtual const char* GetSimNo         () = 0;
                virtual const char* GetAccount       () = 0;
                virtual const char* GetPassWord      () = 0;
                virtual const char* GetPolicyList    () = 0;
	public:
                virtual void SetGUID( const char* pszGUID ) = 0;
                virtual void SetType( int nType )           = 0;
                virtual void SetUserId( const char* pszUserId ) = 0;
                virtual void SetOrganizationId( const char* pszOrgId ) = 0;
                virtual void SetName( const char* pszName ) = 0;
                virtual void SetOrgName( const char* pszOrgName ) = 0;
                virtual void SetIdCardNo( const char* pszIdCardNo ) = 0;
                virtual void SetWorkId  ( const char* pszWordId )   = 0;
                virtual void SetPhone   ( const char* pszPhone )    = 0;
                virtual void SetTel     ( const char* pszTel )      = 0;
                virtual void SetEmail   ( const char* pszEmail )    = 0;
                virtual void SetBirthday( const char* pszBirthday ) = 0;
                virtual void SetLocation( const char* pszLocation ) = 0;
                virtual void SetSimNo   ( const char* pszSimNo )    = 0;
                virtual void SetAccount ( const char* pszAccount )  = 0;
                virtual void SetPassWord( const char* pszPassWord ) = 0;
                virtual void SetPolicyList( const char* pszPolicyList ) = 0;
	public:
                virtual void SetUserCodeUI( const char* pszUserCodUI )  = 0;
                virtual const char* GetUserCodeUI()                     = 0;
	};
	//////////////////////////////////////////////////////////////////////////

	//CEMS_USER表记录集定义
	class IDBUserRecord
	{
	public:
                virtual int GetRecordCount() = 0;
	public:
                virtual IDBUserItem* GetFirstItem() = 0;
                virtual IDBUserItem* GetNextItem () = 0;
	};
	//////////////////////////////////////////////////////////////////////////

	//产品列表表项定义
        class IDBProductItem : public IDBItem
        {
        public:
                virtual const char* GetProductType() = 0;
                virtual const char* GetProductName() = 0;
                virtual const char* GetBaseVersion() = 0;
                virtual const char* GetCustomVersion() = 0;
                virtual const char* GetInstallTime  () = 0;
                virtual const char* GetUpdataTime   () = 0;
                virtual const char* GetUnInstallTime() = 0;
                virtual int         GetState        () = 0;
                virtual const char* GetOsType       () = 0;
                virtual const char* Getkernelmodule () = 0;
                virtual const char* GetkernelmoduleVersion() = 0;
                virtual const char* GetClientSign   () = 0;
                virtual const char* GetProductId    () = 0;
        public:
                virtual void SetProductType( const char* pszProductType ) = 0;
                virtual void SetProductName( const char* pszProductName ) = 0;
                virtual void SetBaseVersion( const char* pszBaseVersion ) = 0;
                virtual void SetCustomVersion( const char* pszCustomVersion ) = 0;
                virtual void SetInstallTime( const char* pszInstallTime ) = 0;
                virtual void SetUpdataTime ( const char* pszUpdataTime )  = 0;
                virtual void SetUnInstallTime( const char* pszUnInstallTime ) = 0;
                virtual void SetState( int nState ) = 0;
                virtual void SetOsType( const char* pszOsType ) = 0;
                virtual void Setkernelmodule( const char* pszkernelmodule ) = 0;
                virtual void SetkernelmoduleVersion( const char* pszkernelmoduleVersion ) = 0;
                virtual void SetClientSign( const char* pszClientSign ) = 0;
                virtual void SetProductId ( const char* pszProductId  ) = 0;
        };
	//////////////////////////////////////////////////////////////////////////

	//产品列表记录集
	class IDBProductRecord
	{
	public:
                virtual int GetRecordCount() = 0;
	public:
                virtual IDBProductItem* GetFirstItem() = 0;
                virtual IDBProductItem* GetNextItem () = 0;
	};
    //////////////////////////////////////////////////////////////////////////

    class ISafeStorePublish
    {
    public:
        //读写general表，提供类似ini的标准存储
        virtual int ReadGeneral(const char* pszSection, const char* pszKey, char* pszValue, int& nLen) = 0;
        virtual int WriteGeneral(const char* pszSection, const char* pszKey, char* pszValue, int nLen) = 0;
        virtual int DelGeneral(const char* pszSection, const char* pszKey) = 0;
        virtual int ClearGeneral() = 0;
        virtual BOOL ReadDevice( IDBDeviceRecord* pszCachRecord, const char* pszDeviceId = NULL, const char* pszUserId = NULL ) = 0;
        virtual BOOL WriteDevice( IDbDeviceItem* pDbItem ) = 0;
        virtual BOOL DelDevice( const char* pszDeviceId = NULL, const char* pszUserId = NULL ) = 0;
        virtual LONG NewDeviceItem (IDbDeviceItem** ppPolicyItem ) = 0;
        virtual LONG FreeDeviceItem(IDbDeviceItem* pPolicyItem ) = 0;
        virtual LONG NewDeviceRecord(IDBDeviceRecord** ppDbRecord ) = 0;
        virtual LONG FreeDeviceRecord(IDBDeviceRecord* pDbRecord ) = 0;
        virtual BOOL ReadREGSERVER( IDBRegSrvRecord* pszCachRecord  ) = 0;
        virtual BOOL WriteREGSERVER( IDBRegSrvItem* pDbItem  ) = 0;
        virtual BOOL ClearREGSERVER() = 0;
        virtual LONG NewRegSrvItem (IDBRegSrvItem** ppPolicyItem ) = 0;
        virtual LONG FreeRegSrvItem(IDBRegSrvItem* pPolicyItem ) = 0;
        virtual LONG NewRegSrvRecord(IDBRegSrvRecord** ppDbRecord ) = 0;
        virtual LONG FreeRegSrvRecord(IDBRegSrvRecord* pDbRecord ) = 0;
        virtual BOOL ReadSERVERLIST( IDBSrvListRecord* pszCachRecord ) = 0;
        virtual BOOL WriteSERVERLIST( IDBSrvListItem* pDbItem ) = 0;
        virtual BOOL ClearSERVERLIST() = 0;
        virtual LONG NewSrvListItem (IDBSrvListItem** ppPolicyItem ) = 0;
        virtual LONG FreeSrvListItem(IDBSrvListItem* pPolicyItem ) = 0;
        virtual LONG NewSrvListRecord(IDBSrvListRecord** ppDbRecord ) = 0;
        virtual LONG FreeSrvListRecord(IDBSrvListRecord* pDbRecord ) = 0;
        virtual BOOL ReadUser( IDBUserRecord* pszCachRecord, const char* pszUserId = NULL ) = 0;
        virtual BOOL WriteUser( IDBUserItem* pdbItem ) = 0;
        virtual BOOL DelUser( const char* pszUserId = NULL ) = 0;
        virtual LONG NewUserItem (IDBUserItem** ppPolicyItem ) = 0;
        virtual LONG FreeUserItem(IDBUserItem* pPolicyItem ) = 0;
        virtual LONG NewUserRecord(IDBUserRecord** ppDbRecord ) = 0;
        virtual LONG FreeUserRecord(IDBUserRecord* pDbRecord ) = 0;
        virtual BOOL ReadProduct( IDBProductRecord* pszCachRecord, const char* pszProductType = NULL, const char* pszProductName = NULL ) = 0;
        virtual BOOL WriteProduct( IDBProductItem* pszDbItem ) = 0;
        virtual BOOL DelProduct( const char* pszProductType = NULL, const char* pszProductName = NULL ) = 0;
        virtual LONG NewProductItem (IDBProductItem** ppPolicyItem ) = 0;
        virtual LONG FreeProductItem(IDBProductItem* pPolicyItem ) = 0;
        virtual LONG NewProductRecord(IDBProductRecord** ppDbRecord ) = 0;
        virtual LONG FreeProductRecord(IDBProductRecord* pDbRecord ) = 0;
        virtual BOOL ReadLocalConfig( IDBLocalInfoRecord* pszCachRecord, const char* pszUserId = NULL, const char* pszAccount = NULL, const char* pszType = NULL ) = 0;
        virtual BOOL WriteLocalConfig( IDbLocalInfoItem* pszDbItem ) = 0;
        virtual BOOL DelLocalConfig( const char* pszUserId = NULL, const char* pszAccount = NULL, const char* pszType = NULL ) = 0;
        virtual LONG NewLocalInfoItem (IDbLocalInfoItem** ppPolicyItem ) = 0;
        virtual LONG FreeLocalInfoItem(IDbLocalInfoItem* pPolicyItem ) = 0;
        virtual LONG NewLocalInfoRecord(IDBLocalInfoRecord** ppDbRecord ) = 0;
        virtual LONG FreeLocalInfoRecord(IDBLocalInfoRecord* pDbRecord ) = 0;
        virtual HANDLE SQLOpen(const char* pcszDbPath, LPVOID pParam = NULL) = 0;
        virtual void SQLClose(HANDLE hDbCtx) = 0;
        virtual LONG SQLExecute(HANDLE hDbCtx, const char* pcszSql, MBC_DB_PARAM_TYPE_PTR pData, ULONG uDataCount, ULONG uTimeOut = MBC_DB_DEFAULT_TIMEOUT) = 0;
        virtual LONG SQLRead(HANDLE hDbCtx, const char* pcszSql, MBC_DB_PARAM_TYPE_PTR pData, ULONG uDataCount, IDBRecord* ppDbRecord, ULONG uTimeOut = MBC_DB_DEFAULT_TIMEOUT) = 0;
        virtual LONG NewDBItem (IDBItem** ppDbItem ) = 0;
        virtual LONG FreeDBItem(IDBItem* pDbItem ) = 0;
        virtual LONG NewDbRecord(IDBRecord** ppDbRecord ) = 0;
        virtual LONG FreeDbRecord(IDBRecord* pDbRecord ) = 0;
        virtual const char* GetProductList() = 0;
        virtual long GetProductListEx(char * pBuf, unsigned long* nLen) = 0;
        virtual BOOL SetBindUser(const char* pszUserInfo) = 0;
        virtual BOOL ModifyDevice(IDbDeviceItem* pDbItem) = 0;
        virtual BOOL ReadPolicy(IDBPolicyRecord * pszPolicyRecord, char const * pszPolicyId = NULL) = 0;
        virtual BOOL WritePolicy(IDBPolicyItem * pszPolicyItem) = 0;
        virtual BOOL DelPolicy(const char* pszPolicyId = NULL) = 0;
        virtual LONG NewPolicyItem(IDBPolicyItem** ppPolicyItem) = 0;
        virtual LONG FreePolicyItem(IDBPolicyItem * pPolicyItem) = 0;
        virtual LONG NewPolicyRecord(IDBPolicyRecord** ppPolicyRecord) = 0;
        virtual LONG FreePolicyRecord(IDBPolicyRecord * pPolicyRecord) = 0;
    };


	/*
	* 函数名称：MBC_WritePolicy
	* 功能    ：写或者更新策略缓存
	* 参数    ：pszCachItem   ,传入要写入或要更新的记录 
	* 返回值  ：成功 TRUE
	*            其他,失败
	*/

    inline BOOL MBC_WritePolicy(IDBPolicyItem * pszPolicyItem)
    {
        if(g_pfnMainBusCall == NULL)
        {
            return FALSE;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x01000801, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return FALSE;
            }
            ISafeStorePublish *pStorePublish  = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->WritePolicy(pszPolicyItem);
            }
        }
        return FALSE;
    }

	/*
	* 函数名称：MBC_ReadPolicy
	* 功能    ：从缓存中读取策略
	* 参数    ：pszPolicyRecord ,返回符合条件的记录集
	*           pszPolicyId     ,策略Id, 如果为NULL表明获取所有策略
	* 返回值  ：成功 TRUE
	*            其他,失败
	*/
	inline BOOL MBC_ReadPolicy(IDBPolicyRecord * pszPolicyRecord, char const * pszPolicyId = NULL)
	{
        if(g_pfnMainBusCall == NULL)
        {
            return FALSE;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x01000802, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return FALSE;
            }
            ISafeStorePublish *pStorePublish  = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->ReadPolicy(pszPolicyRecord,pszPolicyId);
            }
        }
        return FALSE;
	}

	/*
	* 函数名称：MBC_DelPolicy
	* 功能    ：从缓存中删除策略
	* 参数    ：pszPolicyId     ,策略Id
	* 返回值  ：成功 TRUE
	*            其他,失败
	*/
	inline BOOL MBC_DelPolicy(const char* pszPolicyId = NULL)
	{
        if(g_pfnMainBusCall == NULL)
        {
            return FALSE;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x01000803, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return FALSE;
            }
            ISafeStorePublish *pStorePublish  = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->DelPolicy(pszPolicyId);
            }
        }
        return FALSE;
	}

	/*
	* 函数名称：MBC_NewPolicyItem
	* 功能    ：分配策略表表项
	* 参数    ：ppPolicyItem, 输出参数,表项地址指针
	* 返回值  ：0   , 成功
	*           其他,失败
	*/
	inline long MBC_NewPolicyItem(IDBPolicyItem** ppPolicyItem)
	{
        if(g_pfnMainBusCall == NULL)
        {
            return 1;
        }
        IBaseComponent* pIBaseComponent = 0;
        long nRet = g_pfnMainBusCall(0x01000804, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return 1;
            }
            ISafeStorePublish *pStorePublish  = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->NewPolicyItem(ppPolicyItem);
            }
        }
        return 1;
	}

	/*
	* 函数名称：MBC_FreePolicyItem
	* 功能    ：释放策略表表项
	* 参数    ：pPolicyItem, 待释放的指针
	* 返回值  ：0   , 成功
	*           其他,失败
	*/
	inline long MBC_FreePolicyItem(IDBPolicyItem * pPolicyItem)
	{
        if(g_pfnMainBusCall == NULL)
        {
            return 1;
        }
        IBaseComponent* pIBaseComponent = 0;
        long nRet = g_pfnMainBusCall(0x01000805, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return 1;
            }
            ISafeStorePublish *pStorePublish  = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->FreePolicyItem(pPolicyItem);
            }
        }
        return 1;
	}

	/*
	* 函数名称：MBC_NewPolicyRecord
	* 功能    ：分配策略表记录集
	* 参数    ：ppPolicyRecord, 记录集地址指针
	* 返回值  ：0   , 成功
	*           其他,失败
	*/
	inline long MBC_NewPolicyRecord(IDBPolicyRecord** ppPolicyRecord )
	{
        if(g_pfnMainBusCall == NULL)
        {
            return 1;
        }
        IBaseComponent* pIBaseComponent = 0;
        long nRet = g_pfnMainBusCall(0x01000806, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return 1;
            }
            ISafeStorePublish *pStorePublish  = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->NewPolicyRecord(ppPolicyRecord);
            }
        }
        return 1;
	}

	/*
	* 函数名称：BC_FreePolicyRecord
	* 功能    ：释放策略表记录集
	* 参数    ：pPolicyRecord, 待释放记录集地址指针
	* 返回值  ：0   , 成功
	*           其他,失败
	*/
	inline long MBC_FreePolicyRecord(IDBPolicyRecord * pPolicyRecord)
	{
        if(g_pfnMainBusCall == NULL)
        {
            return 1;
        }
        IBaseComponent* pIBaseComponent = 0;
        long nRet = g_pfnMainBusCall(0x01000807, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return 1;
            }
            ISafeStorePublish *pStorePublish  = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->FreePolicyRecord(pPolicyRecord);
            }
        }
        return 1;
	}

	/*
	* 函数名称：MBC_ReadGeneral
	* 功能  ：读取CEMS_GENERAL缓存的值
	* 参数  ：pszSection,表中Type值
	*         ：pszKey    ,表中Key中
	*         ：pszValue  ,表中Data值
	*           nLen      ,返回pszValue值长度
	* 返回值：  0,成功
	*           1,内存不足
	*           2,读取失败
	*/
	inline int MBC_ReadGeneral(const char* pszSection, const char* pszKey, char* pszValue, int& nLen)
	{
        if(g_pfnMainBusCall == NULL)
        {
            return 2;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x01000808, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return 2;
            }
            ISafeStorePublish* pStorePublish = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->ReadGeneral(pszSection,pszKey,pszValue,nLen);
            }
        }
        return 1;
	}

	/*
	* 函数名称：MBC_WriteGeneral
	* 功能  ：写CEMS_GENERAL缓存的值
	* 参数  ：pszSection,表中Type值
	*         ：pszKey    ,表中Key中
	*         ：pszValue  ,表中Data值
	*           nLen      ,pszValue值长度
	* 返回值：  0,成功
	*           1,失败
	*/
	inline int MBC_WriteGeneral(const char* pszSection, const char* pszKey, char* pszValue, int nLen)
	{
        //nLen;
        if(g_pfnMainBusCall == NULL)
        {
            return 1;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x01000809, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return 2;
            }
            ISafeStorePublish* pStorePublish = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->WriteGeneral(pszSection,pszKey,pszValue,nLen);
            }
        }
        return 1;
	}

	/*
	* 函数名称：MBC_DelGeneral
	* 功能  ：删除CEMS_GENERAL缓存指定的值
	* 参数  ：pszSection,表中Type值
	*         ：pszKey    ,表中Key中
	* 返回值：  0,成功
	*           1,失败
	*           -1,g_pfnMainBusCall 为 NULL
	*/
	inline int MBC_DelGeneral(const char* pszSection, const char* pszKey)
	{
        if(g_pfnMainBusCall == NULL)
        {
            return -1;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x0100080A, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return 2;
            }
            ISafeStorePublish* pStorePublish = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->DelGeneral(pszSection,pszKey);
            }
        }
        return 1;
	}

	/*
	* 函数名称：MBC_DelGeneral
	* 功能  ：清空CEMS_GENERAL缓存
	* 参数  ：
	* 返回值：  0,成功
	*           1,失败
	*           -1,g_pfnMainBusCall 为 NULL
	*/
	inline int MBC_ClearGeneral()
	{
        if(g_pfnMainBusCall == NULL)
        {
            return -1;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x0100080B, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return 2;
            }
            ISafeStorePublish* pStorePublish = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->ClearGeneral();
            }
        }
        return 1;
	}

	/*
	* 函数名称：MBC_ReadDevice
	* 功能  ：读取CEMS_DEVICE表缓存
	* 参数  ：pszCachRecord,读取的返回结果
	*       ：pszDeviceId  ,指定的设备Id
	*       ：pszUserId    ,指定的用户Id，当pszDeviceId, pszUserId 为 NULL是为读取所有的设备信息
	*                        
	* 返回值：  TRUE,成功
	*           其他,失败
	*/
    inline BOOL MBC_ReadDevice(IDBDeviceRecord * pszCachRecord, char const * pszDeviceId = NULL, char const * pszUserId = NULL)
    {
        if(g_pfnMainBusCall == NULL)
        {
            return FALSE;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x0100080C, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return FALSE;
            }
            ISafeStorePublish* pStorePublish = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->ReadDevice(pszCachRecord,pszDeviceId,pszUserId);
            }
        }
        return FALSE;
    }

	/*
	* 函数名称：MBC_WriteDevice
	* 功能  ：写CEMS_DEVICE表
	* 参数  ：pDbItem,数据库表记录项
	*                        
	* 返回值：  TRUE,成功
	*           其他,失败
	*/
	inline BOOL MBC_WriteDevice(IDbDeviceItem * pDbItem)
	{
        if(g_pfnMainBusCall == NULL)
        {
            return FALSE;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x0100080D, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return FALSE;
            }
            ISafeStorePublish* pStorePublish = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->WriteDevice(pDbItem);
            }
        }
        return FALSE;
	}


	/*
	* 函数名称：MBC_DelDevice
	* 功能  ：删除CEMS_DEVICE表指定表项缓存
	* 参数  ：pszDeviceId,设备Id
	*         pszUserId  ,用户Id
	*         如果pszDeviceId = NULL, pszUserId = NULL,则表示删除所有项
	*                        
	* 返回值：  TRUE,成功
	*           其他,失败
	*/
	inline BOOL MBC_DelDevice(char const * pszDeviceId = NULL, char const * pszUserId = NULL)
	{
        if(g_pfnMainBusCall == NULL)
        {
            return FALSE;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x0100080E, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return FALSE;
            }
            ISafeStorePublish* pStorePublish = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->DelDevice(pszDeviceId,pszUserId);
            }
        }
        return FALSE;
	}

	/*
	* 函数名称：MBC_NewDeviceItem
	* 功能    ：分配设备表表项
	* 参数    ：ppDevItem, 输出参数,表项地址指针
	* 返回值  ：0   , 成功
    *           其他,失败
    */
    inline long MBC_NewDeviceItem(DBUtil::IDbDeviceItem** ppDevItem)
    {
        if(g_pfnMainBusCall == NULL)
        {
            return -1;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x0100080F, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return 1;
            }
            ISafeStorePublish* pStorePublish = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->NewDeviceItem(ppDevItem);
            }
        }
        return 1;
    }

	/*
	* 函数名称：MBC_FreeDeviceItem
	* 功能    ：释放设备表表项
	* 参数    ：pDevItem, 待释放表项地址指针
	* 返回值  ：0   , 成功
	*           其他,失败
	*/
	inline long MBC_FreeDeviceItem(DBUtil::IDbDeviceItem* pDevItem)
	{
        if(g_pfnMainBusCall == NULL)
        {
            return -1;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x01000810, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return 1;
            }
            ISafeStorePublish* pStorePublish = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->FreeDeviceItem(pDevItem);
            }
        }
        return 1;
	}

	/*
	* 函数名称：MBC_NewDeviceRecord
	* 功能    ：分配设备表记录集
	* 参数    ：ppDevRecord, 表项地址指针
	* 返回值  ：0   , 成功
	*           其他,失败
	*/
	inline long MBC_NewDeviceRecord(DBUtil::IDBDeviceRecord** ppDevRecord)
	{
        if(g_pfnMainBusCall == NULL)
        {
            return -1;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x01000811, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return 1;
            }
            ISafeStorePublish *pStorePublish  = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->NewDeviceRecord(ppDevRecord);
            }
        }
        return 1;
	}

	/*
	* 函数名称：MBC_FreeDeviceRecord
	* 功能    ：释放设备表记录集
	* 参数    ：pDevRecord, 表项地址指针
	* 返回值  ：0   , 成功
	*           其他,失败
	*/
	inline long MBC_FreeDeviceRecord(IDBDeviceRecord* pDevRecord)
	{
        if(g_pfnMainBusCall == NULL)
        {
            return -1;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x01000812, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return 1;
            }
            ISafeStorePublish *pStorePublish  = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->FreeDeviceRecord(pDevRecord);
            }
        }
        return 1;
	}

	/*
	* 函数名称：MBC_ReadREGSERVER
	* 功能  ：读注册服务器列表缓存(CEMS_REGSERVER)
	* 参数  ：pCachRecord,返回结果记录集
	*                        
	* 返回值：  TRUE,成功
	*           其他,失败
	*/
	inline BOOL MBC_ReadREGSERVER(IDBRegSrvRecord* pCachRecord)
	{
        if(g_pfnMainBusCall == NULL)
        {
            return FALSE;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x01000813, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return FALSE;
            }
            ISafeStorePublish *pStorePublish  = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->ReadREGSERVER(pCachRecord);
            }
        }
        return FALSE;
	}

	/*
	* 函数名称：MBC_WriteREGSERVER
	* 功能  ：写册服务器列表缓存(CEMS_REGSERVER)
	* 参数  ：pDbItem,写入表项记录
	*                        
	* 返回值：  TRUE,成功
	*           其他,失败
	*/
	inline BOOL MBC_WriteREGSERVER(IDBRegSrvItem * pDbItem)
	{
        if(g_pfnMainBusCall == NULL)
        {
            return FALSE;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x01000814, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return FALSE;
            }
            ISafeStorePublish *pStorePublish  = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->WriteREGSERVER(pDbItem);
            }
        }
        return FALSE;
	}

	/*
	* 函数名称：MBC_ClearREGSERVER
	* 功能  ：清空服务器列表缓存及数据库(CEMS_REGSERVER)
	* 参数  ：
	*                        
	* 返回值：  TRUE,成功
	*           其他,失败
	*/
	inline BOOL MBC_ClearREGSERVER()
	{
        if(g_pfnMainBusCall == NULL)
        {
            return FALSE;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x01000815, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return FALSE;
            }
            ISafeStorePublish *pStorePublish  = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->ClearREGSERVER();
            }
        }
        return FALSE;
	}

	/*
	* 函数名称：MBC_NewRegSrvItem
	* 功能    ：分配注册服务器表表项
	* 参数    ：ppRegSrvItem, 表项地址指针
	* 返回值  ：0   , 成功
	*           其他,失败
	*/
	inline long MBC_NewRegSrvItem(IDBRegSrvItem** ppRegSrvItem)
	{
        if(g_pfnMainBusCall == NULL)
        {
            return -1;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x01000815, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return 1;
            }
            ISafeStorePublish *pStorePublish  = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->NewRegSrvItem(ppRegSrvItem);
            }
        }
        return 1;
	}

	/*
	* 函数名称：MBC_FreeRegSrvItem
	* 功能    ：释放注册服务器表表项
	* 参数    ：pRegSrvItem, 表项指针
	* 返回值  ：0   , 成功
	*           其他,失败
	*/
	inline long MBC_FreeRegSrvItem(DBUtil::IDBRegSrvItem* pRegSrvItem)
	{
        if(g_pfnMainBusCall == NULL)
        {
            return -1;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x01000817, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return 1;
            }
            ISafeStorePublish *pStorePublish  = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->FreeRegSrvItem(pRegSrvItem);
            }
        }
        return 1;
	}

	/*
	* 函数名称：MBC_NewRegSrvRecord
	* 功能    ：分配注册服务器表记录集
	* 参数    ：ppRegSrvRecord, 记录集地址指针
	* 返回值  ：0   , 成功
	*           其他,失败
	*/
	inline long MBC_NewRegSrvRecord(DBUtil::IDBRegSrvRecord** ppRegSrvRecord)
	{
        if(g_pfnMainBusCall == NULL)
        {
            return -1;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x01000818, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return 1;
            }
            ISafeStorePublish *pStorePublish  = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->NewRegSrvRecord(ppRegSrvRecord);
            }
        }
        return 1;
	}

	/*
	* 函数名称：MBC_FreeRegSrvRecord
	* 功能    ：释放注册服务器表记录集
	* 参数    ：pRegSrvRecord, 记录集指针
	* 返回值  ：0   , 成功
	*           其他,失败
	*/
	inline long MBC_FreeRegSrvRecord(DBUtil::IDBRegSrvRecord* pRegSrvRecord)
	{
        if(g_pfnMainBusCall == NULL)
        {
            return -1;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x01000819, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return 1;
            }
            ISafeStorePublish *pStorePublish  = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->FreeRegSrvRecord(pRegSrvRecord);
            }
        }
        return 1;
	}

	/*
	* 函数名称：MBC_ReadSERVERLIST
	* 功能  ：读备用服务器列表(CEMS_SERVERLIST)
	* 参数  ：pCachRecord,返回结果记录集
	*                        
	* 返回值：  TRUE,成功
	*           其他,失败
	*/
	inline BOOL MBC_ReadSERVERLIST(IDBSrvListRecord* pCachRecord)
	{
        if(g_pfnMainBusCall == NULL)
        {
            return -1;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x0100081A, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return 1;
            }
            ISafeStorePublish *pStorePublish  = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->ReadSERVERLIST(pCachRecord);
            }
        }
        return 1;
	}

	/*
	* 函数名称：MBC_WriteSERVERLIST
	* 功能  ：写备用服务器列表(CEMS_SERVERLIST)
	* 参数  ：pDbItem,写入表项记录
	*                        
	* 返回值：  TRUE,成功
	*           其他,失败
	*/
	inline BOOL MBC_WriteSERVERLIST(IDBSrvListItem* pDbItem)
	{
        if(g_pfnMainBusCall == NULL)
        {
            return FALSE;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x0100081B, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return FALSE;
            }
            ISafeStorePublish *pStorePublish  = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->WriteSERVERLIST(pDbItem);
            }
        }
        return FALSE;
	}

	/*
	* 函数名称：MBC_ClearSERVERLIST
	* 功能  ：清空备用服务器列表(CEMS_SERVERLIST)
	* 参数  ：
	*                        
	* 返回值：  TRUE,成功
	*           其他,失败
	*/
	inline BOOL MBC_ClearSERVERLIST()
	{
        if(g_pfnMainBusCall == NULL)
        {
            return FALSE;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x0100081C, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return FALSE;
            }
            ISafeStorePublish *pStorePublish  = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->ClearSERVERLIST();
            }
        }
        return FALSE;
    }

	/*
	* 函数名称：MBC_NewSrvListItem
	* 功能    ：分配备用服务器表表项
	* 参数    ：ppSrvListItem, 表项地址指针
	* 返回值  ：0   , 成功
	*           其他,失败
	*/
	inline long MBC_NewSrvListItem(DBUtil::IDBSrvListItem** ppSrvListItem)
	{
        if(g_pfnMainBusCall == NULL)
        {
            return -1;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x0100081D, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return 1;
            }
            ISafeStorePublish *pStorePublish  = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->NewSrvListItem(ppSrvListItem);
            }
        }
        return 1;
	}

	/*
	* 函数名称：BC_FreeSrvListItem
	* 功能    ：释放备用服务器表表项
	* 参数    ：pSrvListItem, 表项地址指针
	* 返回值  ：0   , 成功
	*           其他,失败
	*/
	inline long MBC_FreeSrvListItem(IDBSrvListItem* pSrvListItem)
	{
        if(g_pfnMainBusCall == NULL)
        {
            return -1;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x0100081E, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return 1;
            }
            ISafeStorePublish *pStorePublish  = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->FreeSrvListItem(pSrvListItem);
            }
        }
        return 1;
    }

	/*
	* 函数名称：MBC_NewSrvListRecord
	* 功能    ：分配备用服务器表记录集
	* 参数    ：ppSrvListRecord, 记录集地址指针
	* 返回值  ：0   , 成功
	*           其他,失败
	*/
	inline long MBC_NewSrvListRecord(IDBSrvListRecord** ppSrvListRecord)
	{
        if(g_pfnMainBusCall == NULL)
        {
            return -1;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x0100081F, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return 1;
            }
            ISafeStorePublish *pStorePublish  = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->NewSrvListRecord(ppSrvListRecord);
            }
        }
        return 1;
	}

	/*
	* 函数名称：BC_FreeSrvListRecord
	* 功能    ：释放备用服务器表记录集
	* 参数    ：pSrvListRecord, 记录集指针
	* 返回值  ：0   , 成功
	*           其他,失败
    */
    inline long MBC_FreeSrvListRecord(IDBSrvListRecord* pSrvListRecord)
    {
        if(g_pfnMainBusCall == NULL)
        {
            return -1;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x01000820, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return 1;
            }
            ISafeStorePublish *pStorePublish  = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->FreeSrvListRecord(pSrvListRecord);
            }
        }
        return 1;
    }

	/*
	* 函数名称：MBC_ReadUser
	* 功能  ：读取用户表(CEMS_USER)
	* 参数  ：pCachRecord, 结果记录集
	*         pszUserId    , 用户Id
	*                        
	* 返回值：  TRUE,成功
	*           其他,失败
	*/
	inline BOOL MBC_ReadUser(IDBUserRecord* pCachRecord, char const* pszUserId = NULL)
	{
        if(g_pfnMainBusCall == NULL)
        {
            return FALSE;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x01000821, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return FALSE;
            }
            ISafeStorePublish *pStorePublish  = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->ReadUser(pCachRecord,pszUserId);
            }
        }
        return FALSE;
    }

	/*
	* 函数名称：MBC_WriteUser
	* 功能  ：写用户表(CEMS_USER)
	* 参数  ：pdbItem, 写入表项
	*                        
	* 返回值：  TRUE,成功
	*           其他,失败
	*/
	inline BOOL MBC_WriteUser(IDBUserItem* pdbItem)
	{
        if(g_pfnMainBusCall == NULL)
        {
            return FALSE;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x01000822, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return FALSE;
            }
            ISafeStorePublish *pStorePublish  = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->WriteUser(pdbItem);
            }
        }
        return FALSE;
	}

	/*
	* 函数名称：MBC_DelUser
	* 功能  ：删除表项
	* 参数  ：pszUserId, 指定的删除表项的用户Id，如果为NULL 删除所有表项
	*                        
	* 返回值：  TRUE,成功
	*           其他,失败
	*/
	inline BOOL MBC_DelUser(char const* pszUserId = NULL)
	{
        if(g_pfnMainBusCall == NULL)
        {
            return FALSE;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x01000823, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return FALSE;
            }
            ISafeStorePublish *pStorePublish  = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->DelUser(pszUserId);
            }
        }
        return FALSE;
	}

	/*
	* 函数名称：MBC_NewUserItem
	* 功能    ：分配用户表表项
	* 参数    ：ppUserItem, 表项指针
	* 返回值  ：0   , 成功
	*           其他,失败
	*/
	inline long MBC_NewUserItem(IDBUserItem** ppUserItem)
	{
        if(g_pfnMainBusCall == NULL)
        {
            return -1;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x01000824, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return 1;
            }
            ISafeStorePublish *pStorePublish  = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->NewUserItem(ppUserItem);
            }
        }
        return 1;
	}

	/*
	* 函数名称：MBC_FreeUserItem
	* 功能    ：释放用户表表项
	* 参数    ：pUserItem, 表项指针
	* 返回值  ：0   , 成功
	*           其他,失败
	*/
	inline long MBC_FreeUserItem(IDBUserItem* pUserItem)
	{
        if(g_pfnMainBusCall == NULL)
        {
            return -1;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x01000825, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return 1;
            }
            ISafeStorePublish *pStorePublish  = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->FreeUserItem(pUserItem);
            }
        }
        return 1;
	}

	/*
	* 函数名称：MBC_NewUserRecord
	* 功能    ：分配用户表记录集
	* 参数    ：ppUserRecord, 记录集指针地址
	* 返回值  ：0   , 成功
	*           其他,失败
	*/
	inline long MBC_NewUserRecord(IDBUserRecord** ppUserRecord)
	{
        if(g_pfnMainBusCall == NULL)
        {
            return -1;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x01000826, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return 1;
            }
            ISafeStorePublish *pStorePublish  = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->NewUserRecord(ppUserRecord);
            }
        }
        return 1;
	}

	/*
	* 函数名称：MBC_FreeUserRecord
	* 功能    ：释放用户表记录集
	* 参数    ：pUserRecord, 记录集指针
	* 返回值  ：0   , 成功
	*           其他,失败
	*/
	inline long MBC_FreeUserRecord(IDBUserRecord* pUserRecord)
	{
        if(g_pfnMainBusCall == NULL)
        {
            return -1;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x01000827, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return 1;
            }
            ISafeStorePublish *pStorePublish  = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->FreeUserRecord(pUserRecord);
            }
        }
        return 1;
	}

	/*
	* 函数名称：MBC_ReadProduct
	* 功能  ：读取产品列表(Product_list)
	* 参数  ：pCachRecord , 返回结果记录集
	*         pszProductType, 产品类型
	*         pszProductName, 产品名称，如果pszProductType = NULL, pszProductName = NULL, 则表示获取所有产品
	* 返回值：  TRUE,成功
	*           其他,失败
	*/
	inline BOOL MBC_ReadProduct(IDBProductRecord* pCachRecord, char const* pszProductType = NULL, char const* pszProductName = NULL)
	{
        if(g_pfnMainBusCall == NULL)
        {
            return FALSE;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x01000828, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return FALSE;
            }
            ISafeStorePublish *pStorePublish  = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->ReadProduct(pCachRecord,pszProductType,pszProductName);
            }
        }
        return FALSE;
	}

	/*
	* 函数名称：MBC_WriteProduct
	* 功能  ：写产品列表(Product_list)
	* 参数  ：pDbItem, 写入表项
	* 返回值：  TRUE,成功
	*           其他,失败
	*/
	inline BOOL MBC_WriteProduct(IDBProductItem * pDbItem)
	{
        if(g_pfnMainBusCall == NULL)
        {
            return FALSE;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x01000829, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return FALSE;
            }
            ISafeStorePublish *pStorePublish  = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->WriteProduct(pDbItem);
            }
        }
        return FALSE;
	}

	/*
	* 函数名称：MBC_DelProduct
	* 功能  ：删除产品列表(Product_list) 指定项
	* 参数  ：pszProductType, 产品类型
	*         pszProductName, 产品名称 ， 如果pszProductType = NULL, 且 pszProductName = NULL, 则删除所项
	* 返回值：  TRUE,成功
	*           其他,失败
	*/
	inline BOOL MBC_DelProduct(char const * pszProductType = NULL, char const * pszProductName = NULL)
	{
        if(g_pfnMainBusCall == NULL)
        {
            return FALSE;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x0100082A, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return FALSE;
            }
            ISafeStorePublish *pStorePublish  = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->DelProduct(pszProductType,pszProductName);
            }
        }
        return FALSE;
	}

	/*
	* 函数名称：MBC_NewProductItem
	* 功能    ：分配产品表表项
	* 参数    ：ppProductItem, 表项指针地址
	* 返回值  ：0   , 成功
	*           其他,失败
	*/
	inline long MBC_NewProductItem(IDBProductItem** ppProductItem)
	{
        if(g_pfnMainBusCall == NULL)
        {
            return -1;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x0100082B, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return 1;
            }
            ISafeStorePublish *pStorePublish  = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->NewProductItem(ppProductItem);
            }
        }
        return 1;
	}

	/*
	* 函数名称：MBC_NewProductItem
	* 功能    ：释放产品表表项
	* 参数    ：pProductItem, 表项指针
	* 返回值  ：0   , 成功
	*           其他,失败
	*/
	inline long MBC_FreeProductItem(IDBProductItem* pProductItem)
	{
        if(g_pfnMainBusCall == NULL)
        {
            return -1;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x0100082C, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return 1;
            }
            ISafeStorePublish *pStorePublish  = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->FreeProductItem(pProductItem);
            }
        }
        return 1;
	}

	/*
	* 函数名称：MBC_NewProductRecord
	* 功能    ：分配产品表记录集
	* 参数    ：ppProductRecord, 记录集指针地址
	* 返回值  ：0   , 成功
	*           其他,失败
	*/
	inline long MBC_NewProductRecord(IDBProductRecord** ppProductRecord)
	{
        if(g_pfnMainBusCall == NULL)
        {
            return -1;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x0100082D, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return 1;
            }
            ISafeStorePublish *pStorePublish  = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->NewProductRecord(ppProductRecord);
            }
        }
        return 1;
	}

	/*
	* 函数名称：MBC_FreeProductRecord
	* 功能    ：释放产品表记录集
	* 参数    ：pProductRecord, 记录集指针
	* 返回值  ：0   , 成功
	*           其他,失败
	*/
	inline long MBC_FreeProductRecord(IDBProductRecord* pProductRecord)
	{
        if(g_pfnMainBusCall == NULL)
        {
            return -1;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x0100082E, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return 1;
            }
            ISafeStorePublish *pStorePublish  = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->FreeProductRecord(pProductRecord);
            }
        }
        return 1;
	}

	/*
	* 函数名称：MBC_ReadLocalConfig
	* 功能  ：读取终端配置信息(CEMS_TRAY)
	* 参数  ：pCachRecord, 返回记结果录集
	*         pszUserId    , 用户Id
	*         pszAccount   , 用户帐户
	*         pszType      , 终端配置key信息,如果pszUserId = NULL, 且 pszAccount = NULL,pszType = NULL 则读取所有项
	* 返回值：  TRUE,成功
	*           其他,失败
	*/
	inline BOOL MBC_ReadLocalConfig(IDBLocalInfoRecord * pCachRecord, char const* pszUserId = NULL, char const* pszAccount = NULL, char const* pszType = NULL)
	{
        if(g_pfnMainBusCall == NULL)
        {
            return FALSE;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x0100082F, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return FALSE;
            }
            ISafeStorePublish *pStorePublish  = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->ReadLocalConfig(pCachRecord,pszUserId,pszAccount,pszType);
            }
        }
        return FALSE;
	}

	/*
	* 函数名称：MBC_WriteLocalConfig
	* 功能  ：写终端配置信息(CEMS_TRAY)
	* 参数  ：pDbItem, 写入表项记录
	* 返回值：  TRUE,成功
	*           其他,失败
	*/
	inline BOOL MBC_WriteLocalConfig(IDbLocalInfoItem* pDbItem)
	{
        if(g_pfnMainBusCall == NULL)
        {
            return FALSE;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x01000830, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return FALSE;
            }
            ISafeStorePublish *pStorePublish  = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->WriteLocalConfig(pDbItem);
            }
        }
        return FALSE;
	}

	/*
	* 函数名称：MBC_DelLocalConfig
	* 功能  ：删除端配置信息(CEMS_TRAY)指定项
	* 参数  ：pszUserId,  用户Id
	*         pszAccount, 用户帐户
	*         pszType   , Key信息,如果 pszUserId, pszAccount, pszType 都为NULL，则清空所有终端配置信息
	* 返回值：  TRUE,成功
	*           其他,失败
	*/
	inline BOOL MBC_DelLocalConfig(char const* pszUserId = NULL, char const* pszAccount = NULL, char const* pszType = NULL)
	{
        if(g_pfnMainBusCall == NULL)
        {
            return FALSE;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x01000831, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return FALSE;
            }
            ISafeStorePublish *pStorePublish  = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->DelLocalConfig(pszUserId,pszAccount,pszType);
            }
        }
        return FALSE;
	}

	/*
	* 函数名称：MBC_NewLocalInfoItem
	* 功能    ：分配终端信息表项
	* 参数    ：ppLocalInfoItem, 表项指针地址
	* 返回值  ：0   , 成功
	*           其他,失败
	*/
	inline long MBC_NewLocalInfoItem(IDbLocalInfoItem** ppLocalInfoItem)
	{
        if(g_pfnMainBusCall == NULL)
        {
            return -1;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x01000832, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return 1;
            }
            ISafeStorePublish *pStorePublish  = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->NewLocalInfoItem(ppLocalInfoItem);
            }
        }
        return 1;
	}

	/*
	* 函数名称：MBC_FreeLocalInfoItem
	* 功能    ：释放终端信息表项
	* 参数    ：pLocalInfoItem, 表项指针
	* 返回值  ：0   , 成功
	*           其他,失败
	*/
	inline long MBC_FreeLocalInfoItem(DBUtil::IDbLocalInfoItem* pLocalInfoItem)
	{
        if(g_pfnMainBusCall == NULL)
        {
            return -1;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x01000833, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return 1;
            }
            ISafeStorePublish *pStorePublish  = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->FreeLocalInfoItem(pLocalInfoItem);
            }
        }
        return 1;
	}

	/*
	* 函数名称：MBC_NewLocalInfoRecord
	* 功能    ：分配终端信息记录集
	* 参数    ：ppLocalInfoRecord, 记录集指针地址
	* 返回值  ：0   , 成功
	*           其他,失败
	*/
	inline long MBC_NewLocalInfoRecord(IDBLocalInfoRecord** ppLocalInfoRecord)
	{
        if(g_pfnMainBusCall == NULL)
        {
            return -1;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x01000834, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return 1;
            }
            ISafeStorePublish *pStorePublish  = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->NewLocalInfoRecord(ppLocalInfoRecord);
            }
        }
        return 1;
	}

	/*
	* 函数名称：MBC_FreeLocalInfoRecord
	* 功能    ：释放终端信息记录集
	* 参数    ：pLocalInfoRecord, 记录集指针
	* 返回值  ：0   , 成功
	*           其他,失败
	*/
	inline long MBC_FreeLocalInfoRecord(IDBLocalInfoRecord* pLocalInfoRecord)
	{
        if(g_pfnMainBusCall == NULL)
        {
            return -1;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x01000835, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return 1;
            }
            ISafeStorePublish *pStorePublish  = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->FreeLocalInfoRecord(pLocalInfoRecord);
            }
        }
        return 1;
	}

	/*
	* 函数名称：MBC_SQLOpen
	* 功能    ：打开指定数据库，获取数据库操作句柄
	* 参数    ：pcszDbPath, 数据库路径，路径必须为UTF8格式字符串 如果传入数据库不包含"\\"会在名称前加入dll所在目录为数据库路径,
	*             pParam, 保留
	* 返回值  ：成功>0
	*             其他,失败
	*/
	inline HANDLE MBC_SQLOpen(const char* pcszDbPath, LPVOID pParam = NULL)
	{
        if(g_pfnMainBusCall == NULL)
        {
            return NULL;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x01000836, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return NULL;
            }
            ISafeStorePublish *pStorePublish  = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->SQLOpen(pcszDbPath,pParam);
            }
        }
        return NULL;
	}

	/*
	* 函数名称：SQLClose
	* 功能    ：关闭指定数据库，释放资源
	* 参数    ：hDbCtx,数据库句柄
	*/
	inline void MBC_SQLClose(HANDLE hDbCtx)
    {
        if(g_pfnMainBusCall == NULL)
        {
            return ;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x01000837, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return;
            }
            ISafeStorePublish *pStorePublish  = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                  pStorePublish->SQLClose(hDbCtx);
            }
        }
    }

	/*
	* 函数名称：MBC_SQLExecute
	* 功能    ：执行Sqlite更新语句
	* 参数    ：hDbCtx    , 打开的数据库句柄
	*             pcszSql   , 执行的Sql语句
	*             pData     , 格式化参数，对应Sql语句中带的?
	*             uDataCount, 格式化参数个数
	*             uTimeOut  , 超时时间
	* 返回值  ：成功0
	*             其他,失败
    */
    inline long MBC_SQLExecute(HANDLE hDbCtx, const char* pcszSql, MBC_DB_PARAM_TYPE_PTR pData, ULONG uDataCount, ULONG uTimeOut = MBC_DB_DEFAULT_TIMEOUT)
    {
        if(g_pfnMainBusCall == NULL)
        {
            return -1;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x01000838, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return 1;
            }
            ISafeStorePublish* pStorePublish = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return  pStorePublish->SQLExecute(hDbCtx,pcszSql,pData,uDataCount,uTimeOut);
            }
        }
        return 1;
    }

	/*
	* 函数名称：MBC_SQLRead
	* 功能    ：执行Sqlite更新语句
	* 参数    ：hDbCtx    , 打开的数据库句柄
	*             pcszSql   , 执行的Sql语句
	*             pData     , 格式化参数，对应Sql语句中带的?
	*             uDataCount, 格式化参数个数
	*             ppDbRecord , 获取到的记录集合
	*             uTimeOut  , 超时时间
	* 返回值  ：成功0
	*             其他,失败
	*/

	inline long MBC_SQLRead(HANDLE hDbCtx, const char* pcszSql, MBC_DB_PARAM_TYPE_PTR pData,ULONG uDataCount, IDBRecord* pDbRecord, ULONG uTimeOut = MBC_DB_DEFAULT_TIMEOUT)
    {
        if(g_pfnMainBusCall == NULL)
        {
            return -1;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x01000839, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return 1;
            }
            ISafeStorePublish* pStorePublish = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->SQLRead(hDbCtx,pcszSql,pData,uDataCount,pDbRecord,uTimeOut);
            }
        }
        return 1;
    }

	/*
	* 函数名称：MBC_NewDBItem
	* 功能    ：分配通用表项
	* 参数    ：ppDbItem, 表项地址指针
	* 返回值  ：0   , 成功
	*           其他,失败
	*/
	inline long MBC_NewDBItem(IDBItem** ppDbItem)
	{
        if(g_pfnMainBusCall == NULL)
        {
            return -1;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x0100083A, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return 1;
            }
            ISafeStorePublish* pStorePublish = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->NewDBItem(ppDbItem);
            }
        }
        return 1;
	}

	/*
	* 函数名称：MBC_FreeDBItem
	* 功能    ：释放通用表项
	* 参数    ：pDbItem, 表项指针
	* 返回值  ：0   , 成功
	*           其他,失败
	*/
	inline long MBC_FreeDBItem(IDBItem* pDbItem)
	{
        if(g_pfnMainBusCall == NULL)
        {
            return -1;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x0100083B, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return 1;
            }
            ISafeStorePublish* pStorePublish = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->FreeDBItem(pDbItem);
            }
        }
        return 1;
	}

	/*
	* 函数名称：MBC_NewDbRecord
	* 功能    ：分配通用记录集
	* 参数    ：ppDbRecord, 记录集指针地址
	* 返回值  ：0   , 成功
	*           其他,失败
	*/
	inline long MBC_NewDbRecord(IDBRecord** ppDbRecord)
	{
        if(g_pfnMainBusCall == NULL)
        {
            return -1;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x0100083C, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return 1;
            }
            ISafeStorePublish* pStorePublish = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->NewDbRecord(ppDbRecord);
            }
        }
        return 1;
	}

	/*
	* 函数名称：MBC_FreeDbRecord
	* 功能    ：释放通用记录集
	* 参数    ：pDbRecord, 记录集指针
	* 返回值  ：0   , 成功
	*           其他,失败
	*/
	inline long MBC_FreeDbRecord(IDBRecord * pDbRecord)
	{
        if(g_pfnMainBusCall == NULL)
        {
            return -1;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x0100083D, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return 1;
            }
            ISafeStorePublish* pStorePublish = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->FreeDbRecord(pDbRecord);
            }
        }
        return 1;
	}
		/*
	* Function Name : MBC_GetProductList
	* Parameter(s)  :
	* Return        : 成功 :
	*                 失败 :
	* Description   : 
    */
    inline char const * MBC_GetProductList()
    {
        if(g_pfnMainBusCall == NULL)
        {
            return "";
        }
        IBaseComponent* pIBaseComponent = 0;
        const char* pProductList = 0;
        int nRet = g_pfnMainBusCall(0x0100083E, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return "";
            }
            ISafeStorePublish* pStorePublish = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pProductList = pStorePublish->GetProductList();
            }
        }
        return "";
    }

    /*
	* Function Name : MBC_GetProductListEx
	* Parameter(s)  : pBuf,产品列表缓冲区
	*                 nLen, 输入pBuf 缓冲区大小，输出实际产品列表大小
	* Return        : 成功 : 0
	*                 失败 : 2,pBuf 空间不足,其他失败
	* Description   : 获取产品列表
    */
    inline long MBC_GetProductListEx( char * pBuf , unsigned long* nLen)
    {
        if(g_pfnMainBusCall == NULL)
        {
            return -1;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x0100083D, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return 1;
            }
            ISafeStorePublish* pStorePublish = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->GetProductListEx(pBuf,nLen);
            }
        }
        return 1;
    }
    
	/*
	* Function Name : MBC_SetBindUser
	* Parameter(s)  : pszUserInfo,输入绑定用户信息(UTF-8串)，格式 {"userid":"22f0f0425cb04fbaa0d14bed076eb0d8"}
	*                 
	* Return        : 成功 : TRUE
	*                 失败 : FALSE
	* Description   : 设置绑定用户
	*/
	inline BOOL  MBC_SetBindUser(const char* pszUserInfo )
	{
        if(g_pfnMainBusCall == NULL)
        {
            return FALSE;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x01000840, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return FALSE;
            }
            ISafeStorePublish* pStorePublish = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->SetBindUser(pszUserInfo);
            }
        }
        return FALSE;
	}

	/*
	* Function Name : MBC_SetBindUser
	* Parameter(s)  : pDbItem,设备信息记录集
	*                 
	* Return        : 成功 : TRUE
	*                 失败 : FALSE
	* Description   : 修改本地设备设备ID
	*/
    inline BOOL  MBC_ModifyDeviceId(IDbDeviceItem* pDbItem )
    {
        if(g_pfnMainBusCall == NULL)
        {
            return FALSE;
        }
        IBaseComponent* pIBaseComponent = 0;
        int nRet = g_pfnMainBusCall(0x01000841, &pIBaseComponent);
        if(nRet == 0)
        {
            if(pIBaseComponent == NULL)
            {
                return FALSE;
            }
            ISafeStorePublish* pStorePublish = dynamic_cast<ISafeStorePublish*>(pIBaseComponent);
            if(pStorePublish != NULL)
            {
                return pStorePublish->ModifyDevice(pDbItem);
            }
        }
        return FALSE;
    }

};  // namespace DBUtil

#endif
