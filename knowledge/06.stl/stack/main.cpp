//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2025-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. std::stack声明
//      2. std::stack方法
//      3. std::stack常用算法配合
//
// Author:
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <stack>
#include <iostream>
#include <algorithm>
#include <string>
#include <unistd.h>
#include <vector>

template<typename T>
void show_container(T container_val, std::string qstring)
{
    //empty, size
    if (!qstring.empty()) {
        for(auto index=qstring.size(); index<13; index++)
            qstring.push_back(' ');
        qstring += ":";
        std::cout << qstring;
    }

    T temp_container(container_val);
    while (!temp_container.empty()) {
        std::cout << temp_container.top() << ", ";
        temp_container.pop();
    }

    std::cout << std::endl;
}

int main(int argc, char* argv[])
{
    // stack
    std::stack<std::string> scon_0;

    // emplace, push, size, empty
    scon_0.push("first");
    scon_0.push("second");
    scon_0.push("third");
    scon_0.emplace("fourth");
    scon_0.push("fifth");
    std::cout << "size:" << scon_0.size()<< std::endl;
    std::cout << "empty:" << scon_0.empty()<< std::endl;
    show_container(scon_0, "scon_0");

    // emplace
    scon_0.emplace("sixth");

    // top
    std::string &strfront = scon_0.top();
    std::cout << "top: " << strfront<< std::endl;

    // swap
    std::stack<std::string> scon_1;
    scon_1.swap(scon_0);

    // pop
    std::cout << "size:" << scon_1.size() << std::endl;
    scon_1.pop();
    std::cout << "size:" << scon_1.size() << std::endl;
    std::cout << "top: " << scon_1.top() << std::endl;
    show_container(scon_1, "scon_1");

    // stack with vector
    std::vector<int> vec_0{2, 1, 4, 5, 5};
    std::stack<int, std::vector<int>> scon_2(vec_0);
    show_container(scon_2, "scon_2");

    return 0;
}
 