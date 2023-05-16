//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      concept.cpp
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
#include "concept.hpp"
#include "../includes.hpp"

using namespace std;

namespace CONCEPT
{
    //41.1 使用std::enable_if约束模板
    #if __MORE_THAN_CPP17__
    template<class T, class U = std::enable_if_t<std::is_integral_v<T>>>
    struct X{
        T val;
    };
    std::ostream &operator<<(std::ostream& os, X<int>& x)
    {
        os<<x.val;
        return os;
    }
    #endif

    #if __MORE_THAN_CPP20__
    //41.2 概念的背景介绍
    //concept关键字定义约束条件
    template <class T>
    concept IntegerType2 = std::is_integral_v<T>;

    template<IntegerType2 T>
    struct X2{
        T val;
    };

    //requires直接约束模板形参
    template<class T>
    requires std::is_integral_v<T>
    struct Y2{
        T val;
    };

    //41.3 使用concept和约束表达式定义概念
    //支持与操作
    template <class T>
    concept SignedIntegerType3 = std::is_integral_v<T> && std::is_signed_v<T>;

    template <class T>
    concept IntegerFloatingType = std::is_integral_v<T> || std::is_floating_point_v<T>;

    template <SignedIntegerType3 T>
    struct X3{
        T val;
    };

    template <IntegerFloatingType T>
    struct Y3{
        T val;
    };

    //41.4 requires子句和约束检查顺序
    template<class T>
    requires std::is_integral_v<T> && std::is_signed_v<T>
    struct X4 
    {
        T val;
    };
    
    template <class T>
    constexpr bool bar4() {
        return std::is_integral_v<T>;
    }
    template <class T>
    requires (bar4<T>())
    struct A4
    {
        T val;
    };

    template <class T>
    requires (bar4<T>()) || is_floating_point_v<T>
    struct S4
    {
        T val;
    };

    //41.5 原子约束
    template <class T>
    concept sad = false;
    template <class T>
    concept not_sad = !sad<T>;
    template <class T>
    int foo5(T ) requires (not_sad<T>)
    {
        cout<<"foo5"<<" | ";
        return 2;
    }
    template <class T>
    int foo5(T) requires (not_sad<T>) && true
    {
        cout<<"foo5 true"<<" | ";
        return 3;
    }

    //41.6 requires表达式
    template <class T>
    concept check6 = requires {
        T().clear();
    };
    template <check6 T>
    struct G6
    {
        T val;
    };

    template<class T>
    using Ref = T&;

    template <class T>
    concept check61 = requires(T a, T b) {
        //简单要求
        a + b;

        //类型要求
        typename Ref<T>;

        //复合要求
        {a+b} noexcept -> std::same_as<int>;

        //嵌套要求
        requires std::same_as<decltype((a+b)), int>;
    };
    template <check61 T>
    struct G61
    {
        T val;
    };

    //41.7 约束可变参数模板
    template<class T> 
    concept C7 = is_integral_v<T>;

    template<C7... T>
    struct S7
    {
        S7()
        {
            cout<<sizeof...(T)<<" | ";
        }
    };

    //41.8 约束类模板特化
    template<class T>
    struct S8
    {
        S8(){
            cout<<"1.call S8"<<" | ";
        }
        
        S8() requires requires(T t) {t.f();} {
            cout<<"2.call S8"<<" | ";
        }
    };

    struct Arg8
    {
        void f(){
        }
    };

    //41.9 约束auto
    template<class T>
    concept IntegerType9 = std::is_integral_v<T>;
    #endif

    void test_study()
    {
        FUNCTION_START()

        //41.1 使用std::enable_if约束模板
        cout<<"\nchapter41.1\n  ";
        #if __MORE_THAN_CPP17__
        X<int> x1 = {1};
        cout<<x1<<" | ";
        #else
        __LOWRE_THAN_CPP17_RUN;
        #endif

        //41.2 概念的背景介绍
        cout<<"\nchapter41.2\n  ";
        #if __MORE_THAN_CPP20__
        X2<int> x2 = {5};
        cout<<x2.val<<" | ";

        Y2<int> y2 = {6};
        cout<<y2.val<<" | ";
        #else
        __LOWRE_THAN_CPP20_RUN;
        #endif

        //41.3 使用concept和约束表达式定义概念
        cout<<"\nchapter41.3\n  ";
        #if __MORE_THAN_CPP20__
        X3<int> x3 = {-10};
        cout<<x3.val<<" | ";
        Y3<int> y3 = {2};
        Y3<float> f3 = {2.5};
        cout<<y3.val<<" "<<f3.val<<" | ";
        #else
        __LOWRE_THAN_CPP20_RUN;
        #endif

        //41.4 requires子句和约束检查顺序
        //1.是一个初等表达式或带括号的任意表达式
        //2.使用&&或||运算符链接表达式
        //约束检查顺序
        //1.模板形参表达式中的形参的约束表达式，其中检查顺序就是形参出现的顺序
        //2.模板形参列表之后的requires子句中的约束表达式
        //3.简写函数模板声明中每个拥有受约束auto占位符类型的形参所引入的约束表达式.
        //4.函数模板声明尾部requires子句中的约束表达式.
        cout<<"\nchapter41.4\n  ";
        #if __MORE_THAN_CPP20__
        A4<int> a4 = {5};
        cout<<a4.val<<" | ";
        S4<float> s4 = {2.5};
        cout<<s4.val<<" | ";
        #else
        __LOWRE_THAN_CPP20_RUN;
        #endif

        //41.5 原子约束
        //原子约束是表达式和表达式中模板形参到模板实参映射的组合
        cout<<"\nchapter41.5\n  ";
        #if __MORE_THAN_CPP20__
        foo5(2);
        #else
        __LOWRE_THAN_CPP20_RUN;
        #endif

        //41.6 requires表达式
        cout<<"\nchapter41.6\n  ";
        #if __MORE_THAN_CPP20__
        G6<std::vector<char>> x6;
        cout<<x6.val.size()<<" | ";

        G61<int> g6 = {6};
        cout<<g6.val<<" | ";
        #else
        __LOWRE_THAN_CPP20_RUN;
        #endif

        //41.7 约束可变参数模板
        cout<<"\nchapter41.7\n  "; 
        #if __MORE_THAN_CPP20__
        S7<int, int, int> s7;
        #else
        __LOWRE_THAN_CPP20_RUN;
        #endif

        //41.8 约束类模板特化
        cout<<"\nchapter41.8\n  "; 
        #if __MORE_THAN_CPP20__
        S8<int> s8;
        S8<Arg8> s81;
        #else
        __LOWRE_THAN_CPP20_RUN;
        #endif

        //41.9 约束auto
        cout<<"\nchapter41.9\n  "; 
        #if __MORE_THAN_CPP20__
        IntegerType9 auto i9 = 11;
        auto bar9 = []()->IntegerType9 auto {
            return 10;
        };
        cout<<i9<<" "<<bar9()<<" | ";
        #else
        __LOWRE_THAN_CPP20_RUN;
        #endif
        FUNCTION_END()
    }
}

