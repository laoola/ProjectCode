#ifndef UTILITY_HPP_INCLUDED
#define UTILITY_HPP_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <chrono>
#include <dirent.h>

namespace cmd_unix
{
    std::vector<std::string> ls(std::string const & file)noexcept;
    void notify_send(std::string const & msg)noexcept;
}
namespace timer
{
    //void lim_cpu(std::chrono::duration const & en)noexcept;
    //ton
    //tof
}

template<class T> T Extract_one_data(std::string const & file)noexcept
{
    std::fstream If;
    T data;

    If.exceptions(std::ifstream::badbit);

    try
    {
        If.open(file,std::ios::in);

        If >> data;
    }
    catch(std::system_error& e)
    {
        std::cerr <<file+"-echec ectract: " << e.what() <<std::endl;
    }
    catch(std::exception const & e)
    {
        std::cerr <<file+"-echec extract: " << e.what() <<std::endl;
    }

    return data;
}

#endif // UTILITY_HPP_INCLUDED
