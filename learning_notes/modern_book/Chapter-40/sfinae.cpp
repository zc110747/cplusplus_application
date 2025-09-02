//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      sfinae.cpp
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
#include "sfinae.hpp"
#include "../includes.hpp"

using namespace std;

namespace SFINAE
{
    char foo(int)
    {
        cout<<"foo int"<<" | ";
        return 0;
    }

    char foo(float)
    {
        cout<<"foo float"<<" | ";
        return 0;
    }

    template<int I> struct X{};

    template<class T>
    X<sizeof(foo((T)0))> f(T)
    {
        foo((T)0);
        return X<sizeof(foo((T)0))>();
    }

    //40.2 SFINAE规则详解
    template<class T>
    T foo2(T& t)
    {
        T tt(t);
        cout<<"foo2 template"<<" | ";
        return tt;
    }

    void foo2(...)
    {
        cout<<"foo2 var"<<" | ";
    }

    class bar2
    {
    public:
        bar2() {};
        bar2(bar2 &&) {};    
    };

    struct X3{};
    struct Y{ Y(X3){} };
    X3 foo3(Y, Y) {
        cout<<"foo3"<<" | ";
        return X3(); 
    }
    template<class T>
    auto foo3(T t1, T t2)->decltype(t1 + t2){
        cout<<"foo3 add"<<" | ";
        return t1 + t2;
    }

    template<int I> void foo4(char(*)[I%2 == 0] = 0){
        cout<<"I%2 == 0"<<" | ";
    }
    template<int I> void foo4(char(*)[I%2 == 1] = 0){
        cout<<"I%2 == 1"<<" | ";
    }

    void test_run()
    {
        FUNCTION_START()

        //40.1 替换失败和编译错误
        //SFINE主要指函数模板重载时，当模板的实参或由函数实参推导出
        //模板形参过程中出现了失败，则放弃重载而不是抛出一个编译错误。
        cout<<"\nchapter40.1\n  ";
        f(1);
        f((float)1.5);
        
        //40.2 SFINAE规则详解
        //模板形参的替换有两个时机
        //在模板推导的最开始阶段，当明确指定替换模板形参的实参时进行替换
        //在模板推导的最后，模板形参会根据实参进行推导或使用默认的模板实参
        //在直接上下文中使用模板实参替换形参后，类型或者表达式不符合语法，则替换失败。
        //而替换后在非直接上下文中产生副作用导致的错误则被当作编译错误
        //1.处理表达式外部某些实体时发生错误，比如实例化某模板或生成某隐式定义的成员函数
        //2.由于实现限制导致的错误
        //3.由于访问违规导致的错误
        //4.由于同一个函数的不同声明的语法顺序不同，导致替换顺序不同或者无法产生替换的错误
        cout<<"\nchapter40.2\n  ";
        double x = 1.5;
        foo2(x);
        foo2(5);

        //bar2删除了复制构造函数，编译出错
        // bar2 bar21;
        // foo2(bar21);

        X3 x1, x2;
        X3 x3 = foo3(x1, x2);

        char a[1];
        foo4<1>(&a);
        foo4<2>(&a);
        foo4<3>(&a);

        FUNCTION_END()
    }
}