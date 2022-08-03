#pragma once
#include <libudev.h>
#include "usb_device.h"
#include "log.h"
#include "UsbControl.h"
#include <map>
#ifdef OS_BOOST
#include <boost/thread.hpp>
#define PMUTEX_LOCK(mutex) boost::mutex::scoped_lock lock(mutex)
#else
#include <pthread.h>
#include <errno.h>
class os_auto_lock
{
public:
	os_auto_lock(pthread_mutex_t& mutex) :m_mutex(mutex) { pthread_mutex_lock(&m_mutex); }
	~os_auto_lock() { pthread_mutex_unlock(&m_mutex); }
private:
	pthread_mutex_t& m_mutex;
};
#define PMUTEX_LOCK(mutex) os_auto_lock lock(mutex)
#endif
#include <list>
class CUsbMg
{
public:
	CUsbMg();
	virtual ~CUsbMg();

	void ControlCurUsb();
	//添加白名单设备
	void AddWhiteUsb(USB_DEVICE* usb);
	//移除白名单设备
	void DelWhiteUsb(USB_DEVICE* usb);
	//清空白名单设备
	void ClsWhiteUsb();
	//判断设备是否在可信名单中
	bool IsUsbWhite(USB_DEVICE* usbdev, bool bsync);

	//新增了USB设备
	void AddUsbDevice(udev_device* usb, bool bsync);
	//移除了USB设备
	void RemoveUsbDevice(udev_device* usb);

	//关闭功能
	void CloseAudit() { LOG_INFO() << "Usb Close";  m_isAudit = -1; }
	//开启审计模式
	void OpenAudit() { LOG_INFO() << "Usb switch to audit mode";  m_isAudit = 0; }
	//开启保护模式
	void OpenProtected() { LOG_INFO() << "Usb switch to protected mode";  m_isAudit = 1; }
	//开启学习模式
	void OpenStudy() { LOG_INFO() << "Usb switch to study mode";  m_isAudit = 2; }


	//生成usb管控列表
	bool GenUsbControlList();
private:
	//初始化当前已有的设备
	void InitCurUsb();
	void ControlDevice(USB_DEVICE* usbdev, bool bsync);
	//停止设备
	void StopDevice(std::string name, std::string drvpath);
	//恢复设备
	void ResumeDevice(std::string name, std::string drvpath);
	//获取设备的信息
	void get_sysattr_info(udev_device* usb, USB_DEVICE& data);
	//获取当前USB设备
	void get_cursys_usb(udev* udev, std::map<std::string, USB_DEVICE>& lsDevice);

private:
	//通过设备路径获取挂载路径
	bool get_mountpath_by_devpath(std::string& mpath, const std::string& devpath);

public:
	bool add_usb_control_list(USB_DEVICE& data);

private:
	std::list<USB_DEVICE>	m_lsWhiteUsb;	//白名单USB设备(U盘，手机)
	std::map<std::string, USB_DEVICE>	m_lsAllUsbDev;	//当前电脑所有USB设备(U盘，手机)
	int m_isAudit;										//当前模式 0审计 1保护 2学习
	UsbControl m_usbControl;

#ifdef OS_BOOST
	boost::mutex m_mutex;
#else
	pthread_mutex_t m_mutex;
#endif
};
