#include "UsbControl.h"
#include "log.h"
#include "CUsbMg.h"
#include <sys/types.h>
#include <dirent.h>
#include <pthread.h>
#include <string.h>


class CloseDir {
public:
	CloseDir(DIR* dir) :m_dir(dir) {}
	~CloseDir() { if (m_dir) closedir(m_dir); }
private:
	DIR* m_dir;
};

UsbControl::UsbControl()
{
	fp = NULL;
	if (pthread_mutex_init(&m_mutex, NULL))
	{
		printf("init mutex failed : %s", strerror(errno));
	}
}

UsbControl::~UsbControl()
{
	CloseUsbControlList();
	pthread_mutex_destroy(&m_mutex);
}

bool UsbControl::AddUsbControlList(USB_DEVICE& data)
{
	if (data.devpath.empty()) return false;
	PMUTEX_LOCK(m_mutex);
	if (!fp && !OpenUsbControlList("ab")) {
		LOG_WARNING() << "OpenUsbControlList ab failed";
		return false;
	}
	usbprotect_t usbinfo;
	usbinfo.perm = data.perm;
	usbinfo.pathlen = data.devpath.size();
	if (usbinfo.pathlen >= sizeof(usbinfo.path)) {
		LOG_WARNING() << "usb mount path too deep";
		return false;
	}
	strcpy(usbinfo.path, data.devpath.c_str());
	memcpy(&usbinfo.usbinfo, &data.info, sizeof(usbinfo.usbinfo));
	if (1 == fwrite(&usbinfo, sizeof(usbinfo), 1, fp)) {
		printf("AddUsbControlList success. usb device vid [%s] pid [%s] sn [%s] path [%s]", data.vid.c_str(), data.pid.c_str(), data.serial.c_str(), data.devpath.c_str());
		return true;
	}
	printf("AddUsbControlList failed. usb device vid [%s] pid [%s] sn [%s] path [%s]", data.vid.c_str(), data.pid.c_str(), data.serial.c_str(), data.devpath.c_str());
	CloseUsbControlList();
	return false;
}

bool UsbControl::SaveUsbControlList()
{
	CloseUsbControlList();
	return NotifyUpdateControlList();
}

bool UsbControl::ClearUsbControlList()
{
	if (OpenUsbControlList("wb+")) return true;
	LOG_WARNING() << "ClearUsbControlList failed";
	return false;
}

bool UsbControl::GetDevpathBySyspath(std::string& devpath, const std::string& syspath)
{
	DIR* dir;
	struct dirent* ptr;
	std::string devname;
	size_t pos = syspath.rfind('/');
	if (pos == std::string::npos) return false;
	devname = &syspath.c_str()[pos + 1];
	if (devname.empty()) return false;
	if ((dir = opendir(syspath.c_str())) == NULL) {
		printf("opendir [%s] failed", syspath.c_str());
		return false;
	}
	CloseDir autoclose(dir);
	while ((ptr = readdir(dir)) != NULL)
	{
		if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)
			continue;
		if (DT_DIR == ptr->d_type) {
			if (0 == strncmp(ptr->d_name, devname.c_str(), devname.size())) {
				devpath = "/dev/";
				devpath.append(ptr->d_name);
				struct stat devstat;
				if (0 == stat(devpath.c_str(), &devstat)) {
					return true;
				}
			}
		}
	}
	devpath.clear();
	return false;
}

bool UsbControl::OpenUsbControlList(const std::string& mode)
{
	CloseUsbControlList();
	fp = fopen(USBAC_FILE, mode.c_str());
	return NULL != fp;
}

void UsbControl::CloseUsbControlList()
{
	if (fp) {
		fclose(fp);
		fp = NULL;
	}
}

bool UsbControl::NotifyUpdateControlList()
{
	return true;
}

bool read_num_dir(std::string& pathtmp)
{
	DIR* dir;
	struct dirent* ptr;
	if ((dir = opendir(pathtmp.c_str())) == NULL) return false;
	CloseDir autoclose(dir);
	while ((ptr = readdir(dir)) != NULL)
	{
		if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)
			continue;
		if (DT_DIR == ptr->d_type) {
			if (isdigit(ptr->d_name[0])) {
				pathtmp.append("/");
				pathtmp.append(ptr->d_name);
				return true;
			}
		}
	}
	return false;
}
bool read_str_dir(std::string& pathtmp, const std::string& str)
{
	DIR* dir;
	struct dirent* ptr;
	if ((dir = opendir(pathtmp.c_str())) == NULL) return false;
	CloseDir autoclose(dir);
	while ((ptr = readdir(dir)) != NULL)
	{
		if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)
			continue;
		if (DT_DIR == ptr->d_type) {
			if (0 == strncmp(ptr->d_name, str.c_str(), str.size())) {
				pathtmp.append("/");
				pathtmp.append(ptr->d_name);
				return true;
			}
		}
	}
	return false;
}
bool get_devpath(std::string& pathtmp, std::string& devpath)
{
	pathtmp.append("/block/");
	DIR* dir;
	struct dirent* ptr;
	std::string devname;
	if ((dir = opendir(pathtmp.c_str())) == NULL) return false;
	CloseDir autoclose(dir);
	while ((ptr = readdir(dir)) != NULL)
	{
		if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)
			continue;
		if (DT_DIR == ptr->d_type) {
			pathtmp.append("/");
			pathtmp.append(ptr->d_name);
			devname = ptr->d_name;
			if ((dir = opendir(pathtmp.c_str())) == NULL) break;
			CloseDir autoclose(dir);
			while ((ptr = readdir(dir)) != NULL)
			{
				if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)
					continue;
				if (DT_DIR == ptr->d_type) {
					if (0 == strncmp(ptr->d_name, devname.c_str(), devname.size())) {
						devpath = "/dev/";
						devpath.append(ptr->d_name);
						struct stat devstat;
						if (0 == stat(devpath.c_str(), &devstat)) {
							return true;
						}
					}
				}
			}
		}
	}
	devpath.clear();
	return false;
}
bool UsbControl::get_devpath_by_syspath(std::string& devpath, const std::string& syspath)
{
	std::string pathtmp = syspath;
	if (!read_num_dir(pathtmp)) {
		return false;
	}
	if (!read_str_dir(pathtmp, "host")) {
		return false;
	}
	if (!read_str_dir(pathtmp, "target")) {
		return false;
	}
	if (!read_num_dir(pathtmp)) {
		return false;
	}
	return get_devpath(pathtmp, devpath);
}

bool UsbControl::get_devpath_by_syspath2(std::string& devpath, const std::string& syspath)
{
	std::string pathtmp = syspath;
	if (!read_str_dir(pathtmp, "host")) {
		return false;
	}
	if (!read_str_dir(pathtmp, "target")) {
		return false;
	}
	if (!read_num_dir(pathtmp)) {
		return false;
	}
	return get_devpath(pathtmp, devpath);
}
