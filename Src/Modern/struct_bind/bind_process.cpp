
#include "bind_process.hpp"
#include <tuple>
#include <iostream>
#include <string>
#include <vector>

using namespace std;


class BindBase{
public:
    int a = 1;
};

class BindTest: public BindBase{
public:
    double b = 1.5;
    template<std::size_t Idex> auto& get() = delete;
};

template<> auto& BindTest::get<0>() {return a;}
template<> auto& BindTest::get<1>() {return b;}

namespace std{
    template<> 
    struct tuple_size<BindTest> {
        static constexpr size_t value = 2;
    };

    template<> 
    struct tuple_element<0, BindTest> {
        using type = int;
    };

    template<> 
    struct tuple_element<1, BindTest> {
        using type = double;
    };
}

class BindTest1{
private:
    int a = 1;
    int b = 2;
    double c = 1.2;
public:
    auto& get_a() {return a;}
    auto& get_b() {return b;}
    auto& get_c() {return c;}

    template<std::size_t Idex> auto& get() = delete;
};

template<> auto& BindTest1::get<0>() {return get_a();}
template<> auto& BindTest1::get<1>() {return get_b();}
template<> auto& BindTest1::get<2>() {return get_c();}

namespace std{
    template<> 
    struct tuple_size<BindTest1> {
        static constexpr size_t value = 3;
    };

    template<> 
    struct tuple_element<0, BindTest1> {
        using type = int;
    };

    template<> 
    struct tuple_element<1, BindTest1> {
        using type = int;
    };

    template<> 
    struct tuple_element<2, BindTest1> {
        using type = double;
    };
}

#define FUNCTION_START()  {cout<<__func__<<":\n  ";}{
#define FUNCTION_END()    }{cout<<"\n\n";  }   

void struct_bind_process(void)
{
    FUNCTION_START()

    {
        int x;
        double y;

        auto func = []()->std::tuple<int, double>{
            return make_tuple(1, 2.9);
        };
        std::tie(x, y) = func();
        cout<<x<<", "<<y<<" | ";
    }

    //结构化绑定
    {
        auto func = []()->auto{
            return make_tuple(1, 2, "hello");
        };
        auto[a, b, c] = func();
        cout<<a<<", "<<b<<", "<<c<<" | ";
    }

    {
        struct BindTest{
            int a;
            string str;
        };
        std::vector<BindTest> bt{
            {11, "hello"}, {7, "C++"}, {42, "world"}
        };
        for(const auto&[x, y]: bt){
            cout<<x<<", "<<y<<" | ";
        }
    }

    //数组的结构化绑定
    {
        int a[3] = {1, 2, 3};
        auto[x, y, z] = a;
        cout<<x<<", "<<y<<", "<<z<<" | "; 
    }

    //类和结构体的结构化绑定
    //1.结构化绑定的所有数据必须再同一个类或者基类中
    //2.内部不能存在匿名联合体
    {
        class A1{
        public:
            int a = 1;
            double b = 1.1;
        };
        class B1: public A1{
        };
        class C1: public A1{
        public:
            int c = 2;
            int d = 3;
        };

        B1 b{2, 3.5};
        auto[x, y] = b;
        cout<<x<<", "<<y<<" | ";

        //基类和派生类都有变量，不支持结构化绑定
        // C1 c{1, 1.5, 3, 4};
        // auto[x1, y1] = c;
    }

    {
        class A{
        };
        class B: public A{
        public:
            int a = 1;
            double b = 1.5;
        };

        B b;
        auto[x, y] = b;
        cout<<x<<", "<<y<<" | ";
    }

    /*
    支持绑定到元组和类元组对象的条件
    1.满足std::tuple_size<T>::value是符合语法的表达式
    2.std::tuple_element<i, T>::type是符合语法的表达式，其中i的值小于std::tuple_size<T>::value
    3.类型T必须存在合法的模板函数
    */
    {
        BindTest bt;
        auto& [x, y] = bt;
        x = 3;
        cout<<bt.a<<", "<<bt.b<<" | ";
    }

    {
        BindTest1 bt1;
        cout<<bt1.get_a()<<", "<<bt1.get_b()<<", "<<bt1.get_c()<<" | ";

        auto[x, y, z] = bt1;
        cout<<x<<", "<<y<<", "<<z<<" | ";
    }

    FUNCTION_END()
}
