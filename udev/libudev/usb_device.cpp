#include "usb_device.h"
#include <stdio.h>

const static struct VENDOR_PRODUCT_ITEM g_arrInnerVendorList[] =
{
	{0x2717, 0},		// xiaomi, hongmi
	{0x12D1, 0},		// huawei
	{0x22D9, 0},		// oppo
	{0x2D95, 0},		// vivo
	{0x05AC, 0},		// apple(maybe disable apple mouse&keyboard)
	{0x05C6, 0x6764},	// one plus one
	// {0x2A70, 0},		// one plus
	// {0x04E8, 0},		// samsung
	{0x2a45, 0},		// meizu
	{0x17ef, 0x7435},	// Lenovo(Mass Storage mode, with debug)
	{0x2a45, 0x743a},	// Lenovo(Mass Storage mode)
};

long hexcstol(const char* start)
{
	if (start == NULL)
		return 0;
	long value = 0;
	while (*start != '\0') {
		char word = *start;
		start++;

		value *= 16;
		if (word >= '0' && word <= L'9') {
			value += word - '0';
		}
		else if (word >= 'a' && word <= 'f') {
			value += word - 'a' + 10;
		}
		else if (word >= 'A' && word <= 'F') {
			value += word - 'A' + 10;
		}
		else {
			value /= 16;
			break;
		}
	}
	return value;
}

bool is_phone(const char* vid, const char* pid)
{
	if (vid && pid)
	{
		ushort nVid = (ushort)hexcstol(vid);
		ushort nPid = (ushort)hexcstol(pid);
		for (uint i = 0; i < sizeof(g_arrInnerVendorList) / sizeof(VENDOR_PRODUCT_ITEM); i++)
		{
			if (nVid == g_arrInnerVendorList[i].nVendor &&
				(nPid == g_arrInnerVendorList[i].nProduct || g_arrInnerVendorList[i].nProduct == 0)
				)
			{
				return true;
			}
		}
	}
	return false;
}
