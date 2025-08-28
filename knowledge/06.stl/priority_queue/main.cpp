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
//      3. std::priority_queue常用算法配合
//
// Author:
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

int main(int argc, char** argv) 
{
    std::priority_queue<int> p_queue;

    // size, top, push, pop
    p_queue.push(1);
    p_queue.push(2);
    p_queue.push(3);
    std::cout<<"pp_queueq.size() = "<<p_queue.size()<<std::endl;
    std::cout<<"p_queue.top() = "<<p_queue.top()<<std::endl;

    while(!p_queue.empty()) {
        cout<<p_queue.top()<<", ";
        p_queue.pop();
    }
    cout<<endl;

    // 构造函数
    std::vector<int> vec = {1, 6, 4, 2, 3};
    std::priority_queue<int> p_queue2(vec.begin(), vec.begin()+3);
    while(!p_queue2.empty()) {
        cout<<p_queue2.top()<<", ";
        p_queue2.pop();
    }
    cout<<endl;

    std::priority_queue<int, std::vector<int>, std::greater<int>> p_queue3(vec.begin(), vec.begin()+3);
    while(!p_queue3.empty()) {
        cout<<p_queue3.top()<<", ";
        p_queue3.pop();
    }
    cout<<endl;
    
    return 0;
}
