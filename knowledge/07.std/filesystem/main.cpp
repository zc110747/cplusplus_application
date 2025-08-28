/*
std::filesystem
C++17 引入的标准库，定义在 <filesystem> 头文件中，提供了跨平台的文件系统操作功能，替代了传统的依赖于操作系统的文件操作函数，让文件和目录操作更加方便、安全和可移植
*/
#include <iostream>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

int main()
{
    // 路径类std::filesystem::path
    fs::path p("file.txt");
    std::cout << "Filename: " << p.filename() << std::endl;
    std::cout << "Parent path: " << p.parent_path() << std::endl;

    //文件和目录操作
    {
        fs::path dirPath("test_dir");
        if (!fs::exists(dirPath)) {
            fs::create_directory(dirPath);
            std::cout << "Directory created." << std::endl;
        }

        fs::path filePath = dirPath / "test.txt";
        std::ofstream(filePath) << "Hello, World!";
        std::cout << "File created." << std::endl;

        fs::remove(filePath);
        std::cout << "File removed." << std::endl;

        fs::remove(dirPath);
        std::cout << "Directory removed." << std::endl;

        // 遍历目录
        {
            fs::path dirPath(".");
            for (const auto& entry : fs::directory_iterator(dirPath)) {
                std::cout << entry.path() << std::endl;
            }      
        }
    }

    // 文件状态查询，文件读写
    {
        fs::path filePath("test.txt");
        if (fs::exists(filePath)) {
            std::cout << "File size: " << fs::file_size(filePath) << " bytes" << std::endl;
            std::ifstream file(filePath);
            if (file.is_open()) {
                std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
                std::cout << "File content:\n" << content << std::endl;
                file.close();
            } else {
                std::cerr << "Unable to open file." << std::endl;
            }
        }
    }

    return 0;
}


