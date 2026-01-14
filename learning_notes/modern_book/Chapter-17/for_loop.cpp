//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      for_loop.cpp
//
//  Purpose:
//
//  Author:
//     	@公众号 <嵌入式技术总结>
//
//  Assumptions:
//
//  Revision History:
//      4/19/2023   Create New Version
/////////////////////////////////////////////////////////////////////////////
#include "for_loop.hpp"
#include "../includes.hpp"

using namespace std;

namespace FOR_LOOP
{
    //17.1 烦琐的容器遍历 
    void print(std::map<int, std::string>::const_reference e)
    {
        cout<<e.first<<"="<<e.second<<" | ";
    }

    //17.4 临时范围表达式的陷阱
    class T
    {
    public:
        std::vector<int> &items() {return data_;};
    private:
        std::vector<int> data_{1, 2, 3};
    };

    T foo()
    {
        T t;
        return t;
    }

    //17.5 实现一个支持基于范围的for循环的类
    template<unsigned int array_size>
    class UserObject
    {
    public:
        UserObject(std::initializer_list<int> list)
        {
            auto ptr = data_;
            for(auto &e:list)
            {
                *ptr = e;
                ptr++;
            }
        }

        int *begin(){
            return data_;
        }
        int *end(){
            return &data_[array_size];
        }
    private:
        int data_[array_size];
    };

    void test_run()
    {
        FUNCTION_START()

        //17.1 烦琐的容器遍历 
        cout<<"\nchapter17.1\n  ";
        std::map<int, std::string> index_map = {
            {1, "first"},
            {2, "second"},
            {3, "three"}
        };
        
        for(auto it = index_map.begin(); it != index_map.end(); ++it)
        {
            cout<<(*it).first<<"="<<(*it).second<<" | ";
        }
        cout<<"\n  ";
        std::for_each(index_map.begin(), index_map.end(), print);

        //17.2 基于范围的for循环语法（满足任意一个条件）
        //1.对象类型定义了begin和end成员函数
        //2.定义了以对象类型为参数的的begin和end普通函数
        //for(range_declaration:range_expression) loop_statement
        cout<<"\nchapter17.2\n  ";
        int int_array[] = {1, 2, 3, 4, 5};
        for(const auto& val:index_map)
        {
            cout<<val.first<<"="<<val.second<<" | ";
        }
        for(const auto& val:int_array)
        {
            cout<<val<<" | ";
        }

        //17.3 begin和end函数不必返回相同类型(C++17)
        //伪代码, 只要__begin != __end返回bool值即可
        // auto && __range = range_expression;
        // auto __begin = begin_expr;
        // auto __end = end_expr;
        // for(; __begin != __end; ++__begin){
        //     range_declaration = *begin;
        //     loop_statement;
        // }

        //17.4 临时范围表达式的陷阱
        //range_expression时纯右值时正常，如果时泛左值，则有问题
        //auto && __range = range_expression
        cout<<"\nchapter17.4\n  ";
        //值出错，未定义行为
        for(auto &x:foo().items()) {
            cout<<x<<" | ";
        }

        auto thing = foo();
        for(auto &x:thing.items()) {
            cout<<x<<" | ";
        }

        #if __MORE_THAN_CPP20__
        for(T thing=foo(); auto &x:thing.items())
        {
            cout<<x<<" | ";
        }
        #else
            __LOWRE_THAN_CPP20_RUN
        #endif

        //17.5 实现一个支持基于范围的for循环的类
        //1.该类型必须具有一组和其类型相关的begin和end函数(成员函数或普通函数)
        //2.begin和end函数需要返回一组迭代器对象，支持operator *， operator++和operator!=
        cout<<"\nchapter17.5\n  ";
        UserObject<3> obj{1, 2, 3};
        for(auto &val:obj)
        {   
            cout<<val<<" | ";
        }
        FUNCTION_END()
    }
}

