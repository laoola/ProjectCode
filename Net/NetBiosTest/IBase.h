#ifndef _IBASE_H_2020_05_26_
#define _IBASE_H_2020_05_26_

#ifdef WIN32
#pragma once
#pragma warning(disable:4127) // do { }while (FALSE);
#endif

#ifndef WIN32
#include "wintolinuxdef.h"
#endif

#include "IObject.h"
#include "EdpBaseDefine.h"
#include <string>
#include <list>
#include <map>
#include <typeinfo>


using namespace std;


#ifdef _UNICODE
#define _tstring	std::string
#define _tostring	std::wostringstream
#define _tcsmemcpy_s	wmemcpy_s
#else
#define _tstring	std::string
#define _tostring	std::ostringstream
#define _tcsmemcpy_s	memcpy_s
#endif

//////////////////////////////////////////////////////////////////////////
//
//                       MACRO DEFINE
//
//////////////////////////////////////////////////////////////////////////

// 所有功能组件基类
#define EDP_OBJ_BASE_CLASS(T) CBaseComponentImpl< T, CEdpObjectImpl< T, IBaseComponent > >
#define EDP_OBJ_CLASS_IMPL(T, I) CBaseComponentImpl< T, CEdpObjectImpl< T, I > >


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //
//
//  定义 模块中所发布函数
//

// 开始定义 发布函数
#define BEGIN_SERVICE_DEF(T) \
public:\
    virtual PUBLISH_SERVICE_INFO_PTR QueryPublish() \
{\
    typedef T	  thisClass;\
    static const  ID_SERVICE_INFO Map[]= {


// 添加发布 “类成员函数” 定义
#define DEF_SERVICE_NAME(ID, N)	\
{ ID, ANY_CAST<FARPROC>(&thisClass::N), #N, typeid(&thisClass::N).name() },

#define DEF_SERVICE_TYPE_NAME(ID, T, N, SUFFIX)	\
{ ID, ANY_CAST<FARPROC, T>(&thisClass::N), #N#SUFFIX, typeid((T)&thisClass::N).name() },

// 添加发布 “全局函数” 定义
#define DEF_GLOBAL_SERVICE_NAME(ID, N)	\
{ ID, ANY_CAST<FARPROC>(&N), #N, typeid(&N).name() },


// 结束定义 发布函数
#define END_SERVICE_DEF() \
};	\
    static PUBLISH_SERVICE_INFO publish = { _countof(Map), (ID_SERVICE_INFO_PTR)Map }; \
    return &publish;\
}
// ------------------------------------------------------------ //

#define BEGIN_OBJECT_ID() \
static const ULONG_PTR g_ulObjectIds[] = \
{

#define	OBJECT_ID(id)    id, 

#define END_OBJECT_ID() \
	0\
}; \
*pObject = (IEdpObject*)g_ulObjectIds; \
return ERROR_SUCCESS;

//////////////////////////////////////////////////////////////////////////
//
//                       TYPE DEFINE
//
//////////////////////////////////////////////////////////////////////////

// 
#pragma pack(1)


typedef struct _ID_SERVICE_INFO
{
    ULONG   SubID;
    FARPROC Produce;
    LPCSTR  FuncName;
    LPCSTR  FuncDefine;
}ID_SERVICE_INFO, *ID_SERVICE_INFO_PTR;
#define ID_SERVICE_INFO_LEN sizeof(ID_SERVICE_INFO)

typedef struct _PUBLISH_SERVICE_INFO
{
    ULONG				Count;
    ID_SERVICE_INFO_PTR Info;
}PUBLISH_SERVICE_INFO, *PUBLISH_SERVICE_INFO_PTR;
#define PUBLISH_SERVICE_INFO_LEN sizeof(PUBLISH_SERVICE_INFO) 

//////////////////////////////////////////////////////////////////////////
// ANY_CAST 类型转换 PVOID t = ANY_CAST<PVOID>(&CTest::Any);
//////////////////////////////////////////////////////////////////////////

template< class T, class F >
inline T ANY_CAST(const F &f)
{
    union
    {
        T To;
        F From;
    }t;

    t.From = f;

    return t.To;
};

#pragma pack()


//////////////////////////////////////////////////////////////////////////
//
//                       PUBLIC INTERFACE
//
//////////////////////////////////////////////////////////////////////////



/*////////////////////////////////////////////////////////////////////////
Class Name  : IBaseComponent
Description : 所有组件基接口
////////////////////////////////////////////////////////////////////////*/
class IBaseComponent : public IEdpObject
{
public:
    /*
    * Function Name : Start
    * Parameter(s)  : void
    * Return        : 成功：0    失败：其他
    * Description   : 启动组件功能
    */
    virtual LONG  Start () = 0;


    /*
    * Function Name : Stop
    * Parameter(s)  : void
    * Return        : 成功：0    失败：其他
    * Description   : 停止组件功能
    */
    virtual LONG  Stop () = 0;

public:

    /*
    * Function Name : QueryPublish
    * Parameter(s)  : void
    * Return        : NULL:   模块中没有要发布的服务
    *                 非NULL: 模块中发布的所有服务函数
    * Description   : 
    */
    virtual PUBLISH_SERVICE_INFO_PTR  QueryPublish () = 0;
};


/*////////////////////////////////////////////////////////////////////////
Class Name  : IBaseDbgMsg
Description : 调试日志对象基接口
////////////////////////////////////////////////////////////////////////*/
class IBaseDbgMsg : public IBaseComponent
{
public:

    ////////////////////////////////////////////////////////////////////////////////
    //函数名：IsDebug()
    //
    //功能  ：是否打印日志
    //			
    //参数  ：moduleId:模块ID  curLevel：输出等级
    //
    //返回值：TRUE：打印日志； FALSE:不打印日志
    ////////////////////////////////////////////////////////////////////////////////
    virtual int	IsDebug( int moduleId,int curLevel )= 0;

    ////////////////////////////////////////////////////////////////////////////////
    //函数名：WriteLogA()
    //
    //功能  ：写日志
    //
    //参数  ：moduleId:模块ID  curLevel：输出等级 file：文件名 line：行数 fmt：输入参数
    //
    //返回值：
    ////////////////////////////////////////////////////////////////////////////////
    virtual void	WriteLogA(int moduleId,int curLevel,const char *file,int line, const char *fmt, va_list ap)= 0 ;
    virtual void	WriteLogW(int moduleId,int curLevel,const char *file,int line, const char *fmt, va_list ap)= 0 ;

    ////////////////////////////////////////////////////////////////////////////////
    //函数名：WriteLogA()
    //
    //功能  ：写日志
    //
    //参数  ：moduleId:模块ID  curLevel：输出等级 file：文件名 line：行数 fmt：输入参数
    //
    //返回值：
    ////////////////////////////////////////////////////////////////////////////////
    virtual void	WriteLogExA(int moduleId,int curLevel,LPSTR file,int line, LPSTR  fmt, ...) = 0;
    ////////////////////////////////////////////////////////////////////////////////
    //函数名：ResetEvent()
    //
    //功能  ：触发消息重新初始化输出设置
    //
    //参数  ：
    //
    //返回值：
    ////////////////////////////////////////////////////////////////////////////////
    virtual	void    ResetEvent( LPSTR lpName) = 0;
} ;



/*////////////////////////////////////////////////////////////////////////
Class Name  : IBaseMainBus
Description :总线组件基接口
////////////////////////////////////////////////////////////////////////*/
class IBaseMainBus : public IBaseComponent
{
public:
    
    /*
    * Function Name : SetDbgObject
    * Parameter(s)  : pDbgObj 调试日志对象指针
    * Return        : 无
    * Description   : 发布调试日志功能到总线
    */
    virtual void SetDbgObject ( IBaseComponent * pDbgObj ) = 0 ;
    
    /*
    * Function Name : SetWorkMode
    * Parameter(s)  : uMode  当前工作在 VPEngine or vUserHci
    * Return        : 无
    * Description   : 设置工作模式
    */
    virtual void  SetWorkMode (ULONG uMode) = 0;

    /*
    * Function Name : GetMainBusCallAddress
    * Parameter(s)  : 无
    * Return        : 总线函数地址指针
    * Description   : 获取总线调用函数地址
    */

    virtual PFN_MAIN_BUS_CALL GetMainBusCallAddress() = 0;

    /*
    * Function Name : SetWorkingPath
    * Parameter(s)  : f_WorkPath : 工作目录
    * Return        : 无
    * Description   : 设置 vMainbus.dll 工作路径(目录下必须有 vMbusMod.xml)
    */

    virtual void SetWorkingPath(LPCSTR f_WorkPath) = 0;
    //virtual void SetWorkingPath(LPCSTR f_WorkPath) = 0;
} ;

class ICfgToolBase : public IBaseComponent
{
public:
    virtual BOOL EncryptFile(LPCSTR f_Source, LPCSTR f_Dest) = 0;
    virtual BOOL DncryptFile(LPCSTR f_Source, LPCSTR f_Dest) = 0;
	
    //virtual BOOL EncryptFile(LPCSTR f_Source, LPCSTR f_Dest) = 0;
    //virtual BOOL DncryptFile(LPCSTR f_Source, LPCSTR f_Dest) = 0;

    virtual LPSTR LoadToMem(LPCSTR f_FileName) = 0;
    //virtual LPSTR LoadToMem(LPCSTR f_FileName) = 0;
	
    virtual BOOL FreeMem(LPSTR f_Address) = 0;
};

template<class T, class I>
class CBaseComponentImpl : public I
{
public:
    CBaseComponentImpl ()
    {
    }

public:
    virtual LONG  Start ()
    {
        return 0 ;
    }
    virtual LONG  Stop ()
    {
        return 0 ;
    }

public:
    virtual PUBLISH_SERVICE_INFO_PTR  QueryPublish ()
    {
        return (PUBLISH_SERVICE_INFO_PTR) (NULL) ;
    }
} ;

#endif
