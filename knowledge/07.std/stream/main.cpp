//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. iostream: std::cin, std::cout, std::cerr, std::clog，std::wcout, std::wcerr, std::wclog, std::ostream
//      2. sstream: std::stringstream
//      3. fstream: std::ofstream, std::ifstream, std::fstream
//
// Author:
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream> // 包含<ios> <istream> <ostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdio>
#include <iomanip>

namespace IOSTREAM
{
    void log_info(std::ostream& os, const std::string& str) {
        os << "\"" << str << "\"" << std::endl;
    }

    class object {
    public:
        object() = default;
        int a{1};
        std::string b{"hello world"};
    };

    std::ostream& operator<<(std::ostream& os, const object& obj) {
        os << "a: " << obj.a << ", b: " << obj.b;
        return os;
    }

    int test(void) 
    {
        std::cout << "====================== IOSTREAM =======================" << std::endl;

         // 使用 std::cin 从标准输入读取数据
        int num;
        std::cout << "Enter an integer: ";
        std::cin >> num;
        std::cout << "You entered: " << num << std::endl;

        // 使用 std::getline 从标准输入读取一行字符串
        std::string line;
        std::cout << "Enter a line of text: ";
        std::getline(std::cin, line);
        std::cout << "You entered: " << line << std::endl;

        // 检查输入流的状态
        if (std::cin.good()) {
            std::cout << "Input is good" << std::endl;
        } else if (std::cin.fail()) {
            std::cout << "Input failed" << std::endl;
        } else if (std::cin.bad()) {
            std::cout << "Input is bad" << std::endl;
        } else if (std::cin.eof()) {
            std::cout << "End of file reached" << std::endl;
        }

        // std::cout 标准输出流
        // std::cerr 标准错误流
        // std::clog 标准日志流
        std::cout << "cout test!" << std::endl;
        std::cerr << "err test!" << std::endl;
        std::clog << "log test!" << std::endl; 
  
        log_info(std::cout, "log info");

        // 同步C/C++ 输出
        std::ios::sync_with_stdio(true);
        for(int i=0; i<3; i++) {
            printf("c out:%d\n", i);
            std::cout << "c++ out:" << i << std::endl;
        }

        object obj;
        std::cout << obj << std::endl;

        return 0; 
    }
}

namespace FSTREAM
{
    int test(void) 
    {
        std::cout << "====================== FSTREAM =======================" << std::endl;

        // 创建一个文件输出流对象，并打开一个文件
        std::ofstream outFile("example.txt");

        // 检查文件是否成功打开
        if (!outFile.is_open()) {
            std::cerr << "Error opening file for writing." << std::endl;
            return 1;
        }

        // 向文件中写入数据
        outFile << "Hello, World!" << std::endl;
        outFile << "This is a test file." << std::endl;

        // 关闭文件输出流
        outFile.close();

        // 创建一个文件输入流对象，并打开同一个文件
        std::ifstream inFile("example.txt");

        // 检查文件是否成功打开
        if (!inFile.is_open()) {
            std::cerr << "Error opening file for reading." << std::endl;
            return 1;
        }

        // 从文件中读取数据并输出
        std::string line;
        while (std::getline(inFile, line)) {
            std::cout << line << std::endl;
        }

        // 关闭文件输入流
        inFile.close();

        // 创建一个文件流对象，并打开同一个文件(读写模式)
        std::fstream file("example.txt", std::ios::in | std::ios::out | std::ios::app);
        if (!file.is_open()) {
            std::cerr << "Error opening file for reading and writing." << std::endl;
            return 1;
        }

        // 向文件中追加数据
        file << "Appended line." << std::endl;

        // 关闭文件流
        file.close();

        return 0; 
    } 
}

namespace SSTREAM
{
    int test(void) 
    {
        std::cout << "====================== SSTREAM =======================" << std::endl;

        // 创建一个 stringstream 对象
        std::stringstream ss;

        // 向 stringstream 中写入数据
        ss << "Hello,World!";
        ss << "This is a test.";

        // 从stringstream中读取数据
        std::string str;
        ss >> str;

        // 输出读取到的数据
        std::cout << "Read from stringstream: " << str << std::endl;
        
        std::cout << "good:" << ss.good() << std::endl;
        std::cout << "eof:" << ss.eof() << std::endl;
        std::cout << "fail:" << ss.fail() << std::endl;
        std::cout << "bad:" << ss.bad() << std::endl;

        // 清空 stringstream
        ss.str("");
        ss.clear();

        // 向 stringstream 中写入不同类型的数据
        int num = 42;
        double pi = 3.14159;
        ss << "The answer is " << num << " and pi is " << pi;

        // 从 stringstream 中读取数据到不同类型的变量
        std::string result;
        ss >> result >> num >> result >> pi;

        // 输出读取到的数据
        std::cout << "Read from stringstream: " << result << ", " << num << ", " << result << ", " << pi << std::endl;

        std::vector<std::string> tokens;
        std::istringstream tokenStream("This is a test string");
        std::string token;

        while (tokenStream >> token) {
            tokens.push_back(token);
        }
        // 输出分割后的字符串
        for (const auto& token : tokens) {
            std::cout << token << " ";
        }
        std::cout<<std::endl;

        tokens.clear();
        std::istringstream tokenStream2("This is a test string, now with commas");

        while (std::getline(tokenStream2, token, ',')) {
            tokens.push_back(token);
        }
        // 输出分割后的字符串
        for (const auto& token : tokens) {
            std::cout << token << "; ";
        }
        std::cout<<std::endl;

        tokens.clear();

        return 0; 
    }
}

int main(int argc, char* argv[]) 
{
    IOSTREAM::test();

    FSTREAM::test();

    SSTREAM::test();
    return 0;
}