#pragma once
#include <sys/types.h>
#include <string>

#define USBAC_DEV_PATH_LEN 64
#define USBAC_FILE "/etc/os_safe.d/usbaclist"

typedef struct usbbaseinfo {
	int type;
	char hardware[256];
	char usblabel[128];
}usbinfo_t;

typedef struct usbprotect {
	unsigned short perm;         //默认访问权限(bit 0 是否可读，bit 1 是否可写)
	unsigned short pathlen;      //保护对象路径长度
	char path[USBAC_DEV_PATH_LEN];  //usb挂载文件夹绝对路径
	usbinfo_t	usbinfo; //usb基本信息
}usbprotect_t;

struct VENDOR_PRODUCT_ITEM
{
	ushort nVendor;
	ushort nProduct;
};

enum USB_DEVICE_TYPE
{
	emUSB = 0,
	emPhone = 1,
};

struct USB_DEVICE
{
	USB_DEVICE_TYPE type;
	std::string name;
	std::string path;
	std::string devpath;
	std::string vid;
	std::string pid;
	std::string serial;
	int perm;
	usbinfo_t info;
};

bool is_phone(const char* vid, const char* pid);
