//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2025-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. std::priority_queue声明
//      2. std::priority_queue方法
//
// Author:
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <queue>
#include <string>
#include <vector>

template<typename T>
void show_container(T container_val, std::string qstring)
{
    //empty, size
    if (!qstring.empty()) {
        for(auto index=qstring.size(); index<13; index++) {
            qstring.push_back(' ');
        }
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

int main(int argc, char** argv) 
{
    std::priority_queue<int> qcon_0;

    // size, top, push, pop
    qcon_0.push(1);
    qcon_0.push(2);
    qcon_0.push(3);
    std::cout << "size: " << qcon_0.size() << std::endl;
    std::cout << "top: " << qcon_0.top() << std::endl;
    show_container(qcon_0, "qcon_0");

    // 构造函数
    std::vector<int> vec = {1, 6, 4, 2, 3};
    std::priority_queue<int> qcon_1(vec.begin(), vec.begin()+3);
    show_container(qcon_1, "qcon_1");

    // 自定义allocator和比较函数
    std::priority_queue<int, std::vector<int>, std::greater<int>> qcon_2(vec.begin(), vec.begin()+3);
    show_container(qcon_2, "qcon_2");

    // 赋值运算符
    struct classcomp{
        bool operator() (const char& lhs, const char& rhs) const
            { return rhs < lhs; }
    };
    std::priority_queue<int, std::vector<int>, classcomp> qcon_3;
    qcon_3.push(1);
    qcon_3.push(2);
    qcon_3.push(3);
    show_container(qcon_3, "qcon_3");

    // lambda对象初始化
    constexpr auto funcomp = [](const char& lhs, const char& rhs) {return lhs<rhs;};
    std::priority_queue<int, std::vector<int>, decltype(funcomp)> qcon_4(funcomp);
    qcon_4.push(1);
    qcon_4.push(2);
    qcon_4.push(3);
    show_container(qcon_4, "qcon_4");

    return 0;
}
