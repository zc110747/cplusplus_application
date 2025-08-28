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

using std::string;
using std::cout;
using std::endl;


int main(int argc, char* argv[])
{
    //stack
    std::stack<string> usr_stack;

    //emplace, push, size, empty
    usr_stack.push("first");
    usr_stack.push("second");
    usr_stack.push("third");
    usr_stack.emplace("fourth");
    usr_stack.push("fifth");
    cout<<"size:"<<usr_stack.size()<<endl;
    cout<<"empty:"<<usr_stack.empty()<<endl;

    //emplace
    usr_stack.emplace("sixth");

    //top
    string &strfront = usr_stack.top();
    cout<<"top: "<<strfront<<endl;

    //swap
    std::stack<string> swapstack;
    swapstack.swap(usr_stack);

    //pop
    cout<<"swapstack:";
    int size = swapstack.size();
    for(int index=0; index<size; index++)
    {
        cout<<swapstack.top()<<" ";
        swapstack.pop();
    }
    cout<<endl;

    return 0;
}
 