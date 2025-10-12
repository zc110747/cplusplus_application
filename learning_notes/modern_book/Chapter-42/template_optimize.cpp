//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      template_optimize.cpp
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
#define _EXTERN
#include "template_optimize.hpp"
#include "../includes.hpp"

using namespace std;

static int const c = 4;

namespace TEMPLATE_OPTIMIZE
{
    //42.2 连续右尖括号的解析优化
    template<int I>
    struct X
    {
        static int const c = 2;
    };
    
    template<> 
    struct X<0>
    {
        typedef int c;
    };

    template<typename T> 
    struct Y
    {
        static int const c = 3;
    };

    //42.3 friend声明模板形参
    template<typename T>
    class C
    {
        friend T;
    private:
        int a = 5;
    };

    class X3
    {
    public:
        static void show()
        {
            C<X3> c1;
            cout<<c1.a<<" | ";
        }
    };

    class Y3 
    {
    public:    
        static void show()
        {
            C<Y3> c1;
            cout<<c1.a<<" | ";
        }
    };

    //42.4 变量模板
    template<class T>
    struct PI
    {
        static constexpr T value = static_cast<T>(3.14155926535);
    };
    #if __MORE_THAN_CPP14__
    template<class T>
    constexpr T PI4 = static_cast<T>(3.1415926535L);
    #endif

    //42.5 explicit(bool)
    #if __MORE_THAN_CPP20__
    template <typename T1, typename T2>
    struct pair5
    {
        template<typename U1=T1, typename U2=T2,
        std::enable_if_t<
            std::is_constructible_v<T1, U1> &&
            std::is_constructible_v<T2, U2>, 
            int> = 0 >
        explicit(!std::is_convertible_v<U1, T1> ||
        !std::is_convertible_v<U2, T2>)
        constexpr pair5(U1&& u1, U2&& u2): first_(u1), second_(u2)
        {}

        T1 first_;
        T2 second_;
    };

    template <typename T1, typename T2>
    struct pair51
    {
        template<typename U1=T1, typename U2=T2>
            requires std::is_constructible_v<T1, U1> &&
            std::is_constructible_v<T2, U2>
            explicit(!std::is_convertible_v<U1, T1> ||
        !std::is_convertible_v<U2, T2>)
        constexpr pair51(U1&& u1, U2&& u2): first_(u1), second_(u2)
        {}

        T1 first_;
        T2 second_;
    };
    #endif
    void test_run()
    {
        FUNCTION_START()

        //42.1 外部模板
        //使用extern声明外部模板帮助构建
        cout<<"\nchapter42.1\n  "; 
        bar<int> b1;
        cout<<b1.foo(5)<<" | ";

        //42.2 连续右尖括号的解析优化
        //右尖括号号会优先和左尖括号匹配
        cout<<"\nchapter42.2\n  "; 
        using veflags = std::vector<std::vector<bool>>;
        cout<<(Y<X<1> >::c > ::c > ::c)<<" | ";
        cout<<(Y<X<1>>::c > ::c > ::c)<<" | ";

        //42.3 friend声明模板形参
        cout<<"\nchapter42.3\n  "; 
        X3::show();
        Y3::show();

        //42.4 变量模板
        cout<<"\nchapter42.4\n  "; 
        cout<<PI<float>::value<<" | ";
        cout<<PI<int>::value<<" | ";
        #if __MORE_THAN_CPP14__
        cout<< PI4<float> <<" | ";
        cout<< PI4<int> <<" | ";
        #else
        __LOWRE_THAN_CPP14_RUN;
        #endif

        //42.5 explicit(bool)
        cout<<"\nchapter42.5\n  "; 
        #if __MORE_THAN_CPP20__
        pair5<std::string, std::string> val1{"hello", "world"};
        cout<<val1.first_<<" "<<val1.second_<<" | ";
        pair51<std::string, std::string> val2{"hello", "world"};
        cout<<val2.first_<<" "<<val2.second_<<" | ";
        #else
        __LOWRE_THAN_CPP20_RUN;
        #endif

        FUNCTION_END()
    }
}
