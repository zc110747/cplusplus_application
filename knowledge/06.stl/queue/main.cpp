//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2025-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. std::queue声明
//      2. std::queue方法
<<<<<<< HEAD
=======
//      3. std::queue常用算法配合
>>>>>>> 62558f2a9fe27a1dd64315cc3ed8ad9fadd71fcb
//
// Author:
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <queue>
#include <iostream>
#include <algorithm>
#include <string>
#include <unistd.h>

int main(int argc, char* argv[])
{
    //queue
    std::queue<std::string> qcon_0;

    //emplace, push, size, empty
    qcon_0.push("first");
    qcon_0.push("second");
    qcon_0.push("third");
    qcon_0.emplace("fourth");
    qcon_0.push("fifth");
    std::cout << "size:" << qcon_0.size()  << std::endl;
    std::cout << "empty:" << qcon_0.empty()  << std::endl;

    //front
    std::string &strfront = qcon_0.front();
    std::cout << "front: " << strfront  << std::endl;

    //back
    std::string &strback = qcon_0.back();
    std::cout << "back: " << strback  << std::endl;

    //swap
    std::queue<std::string> qcon_1;
    qcon_1.swap(qcon_0);

    //pop
    std::cout << "qcon_1:";
    int size = qcon_1.size();
    for(int index=0; index<size; index++)
    {
        std::cout << qcon_1.front() << " ";
        qcon_1.pop();
    }
    std::cout << std::endl;

    //priority
    std::priority_queue<int> qcon_2;

    //emplace, push, size, empty
    qcon_2.push(7);
    qcon_2.push(5);
    qcon_2.push(6);
    qcon_2.emplace(1);
    qcon_2.push(3);
    std::cout << "pr_size:" << qcon_2.size()  << std::endl;
    std::cout << "pr_empty:" << qcon_2.empty()  << std::endl;

    //swap
    std::priority_queue<int> qcon_3;
    qcon_3.swap(qcon_2);

    //pop
    std::cout << "qcon_3:";
    int pr_size = qcon_3.size();
    for(int index=0; index<pr_size; index++)
    {
        std::cout << qcon_3.top() << " ";
        qcon_3.pop();
    }
    std::cout  << std::endl;

    return 0;
}
