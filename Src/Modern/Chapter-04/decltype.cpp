//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      decltype.cpp
//
//  Purpose:
//      第4章 decltype说明符
//
//  Author:
//     	@听心跳的声音
//
//  Assumptions:
//
//  Revision History:
//      4/19/2023   Create New Version
/////////////////////////////////////////////////////////////////////////////
#include "decltype.hpp"
#include "../includes.hpp"

using namespace std;

namespace DECLTYPE
{
    //4.2 使用decltype说明符
    //在结构体非静态成员变量中使用
    struct S1
    {
        int x1;
        decltype(x1) x2;
        double x3;
        decltype(x2+x3) x4;
    };

    int x = 5;
    decltype(x) sum(decltype(x) a1, decltype(x) a2)
    {
        return a1+a2;
    }

    template<class T1, class T2>
    auto sum_e(T1 a1, T2 a2)->decltype(a1+a2)
    {
        return a1+a2;
    }

    template<class T>
    auto return_ref(T& t)->decltype(t)
    {
        return t;
    }

    //4.3 推导规则
    const int &&foo();
    struct A{
        double x;
    };

    //4.5 decltype(auto)
    #if __MORE_THAN_CPP14__
    int &&f();

    template<class T>
    decltype(auto) return_ref_1(T& t)
    {
        return t;
    }
    #endif

    //4.6 decltype(auto)作为非类型模板形参占位符
    #if __MORE_THAN_CPP14__
    template<decltype(auto) N>
    void func_6()
    {
        cout<<N<<" | ";
    }
    #endif

    void test_study()
    {
        FUNCTION_START()

        //4.1 回顾typeof和typeid
        //1.typeid的返回值是一个左值，生命周期被扩展到程序生命周期结束
        //2.typeid的返回值std::type_info删除了复制构造函数，若想保存std::type_info,只能获取引用或者指针
        //3.typeid的返回值总是忽略类型的cv限定符
        cout<<"\nchapter4.1\n  ";
        int a_1 = 0;
        double b_1 = 2.3;
        cout<<typeid(a_1).name()<<" | ";
        cout<<typeid(a_1+b_1).name()<<" | ";
        cout<<typeid(int).name()<<" | ";
        auto &t_1 = typeid(int);
        auto f_1 = &typeid(double);
        cout<<t_1.name()<<" | ";
        cout<<f_1->name()<<" | ";
        cout<<(typeid(const int) == typeid(int))<<" | ";

        //4.2 使用decltype说明符
        //使用decltype可以获取对象或者表达式的类型
        cout<<"\nchapter4.2\n  ";
        int a_2 = 0;
        decltype(a_2) b_2 = 0;
        cout<<typeid(b_2).name()<<" | ";

        double x_2 = 0;
        decltype(a_2 + x_2) y_2 = a_2 + x_2;
        cout<<typeid(y_2).name()<<" | ";

        S1 s_2{1, 2, 1.5, 1.6};
        cout<<s_2.x1<<" | "<<s_2.x2<<" | "<<s_2.x3<<" | "<<s_2.x4<<" | ";
        auto val_2 = sum(5, 6);
        cout<<val_2<<" | ";

        auto sum_2 = sum_e(5, 10.5);
        cout<<sum_2<<" | ";

        cout<<std::is_reference<decltype(return_ref(sum_2))>::value<<" | ";

        //4.3 推导规则
        //1.如果e是一个未加括号的标识符表达式（结构化绑定除外）或者未加括号的类成员访问，
        //  则decltype(e)推断出的类型是e的类型T
        //2.如果e是一个函数调用或者仿函数调用，那么decltype(e)推断出的类型是其返回值的类型
        //3.如果e是一个类型为T的左值，则decltype(e)是T&
        //4.如果e是一个类型为T的将亡值，则decltype(e)是T&&
        //5.除去以上情况，则decltype(e)是T 
        cout<<"\nchapter4.3\n  ";
        int x_3, y_3;
        decltype(x_3+y_3) z_3;              //int
        cout<<typeid(z_3).name()<<" | ";
        decltype(x_3) f_3;                  //int
        cout<<typeid(f_3).name()<<" | ";
        decltype(foo()) i_3 = 5;            //const int&& 函数调用是其返回值的类型
        cout<<typeid(i_3).name()<<" | ";

        A a_3;
        const A* d_3 = new A();
        decltype(a_3.x) b_3;                            //double
        decltype((a_3.x)) c_3 = b_3;                    //double &
        decltype((d_3->x)) e_3 = b_3;                   //const double &

        int *j_3;
        decltype(j_3) h_3;                              //int *
        int n_3[10];
        decltype(n_3) s_3;                              //int[10]
        decltype(foo) func_;                            //int const &&(void)
        decltype(x_3=1) l_3 = x_3;                      //int&
        decltype(0, x_3)  a1_3 = x_3;                   //int& 表达式返回的是后面的值，左值
        decltype(x_3, 0)  b1_3;                         //int
        decltype(*j_3)  c1_3 = b1_3;                    //int &
        decltype(static_cast<int &&>(b1_3)) d1_3 = 53;  //int &&
        cout<<typeid(decltype("hello world!")).name()<<" | ";   //const char (&str)[13]

        //4.4 cv限定符的推导
        //通常情况下，decltyoe(e)所推导的类型会同步e的cv限定符
        //当e是未加括号的成员变量时，父对象表达式的cv限定符会被忽略，不能同步到推导结果
        //当e是加括号的数据成员是，父对象表达式的cv限定符会同步到推断结果
        cout<<"\nchapter4.4\n  ";
        const int i_4 = 0;
        decltype(i_4) j_4 = 1;              //const int
        const A* k_4 = new A();
        decltype(k_4->x) a_4;               //double，const属性被忽略
        decltype((k_4->x)) b_4 = 2.5;       //const double&
        cout<<i_4<<" | "<<j_4<<" | "<<a_4<<" | "<<b_4<<" | ";

        //4.5 decltype(auto)
        //C++14标准支持decltyp(auto),救赎告知编译器用decltype的格式推导auto
        #if __MORE_THAN_CPP14__
        cout<<"\nchapter4.5\n  ";
        decltype(auto) x_5 = 1;
        int i_5 = 5;
        decltype(auto) y_5 = i_5;
        decltype(foo()) z_5 = 6;
        cout<<std::is_reference_v<decltype(return_ref_1(i_5))>;
        #else
        __LOWRE_THAN_CPP14_RUN;
        #endif

        //4.6 decltype(auto)作为非类型模板形参占位符
        #if __MORE_THAN_CPP14__
        cout<<"\nchapter4.6\n  ";
        func_6<5>();
        func_6<4>();
        #else
        __LOWRE_THAN_CPP14_RUN;
        #endif
        FUNCTION_END()
    }
}

