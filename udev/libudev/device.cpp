#include "udev.h"
#include "istring.h"
#include "log.h"

#include <limits.h>
#include <stdlib.h>


namespace mech
{
    namespace udev
    {
        /************************************************************
        * Author:       Li Jianbo
        * Date:         2018-10-19
        * Description:  构造设备对象(USB)，由monitor调用
        ************************************************************/
       	UdevDevice::UdevDevice() :
            name(""),name_orig(""),driver(""),drvpath(""),
            syspath(""),syspath_orig(""),devnode_orig(""),action(""),desc(""),
            class_mask(""),vendor_id(""),product_id(""),serial_number("")
        {
        }
        UdevDevice::UdevDevice(udev_device_t* dev)
        {
            make_usb_device(dev);
        }

        /************************************************************
        * Author:       Li Jianbo
        * Date:         2018-10-22
        * Description:  构造设备对象，由enumerate调用
        ************************************************************/
        UdevDevice::UdevDevice(udev_device_t* dev, udev_device_t* parent)
        {
            if (set_value(m_class_code, udev_device_get_sysattr_value(parent, "class")))
            {
                if (is_usb_device())
                {
                    make_usb_device(dev);
                }
                else
                {
                    make_pci_device(dev, parent);
                }
            }
            else
            {
                LOG_WARNING() << "get pci class code failure";
            }
        }

        /************************************************************
        * Author:       LiJianbo
        * Date:         2018-10-25
        * Description:  判断设备是否在设备管控所属范围
        ************************************************************/
        bool UdevDevice::is_valid_to_control()
        {
            return !syspath.empty();
        }

        /**
         * @brief UdevDevice 对象内容是否为空
         * 
         * @return true 
         * @return false 
         */
        bool UdevDevice::isEmpty()
        {
            if( name.empty() && name_orig.empty() && drvpath.empty() && syspath.empty() 
                && syspath_orig.empty() && devnode_orig.empty() && action.empty() && desc.empty() && 
                class_mask.empty() && vendor_id.empty() && product_id.empty() && serial_number.empty() )
            {
                return true;
            }

            return false;
        }

        /************************************************************
        * Author:       Li Jianbo
        * Date:         2018-10-25
        * Description:  重置所有成员
        ************************************************************/
        void UdevDevice::reset()
        {
            name.clear();
            name_orig.clear();
            driver.clear();
            drvpath.clear();
            syspath.clear();
            syspath_orig.clear();
            devnode_orig.clear();
            action.clear();
            desc.clear();
            class_mask.clear();
            vendor_id.clear();
            product_id.clear();
            serial_number.clear();
        }

        /************************************************************
        * Author:       Li Jianbo
        * Date:         2018-10-25
        * Description:  初始化USB设备
        ************************************************************/
        void UdevDevice::make_usb_device(udev_device_t* dev)
        {
            udev_device_t* orig   = NULL;
            udev_device_t* child  = NULL;
            udev_device_t* parent = NULL;

            reset();
            /* do...while(0) 实现goto的效果 */
            do
            {
                // 添加引用，避免资源自动释放
                orig = udev_device_ref(dev);
                if (!orig)
                {
                    reset();
                    LOG_WARNING() << "NULL device object is invalid";
                    break;
                }

                // 尽早保存action，避免被冲掉
                set_value(action, udev_device_get_action(orig));

                // 查找USB设备的父节点及父节点的子节点
                parent = find_parent(orig); // 没有引用
                child  = find_child(parent);
                if (!parent )
                {
                    reset();
                    //LOG_DEBUG() << "find parent failure";
                    break;
                }

                if (!child )
                {
                    reset();
                    //LOG_DEBUG() << "find  child failure";
                    break;
                }

                if (!set_value(driver, udev_device_get_driver(child)) || driver == "hub")
                {
                    reset();
                    //LOG_DEBUG() << "invalid driver";
                    break;
                }

                // 设置各项成员的值
                if (!set_value(name_orig,    udev_device_get_sysname(orig)) ||
                    !set_value(syspath_orig, udev_device_get_syspath(orig)) ||
                    !set_value(name,         udev_device_get_sysname(child)) ||
                    !set_value(syspath,      udev_device_get_syspath(child)) ||
                    !set_value(vendor_id,    udev_device_get_sysattr_value(parent, "idVendor")) ||
                    !set_value(product_id,   udev_device_get_sysattr_value(parent, "idProduct")) ||
                    !find_driver_path() ||
                    !format_class_mask(child))
                {
                    reset();
                    LOG_WARNING() << "some member as empty is invalid";
                    break;
                }

                set_value(devnode_orig,  udev_device_get_devnode(orig));
                set_value(serial_number, udev_device_get_sysattr_value(parent, "serial"));
                format_desc(parent, "manufacturer", "product");
            } while (0);

            if (orig) { udev_device_unref(orig); }
            if (child) { udev_device_unref(child); }
        }

        /************************************************************
        * Author:       Li Jianbo
        * Date:         2018-10-25
        * Description:  初始化非USB外设的PCI设备
        ************************************************************/
        void UdevDevice::make_pci_device(udev_device_t* dev, udev_device_t* parent)
        {
            udev_device_t* child  = NULL;

            /* do...while(0) 实现goto的效果 */
            do
            {
                // 添加引用，避免资源自动释放
                child = udev_device_ref(dev);
                if (!child)
                {
                    LOG_WARNING() << "NULL device object is invalid";
                    break;
                }

                // 设置各项成员的值
                if (!set_value(name_orig,    udev_device_get_sysname(child)) ||
                    !set_value(syspath_orig, udev_device_get_syspath(child)) ||
                    !set_value(name,         udev_device_get_sysname(child)) ||
                    !set_value(syspath,      udev_device_get_syspath(child)) ||
                    !set_value(vendor_id,    udev_device_get_sysattr_value(parent, "vendor")) ||
                    !set_value(product_id,   udev_device_get_sysattr_value(parent, "device")) ||
                    // 拼class mask时用到driver，必须在class mask前调用
                    !set_value(driver,       udev_device_get_driver(child)) ||
                    !find_driver_path() ||
                    !format_class_mask(child))
                {
                    reset();
                    // LOG_WARNING() << "some member as empty is invalid";
                    break;
                }

                set_value(devnode_orig,  udev_device_get_devnode(child));
                set_value(action,        udev_device_get_action(child));
                format_desc(child, "vendor", "model");
            } while (0);

            if (child) { udev_device_unref(child); }
        }

        /************************************************************
        * Author:       Li Jianbo
        * Date:         2018-10-22
        * Description:  查找USB设备的父节点
        ************************************************************/
        udev_device_t* UdevDevice::find_parent(udev_device_t* dev)
        {
            if (!dev)
            {
                LOG_DEBUG() << "dev null for find_parent";
                return NULL;
            }

            udev_device_t* parent = NULL;
            utils::IStr subsystem(udev_device_get_subsystem(dev));
            if (subsystem.equal("block"))
            {
                parent = udev_device_get_parent_with_subsystem_devtype(dev, "usb", "usb_device");
            }
            else if (subsystem.equal("usb"))
            {
                parent = dev;
            }
            else
            {
                LOG_TRACE() << "invalid device type with subsystem " << subsystem.data();
            }
            //LOG_DEBUG() << "subsystem is " << subsystem.data();

            return parent;
        }

        /************************************************************
        * Author:       Li Jianbo
        * Date:         2018-10-25
        * Description:  查找USB设备的字节点
        ************************************************************/
        udev_device_t* UdevDevice::find_child(udev_device_t* parent)
        {
            udev_t* pudev = NULL;;
            udev_device_t* pUdevRet = NULL;
            std::string syspath, name;
            if (parent && (pudev = udev_device_get_udev(parent)) &&
                set_value(syspath, udev_device_get_syspath(parent)) &&
                set_value(name,    udev_device_get_sysname(parent)))
            {
                std::string child_syspath = syspath + "/" + name + ":1.0";
                //LOG_DEBUG() << "child syspath=" << child_syspath;

                pUdevRet = udev_device_new_from_syspath(pudev, child_syspath.c_str());

                if(NULL == pUdevRet)
                {
                    child_syspath = syspath + "/" + name + ":4.0";
                    pUdevRet = udev_device_new_from_syspath(pudev, child_syspath.c_str());
                }
                
                return pUdevRet;
            }
            //LOG_DEBUG() << "parent NULL ?=" << (parent) << ", pudev NULL ?=" << (pudev)
            //<< ", syspath=" << syspath << ", name=" << name;

            return NULL;
        }

        /************************************************************
        * Author:       Li Jianbo
        * Date:         2018-10-25
        * Description:  判断一个PCI设备是否USB外设
        ************************************************************/
        bool UdevDevice::is_usb_device()
        {
            return utils::IStr(m_class_code).starts_with("0x0c");
        }

        /************************************************************
        * Author:       Li Jianbo
        * Date:         2018-10-25
        * Description:  安全地设置成员的值
        ************************************************************/
        bool UdevDevice::set_value(std::string& m, const char* pv)
        {
            m = utils::IStr(pv).data();
            //LOG_DEBUG() << "MbVal: " << m;
            return !m.empty();
        }

        /************************************************************
        * Author:       Li Jianbo
        * Date:         2018-10-25
        * Description:  获取真实的驱动路径
        ************************************************************/
        bool UdevDevice::find_driver_path()
        {
            if (syspath.empty()) { return false; }

            char path[4096] = {0};
            std::string drvlnk = syspath + "/driver";
            return set_value(drvpath, realpath(drvlnk.c_str(), path));
        }

        /************************************************************
        * Author:       Li Jianbo
        * Date:         2018-10-25
        * Description:  格式化设备类型掩码，标识一种类型的设备，用于匹配管控规则
        ************************************************************/
        bool UdevDevice::format_class_mask(udev_device_t* dev)
        {
            /// 非USB设备用driver，USB设备用USB class code
            /// http://www.usb.org/developers/defined_class
            class_mask = driver;
            if (dev)
            {
                const char* cls    = udev_device_get_sysattr_value(dev, "bInterfaceClass");
                const char* subcls = udev_device_get_sysattr_value(dev, "bInterfaceSubClass");
                const char* prot   = udev_device_get_sysattr_value(dev, "bInterfaceProtocol");
                if (cls && subcls && prot)
                {
                    class_mask = std::string(cls) + "/" + subcls + "/" + prot;
                }
            }
            return !class_mask.empty();
        }

        /************************************************************
        * Author:       Li Jianbo
        * Date:         2018-10-25
        * Description:  格式化描述信息
        ************************************************************/
        void UdevDevice::format_desc(udev_device_t* dev, const char* pkv, const char* pkp)
        {
            std::string vendor, product;
            set_value(vendor,  udev_device_get_sysattr_value(dev, pkv));
            set_value(product, udev_device_get_sysattr_value(dev, pkp));

            if (!vendor.empty())
            {
                desc = vendor + " ";
            }
            if (!product.empty())
            {
                desc += product;
            }
        }

        void UdevDevice::debug_show_info()
        {
            LOG_DEBUG() << "--------------------------------";
            LOG_DEBUG() << "device.name    = " << name;
            LOG_DEBUG() << "device.desc    = " << desc;
            LOG_DEBUG() << "device.driver  = " << driver;
            LOG_DEBUG() << "device.drvpath = " << drvpath;
            LOG_DEBUG() << "device.syspath = " << syspath;
            LOG_DEBUG() << "device.devnode = " << devnode_orig;
            LOG_DEBUG() << "device.clsmask = " << class_mask;
            LOG_DEBUG() << "device.vid     = " << vendor_id;
            LOG_DEBUG() << "device.pid     = " << product_id;
            LOG_DEBUG() << "device.sn      = " << serial_number;
            LOG_DEBUG() << "device.active  = " << action;
            LOG_DEBUG() << "--------------------------------";
        }
    }
}