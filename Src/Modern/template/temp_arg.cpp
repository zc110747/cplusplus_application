/******************************************************************
 * 第三十五章 可变参数模板
********************************************************************/
#include "temp_arg.hpp"
#include <iostream>
#include <functional>
#include <tuple>
#include <vector>
#include <string>

using namespace std;

#define FUNCTION_START()  {cout<<__func__<<":\n  ";}{
#define FUNCTION_END()    }{cout<<"\n\n";  }  

namespace Template1
{
    template<class ...Args>
    void foo(Args ...args)
    {
        cout<<sizeof...(args)<<" | ";
    }

    template<class ...Args, class T, class U = double>
    void foo_1(T t1, U u1, Args ...args)
    {
        cout<<t1<<" "<<u1<<" ";
        auto tp = make_tuple(args...);
        auto[x, y] = tp;
        cout<<x<<" "<<y<<" | ";
    }

    template<class T, class ...Args> //类模板，模板形参列表必须为最后一个形参
    class bar
    {
    public:
        bar(T n, Args ...args){
            cout<<sizeof...(args)<<" | ";
            foo(n, args...);
        }
    };

    template<int ...args>
    void foo_2(void)
    {
        auto tp = make_tuple(args...);
        auto[x, y] = tp;
        cout<<x<<" "<<y<<" | ";
    }

    static void arg_template(void)
    {
        FUNCTION_START()

        foo(1, 2, 3);
        bar b1{2, 2.5, "hello"};

        foo_1(1, 2, 3.5, "hello");
        foo_2<4, 3>();

        FUNCTION_END()
    }
}

namespace Template2
{
    template<class T, class U>
    auto baz(T t, U u) -> T
    {
        cout<<t<<" "<<u<<" | ";
        return t;
    }

    template<class ...Args>
    void foo(Args ...args){}

    template<class ...Args>
    class bar{
    public:
        bar(Args ...args)
        {
            foo(baz(&args, args)...);
        }
    };

    template<class ...Args>
    void foo_f(Args ...args)
    {
        auto tp = make_tuple(args...);
        cout<<get<0>(tp)(4, 3)<<" ";
        cout<<get<1>(tp)(4, 3)<<" | ";
    }

    template<class ...Args>
    class bar_1{
    public:
        bar_1(Args ...args){
            auto lm = [args...]{ 
                cout<<sizeof...(args)<<" | ";
                foo(&args...); 
            };
            lm();
        }
    };

    static void arg_template(void)
    {
        FUNCTION_START()

        bar b{1, 5.0, 9};

        foo_f(
            [](int a, int b){return a+b;},
            [](int a, int b){return a-b;});

        bar_1 b1{1, 2.5, 3};
        FUNCTION_END()
    }
}

namespace Template3
{
    template<class T>
    T sum(T arg)
    {
        return arg;
    }

    template<class T1, class ...Args>
    auto sum(T1 arg1, Args ...args)
    {
        return arg1 + sum(args...);
    }

    //折叠方式
    //args op ... 一元向右折叠
    //... op args 一元向左折叠
    //args op ... int 二元向右折叠
    template<class ...Args>
    auto sum_m(Args ...args)
    {
        return (args + ...);
    }

    template<class ...Args>
    auto sum_int(Args ...args)
    {
        return (args + ... + 0);
    }

    template<class ...Args>
    auto sum_arg(Args ...args)
    {
        return (args && ...);
    }

    //一元折叠表达式支持参数为空
    //1. &&, ||能在一元折叠表达式中空用
    //1. &&返回true, ||返回false
    static void arg_template(void)
    {
        FUNCTION_START()

        cout<<sum(1, 5.0, 11.7)<<" | ";
        
        cout<<sum_m(1, 2.5, 3.5)<<" | ";

        cout<<sum_m("a", "b", std::string("c"))<<" | ";

        cout<<sum_int()<<" | ";

        cout<<std::boolalpha<<sum_arg()<<" | ";

        FUNCTION_END()
    }
}

namespace Template4
{
    template<class T>
    class base
    {
    public:
        base(){}
        base(T t):t_(t){}
        void print()
        {
            cout<<t_<<" | ";
        }
    private:
        T t_;
    };

    template<class ...Args>
    class derived: public base<Args>...
    {
    public:
        using base<Args>::base...;
    };

    template<class F, class ...Args>
    auto delay_invoke(F f, Args... args)
    {
        return [f = std::move(f), tup = std::make_tuple(std::move(args)...)]() -> decltype(auto){
            return std::apply(f, tup);
        };
    }

    static void arg_template(void)
    {
        FUNCTION_START()

        derived<int, std::string, bool> d1 = 1;
        derived<int, std::string, bool> d2 = std::string("hello");
        derived<int, std::string, bool> d3 = true;

        //多重继承解决二义性
        d1.base<int>::print();
        d2.base<std::string>::print();
        d3.base<bool>::print();

        cout<<delay_invoke([](auto a, auto b)->decltype(a+b){
            return a+b;}, 1, 2)()<<" | ";
        FUNCTION_END()
    }
}

static void basic_template(void)
{
    FUNCTION_START()

    auto lambdaF = std::bind([](int a, int b){
        cout<<a+b<<" | ";
    }, std::placeholders::_1, std::placeholders::_2);
    lambdaF(1, 2);

    std::tuple tp{1, 2.5, "hello"};
    cout<<std::get<0>(tp)<<" | ";
    auto[x, y, z] = tp;
    cout<<x<<" "<<y<<" "<<z<<" | ";

    FUNCTION_END()
}

void temp_arg_process(void)
{
    basic_template();

    Template1::arg_template();

    Template2::arg_template();

    Template3::arg_template();

    Template4::arg_template();
}