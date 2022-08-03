#include "net_device.hpp"

Net_devices::Net_devices(void):file_src("/sys/class/net/"),file_stat("/statistics"){}

void Net_devices::load(void)noexcept
{
    for(auto rep:cmd_unix::ls(this->file_src))
    {
        Device tps;
        tps.identity.name=rep;

        tps.identity.mac_addr=Extract_one_data<std::string>(this->file_src+"/"+rep+"/address");
        tps.identity.broadcast=Extract_one_data<std::string>(this->file_src+"/"+rep+"/broadcast");
        tps.identity.addr_len=Extract_one_data<short>(this->file_src+"/"+rep+"/addr_len");

        this->devices.push_back(tps);
    }
    this->update();
}

void Net_devices::update(void)noexcept
{
    for(auto &i:this->devices)
    {
        i.identity.operstate=Extract_one_data<std::string>(this->file_src+"/"+i.identity.name+"/operstate");
        i.tx.bytes=Extract_one_data<long int>(this->file_src+"/"+i.identity.name+this->file_stat+"/tx_bytes");
        i.rx.bytes=Extract_one_data<long int>(this->file_src+"/"+i.identity.name+this->file_stat+"/rx_bytes");
        i.tx.packets=Extract_one_data<long int>(this->file_src+"/"+i.identity.name+this->file_stat+"/tx_packets");
        i.rx.packets=Extract_one_data<long int>(this->file_src+"/"+i.identity.name+this->file_stat+"/rx_packets");
    }
}

void Net_devices::display(void)noexcept
{
    std::cout <<this->size() <<" devices dectected\n"<<std::endl;
    for(auto & i:this->devices)
    {
       std::cout <<i.identity.name <<std::endl;
       std::cout <<'\t'<<"mac_addr: "<<i.identity.mac_addr <<std::endl;
       std::cout <<'\t'<<"broadcast: "<<i.identity.broadcast <<std::endl;
       std::cout <<'\t'<<"addr_len: "<<i.identity.addr_len <<std::endl;
       std::cout <<'\t'<<"operstate: "<<i.identity.operstate <<std::endl;
       std::cout <<'\t'<<"tx bytes: "<<i.tx.bytes <<" ("<<static_cast<float>(i.tx.bytes)/1000000.0<<" Mb)"<<std::endl;
       std::cout <<'\t'<<"tx packets: "<<i.tx.packets <<std::endl;
       std::cout <<'\t'<<"rx bytes: "<<i.rx.bytes <<" ("<<static_cast<float>(i.rx.bytes)/1000000.0<<" Mb)"<<std::endl;
       std::cout <<'\t'<<"rx packets: "<<i.rx.packets <<std::endl;
       std::cout <<std::endl;
    }
}
std::vector<Net_devices::Device> Net_devices::get_device(void) const
{
    return this->devices;
}

size_t Net_devices::size(void)const noexcept
{
    return this->devices.size();
}
