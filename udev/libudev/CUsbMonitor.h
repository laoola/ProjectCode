#pragma once

#include "CUsbMg.h"
#ifdef OS_BOOST
#include <boost/thread.hpp>
#else
#include <string.h>
#endif

class CUsbMonitor
{
public:
	CUsbMonitor(CUsbMg* usbMg);
	virtual ~CUsbMonitor();
	void WaitEvent();
	bool InitMonitor();
	bool StartMonitor();
	bool StopMonitor();
private:
#ifndef OS_BOOST
	static void* pthread_waitEvent(void* arg) { ((CUsbMonitor*)arg)->WaitEvent(); return NULL; }
#endif // !OS_BOOST
	void Release();
private:
	CUsbMg* m_usbMg;
	struct udev* m_pUdev;
	struct udev_monitor* m_pMon;
	bool							m_bRun;
#ifdef OS_BOOST
	boost::thread			m_bWorkThread;
#else
	pthread_t					m_bWorkThread;
#endif
};
