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
//      3. std::queue常用算法配合
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

using namespace std;

int main(int argc, char* argv[])
{
    //queue
    std::queue<string> usr_queue;

    //emplace, push, size, empty
    usr_queue.push("first");
    usr_queue.push("second");
    usr_queue.push("third");
    usr_queue.emplace("fourth");
    usr_queue.push("fifth");
    cout<<"size:"<<usr_queue.size()<<endl;
    cout<<"empty:"<<usr_queue.empty()<<endl;

    //front
    string &strfront = usr_queue.front();
    cout<<"front: "<<strfront<<endl;

    //back
    string &strback = usr_queue.back();
    cout<<"back: "<<strback<<endl;

    //swap
    std::queue<string> swapqueue;
    swapqueue.swap(usr_queue);

    //pop
    cout<<"swapqueue:";
    int size = swapqueue.size();
    for(int index=0; index<size; index++)
    {
        cout<<swapqueue.front()<<" ";
        swapqueue.pop();
    }
    cout<<endl;

    //priority
    std::priority_queue<int> userpriority_queue;

    //emplace, push, size, empty
    userpriority_queue.push(7);
    userpriority_queue.push(5);
    userpriority_queue.push(6);
    userpriority_queue.emplace(1);
    userpriority_queue.push(3);
    cout<<"pr_size:"<<userpriority_queue.size()<<endl;
    cout<<"pr_empty:"<<userpriority_queue.empty()<<endl;

    //swap
    std::priority_queue<int> swappriority_queue;
    swappriority_queue.swap(userpriority_queue);

    //pop
    cout<<"swappriority_queue:";
    int pr_size = swappriority_queue.size();
    for(int index=0; index<pr_size; index++)
    {
        cout<<swappriority_queue.top()<<" ";
        swappriority_queue.pop();
    }
    cout<<endl;

    return 0;
}
