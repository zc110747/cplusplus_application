/******************************************************************
 * 第三十七章 模板参数优化
********************************************************************/

#include "temp_improve.hpp"
#include <iostream>
#include <algorithm>

using namespace std;

#define FUNCTION_START()  {cout<<__func__<<":\n  ";}{
#define FUNCTION_END()    }{cout<<"\n\n";  }  

/*
1.允许常量求值作为所有非类型模板的实参
2.如果对象指针作为模板实参，必须是静态或者是有内部或
外部链接的完整对象
3.如果函数指针作为模板实参，则必须是有链接的函数指针
4.如果左值引用的形参作为模板实参，则必须有内部和外部链接
5.成员指针作为模板实参，必须是静态成员
*/
namespace Template1
{
    template<typename T>
    constexpr auto get_func(T a, T b)->decltype(a+b){
        return a+b;
    }
    constexpr int v = 42;

    template<int N>
    class X 
    {
    public:
        int L{N};
    };

    //内部链接
    static const char str1[] = {"one"};
    //外部链接
    extern const char str2[] = {"two"};

    template<const char *str>
    class X1
    {
    public:
        const char *ptr{str};
    };

    void foo()
    {
        cout<<"foo"<<" | ";
    }

    template<void(*func)()>
    class X2
    {
    public:
        void(*f)() = func;
    };

    class Y
    {
    public:
        static constexpr int v = 1;
    };
    const int &v1 = v;

    void template_test(void)
    {
        FUNCTION_START()

        X<get_func(1, 2)> x1;
        cout<<x1.L<<" | ";

        X<v> x2;
        cout<<x2.L<<" | ";

        X1<str1> x3;
        cout<<x3.ptr<<" | ";

        X1<str2> x4;
        cout<<x4.ptr<<" | ";

        X2<foo> x5;
        x5.f();

        X<Y::v> x6;
        cout<<x6.L<<" | ";

        X<v1> x7;
        cout<<x7.L<<" | ";

        FUNCTION_END()
    }
}

/*
1.允许局部和匿名类型作为模板实参
2.允许函数模板的默认模板函数
3.函数模板添加到ADL查找规则(C++20 future)
4.允许非类型模板形参中的字面量类类型
*/
namespace Template2
{
    template<class T>
    class X
    {
    public:
        T v;
    };

#if __cplusplus > 201806L
    template<class T=double, double N=1.5>
    void foo()
    {
        T t = N;
        cout<<t<<" | ";
    }
#endif

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
    
    template <template <auto> class T, auto N>
    auto foo1() -> T<N>
    {
        T<N> n;
        return n;
    }

    template <auto> 
    class bar
    {
    public:
        static inline int a{5}; //类的静态变量不允许在类内部初始化, C++17通过inline支持
    };
    

    void template_test(void)
    {
        FUNCTION_START()

        struct A{
            int a;
        };
        X<A> x1{{3}};   //聚合类型
        cout<<x1.v.a<<" | ";

        #if __cplusplus > 201806L    
        enum {e1=10};
        X x2{e1};
        cout<<x2.v<<" | ";

        foo<>();

        Z<"hello world"> z1;
        #endif

        auto l = foo1<bar, 5>();
        cout<<l.a<<" | ";

        FUNCTION_END()
    }
}

void template_improve(void)
{
    FUNCTION_START()

    Template1::template_test();

    Template2::template_test();

    FUNCTION_END()
}

