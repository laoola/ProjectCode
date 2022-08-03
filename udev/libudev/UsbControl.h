#pragma once
#include "usb_device.h"
#include <stdio.h>

class UsbControl
{
public:
	UsbControl();
	~UsbControl();
public:
	bool AddUsbControlList(USB_DEVICE& data);
	bool SaveUsbControlList();
	bool ClearUsbControlList();
public:
	//通过syspath路径获取设备路径
	bool GetDevpathBySyspath(std::string& devpath, const std::string& syspath);

private:
	bool OpenUsbControlList(const std::string& mode);
	void CloseUsbControlList();
	bool NotifyUpdateControlList();
public:
	//通过syspath路径获取设备路径
	bool get_devpath_by_syspath(std::string& devpath, const std::string& syspath);
	bool get_devpath_by_syspath2(std::string& devpath, const std::string& syspath);
private:
	FILE* fp;
	pthread_mutex_t m_mutex;
};
