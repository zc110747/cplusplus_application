/******************************************************************
 * 第三十七章 模板参数优化
 * 1.允许常量求值作为所有非类型模板的实参
 * 2.允许局部和匿名类型作为类型模板实参
 * 3.允许函数模板的默认模板参数
 * 4.函数模板添加到ADL查找规则
 * 5.允许非类型模板形参中的字面量类类型
 * 非类型模板形参可以是整数类型，枚举类型，指针类型，引用类型和std::nullptr_t类型
********************************************************************/

#include "temp_improve.hpp"
#include <iostream>
#include <algorithm>
#include <functional>

using namespace std;

#define FUNCTION_START()  {cout<<__func__<<":\n  ";}{
#define FUNCTION_END()    }{cout<<"\n\n";  }  


//允许常量求值作为所有非类型模板的实参
namespace TemplateParameter
{
    //整型作为模板实参，常量表达式需要支持对模板形参类型的隐式转换
    constexpr char vchar = 5;
    template <int N> struct TS {
        void print(){
            cout<<N<<" | ";
        }
    };

    //对象指针作为模板实参，需要静态或者有内部，外部链接的完整对象
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

    //函数指针作为模板形参，需要具有内部或者外部链性
    static constexpr auto lambda_func = [](int a, int b)->int{
        cout<<a+b<<" | ";
        return a+b;
    };
    template<int (*func_N)(int, int)> struct TFS{
        int (*func)(int, int) = func_N;
    };

    //左值引用作为模板形参，也必须具有内部或者外部链接性
    constexpr auto& vchar_r = vchar;

    //成员指针作为模板形参，则必须是静态变量
    struct SLINK{
        inline static constexpr int val = 6;
    };

    void template_test(void)
    {
        FUNCTION_START()

        TS<vchar> ts1;
        ts1.print();
        TS<vchar_r> ts2;
        ts2.print();

        TS<SLINK::val> ts3;
        ts3.print();

        TCS<static_str> tcs1;
        TCS<extern_str> tcs2;
        tcs1.print();
        tcs2.print();
        static constexpr char str[] = "str";
        TCS<str> tcs3;
        tcs3.print();
        TCS<return_ptr()> tcs4;
        tcs4.print();

        TFS<lambda_func> tfs1;
        tfs1.func(1, 2);
        FUNCTION_END()
    }
}

/*
3.函数模板添加到ADL查找规则(C++20 future)
4.允许非类型模板形参中的字面量类类型
*/
namespace TemplateType
{
    template<class T>
    struct X{T v;};
    
    //允许匿名类型作为模板实参
    struct{
        int b;
    }ObjB;
    template<class T> void f(T t){
        cout<<typeid(T).name()<<" | ";
    }

    //允许函数模板的默认模板参数
    template<class T=double, class U>
    void func_default(U t)
    {
        cout<<t<<" | ";
    }

    #if __cplusplus > 202002L    
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

        struct A{int a;};
        
        //允许局部类型作为模板实参
        X<A> xa1 = {{3}};           //聚合类型
        cout<<xa1.v.a<<" | ";

        f(ObjB);

        func_default(1.01);

        #if __cplusplus > 202002L    
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

    TemplateParameter::template_test();

    TemplateType::template_test();

    FUNCTION_END()
}

