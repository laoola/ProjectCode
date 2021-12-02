/*
 * @Descripttion: 
 * @version: 
 * @Author: zhangwenhao
 * @Date: 2021-11-26 15:55:22
 * @LastEditTime: 2021-11-26 16:42:24
 */
/******************************************************************************
 * Copyright 2020-2021 Co., Ltd.
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       main.cpp
 * @brief      PDF加水印，调用pdfix第三方库
 *
 * @author     zhangwenhao
 * @date       2021/11/26
 * @history
 *****************************************************************************/

#include <string>
#include <system_error>
#include <iostream>
#include <unistd.h>

#include "include/Initialization.h"
#include "include/AddWatermark.h"
#include "include/RegexSearch.h"
#include "include/RegexSetPattern.h"
#include "include/Utils.h"

extern std::wstring GetAbsolutePath(const std::wstring& path);

int main(int argc, char* argv[])
{
    for(int i = 0; i < argc; ++i)
    {
        printf("%s ", argv[i]);
    }
    printf("\n");

    // update current working directory
    std::string path = argv[0];
    auto pos = path.find_last_of("/\\");
    if (pos != std::string::npos)
    {
        path.erase(path.begin() + pos, path.end());
        auto ok = chdir(path.c_str());
        if (ok != 0)
            throw std::system_error(errno, std::generic_category(), "Failed to set working directory");
    }

    std::wstring resources_dir = GetAbsolutePath(L"../resources");
    std::wstring output_dir = GetAbsolutePath(L"../output");

    std::wstring open_path = resources_dir + L"/QtCode.pdf";        // source PDF document
    std::wstring password = L"";                                  // open password
    //std::wstring save_path = output_dir + L"/test.pdf";        // source PDF document
    std::wstring config_path = resources_dir + L"/config.json";   // configuration file

    try
    {
        if (!DirectoryExists(output_dir, true))
            throw std::runtime_error("Output directory does not exist");

        Initialization();
        AddWatermark(open_path, output_dir + L"/AddWatermark.pdf", resources_dir + L"/watermark.png",
         0, -1, 1, false, kAlignmentLeft, kAlignmentTop, 0.0f, 0.0f, 2.0f, 0.0f, 0.5f);


        // Regex
        RegexSearch(open_path, L"(\\d{4}[- ]){3}\\d{4}");
        RegexSetPattern(open_path);
    }
    catch (std::exception& ex)
    {
        std::cout << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
