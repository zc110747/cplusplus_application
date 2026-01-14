//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      template_improve.cpp
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
#include "template_improve.hpp"
#include "../includes.hpp"

using namespace std;

namespace TEMPLATE_IMPROVE
{   
    //37.1 允许常量求值作为所有非类型模板的实参
    constexpr char v1 = 42;
    constexpr char foo1() {return 53;}
    template<int N>
    struct X1
    {
        int value = N;
    };

    static const char static_str[] = {"static"}; //内部链接
    extern const char extern_str[] = {"extern"}; //外部链接
    constexpr const char* return_ptr() {
        return static_str;
    }
    template<const char *str> struct TCS
    {
        const char *ptr{str};
        void print(){
            cout<<ptr<<" | ";
        }
    };

    #if __MORE_THAN_CPP20__
    static constexpr auto lambda_func = [](int a, int b)->int
    {
        cout<<a+b<<" | ";
        return a+b;
    };

    template<int (*func_N)(int, int)> 
    struct TFS
    {
        int (*func)(int, int) = func_N;
    };

    struct init_struct{
        inline static constexpr int val = 6;
    };
    #endif

    constexpr auto& vchar_r = v1;

    //37.2 允许局部和匿名类型作为模板实参
    template<class T>
    struct X2
    {
        T v;
    };

    struct
    {
        int b;
    }Obj2;
    template<class T> void f2(T t){
        cout<<typeid(T).name()<<" | ";
    }

    template<class T=double, class U>
    void func_default2(U u1)
    {
        cout<<u1<<" | ";
    }

    //37.4 函数模板添加ADL查找规则
    int h4 = 0;
    void g4()
    {
        cout<<"g"<<" | ";
    }
    namespace N
    {
        struct A{};
        template<class T> void f4(T) {
            cout<<"N::f"<<" | ";
        }
        template<class T> void g4(T) {
            cout<<"N::g"<<" | ";
        }
        template<class T> void h4(T) {
            cout<<"N::h"<<" | ";
        }
    }

    #if __MORE_THAN_CPP20__
    template <typename T, std::size_t N>
    struct basic_fixed_string
    {
        constexpr basic_fixed_string(const T(&foo)[N+1])
        {
            std::copy_n(foo, N+1, data_);
        }

        T data_[N+1];
    };
    template <typename T, std::size_t N>
    basic_fixed_string(const T(&str)[N])->basic_fixed_string<T, N-1>;

    template <basic_fixed_string Str>
    struct  Z
    {
        Z() {
            cout<<Str.data_<<" | ";
        }
    };
    #endif


    //37.7 扩展的模板参数匹配规则
    #if __MORE_THAN_CPP17__
    template <template <auto> class T, auto N>
    auto foo7() -> T<N>
    {
        T<N> n;
        return n;
    }

    template <auto> 
    class bar7
    {
    public:
        inline static int a{5}; 
    };
    #endif

    void test_run()
    {
        FUNCTION_START()

        //37.1 允许常量求值作为所有非类型模板的实参
        //1.如果整型作为模板实参，则必须是模板形参类型的经转换常量表达式
        //2.对象指针作为模板实参，需要静态或者有内部，外部链接的完整对象
        //3.函数指针作为模板形参，需要具有内部或者外部链性
        //4.左值引用作为模板形参，也必须具有内部或者外部链接性
        //5.对于成员指针作为模板实参的情况，必须是静态成员。
        cout<<"\nchapter37.1\n  ";
        X1<v1> x1;
        X1<foo1()> y1;
        cout<<x1.value<<" "<<y1.value<<" | ";
        
        TCS<static_str> tcs1;
        TCS<extern_str> tcs2;
        tcs1.print();
        tcs2.print();

        X1<vchar_r> z1;
        cout<<z1.value<<" | ";

        #if __MORE_THAN_CPP20__
        X1<init_struct::val> s1;
        cout<<s1.value<<" | ";

        TFS<lambda_func> tfs1;
        tfs1.func(1, 2);
        #else
        __LOWRE_THAN_CPP20_RUN;
        #endif

        //37.2 允许局部和匿名类型作为模板实参
        cout<<"\nchapter37.2\n  ";
        struct A2{int a;};
        X2<A2> x2 = {{3}};
        cout<<x2.v.a<<" | ";

        f2(Obj2);

        //37.3 允许函数模板的默认模板参数
        cout<<"\nchapter37.3\n  ";
        func_default2(5);

        //37.4 函数模板添加ADL查找规则
        cout<<"\nchapter37.4\n  ";
        #if __MORE_THAN_CPP20__
        f4<N::A>(N::A());
        g4<N::A>(N::A());
        g4();
        #else
        __LOWRE_THAN_CPP20_RUN;
        #endif

        //37.5 允许非类型模板形参中的字面量类型
        //C++20之前，非类型模板形参只能是以下类型
        //整型，枚举类型，指针类型，引用类型和std::nullptr_t
        //C++20之后，字面量类型也可以作为非类型模板形参
        //1.所有基类和非静态数据成员都是公开且不可变的。
        //2.所有基类和非静态数据成员的类型是标量类型、左值引用或前者的（可能是多维）数组
        cout<<"\nchapter37.5\n  ";
        #if __MORE_THAN_CPP20__
        Z<"hello world"> z2;
        #else
         __LOWRE_THAN_CPP20_RUN;
        #endif

        //37.6 扩展的模板参数匹配规则
        //C++17中非类型模板形参可以使用auto作为占位符
        //在C++17标准中放宽了对模板参数的匹配规则，它要求模板形参至少和实参列表一样特化。
        //换句话说，模板形参可以和实参列表精确匹配
        cout<<"\nchapter37.6\n  ";
        #if __MORE_THAN_CPP17__
        auto v7 = foo7<bar7, 5>();
        cout<<v7.a<<" | ";
        #else
         __LOWRE_THAN_CPP17_RUN;
        #endif
        FUNCTION_END()
    }
}
