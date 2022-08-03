#include "udev.h"
#include "istring.h"

namespace mech
{
    namespace udev
    {
        /*************************************************************
        * Author:       LiJianbo
        * Date:         2018-03-01
        * Description:  构造udev对象
        ************************************************************/
        Udev::Udev() : m_udev(udev_new())
        {
        }

        /*************************************************************
        * Author:       LiJianbo
        * Date:         2018-03-06
        * Description:  拷贝构造，增加udev的引用计数
        ************************************************************/
        Udev::Udev(const Udev& other) : m_udev(udev_ref(other.m_udev))
        {
        }

        /*************************************************************
        * Author:       LiJianbo
        * Date:         2018-03-01
        * Description:  析构，解引用udev对象
        ************************************************************/
        Udev::~Udev()
        {
            if (m_udev)
            {
                udev_unref(m_udev);
                m_udev = NULL;
            }
        }

        /*************************************************************
        * Author:       LiJianbo
        * Date:         2018-03-01
        * Description:  创建enumerate对象
        ************************************************************/
        UdevEnumerate Udev::create_enumerate()
        {
            return UdevEnumerate(m_udev);
        }

        /*************************************************************
        * Author:       LiJianbo
        * Date:         2018-03-02
        * Description:  创建monitor对象
        ************************************************************/
        UdevMonitor Udev::create_monitor_from_netlink(const char* name)
        {
            if (utils::IStr(name).equal("kernel")) // kernel 可能收到多级事件
            {
                return UdevMonitor(udev_monitor_new_from_netlink(m_udev, "kernel"));
            }
            else
            {
                return UdevMonitor(udev_monitor_new_from_netlink(m_udev, "udev"));
            }
        }
    }
}