#pragma once
#include <string>
#include <vector>
#include <libudev.h>

namespace mech
{
    namespace udev
    {
        typedef struct udev            udev_t;
        typedef struct udev_device     udev_device_t;
        typedef struct udev_enumerate  udev_enumerate_t;
        typedef struct udev_monitor    udev_monitor_t;
        typedef struct udev_list_entry udev_list_entry_t;

        class  UdevEnumerate;
        class  UdevMonitor;
        struct UdevDevice;

        typedef std::vector<UdevDevice> dev_list_t;


        /*
        ** 提供udev上下文
        */
        class Udev
        {
        public:
            explicit Udev();
            Udev(const Udev& other);
            ~Udev();

            UdevEnumerate create_enumerate();
            UdevMonitor create_monitor_from_netlink(const char* name = "udev");

        private:
            udev_t* m_udev;
        };


        /*
        ** 封装udev提供的enumerate功能
        */
        class UdevEnumerate
        {
        public:
            UdevEnumerate(udev_t* m_udev);

            bool list_devices();
            dev_list_t& all_devices();

        private:
            bool is_pci_device(const char* syspath);
            bool find_pci_child(udev_list_entry_t* parent);
            bool is_replicate(std::string name); // 去重，临时方案

            udev_t* m_udev;
            dev_list_t m_devices;
        };


        /*
        ** 封装udev提供的monitor功能
        */
        class UdevMonitor
        {
        public:
            UdevMonitor(udev_monitor_t* monitor);
            UdevMonitor(const UdevMonitor& other);
            ~UdevMonitor();

            int monitor_usb_devices_event();
            udev_device_t* get_devinfo();

        private:
            udev_monitor_t* m_monitor;
        };


        /*
        ** 封装udev提供的device对象
        */
        struct UdevDevice
        {
            std::string name;
            std::string name_orig;
            std::string driver;
            std::string drvpath;
            std::string syspath;
            std::string syspath_orig;
            std::string devnode_orig;
            std::string action;
            std::string desc;
            std::string class_mask;
            std::string vendor_id;
            std::string product_id;
            std::string serial_number;

            UdevDevice();
            UdevDevice(udev_device_t* dev);
            UdevDevice(udev_device_t* dev, udev_device_t* parent);
            
            bool is_valid_to_control();
            bool is_usb_device();
            bool isEmpty();
            void reset();
            void debug_show_info();

        private:
            void make_usb_device(udev_device_t* dev);
            void make_pci_device(udev_device_t* dev, udev_device_t* parent);
            udev_device_t* find_parent(udev_device_t* dev);
            udev_device_t* find_child(udev_device_t* parent);

            bool set_value(std::string& m, const char* pv);
            bool find_driver_path();
            bool format_class_mask(udev_device_t* dev);
            void format_desc(udev_device_t* dev, const char* pkv, const char* pkp);

            std::string m_class_code;
        };
    }
}