//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      aggregate.cpp
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
#include "aggregate.hpp"
#include "../includes.hpp"

using namespace std;

namespace AGGREGATE
{
    //15.1 聚合类型的新定义
    class T1
    {
    public:
        int a = 1;
        float b = 1.5;
    };

    class U1:public T1
    {
    };
    
    class MyString:public std::string
    {
    };

    //15.2 聚合类型的初始化
    class Count{
    public:
        int get() {return count_;}
        int count_ = 0;
    };

    class MyStringWithIndex:
        public std::string,
        public Count
    {
    public:
        int index_ = 0;
    };
    
    std::ostream& operator<<(std::ostream& os, MyStringWithIndex& s)
    {
        os<<s.index_<<" "<<s.get()<<" "<<s.c_str();
        return os;
    }

    //15.4 禁止聚合类型使用用户声明的构造函数
    struct X4
    {
        X4() = default;
    };

    struct Y4
    {
        Y4() = delete;
    };

    //15.5 使用带小括号的列表初始化聚合类型
    struct X5
    {
        int i;
        float f;
    };

    void test_run()
    {
        FUNCTION_START()

        //15.1 聚合类型的新定义
        //1.没有用户提供的构造函数
        //2.没有私有的和受保护的非静态数据成员
        //3.没有虚函数
        //如果类存在继承关系
        //4.必须公开的基类，不能是私有或者受保护的基类
        //5.必须是非虚继承
        cout<<"\nchapter15.1\n  ";
        #if __MORE_THAN_CPP17__
        cout<<std::boolalpha<<std::is_aggregate<T1>::value<<" | ";
        cout<<std::boolalpha<<std::is_aggregate<U1>::value<<" | ";
        cout<<std::boolalpha<<std::is_aggregate<std::string>::value<<" | ";
        cout<<std::boolalpha<<std::is_aggregate<MyString>::value<<" | ";
        #else
        __LOWRE_THAN_CPP17_RUN;
        #endif
        
        //15.2 聚合类型的初始化
        //对于聚合类型，按照继承的顺序进行初始化
        cout<<"\nchapter15.2\n  ";
        #if __MORE_THAN_CPP17__
        MyStringWithIndex s2 = {"hello", 5, 11};
        cout<<s2<<" | ";
        MyStringWithIndex val2{"world", 2, 3};
        cout<<val2<<" | ";
        #else
        __LOWRE_THAN_CPP17_RUN;
        #endif

        //15.3 扩展聚合类型的兼容性问题
        //对于聚合类型会调用基类的构造函数，
        cout<<"\nchapter15.3\n  ";

        //15.4 禁止聚合类型使用用户声明的构造函数
        cout<<"\nchapter15.4\n  ";
        #if __MORE_THAN_CPP17__
        cout<<std::boolalpha<<std::is_aggregate<X4>::value<<" | ";
        cout<<std::boolalpha<<std::is_aggregate<Y4>::value<<" | ";
        #else
        __LOWRE_THAN_CPP17_RUN;
        #endif
        
        //15.5 使用带小括号的列表初始化聚合类型
        //聚合类型可以使用小括号初始化
        cout<<"\nchapter15.5\n  ";
        X5 x5{11, 4.5};
        cout<<x5.i<<" "<<x5.f<<" | ";

        #if __MORE_THAN_CPP20__
            X5 y5(4, 3.5);
            cout<<y5.i<<" "<<y5.f<<" | ";
        #else
            __LOWRE_THAN_CPP20_RUN;
        #endif
        FUNCTION_END()
    }
}
