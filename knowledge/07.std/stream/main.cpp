//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. std::iostream
//
// Author:
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <istream>
#include <sstream>
#include <ostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdio>

void istream_test(void)
{
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

    // 使用 std::istringstream 从字符串中读取数据
    std::string data = "123 456 789";
    std::istringstream iss(data);
    int a, b, c;
    iss >> a >> b >> c;
    std::cout << "a = " << a << ", b = " << b << ", c = " << c << std::endl;

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
}

void sstream_test(void)
{
    // 创建一个 stringstream 对象
    std::stringstream ss;

    // 向 stringstream 中写入数据
    ss << "Hello, " << "World!";

    // 从 stringstream 中读取数据
    std::string str;
    ss >> str;

    // 输出读取到的数据
    std::cout << "Read from stringstream: " << str << std::endl;

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
}

int ffstream_test(void)
{
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

    return 0;
}
void log_info(std::ostream& os, const std::string& str) {
    os << "\"" << str << "\"" << std::endl;
}

int main(int argc, char* argv[]) 
{
    //istream_test();

    sstream_test();

    ffstream_test();

    std::cerr << "err test!" << std::endl;

    std::clog << "log test!" << std::endl;

    // 输出ostream
    log_info(std::cout, "log info");

    // 输出ofstream
    {
        std::ofstream outFile("example.txt");
        log_info(outFile, "log info step-1");
        log_info(outFile, "log info step-2");
    }

    // 输出stringstream
    {
        std::stringstream ss;
        log_info(ss, "log info step-1");
        log_info(ss, "log info step-2");
        std::cout << ss.str();
    }

    // sync_with_stdio设置C++和C同步
    std::ios::sync_with_stdio(true);
    {
        for(int i=0; i<3; i++) {
            printf("c out:%d\n", i);
            std::cout << "c++ out:" << i << std::endl;
        }
    }
    return 0;
}