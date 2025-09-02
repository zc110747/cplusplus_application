//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      nullptr.cpp
//
//  Purpose:
//
//  Author:
//     	@听心跳的声音
//
//  Assumptions:
//
//  Revision History:
//      4/19/2023   Create New Version
/////////////////////////////////////////////////////////////////////////////
#include "nullptr.hpp"
#include "../includes.hpp"

using namespace std;

namespace NULLPTR
{
    //23.1 零值整数字面量
    void f(int)
    {
        std::cout<<"int"<<std::endl;
    }
    void f(int *)
    {
        std::cout<<"int *"<<std::endl;
    }

    //23.2 nullptr关键字
    template<class T>
    class Widget
    {
    public:
        Widget(){
            cout<<"Widget"<<" | ";
        }
    };

    template<>
    class Widget<std::nullptr_t>
    {
    public:
        Widget(){
            cout<<"nullptr Widget"<<" | ";
        }
    };

    template<class T>
    Widget<T> *make_widget(T)
    {
        return new Widget<T>();
    }

    void test_run()
    {
        FUNCTION_START()

        //23.1 零值整数字面量
        cout<<"\nchapter23.1\n  ";
        char *p_1 = NULL;

        //f(NULL);  //call of overloaded ‘f(NULL)’ is ambiguous
        f(reinterpret_cast<int *>(NULL));

        //23.2 nullptr关键字
        //nullptr时纯右值，为nullptr_t类型
        //nullptr可以隐式转换为所有类型
        //nullptr_t声明的对象等同于nullptr
        cout<<"\nchapter23.2\n  ";    
        char *p_2 = nullptr;
        if(!p_2)
            cout<<"nullptr"<<" | ";

        //可以将NULL和nullptr比较
        char *p1_2 = NULL;
        if(p1_2 == nullptr)
            cout<<"nullptr"<<" | ";
        
        //nullptr是关键字
        //null1, null2, nullptr可以进行比较，相同
        //nullptr是纯右值,null1, null2是左值，可以取地址
        decltype(nullptr) null1;
        std::nullptr_t null2;
        char *p2_2 = null1;
        char *p3_2 = null2;
        if(p2_2 == nullptr)
            cout<<"nullptr"<<" | ";
        if(p3_2 == nullptr)
            cout<<"nullptr"<<" | ";

        //NULL定义为0,默认会执行int类型，nullptr可以正确执行特化
        auto w1_2 = make_widget(NULL);
        auto w2_2 = make_widget(nullptr);
        delete w1_2;
        delete w2_2;

        FUNCTION_END()
    }
}