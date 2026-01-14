//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. std::filesystem
//
// Author:
//      @公众号 <嵌入式技术总结>
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <filesystem>
#include <fstream>

int main(int argc, const char* argv[])
{
    // 路径类std::filesystem::path
    std::filesystem::path p("file.txt");
    std::cout << "Filename: " << p.filename() << std::endl;
    std::cout << "Parent path: " << p.parent_path() << std::endl;

    //文件和目录操作
    {
        std::filesystem::path dirPath("test_dir");
        if (!std::filesystem::exists(dirPath)) {
            std::filesystem::create_directory(dirPath);
            std::cout << "Directory created." << std::endl;
        }

        std::filesystem::path filePath = dirPath / "test.txt";
        std::ofstream(filePath) << "Hello, World!";
        std::cout << "File created." << std::endl;

        std::filesystem::remove(filePath);
        std::cout << "File removed." << std::endl;

        std::filesystem::remove(dirPath);
        std::cout << "Directory removed." << std::endl;

        try {
            std::filesystem::path dirPath2("test_dir2");
            if (!std::filesystem::exists(dirPath2)) {
                std::filesystem::create_directory(dirPath2);
                std::cout << "Directory created." << std::endl;
            }
            std::filesystem::remove_all(std::filesystem::path("test_dir3"));
            std::cout << "Directory remove_all:" << std::filesystem::path("test_dir3") << std::endl;
            std::filesystem::rename(dirPath2, "test_dir3");
            std::cout << "Directory renamed, name:" << dirPath2 << std::endl;
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Filesystem error: " << e.what() << std::endl;
        }

        // 遍历目录
        {
            std::filesystem::path dirPath(".");
            for (const auto& entry : std::filesystem::directory_iterator(dirPath)) {
                std::cout << entry.path() << std::endl;
            }      
        }
    }

    // 文件状态查询，文件读写
    {
        std::filesystem::path filePath("test.txt");
        if (std::filesystem::exists(filePath)) {
            std::cout << "File size: " << std::filesystem::file_size(filePath) << " bytes" << std::endl;
            std::ifstream file(filePath);
            if (file.is_open()) {
                std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
                std::cout << "File content:\n" << content << std::endl;
                file.close();
            } else {
                std::cerr << "Unable to open file." << std::endl;
            }
        }

        std::filesystem::copy(filePath, "test_dir3/test.txt");
        std::cout << "File copied." << std::endl;
    }

    return 0;
}


