/******************************************************************
 * 第三十五章 可变参数模板
 * 第三十六章 typename优化 
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
    template<typename ...Args>
    void foo(Args ...args)
    {
        cout<<sizeof...(args)<<" | ";
    }

    template<typename ...Args, typename T, typename U = double>
    void foo_1(T t1, U u1, Args ...args)
    {
        cout<<t1<<" "<<u1<<" ";
        auto tp = make_tuple(args...);
        auto[x, y] = tp;
        cout<<x<<" "<<y<<" | ";
    }

    template<typename T, typename ...Args> //类模板，模板形参列表必须为最后一个形参
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
    template<typename T, class U>
    auto baz(T t, U u) -> T
    {
        cout<<t<<" "<<u<<" | ";
        return t;
    }

    template<typename ...Args>
    void foo(Args ...args){}

    template<typename ...Args>
    class bar{
    public:
        bar(Args ...args)
        {
            foo(baz(&args, args)...);
        }
    };

    template<typename ...Args>
    void foo_f(Args ...args)
    {
        auto tp = make_tuple(args...);
        cout<<get<0>(tp)(4, 3)<<" ";
        cout<<get<1>(tp)(4, 3)<<" | ";
    }

    template<typename ...Args>
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
    template<typename T>
    T sum(T arg)
    {
        return arg;
    }

    template<typename T1, typename ...Args>
    auto sum(T1 arg1, Args ...args)
    {
        return arg1 + sum(args...);
    }

    //折叠方式
    //args op ... 一元向右折叠
    //... op args 一元向左折叠
    //args op ... int 二元向右折叠
    template<typename ...Args>
    auto sum_m(Args ...args)
    {
        return (args + ...);
    }

    template<typename ...Args>
    auto sum_int(Args ...args)
    {
        return (args + ... + 0);
    }

    template<typename ...Args>
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
    template<typename T>
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

    template<typename ...Args>
    class derived: public base<Args>...
    {
    public:
        using base<Args>::base...;
    };

    template<typename F, typename ...Args>
    auto delay_invoke(F f, Args... args)
    {
        return [f = std::move(f), tup = std::make_tuple(std::move(args)...)]() -> decltype(auto){
            return std::apply(f, tup);
        };
    }

    template<typename T>
    struct A
    {
        using type = std::vector<T>;
    };

    //template <typename> 告知T是一个模板
    template <template <typename> typename T>
    struct B
    {
        T<int> a;
    };

    using AType = typename A<int>::type;

    template<typename T>
    class A1
    {
    public:    
        void f()
        {   
            cout << "A::f()" << " | ";    
        }
    };

    template<typename T>
    class B1: public A1<T>
    {
    public:  
        void g()
        { 
            /*不加this会报错
            1.模板会二段式查找, B基类A<T>依赖模板参数T, 在定义阶段, A<T>是不存在的，于是A<T>::f也不存在，这时会认为f()是非成员函数，
            2.当B模板实例化阶段, 可以查到A<T>::f, 但编译器仍然认为f()是非成员函数，不会回头去找
            3.加this会告诉编译器是成员函数，实例化阶段，会先在B内查找，在去基类A1查找
            */
            this->f();
            A1<T>::f();  //访问基类的函数
        } 
    };

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

        {
            A<int>::type a;
            B<A> b;
            cout<<sizeof(b)<<" | ";
        }

        {
            B1<int> b;
            b.g();
        }
  
        AType a1;
        cout<<typeid(a1).name()<<" | ";

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