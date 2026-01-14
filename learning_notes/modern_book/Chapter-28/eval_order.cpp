//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      eval_order.cpp
//
//  Purpose:
//      第28章 确定的表达式求值顺序
//
//  Author:
//     	@公众号 <嵌入式技术总结>
//
//  Assumptions:
//
//  Revision History:
//      4/19/2023   Create New Version
/////////////////////////////////////////////////////////////////////////////

#include "eval_order.hpp"
#include "../includes.hpp"

using namespace std;

// void *operator new(size_t size)
// {
//     void *pT = static_cast<void *>(malloc(size));
    
//     cout<<"malloc memory!"<<" | ";
    
//     if(pT == NULL)
//         return NULL;

//     return pT;
// }

namespace EVAL_ORDER
{
    void test_run()
    {
        FUNCTION_START()

        //28.1  表达式求值顺序的不确定性
        cout<<"\nchapter 28.1\n  ";
        std::string s = "but i have heard it works even if you don't believe in it";
        s.replace(0, 4, "").replace(s.find("even"), 4, "only").replace(s.find(" don't"), 6, "");
        cout<<s<<" | ";

        //28.2 表达式求值顺序详解
        // 对于后缀表达式和移位操作符，表达式总是从左到右
        //  E1[E2], E1.E2, E1.*E2, E1->*E2, E1<<E2, E1>>E2
        // 对于赋值表达式，表达式求值总是从右到左
        //  E1=E2, E1+=E2, E1-=E2, E1*=E2, E1/=E2
        // c++17后
        // new表达式的内存分配优先于构造函数中参数的E的求值(E)
        cout<<"\nchapter 28.2\n  ";
        auto E1 = [](){
            cout<<"E1"<<" | ";
            return 1;
        };
        auto E2 = [](){
            cout<<"E2"<<" | ";
            return 1;
        };
        cout<<(E1()<<E2())<<" | "; //先执行E1,再执行E2
        
        auto E3 = [](){
            char *ptr = new char(1);
            cout<<"E3"<<" | ";
            return ptr;
        };
        *E3() = E2();              //先执行E2,再执行E3

        //c++17要求，new比E1先执行
        //gcc12.1后测试才和要求一致
        auto *ptr = new int(E1());
        cout<<*ptr<<" | ";
        delete(ptr);

        FUNCTION_END()
    }
}