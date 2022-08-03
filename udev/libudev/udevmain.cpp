#include "CUsbMg.h"
#include "CUsbMonitor.h"
#include <stdio.h>

int testmain()
{
	CUsbMg g_usbMg;
	g_usbMg.ControlCurUsb();

	USB_DEVICE data;
	data.vid = "23a9";
	data.pid = "ef18";
	g_usbMg.AddWhiteUsb(&data);

	CUsbMonitor usb(&g_usbMg);
	usb.StartMonitor();

	getchar();

	usb.StopMonitor();
	return 0;
}
