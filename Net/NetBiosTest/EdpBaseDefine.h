#ifndef _EDP_BASE_DEFINE_H_2015_03_26_
#define _EDP_BASE_DEFINE_H_2015_03_26_

#define  FaultValue -1
#define  SucessValue 1
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //
//
// 各模块导出函数接口定义
//
#define EdpQueryInterface	V13712888C8D42EB864879ADF36F8A20
#define EDP_QI_NAME		    "V13712888C8D42EB864879ADF36F8A20"
#define MainBusCall		    V385A33449534892BFDFE9EEE658CACC
#define EDP_MBC_NAME	    "V385A33449534892BFDFE9EEE658CACC"
// ------------------------------------------------------------ //




// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //
//
//                      配置文件、数据库文件名定义
//

//
// A 版本
//
#define EDP_MAIN_BUS_MOD_CFG_NAME_A			"vmbusmod.xml"      // 总线加载模块配置文件
#define EDP_CFG_NAME_A                      "vcltcfg.xml"       // 产品配置文件名

//
// _T 版本
//
#define EDP_MAIN_BUS_MOD_CFG_NAME      _T("vmbusmod.xml")     // 总线加载模块配置文件
#define EDP_CFG_NAME                   _T("vcltcfg.xml")      // 产品配置文件名

// ------------------------------------------------------------ //




// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //
//
//                      调试输出Level
//
//  用于 MBC_DBG_OUTPUT()、MBC_DBG_OUTPUT() 调试输出功能的 DbgLevel 参数
//  函数声明详见 MainBusCall.h

#define  VDBG_LEVEL_DEBUG               0x010000	// 调试
#define  VDBG_LEVEL_INFO                0x020000	// 信息
#define  VDBG_LEVEL_WARN                0x040000	// 警告
#define  VDBG_LEVEL_ERROR               0x080000	// 错误
#define  VDBG_LEVEL_FATAL               0x100000    // 崩溃

// 
// 打印级别扩展级别：
// 用于增加分类扩展，_T输入子级别号。
//
// 例如：
// MBC_DBG_OUTPUT (3000, LOG_LEVEL_DEBUG_EX(10), _T("调试日志，设置等级低于错误等级输出此日志，线程ID=%d"),gettid() );
//
// 上面代码含义：模块号 3000，输出调试日志，调试日志的子功能号为10
// 只有在配置文件中设置了 Exlevel=“0”或者 Exlevel=“10”，并且level <= 1 的时候才会打印此日志

#define  VDBG_LEVEL_DEBUG_EX(_T)        (VDBG_LEVEL_DEBUG|_T)       //调试
#define  VDBG_LEVEL_INFO_EX(_T)         (VDBG_LEVEL_INFO |_T)       //信息
#define  VDBG_LEVEL_WARN_EX(_T)         (VDBG_LEVEL_WARN |_T)       //警告
#define  VDBG_LEVEL_ERROR_EX(_T)        (VDBG_LEVEL_ERROR|_T)       //错误
#define  VDBG_LEVEL_FATAL_EX(_T)        (VDBG_LEVEL_FATAL|_T)       //崩溃

// ------------------------------------------------------------ //





// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //
//
// 组件工作模式
//
typedef enum _VP_COMP_WORK_MODE
{
    VWM_UNKNOW     = 0x0000,  // 未设置 工作模式

    VWM_ENGINE             ,  // 当前在 策略引擎     进程中
    VWM_USER_HCI           ,  // 当前在 用户交互     进程中
    VWM_WATCH              ,  // 当前在 守护         进程中
    VWM_SETUP              ,  // 当前在 安装/卸载    进程中
    VWM_SVC_X64            ,  // 当前在 64位功能服务 进程中
    VWM_CHILD_PROCESS      ,  //当前在子产品 进程中
    VWM_MAX_VALUE             // 
} VP_COMP_WORK_MODE;
// ------------------------------------------------------------ //





#endif // _EDP_BASE_DEFINE_H_2015_03_26_
