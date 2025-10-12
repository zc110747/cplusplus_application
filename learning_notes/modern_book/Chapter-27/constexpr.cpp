//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      constexpr.cpp
//
//  Purpose:
//      第27章 常量表达式
//
//  Author:
//     	@听心跳的声音
//
//  Assumptions:
//
//  Revision History:
//      4/19/2023   Create New Version
/////////////////////////////////////////////////////////////////////////////

#include "constexpr.hpp"
#include "../includes.hpp"

using namespace std;

namespace CONSTEXPR
{
    //27.3 constexpr函数
    constexpr int multi(int x)  {return x*x;}
    constexpr int abs(int x)    {return x>0?x:-x;} 
    constexpr int sum(int x)    {return x>0?x+sum(x-1):0;};
    constexpr int factorial(int x)    {return x>0?x*factorial(x-1):1;};

    //27.4 constexpr构造函数
    class x_const
    {
    public:
        constexpr x_const():x_(5){
        }
        constexpr x_const(int i):x_(i){
        }
        constexpr int get_x() const {
            return x_;
        }

    private:
        int x_;
    };

    //27.5 对浮点的支持
    constexpr double sum(double x)
    {
        return x>0?x+sum(x-1):0;
    }

    //27.6 C++14标准对常量表达式函数的增强
    #if __MORE_THAN_CPP14__
    constexpr int abs_update(int x){
        if(x > 0) {
            return x;
        } else {
            return -x;
        }
    }

    constexpr int sum_update(int x){
        int result = 0;
        while(x>0)
        {
            result += x;
            x--;
        }
        return result;
    }

    class y_const
    {
    public:
        constexpr y_const():y(1){
        }
        constexpr y_const(int i):y(i){
        }
        constexpr int get() const{
            return y;
        }
        constexpr void set(int i){
            y = i;
        }

    private:
        int y;
    };

    constexpr y_const make_y_const(int x)
    {
        y_const y;
        y.set(x);
        return y;
    }
    #endif

    //27.8 constexpr的内联属性
    #if __MORE_THAN_CPP17__
    class z_const
    {
    public:
        static constexpr int a{5};
        inline static constexpr int b{6};
    };
    #endif

    //27.9 if constexpr
    #if __MORE_THAN_CPP17__
    template<class T> bool is_same_value(T a, T b)
    {
        if constexpr (std::is_same_v<T, double>){
            if(std::abs(a-b)<0.001) {
                return true;
            } else {
                return false;
            }
        } else {
            return a == b;
        }
    }

    template<class T> auto any2i(T t)
    {
        if constexpr (std::is_same_v<T, std::string>) {
            if constexpr(T::npos == -1) {
                return atoi(t.c_str());    
            }

        } else {
            return t;
        }
    }
    #endif  

    //27.10 允许constexpr虚函数
    #if __MORE_THAN_CPP20__
    struct x_virtual
    {
        constexpr virtual int f() const {return 1;}
    };

    //27.12 允许constexpr中进行平凡的默认初始化
    struct x_trivial
    {
        int x = 1;
    };

    //平凡的默认构造，在c++20后支持
    //可通过x_trivial中提供默认值，C++17即可正常工作
    constexpr int x_trivial_func()
    {
        x_trivial x;
        return x.x;
    }

    //27.13 允许在constexpr中更改联合类型的有效成员
    union Foo{
        int i;
        float f;
    };
    constexpr int union_func()
    {
        Foo foo{};

        foo.i = 3;
        foo.f = 12.5;
        return 5;
    }

    //27.14 允许在constexpr中更改联合类型的有效成员
    consteval int sqr_evel(int n){
        return n*n;
    }


    //27.16 判断常量求值环境
    constexpr int func_const(int x)
    {
        if constexpr(std::is_constant_evaluated()) {
            return x;
        } else {
            return 0;
        }
    };
    #endif

    void test_run()
    {
        FUNCTION_START()

        //27.1 常量的不确定性
        //const部分可以代替常量
        cout<<"chapter27.1\n  ";
        const int index0 = 0;
        const int index1 = 1;

        std::tuple<int, char> tp = std::make_tuple(4, '3');
        auto x1 = std::get<index0>(tp);
        auto x2 = std::get<index1>(tp); 
        cout<<x1<<" "<<x2<<" | ";

        //27.2 constexpr值
        //constexpr可以声明基础类型从而直接获得常量表达式
        cout<<"\nchapter27.2\n  ";
        constexpr int size_1 = 2;
        int buffer[size_1] = {0};
        cout<<buffer[0]<<" "<<buffer[1]<<" | ";

        //27.3 constexpr函数
        //1.函数必须返回一个值，所以它的返回值类型不能是void(C++14后放宽)
        //2.函数必须只有一条语句: return expr
        //3.函数使用之前必须有定义(非声明)
        //4.函数必须用constexpr声明
        //当函数输入不是常量时，constexpr会自动退回到普通函数
        cout<<"\nchapter27.3\n  ";
        auto value = multi(5);
        cout<<value<<" | ";
        value = abs(-1);
        cout<<value<<" | ";
        value = sum(10);
        cout<<value<<" | ";
        value = factorial(5);
        cout<<value<<" | ";
        value = multi(value);   //退回到普通函数
        cout<<value<<" | ";

        //27.4 constexpr构造函数
        //1.构造函数必须使用constexpr声明
        //2.构造函数初始化列表中必须是常量表达式
        //3.构造函数的函数体必须为空
        //4.析构函数必须是平凡的
        //自定义类型不能有用户自定义的析构函数，
        //析构函数不能是虚函数
        //基类和成员的析构函数必须是平凡的
        cout<<"\nchapter27.4\n  ";
        constexpr x_const x_0;
        constexpr auto x_1 = x_0.get_x(); 
        constexpr x_const x_2(x_0.get_x());
        cout<<x_1<<" | "<<x_2.get_x()<<" | ";

        int i = 0;
        x_const x_3(i);
        cout<<x_3.get_x()<<" | ";

        //27.5 对浮点的支持
        cout<<"\nchapter27.5\n  ";
        auto f_val = sum(1.7);
        cout<<f_val<<" | ";

        cout<<__cplusplus<<"\n";

        //27.6 C++14标准对常量表达式函数的增强
        //1.函数体允许声明变量，需要初始化，不能是static和thread_local变量
        //2.允许出现if和switch语句，不能出现goto语句
        //3.函数允许实现循环语句，for, while, do-while
        //4.函数可以修改声明周期和常量表达式相同的对象
        //5.函数返回值可以声明为void
        //6.constexpr声明的成员函数不在具有const属性
        cout<<"\nchapter27.6\n  ";
    #if __MORE_THAN_CPP14__
        constexpr int y_0 = abs_update(-2);
        constexpr int sum_0 = sum_update(y_0);
        cout<<y_0<<" | "<<sum_0<<" | ";

        constexpr auto y_1 = make_y_const(1);
        constexpr auto y_v = y_1.get();
        cout<<y_v<<" | ";
    #else
        __LOWRE_THAN_CPP14_RUN;
    #endif

        //27.7 constexpr lambda表达式
        //C++17下，lambda表达式在条件允许的情况下会隐式声明为constexpr
        //也可已通过后缀强制声明lambda为constexpr类型
        cout<<"\nchapter27.7\n  ";
    #if __MORE_THAN_CPP17__
        auto func = []() constexpr -> int {
            int x = 5;
            return x;
        };
        cout<<func()<<" | ";
    #else
        __LOWRE_THAN_CPP17_RUN;
    #endif

        //27.8 constexpr的内联属性
        //c++17后，声明constexpr类型的静态成员变量默认带inline属性
        cout<<"\nchapter27.8\n  ";
    #if __MORE_THAN_CPP17__
        cout<<z_const::a<<" | "<<z_const::b<<" | ";
        cout<<&z_const::a<<" | "<<&z_const::b<<" | ";
    #else
        __LOWRE_THAN_CPP17_RUN;
    #endif

        //27.9 if constexpr
        //1.if constexpr的条件必须编译期能确定结果的常量表达式
        //2.条件结果一确认，编译器只编译符合条件的代码块
        //3.if constexpr不支持短路原则，都会进行判断
    #if __MORE_THAN_CPP17__
        cout<<"\nchapter27.9\n  ";
        auto check = [](){
            if constexpr (sizeof(int) > sizeof(char)){
                cout<<"sizeof(int) > sizeof(char)"<<" | ";
            } else {
                cout<<"sizeof(int) <= sizeof(char)"<<" | ";
            }
        };
        check();
        cout<<std::boolalpha<<is_same_value(2, 2)<<" | "<<is_same_value(2.1, 2.100001)<<" | ";
        cout<<any2i(5)<<" | "<<any2i("15")<<" | ";
    #else
        __LOWRE_THAN_CPP17_RUN;
    #endif

        //27.10 允许constexpr虚函数
        cout<<"\nchapter27.10\n  ";
    #if __MORE_THAN_CPP20__
        constexpr x_virtual x_value;
        constexpr auto x_v1 = x_value.f();
        cout<<x_v1<<" | ";
    #else
        __LOWRE_THAN_CPP20_RUN;
    #endif     

        //27.11 允许constexpr函数中出现try catch
        //throw语句被禁止
        cout<<"\nchapter27.11\n  ";
    #if __MORE_THAN_CPP20__
        auto func_try = [](int x) constexpr ->int{
            try{
                return x+1;
            }catch(...){
                return 0;
            }
        };
        constexpr auto val_11 = func_try(5);
        cout<<val_11;
    #else
        __LOWRE_THAN_CPP20_RUN;
    #endif     

        //27.12 允许constexpr中进行平凡的默认初始化
        cout<<"\nchapter27.12\n  ";
    #if __MORE_THAN_CPP20__
        constexpr auto val_12 = x_trivial_func();
        cout<<val_12<<" | ";
    #else
        __LOWRE_THAN_CPP20_RUN;
    #endif         

        //27.13 允许在constexpr中更改联合类型的有效成员
        cout<<"\nchapter27.13\n  ";
    #if __MORE_THAN_CPP20__
        cout<<union_func()<<" | ";
    #else
        __LOWRE_THAN_CPP20_RUN;
    #endif    

        //27.14 使用consteval声明立即函数
        //指定函数必须在编译期执行，不会退化到普通函数
        cout<<"\nchapter27.14\n  ";
    #if __MORE_THAN_CPP20__
        constexpr int r = sqr_evel(10);
        cout<<r<<" | ";
        auto sqrsqr = [](int n) consteval { return n*n; };
        constexpr auto val = sqrsqr(50);
        cout<<val<<" | ";
    #else
        __LOWRE_THAN_CPP20_RUN;
    #endif 

        //27.15 使用constinit检查常量初始化
        //主要用于具有静态存储持续时间的变量声明上，它要求变量具有常量初始化程序
        //声明为常量初始化的程序不要求具有常量属性
        cout<<"\nchapter27.15\n  ";
    #if __MORE_THAN_CPP20__
        constinit static int x_init = 11;
        cout<<"x_init"<<" | ";
    #else
        __LOWRE_THAN_CPP20_RUN;
    #endif 

        //27.16 判断常量求值环境
        //使用is_constant_evaluated判断当前是否是const环境
        cout<<"\nchapter27.16\n  ";
    #if __MORE_THAN_CPP20__
        int x_val16 = 5;
        cout<<std::is_constant_evaluated()<<" | ";
        constexpr int a_16 = func_const(5);
        auto b_16 = func_const(x_val16);
        cout<<a_16<<" | "<<b_16<<" | ";
    #else
        __LOWRE_THAN_CPP20_RUN;
    #endif
    
        FUNCTION_END()
    }
}