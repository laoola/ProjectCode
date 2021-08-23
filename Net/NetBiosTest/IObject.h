#ifndef _I_EDP_OBJECT_H_2015_02_05_
#define _I_EDP_OBJECT_H_2015_02_05_

#include <stdio.h>
#include <assert.h>
#include "wintolinuxdef.h"


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //
//
//                    函数接口 定义
//

class IBaseComponent ;
class IEdpObject;

typedef ULONG_PTR (*PFN_MAIN_BUS_CALL) (ULONG ServiceID,IBaseComponent** pBaseComponent);
extern  PFN_MAIN_BUS_CALL  g_pfnMainBusCall ;


// 需要在 DllMain.cpp 中调用此宏，初始化定义MainBusCall函数指针。
#define MAIN_BUS_CALL_INIT()  PFN_MAIN_BUS_CALL  g_pfnMainBusCall = NULL




typedef LONG (WINAPI * PFN_QUERY_INTERFACE) (IN ULONG_PTR uID, OUT IEdpObject ** pObject) ;

// ------------------------------------------------------------------------ //




// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //
//
//                    全部对象基接口
//
class IEdpObject
{
public:

    /*
    * Function Name : SetType
    * Parameter(s)  : void
    * Return        : void
    * Description   : 设置插件类型
    */
    virtual void  SetType ( LONG ) = 0;


    /*
    * Function Name : GetType
    * Parameter(s)  : void
    * Return        : LONG 返回组件type
    * Description   : 得到当前组件插件类型
    */
    virtual LONG  GetType () const = 0;

    /*
    * Function Name : AddRef
    * Parameter(s)  : 无
    * Return        : 返回当前引用计数
    * Description   : 增加对象引用计数
    */
    virtual LONG AddRef() = 0 ;


    /*
    * Function Name : Initialize
    * Parameter(s)  : void
    * Return        : 成功：0    失败：其他
    * Description   : 初始化当前接口
    */
    virtual LONG  Initialize () = 0;


    /*
    * Function Name : Clearup
    * Parameter(s)  : void
    * Return        : 无
    * Description   : 释放对象申请的资源
    */
    virtual void  Clearup () = 0;


    /*
    * Function Name : Release
    * Parameter(s)  : 无
    * Return        : 返回当前引用计数
    * Description   : 减少引用计数，若计数为0 则对象内部删除自身。
    */
    virtual LONG Release() = 0 ;
    
    /*
    * Function Name : GetVersion
    * Parameter(s)  : 无
    * Return        : 返回当前对象版本号
    * Description   : 返回当前对象版本号，从0开始。
    */
    virtual LONG GetVersion () = 0 ;

    /*
    * Function Name : SetMainBusCallAddress
    * Parameter(s)  : PFN_MAIN_BUS_CALL  总线请求函数地址
    * Return        : 无
    * Description   : 
    */
    virtual void  SetMainBusCallAddress ( IN PFN_MAIN_BUS_CALL ) = 0;


    /*
    * Function Name : EventNotify
    * Parameter(s)  : EdpMsgType:
    *                 MajorType:
    *                 wParam:
    *                 lParam:
    * Return        : 0：成功，其他：错误代码
    * Description   : 消息通知，把组件关心的消息通过此接口通知给组件对象。
    */
    virtual LONG EventNotify ( ULONG EdpMsgType, ULONG MajorType, ULONG_PTR wParam, ULONG_PTR lParam )  = 0 ;
    

    /*
    * Function Name : SetCallBack
    * Parameter(s)  : pParentObj: 回调对象指针
    *                 bRemove:    TRUE: 添加回调接口
    *                             FALSE:移除回调接口
    * Return        : void
    * Description   : 设置回调通知接口
    */
    virtual void SetCallBack ( IEdpObject * pParentObj, BOOL bRemove )  = 0 ;

    
    /*
    * Function Name : SetCallBack
    * Parameter(s)  : EdpMsgType:  主消息类型
    *                 MajorType:  子消息类型
    *                 pParentObj: 回调对象指针
    *                 bRemove:    TRUE: 添加回调接口
    *                             FALSE:移除回调接口
    * Return        : void
    * Description   : 设置回调通知接口
    */
    virtual void SetCallBack ( ULONG MajorType, ULONG MinorType, IEdpObject * pParentObj, BOOL bRemove )  = 0 ;

} ;

// ------------------------------------------------------------------------ //





// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //
//
//                    全部对象基类
//
template<class T, class I = IEdpObject>
class CEdpObjectImpl : public I
{
public:
    CEdpObjectImpl() { m_RefCount = 1; m_CurrentVersion = 0; m_lComID = -1; }
    virtual ~CEdpObjectImpl(){}
public:
    virtual void SetType ( LONG lComID )
    {
        m_lComID = lComID;
    }

    virtual LONG GetType () const
    {
        return m_lComID;
    }

    virtual LONG AddRef() 
    {
        return __sync_add_and_fetch(&m_RefCount,1);
        //return InterlockedIncrement(&m_RefCount) ;
    } 

    virtual LONG  Initialize ()
    {
        return 0 ;
    }

    virtual void  Clearup ()
    {
    }

    virtual LONG Release() 
    { 
        LONG lRetVal = __sync_sub_and_fetch(&m_RefCount,1);//InterlockedDecrement(&m_RefCount) ;
        if ( 0 == lRetVal)
        {
            Clearup () ;
            delete (T*) this ;
        }
#ifdef _DEBUG
        else if ( 0 > lRetVal )
        {
            assert(0) ;
        }
#endif // _DEBUG

        return lRetVal ;
    }

    virtual LONG GetVersion ()
    {
        return m_CurrentVersion ;
    }

public:
    virtual void  SetMainBusCallAddress ( IN PFN_MAIN_BUS_CALL pfnMainBusCall )
    {
		assert(pfnMainBusCall);
        if ( pfnMainBusCall!= NULL )
		{
			 ::g_pfnMainBusCall = pfnMainBusCall ;
		}
    }

    virtual LONG EventNotify ( ULONG EdpMsgType, ULONG MajorType, ULONG_PTR wParam, ULONG_PTR lParam )
    {
        UNREFERENCED_PARAMETER(EdpMsgType) ;
        UNREFERENCED_PARAMETER(MajorType) ;
        UNREFERENCED_PARAMETER(wParam) ;
        UNREFERENCED_PARAMETER(lParam) ;
        return 0 ;
    }

    virtual void SetCallBack ( IEdpObject * pParentObj, BOOL bRemove )
    {
        UNREFERENCED_PARAMETER(pParentObj) ;
        UNREFERENCED_PARAMETER(bRemove) ;
    }

    virtual void SetCallBack ( ULONG MajorType, ULONG MinorType, IEdpObject * pParentObj, BOOL bRemove )
    {
        UNREFERENCED_PARAMETER(MajorType) ;
        UNREFERENCED_PARAMETER(MinorType) ;
        UNREFERENCED_PARAMETER(pParentObj) ;
        UNREFERENCED_PARAMETER(bRemove) ;
    }
protected:
    LONG  m_CurrentVersion ;

    // 组件ID
    LONG  m_lComID ;
private:
    LONG  m_RefCount ;
} ;

// ------------------------------------------------------------------------ //



#endif // _I_EDP_OBJECT_H_2015_02_05_
