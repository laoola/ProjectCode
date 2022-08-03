#include "udev.h"
#include "log.h"
#include "istring.h"

namespace mech
{
    namespace udev
    {
        /************************************************************
        * Author:       LiJianbo
        * Date:         2018-03-01
        * Description:  构造udev_enumerate对象
        ************************************************************/
        UdevEnumerate::UdevEnumerate(udev_t* udev) : m_udev(udev)
        {
        }

        /***********************************************************
        * Author:       LiJianbo
        * Date:         2018-04-19
        * Description:  枚举所有pci外设
        ************************************************************/
        bool UdevEnumerate::list_devices()
        {
            bool ret = false;
            struct udev_enumerate *enumerate = udev_enumerate_new(m_udev);
            udev_enumerate_scan_devices(enumerate);

            struct udev_list_entry* entry = NULL;
            struct udev_list_entry* devices = udev_enumerate_get_list_entry(enumerate);
            udev_list_entry_foreach(entry, devices)
            {
                // 枚举pci设备
                if (is_pci_device(udev_list_entry_get_name(entry)))
                {
                    ret |= find_pci_child(entry);
                }

            }
            udev_enumerate_unref(enumerate);
            return ret;
        }

        /************************************************************
        * Author:       LiJianbo
        * Date:         2018-04-19
        * Description:  判断pci设备
        ************************************************************/
        bool UdevEnumerate::is_pci_device(const char* syspath)
        {
            bool ret = false;
            if (!syspath ||
                (!utils::IStr(syspath).starts_with("/sys/devices/pci") &&
                !utils::IStr(syspath).starts_with("/sys/devices/platform/soc/40000000.pcie-controller/pci") // fix bug: 银河麒麟无路径/sys/devices/pci  lijianbo 2019-01-15
                ))
            {
                return ret;
            }

            struct udev_device *dev = udev_device_new_from_syspath(m_udev, syspath);
            if (dev && udev_device_get_sysattr_value(dev, "enable")
                    && utils::IStr(udev_device_get_sysattr_value(dev, "enable")).equal("1"))
            {
                ret = true;
            }
            udev_device_unref(dev);
            return ret;
        }

        /************************************************************
        * Author:       LiJianbo
        * Date:         2018-03-01
        * Description:  枚举孩子节点设备
        ************************************************************/
        bool UdevEnumerate::find_pci_child(struct udev_list_entry* parent)
        {
            if (!parent) return false;

            const char* parent_path = udev_list_entry_get_name(parent);
            struct udev_device* parent_dev = udev_device_new_from_syspath(m_udev, parent_path);

            // 遍历子节点
            struct udev_list_entry* child = NULL;
            udev_list_entry_foreach(child, udev_list_entry_get_next(parent))
            {
                const char* path = udev_list_entry_get_name(child);
                if (path && !utils::IStr(path).starts_with(utils::IStr(parent_path).data()))
                {
                    break; // udev_list_entry包含所有设备，这里过滤当前父节点
                }

                struct udev_device* dev = udev_device_new_from_syspath(m_udev, path);
                UdevDevice device(dev, parent_dev);
                if (device.is_valid_to_control())
                {
                    if (!is_replicate(device.name))
                    {
                        m_devices.push_back(device);
                    }
                }
                udev_device_unref(dev);
            }

            udev_device_unref(parent_dev);
            return true;
        }

        /************************************************************
        * Author:       LiJianbo
        * Date:         2019-01-30
        * Description:  去重，避免设备被记录多次，临时方案
        ************************************************************/
        bool UdevEnumerate::is_replicate(std::string name)
        {
            dev_list_t::iterator it;
            for (it = m_devices.begin(); it != m_devices.end(); it++)
            {
                if (it->name == name)
                {
                    return true;
                }
            }
            return false;
        }

        /************************************************************
        * Author:       LiJianbo
        * Date:         2018-03-02
        * Description:  获取所有设备数据
        ************************************************************/
        dev_list_t& UdevEnumerate::all_devices()
        {
            return m_devices;
        }
    }
}