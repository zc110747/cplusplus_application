//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. std::string声明
//      2. std::string方法
//
// Author:
//      @公众号：<嵌入式技术总结>
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <string>
#include <iostream>
#include <algorithm>
#include <unistd.h>

int main(int argc, char* argv[])
{
    std::string str_0{"ustr,"};
    std::string str_1 = std::string("Let's go!");

    //append, operator+=, operator[]
    str_0.append("Let's go!");
    std::cout << "append:" << str_0 << std::endl;
    str_0.append(str_1, 6, 3);
    std::cout << "append:" << str_0 << std::endl;
    str_0 += "everyone!";
    std::cout << "operator +:" << str_0 << std::endl;
    std::cout << "operator []:" << str_0[0] << std::endl;
    str_0 = str_0 + " test";
    std::cout << "operator: " << str_0 << std::endl;
    auto equal0 = (str_0 != "test");
    auto equal1 = (str_0 == "test");
    std::cout << "operator !=/== " << equal0 << " | " << equal1 << std::endl;

    //assign
    std::string str_2;
    str_2.assign(str_0);
    std::cout << "assign:" << str_2 << std::endl;

    //at, back, front
    char& atval = str_2.at(1);
    char& backval = str_2.back();
    std::string::reference frontval = str_2.front();
    std::cout << "at:" << atval << std::endl;
    std::cout << "back:" << backval << std::endl;
    std::cout << "front:" << frontval << std::endl;

    //begin, end, cbegin, cend, rbegin, rend, crbegin, crend
    std::string::iterator iterbegin = str_2.begin();
    std::string::iterator iterend = str_2.end();
    std::string::const_iterator itercbegin = str_2.cbegin();
    std::string::const_iterator itercend = str_2.cend();
    std::string::reverse_iterator iterrbegin = str_2.rbegin();
    std::string::reverse_iterator iterrend = str_2.rend();
    std::string::const_reverse_iterator itercrbegin = str_2.crbegin();
    std::string::const_reverse_iterator itercrend = str_2.crbegin();
    std::cout << "begin:" << *iterbegin << " " << "end:" << *(iterend-1) << " ";
    std::cout << "cbegin:" << *itercbegin << " " << "cend:" << *(itercend-1) << " ";
    std::cout << "rbegin:" << *iterrbegin << " " << "rend:" << *(iterrend-1) << " ";
    std::cout << "crbegin:" << *itercrbegin << " " << "crend:" << *(itercrend-1) << " " << std::endl;
    
    //resize, shrink_to_fit
    str_2.resize(5);
    std::cout << "resize:" << str_2 << std::endl;
    str_2.shrink_to_fit();
    std::cout << "shrink_to_fit:" << str_2.capacity() << std::endl;
    str_2.reserve(10);
    std::cout << "reserve:" << str_2.capacity() << std::endl;

    //capacity, size, clear, max_size
    std::cout << "capacity:" << str_2.capacity() << std::endl;
    std::cout << "length:" << str_2.length() << std::endl;
    std::cout << "size:" << str_2.size() << " ";
    str_2.clear();
    std::cout << "size:" << str_2.size() << std::endl;
    std::cout << "max_size:" << str_2.max_size() << std::endl;
    std::cout << "empty:" << str_2.empty() << std::endl;

    // compare
    if(str_1.compare("Let's go!") == 0) {
        std::cout << "comapre equal!" << std::endl;
    } else {
        std::cout << "compare no equal!" << std::endl;
    }

    // copy
    char buffer[10] = {0};
    str_1.copy(buffer, 6, 0);
    std::cout << "copy:" << buffer << std::endl;
    
    // c_str
    const char *pcstr;
    pcstr = str_1.c_str();
    std::cout << "c_str:" << pcstr << std::endl;

    // data
    const char *pcdata;
    pcdata = str_1.data();
    std::cout << "data:" << pcdata << std::endl;

    // for-auto
    std::cout << "for auto:";
    for (auto val:str_1) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    //erase
    str_1.erase(2, 1);
    std::cout << "erase:" << str_1 << std::endl;

    //pop_back, push_back
    str_1.pop_back();
    std::cout << "pop_back:" << str_1 << std::endl;
    str_1.push_back('c');
    std::cout << "push_back:" << str_1 << std::endl;

    //insert
    str_1.insert(0, "hello~ ");
    std::cout << "insert:" << str_1 << std::endl;

    //swap
    std::string str_3;
    str_3.swap(str_1);
    std::cout << "swap:" << str_3 << std::endl;

    //find, rfind, find_first_of
    std::string::size_type position;
    position = str_3.find("he");
    if(position != str_3.npos)
    {
        std::cout << "find position" << position << std::endl;
    }
    std::cout << "find_first_of:" << str_3.find_first_of("he") << std::endl; 
    std::cout << "find_first_not_of:" << str_3.find_first_not_of("he") << std::endl; 
    std::cout << "find_last_of:" << str_3.find_last_of("he") << std::endl; 
    std::cout << "find_last_not_of:" << str_3.find_last_not_of("he") << std::endl;   
    std::cout << "rfind:" << str_3.rfind("he") << std::endl;

    //replace
    str_3.replace(str_3.begin(), str_3.begin()+2, "she");
    std::cout << "replace:" << str_3 << std::endl;

    //substr
    std::string substring = str_3.substr(1, 3);
    std::cout << "substr:" << substring << std::endl;

    return 0;
} 
