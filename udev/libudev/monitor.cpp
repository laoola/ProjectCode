#include "udev.h"

namespace mech
{
        namespace udev
        {
        /*************************************************************
        * Author:       LiJianbo
        * Date:         2018-03-02
        * Description:  构造udev_monitor对象
        ************************************************************/
        UdevMonitor::UdevMonitor(udev_monitor_t* monitor) : m_monitor(monitor)
        {
        }

        /*************************************************************
        * Author:       LiJianbo
        * Date:         2018-03-06
        * Description:  拷贝构造，增加udev_monitor的引用计数
        ************************************************************/
        UdevMonitor::UdevMonitor(const UdevMonitor& other)
            : m_monitor(udev_monitor_ref(other.m_monitor))
        {
        }

        /*************************************************************
        * Author:       LiJianbo
        * Date:         2018-03-02
        * Description:  析构，解引用udev_monitor对象
        ************************************************************/
        UdevMonitor::~UdevMonitor()
        {
            if (m_monitor)
            {
                udev_monitor_unref(m_monitor);
                m_monitor = NULL;
            }
        }

        /*************************************************************
        * Author:       LiJianbo
        * Date:         2018-03-02
        * Description:  监视USB设备插入事件
        ************************************************************/
        int UdevMonitor::monitor_usb_devices_event()
        {
            udev_monitor_filter_add_match_subsystem_devtype(m_monitor, "usb", "usb_device"); // 所有usb设备
            udev_monitor_filter_add_match_subsystem_devtype(m_monitor, "block", "disk"); // U盘，是usb设备的子集
            udev_monitor_enable_receiving(m_monitor);
            return udev_monitor_get_fd(m_monitor);
        }

        /*************************************************************
        * Author:       LiJianbo
        * Date:         2018-03-06
        * Description:  指定监视事件处理回调函数
        ************************************************************/
        udev_device_t* UdevMonitor::get_devinfo()
        {
            return udev_monitor_receive_device(m_monitor);
        }
    }
}
