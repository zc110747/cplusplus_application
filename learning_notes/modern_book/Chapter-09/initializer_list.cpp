//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      initializer_list.cpp
//
//  Purpose:
//      第9章 列表初始化
//
//  Author:
//     	@公众号：<嵌入式技术总结>
//
//  Assumptions:
//
//  Revision History:
//      4/19/2023   Create New Version
/////////////////////////////////////////////////////////////////////////////
#include "initializer_list.hpp"
#include "../includes.hpp"

using namespace std;

namespace INITIALIZE_LIST
{
    //9.1 回顾变量初始化
    //9.2 使用列表初始化
    class C
    {
    public:
        C(int a):x_(a) {}
        C(std::string str):str_(str) {}
        void print()
        {
            cout<<x_<<":"<<str_<<" | ";
        }
    private:
        int x_{0};
        string str_{"default"};
    };
    void foo(C c) 
    {
        c.print();
    }

    //9.3 std::initializer_list详解
    struct C3
    {
        C3(std::initializer_list<std::string> list)
        {
            for(const auto *item = list.begin(); item != list.end(); ++item)
            {
                cout<<*item<<" | ";
            }
        }
        C3(std::string c)
        {
            cout<<"C3:"<<c<<" | ";
        }
    };

    //9.5 指定初始化
    #if __MORE_THAN_CPP20__
    struct Point
    {
        int x;
        int y;
    };
    union u{
        int a;
        const char* b;
    };
    #endif
    void test_run()
    {
        FUNCTION_START()

        //9.1 回顾变量初始化
        //使用括号直接初始化，等号进行拷贝初始化或者隐式调用直接初始化
        cout<<"\nchapter9.1\n  ";
        C x1(6);
        C y1 = 8;   //隐式调用  
        x1.print();
        y1.print();
        
        //9.2 使用列表初始化
        //拷贝初始化和直接初始化
        cout<<"\nchapter9.2\n  ";
        int a2 = {5};
        int a12{3};
        cout<<a2<<" | "<<a12<<" | ";
        
        C x2 = {4};
        C y2{2};
        x2.print();
        y2.print();
        foo({8});
        foo({"hello"});
        
        C *ptr2 = new C{"view"};
        ptr2->print();
        delete ptr2;
        ptr2 = nullptr;
        cout<<"\n  ";
        
        #if __MORE_THAN_CPP14__
            auto func_print = [](auto value){
                for(const auto& val:value)
                {
                    cout<<val<<" | ";
                }
            };
            vector<int> vec2 = {1, 2, 3, 4, 5};
            func_print(vec2);
            list<int> list2 = {1, 2, 3, 4, 5};    
            func_print(list2);
        #else
            __LOWRE_THAN_CPP14_RUN;
        #endif
        
        int arr2[] = {1, 2, 3, 4};
        for(const auto& val:arr2)
        {
            cout<<val<<" | ";
        }

        //9.3 std::initializer_list详解
        cout<<"\nchapter9.3\n  ";
        C3 c3{"hello", "c++"};

        //9.4 使用列表初始化的注意事项
        //隐式缩窄转换问题, 列表初始化不支持隐式缩窄
        //1.从浮点类型转换整数类型
        //2.从long double转换到double或float，double转换为float
        //3.从整数类型或非强枚举类型转换为浮点类型
        //4.从整数类型或非强枚举类型转换为不能代表所有原始类型值的整数类型
        //优先级问题
        //如果一个类拥有同时满足列表初始化的构造函数，且其中一个以initializer_list为参数
        //将优先以initializer_list进行初始化
        cout<<"\nchapter9.4\n  ";
        int x4 = 2;
        double bd4 = 1.5;
        long double lbd4 = 2.4;
        
        //会报警告的缩窄
        //int y4 = {1.5};       //规则1
        //float z4 = {bd};      //规则2
        //double l4{lbd4};      //规则2
        //char c4{x4};
        //float a4 = {x4};      //规则3
        //unsigned int d4{-1};    //规则4
        cout<<x4<<" | "<<bd4<<" | "<<lbd4<<" | ";

        //第一个优先调用initializer_list
        C3 c3_4{"hello"};
        C3 cl_4("hello");

        //9.5 指定初始化
        //1.对象必须是聚合类型
        //2.指定的数据成员必须是非静态数据成员
        //3.每个非静态数据成员最多只能初始化一次
        //4.非静态数据成员的的初始化必须按照声明的顺序
        //5.针对联合体的数据成员只能初始化一次
        //6.不能嵌套指定初始化数据成员
        //7.一旦指定初始化，就不能使用其它初始化方法
        //8.指定初始化不支持初始化数组(lambda冲突)
        cout<<"\nchapter9.5\n  ";
        #if __MORE_THAN_CPP20__
        Point p5{.x = 4, .y = 5};
        cout<<p5.x<<" | "<<p5.y<<" | ";
        Point pl5{.x = 4};
        cout<<pl5.x<<" | "<<pl5.y<<" | ";

        u f5 = {.a = 1};
        u fl5 = {.b = "adsda"};
        #else
        __LOWRE_THAN_CPP20_RUN
        #endif
        FUNCTION_END()
    }
}
