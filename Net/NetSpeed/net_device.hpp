#ifndef NET_DEVICE_HPP_INCLUDED
#define NET_DEVICE_HPP_INCLUDED

#include "utility.hpp"

class Net_devices
{
public:
    struct Identity
    {
        std::string name,mac_addr,broadcast,operstate;
        short addr_len;
    };

    struct RTx
    {
        long int bytes,packets;
    };

    struct Device
    {
        Identity identity;
        RTx rx;
        RTx tx;
    };

    Net_devices(void);

    void load(void)noexcept;
    void update(void)noexcept;

    void display(void)noexcept;

    size_t size(void)const noexcept;

    std::vector<Device> get_device(void) const;

private:
    const std::string file_src;
    const std::string file_stat;
    std::vector<Device> devices;
};

#endif // NET_DEVICE_HPP_INCLUDED
