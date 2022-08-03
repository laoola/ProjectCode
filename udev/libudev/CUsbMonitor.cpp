#include "CUsbMonitor.h"
#include <stdio.h>
#include <cstdlib>
#include <libudev.h>
#include <unistd.h>
#include "CUsbMg.h"

#include "udev.h"

CUsbMonitor::CUsbMonitor(CUsbMg* usbMg)
{
	m_usbMg = usbMg;
	m_pUdev = NULL;
	m_pMon = NULL;
	m_bRun = false;
#ifndef OS_BOOST
	m_bWorkThread = 0;
#endif // !OS_BOOST

	if (m_usbMg == NULL) throw "usbMg do not allow null";
}
CUsbMonitor::~CUsbMonitor()
{
	Release();
}
void CUsbMonitor::WaitEvent()
{
	struct udev_device* dev;

	int fd = udev_monitor_get_fd(m_pMon);

	if (m_usbMg) m_usbMg->ControlCurUsb();
	while (m_bRun)
	{
		fd_set fds;
		struct timeval tv;
		int ret;

		FD_ZERO(&fds);
		FD_SET(fd, &fds);
		tv.tv_sec = 0;
		tv.tv_usec = 0;

		ret = select(fd + 1, &fds, NULL, NULL, &tv);

		if (ret > 0 && FD_ISSET(fd, &fds))
		{
			//printf("\nselect() says there should be data\n");

			dev = udev_monitor_receive_device(m_pMon);
			if (dev)
			{
				mech::udev::UdevDevice device(dev);
				device.debug_show_info();
				const char* action = udev_device_get_action(dev);
				if (strcasecmp(action, "add") == 0)
				{
					m_usbMg->AddUsbDevice(dev, true);
				}
				else if (strcasecmp(action, "remove") == 0)
				{
					m_usbMg->RemoveUsbDevice(dev);
				}
				udev_device_unref(dev);
			}
			else
			{
				//printf("No Device from receive_device(). An error occured.\n");
			}
		}

		usleep(250 * 1000);
	}
}

bool CUsbMonitor::InitMonitor()
{
	if (m_pUdev == NULL)
	{
		m_pUdev = udev_new();
		if (m_pUdev == NULL)
		{
			return false;
		}
	}

	m_pMon = udev_monitor_new_from_netlink(m_pUdev, "kernel");
	if (m_pMon == NULL)
	{
		return false;
	}
	udev_monitor_filter_add_match_subsystem_devtype(m_pMon, "usb", "usb_device");
	udev_monitor_filter_add_match_subsystem_devtype(m_pMon, "block", "disk");
	//udev_monitor_filter_add_match_subsystem_devtype(m_pMon, "block", "partition");
	udev_monitor_filter_add_match_subsystem_devtype(m_pMon, "usb", "usb_interface");
	udev_monitor_enable_receiving(m_pMon);
	m_bRun = true;
	return true;
}

bool CUsbMonitor::StartMonitor()
{
	if(!InitMonitor()) return false;
	try
	{
#ifdef OS_BOOST
		m_bWorkThread = boost::thread(boost::bind(&CUsbMonitor::WaitEvent, this));
#else
		if (pthread_create(&m_bWorkThread, NULL, pthread_waitEvent, this))
		{
			LOG_ERROR() << "usb monitor pthread_create failed";
			Release();
			return false;
		}
		LOG_INFO() << "usb monitor start success";
		pthread_detach(m_bWorkThread);
#endif // OS_BOOST
	}
	catch (const std::exception& e)
	{
		printf(e.what());
		Release();
		return false;
	}
	return true;
}
bool CUsbMonitor::StopMonitor()
{
	m_bRun = false;
	Release();
#ifdef OS_BOOST
	m_bWorkThread.join();
#else
	if (m_bWorkThread > 0)
	{
		pthread_cancel(m_bWorkThread);
		m_bWorkThread = 0;
		LOG_INFO() << "stop usb monitor success";
	}
#endif // OS_BOOST
	return true;
}
void CUsbMonitor::Release()
{
	if (m_pMon)
	{
		udev_monitor_unref(m_pMon);
		m_pMon = NULL;
	}
	if (m_pUdev)
	{
		udev_unref(m_pUdev);
		m_pUdev = NULL;
	}
}
