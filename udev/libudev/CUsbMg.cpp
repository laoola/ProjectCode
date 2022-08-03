#include "CUsbMg.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

CUsbMg::CUsbMg()
{
	m_isAudit = false;
#ifndef OS_BOOST
	if (pthread_mutex_init(&m_mutex, NULL))
	{
	}
#endif
}
CUsbMg::~CUsbMg()
{
#ifndef OS_BOOST
	pthread_mutex_destroy(&m_mutex);
#endif
}
void CUsbMg::ControlCurUsb()
{
	InitCurUsb();
}
//添加白名单设备
void CUsbMg::AddWhiteUsb(USB_DEVICE* usb)
{
	PMUTEX_LOCK(m_mutex);
	for (std::list<USB_DEVICE>::iterator it = m_lsWhiteUsb.begin(); it != m_lsWhiteUsb.end(); ++it)
	{
		if (!strcasecmp(it->vid.c_str(), usb->vid.c_str()) && !strcasecmp(it->pid.c_str(), usb->pid.c_str()) && !strcasecmp(it->serial.c_str(), usb->serial.c_str()))
		{
			return;
		}
	}
	m_lsWhiteUsb.push_back(*usb);
}
//移除白名单设备
void CUsbMg::DelWhiteUsb(USB_DEVICE* usb)
{
	PMUTEX_LOCK(m_mutex);
	for (std::list<USB_DEVICE>::iterator it = m_lsWhiteUsb.begin(); it != m_lsWhiteUsb.end(); ++it)
	{
		if (!strcasecmp(it->vid.c_str(), usb->vid.c_str()) && !strcasecmp(it->pid.c_str(), usb->pid.c_str()) && !strcasecmp(it->serial.c_str(), usb->serial.c_str()))
		{
			m_lsWhiteUsb.erase(it);
			return;
		}
	}
}
//清空白名单设备
void CUsbMg::ClsWhiteUsb()
{
	PMUTEX_LOCK(m_mutex);
	m_lsWhiteUsb.clear();
}
bool CUsbMg::IsUsbWhite(USB_DEVICE* usbdev, bool bsync)
{
	PMUTEX_LOCK(m_mutex);
	if (-1 == m_isAudit) return true;
	for (std::list<USB_DEVICE>::iterator it = m_lsWhiteUsb.begin(); it != m_lsWhiteUsb.end(); ++it)
	{
		if (!strcasecmp(it->vid.c_str(), usbdev->vid.c_str()) && !strcasecmp(it->pid.c_str(), usbdev->pid.c_str()) && (!strcasecmp(it->serial.c_str(), usbdev->serial.c_str()) || (it->serial == "0" && usbdev->serial.size() == 0)))
		{
			it->devpath = usbdev->devpath;
			it->path = usbdev->path;
			it->name = usbdev->name;
			it->type = usbdev->type;
			if (!it->devpath.empty()) {
				m_usbControl.AddUsbControlList(*it);
				if (bsync) m_usbControl.SaveUsbControlList();
			}
			return true;
		}
	}

	if (m_isAudit == 2) {
		printf("study : usbdev is mounted vid [%s] pid [%s] serial [%s]", usbdev->vid.c_str(), usbdev->pid.c_str(), usbdev->serial.c_str());
		return true;
	}
	else {
		bool result = false;
		if (m_isAudit == 1) {
			result = false;
			printf("protected : usdev is not allowed to mount vid [%s] pid [%s] serial [%s]", usbdev->vid.c_str(), usbdev->pid.c_str(), usbdev->serial.c_str());
		}
		else {
			result = true;
			printf("audit : usbdev is mounted vid [%s] pid [%s] serial [%s]", usbdev->vid.c_str(), usbdev->pid.c_str(), usbdev->serial.c_str());
		}
		return result;
	}
	return false;
}
void CUsbMg::AddUsbDevice(udev_device* usb, bool bsync)
{
	const char* vid = udev_device_get_sysattr_value(usb, "idVendor");
	const char* pid = udev_device_get_sysattr_value(usb, "idProduct");
	const char* path = udev_device_get_syspath(usb);
	struct udev_device* usbparent = NULL;
	std::string devpath;
	if (path) {
		if (m_usbControl.GetDevpathBySyspath(devpath, path) ||
			m_usbControl.get_devpath_by_syspath(devpath, path) ||
			m_usbControl.get_devpath_by_syspath2(devpath, path));
	}
	USB_DEVICE data;
	data.devpath.swap(devpath);
	if (is_phone(vid, pid)) {
		data.type = emPhone;
	}
	else {
		bool busb = false;
		const char* driver = udev_device_get_driver(usb);
		if (driver && strcasecmp(driver, "usb-storage") == 0) {
			busb = true;
		}
		if (vid == NULL || pid == NULL) {
			if (!data.devpath.empty()) {
				usbparent = usb;
				while ((usbparent = udev_device_get_parent(usbparent)) != NULL)
				{
					if (!busb) {
						driver = udev_device_get_driver(usbparent);
						if (driver && strcasecmp(driver, "usb-storage") == 0) {
							busb = true;
						}
					}
					vid = udev_device_get_sysattr_value(usbparent, "idVendor");
					pid = udev_device_get_sysattr_value(usbparent, "idProduct");
					if (vid != NULL && pid != NULL) {
						const char* pathtmp = udev_device_get_syspath(usbparent);
						break;
					}
				}
			}
		}
		if (!busb) return;
		data.type = emUSB;
	}
	if (vid == NULL || pid == NULL) {
		return;
	}
	if (usbparent) {
		get_sysattr_info(usbparent, data);
	}
	else {
		get_sysattr_info(usb, data);
	}
	std::map<std::string, USB_DEVICE>::iterator it = m_lsAllUsbDev.find(data.vid + "," + data.pid);
	if (it != m_lsAllUsbDev.end()) {
		if (it->second.devpath == data.devpath) return;
		it->second.devpath = data.devpath;
	}
	else {
		m_lsAllUsbDev[data.vid + "," + data.pid] = data;
	}
	ControlDevice(&data, bsync);
}
void CUsbMg::RemoveUsbDevice(udev_device* usb)
{
#ifndef OS_BOOST
	const char* vid = udev_device_get_sysattr_value(usb, "idVendor");
	const char* pid = udev_device_get_sysattr_value(usb, "idProduct");
	const char* serial = udev_device_get_sysattr_value(usb, "serial");
	if (NULL == vid) vid = "";
	if (NULL == pid) pid = "";
	if (NULL == serial) serial = "";
#endif
	//printf(" Action   : remove\n");
	//如果设备移除了，重新扫描所有设备
	const char* devtype = udev_device_get_devtype(usb);
	if (devtype && strcasecmp(devtype, "usb_device") == 0)
	{
		InitCurUsb();
	}
}
void CUsbMg::InitCurUsb()
{
	m_lsAllUsbDev.clear();
#ifndef OS_BOOST
#endif // !OS_BOOST


	struct udev* udev = udev_new();
	get_cursys_usb(udev, m_lsAllUsbDev);
	udev_unref(udev);

	for (std::map<std::string, USB_DEVICE>::iterator it = m_lsAllUsbDev.begin(); it != m_lsAllUsbDev.end(); ++it)
	{
	}
}
void CUsbMg::ControlDevice(USB_DEVICE* usbdev, bool bsync)
{
	if (!IsUsbWhite(usbdev, bsync))
	{
#ifdef OS_BOOST
		printf("forbid: name:%s, path:%s\n", usbdev->name.c_str(), usbdev->path.c_str());
#else
#endif // OS_BOOST
		StopDevice(usbdev->name, usbdev->path);
	}
}
void CUsbMg::StopDevice(std::string name, std::string drvpath)
{
	std::string cmd("echo " + name + ">" + drvpath + "/driver/unbind");
	system(cmd.c_str());
}
void CUsbMg::ResumeDevice(std::string name, std::string drvpath)
{
	std::string cmd("echo " + name + ">" + drvpath + "/../driver/bind");
	system(cmd.c_str());
}
void CUsbMg::get_sysattr_info(udev_device* usb, USB_DEVICE& data)
{
	const char* syspath = udev_device_get_syspath(usb);
	const char* name = udev_device_get_sysname(usb);
	const char* vid = udev_device_get_sysattr_value(usb, "idVendor");
	const char* pid = udev_device_get_sysattr_value(usb, "idProduct");
	const char* serial = udev_device_get_sysattr_value(usb, "serial");

	data.name = name ? name : "";
	data.path = syspath ? syspath : "";
	data.vid = vid ? vid : "";
	data.pid = pid ? pid : "";
	data.serial = serial ? serial : "";
}
bool CUsbMg::GenUsbControlList()
{
	if (!m_usbControl.ClearUsbControlList()) {
		return false;
	}
	for (std::map<std::string, USB_DEVICE>::iterator usbit = m_lsAllUsbDev.begin(); usbit != m_lsAllUsbDev.end(); ++usbit)
		for (std::list<USB_DEVICE>::iterator it = m_lsWhiteUsb.begin(); it != m_lsWhiteUsb.end(); ++it)
		{
			if (!strcasecmp(it->vid.c_str(), usbit->second.vid.c_str()) &&
				!strcasecmp(it->pid.c_str(), usbit->second.pid.c_str()) &&
				(!strcasecmp(it->serial.c_str(), usbit->second.serial.c_str()) || (it->serial == "0" && usbit->second.serial.size() == 0)))
			{
				it->devpath = usbit->second.devpath;
				m_usbControl.AddUsbControlList(*it);
				break;
			}
		}
	return m_usbControl.SaveUsbControlList();
}

static char* Fgets(char* s, int size, FILE* fp)
{
	char* tmp = NULL;
	while ((tmp = fgets(s, size, fp)) == NULL) if (errno != EINTR) return tmp;
	for (int i = 0, j = 0; s[i] != '\0'; ++i)
	{
		if (s[i] == '#' || s[i] == '\r' || s[i] == '\n')
		{
			s[i] = '\0';
			break;
		}
		s[j++] = s[i];
	}
	return tmp;
}
bool CUsbMg::get_mountpath_by_devpath(std::string& mpath, const std::string& devpath)
{
	if (devpath.empty()) return false;
	FILE* confp = NULL;
	if ((confp = fopen("/proc/mounts", "r")) == NULL)
		return false;
	char line[1024] = { 0 };
	std::string tmp;
	while (Fgets(line, 1024, confp) != NULL)
	{
		if (0 == strncmp(line, devpath.c_str(), devpath.size()))
		{
			size_t i = devpath.size() + 1;
			while (line[i])
			{
				if (line[i] == ' ') {
					break;
				}
				if (line[i] == '\\') {
					if (0 == strncmp(&line[i], "\\040", 4))
					{
						i += 4;
						mpath.append(1, ' ');
						continue;
					}
				}
				mpath.append(1, line[i]);
				++i;
			}
		}
	}
	fclose(confp);
	if (!mpath.empty()) {
		struct stat devstat;
		if (0 == stat(mpath.c_str(), &devstat)) {
			return true;
		}
	}
	return false;
}
bool CUsbMg::add_usb_control_list(USB_DEVICE& data)
{
	return m_usbControl.AddUsbControlList(data) && m_usbControl.SaveUsbControlList();
}
void CUsbMg::get_cursys_usb(udev* udev, std::map<std::string, USB_DEVICE>& lsDevice)
{
	struct udev_enumerate* enumerate = udev_enumerate_new(udev);
	m_usbControl.ClearUsbControlList();
	udev_enumerate_add_match_subsystem(enumerate, "usb");
	udev_enumerate_scan_devices(enumerate);

	struct udev_list_entry* devices = udev_enumerate_get_list_entry(enumerate);
	struct udev_list_entry* entry;

	udev_list_entry_foreach(entry, devices)
	{
		const char* path = udev_list_entry_get_name(entry);
		struct udev_device* usb = udev_device_new_from_syspath(udev, path);
		if (usb)
		{
			AddUsbDevice(usb, false);
			udev_device_unref(usb);
		}
	}
	udev_enumerate_unref(enumerate);
	m_usbControl.SaveUsbControlList();
}
