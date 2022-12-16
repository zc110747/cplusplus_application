
/******************************************************************
 * 第二十章 结构化绑定
********************************************************************/
#include "bind_process.hpp"
#include <tuple>
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

#define FUNCTION_START()  {cout<<__func__<<":\n  ";}{
#define FUNCTION_END()    }{cout<<"\n\n";  }   

namespace BIND_FOR_STD
{
    struct BIND_BASE
    {
        int a; 
        float b;
    };

    void bind_process(void)
    {
        FUNCTION_START() 

        //基于tie来解析tuple
        int x;
        double y;
        std::tie(x, y) = make_tuple(1, 1.5);
        cout<<x<<" "<<y<<" | ";

        //结构化绑定解析tuple数据
        auto tp1 = make_tuple(1, 1.5, "hello");
        auto[x_tp1, y_tp1, z_tp1] = tp1;
        cout<<x_tp1<<" "<<y_tp1<<" "<<z_tp1<<" | ";

        //结构化绑定解析map表
        std::map<int, string> mp1 = {{1, "one"}, {2, "two"}};
        for(const auto[key, value]: mp1)
        {
            cout<<key<<" "<<value<<" | ";
        }

        //结构化绑定解析结构体
        BIND_BASE bindBase1 = {1, 2.5};
        auto[x_b1, y_b1] = bindBase1;
        cout<<x_b1<<" "<<y_b1<<" | ";

        //结构体和容器组合使用结构化绑定解析
        std::vector<BIND_BASE> vb1 = {{1, 1.1}, {2, 2.1}};
        for(const auto[key, value] : vb1)
        {
            cout<<key<<" "<<value<<" | ";
        }

        //结构化绑定解析数组，必须是原生数组，退化成指针则无效
        int array[] = {1, 2, 3};
        auto&[x_a, y_a, z_a] = array; 
        cout<<x_a<<" "<<y_a<<" "<<z_a<<" | ";
        x_a++;
        y_a++;
        z_a++;
        cout<<array[0]<<" "<<array[1]<<" "<<array[2]<<" | "; 

        //类和结构体支持结构化绑定的条件
        //1.类或结构体中的非静态成员数目必须和结构化绑定列表中的标识符一致
        //2.数据成员必须是公有的(C++20之前)
        //3.数据成员必须在同一个类或者基类中
        //4.类和结构体中不能包含匿名联合体
        {
            class A{
            public:
                int a;
                float b;
            };
            class B:public A{
            };
            class C:public A{
            public:
                int c;
            };

            A a1 = {1, 2};
            B b1 = {2, 4};
            C c1 = {4, 8, 16};

            auto[x_a1, y_a1] = a1;
            cout<<x_a1<<" "<<y_a1<<" | ";
            auto[x_b1, y_b1] = b1;
            cout<<x_b1<<" "<<y_b1<<" | ";
            //auto[x_c1, y_c1, z_c1] = c1;  //not allow, no static member must in one class 
        }   
        FUNCTION_END()
    }
}

namespace BIND_FOR_USER
{
    //设计支持基类和派生类都有非静态变量的对象
    class BIND_BASE
    {
    public:
        int value{0};
    };

    class BIND_DERIVE: public BIND_BASE
    {
    public:
        double value_d{1.5};
        template<std::size_t Idex> auto& get() = delete;
    };

    template<> auto& BIND_DERIVE::get<0>() {return value;}
    template<> auto& BIND_DERIVE::get<1>() {return value_d;}
}

namespace std{
    template<> 
    struct tuple_size<BIND_FOR_USER::BIND_DERIVE> {
        static constexpr size_t value = 2;
    };

    template<> 
    struct tuple_element<0, BIND_FOR_USER::BIND_DERIVE> {
        using type = int;
    };

    template<> 
    struct tuple_element<1, BIND_FOR_USER::BIND_DERIVE> {
        using type = double;
    };
}


void bind_process(void)
{
    FUNCTION_START() 

    // 支持绑定到元组和类元组对象的条件
    // 1.满足std::tuple_size<T>::value是符合语法的表达式
    // 2.std::tuple_element<i, T>::type是符合语法的表达式，且其中每个i的值都小于std::tuple_size<T>::value
    // 3.类型T必须存在合法的模板函数get<i>()或者get(i)(t), 返回实例t中的每个元素
    BIND_FOR_USER::BIND_DERIVE bd1;
    auto& [x, y] = bd1;
    x = 3;
    cout<<bd1.value<<" "<<bd1.value_d<<" | ";

    FUNCTION_END() 
}

void struct_bind_process(void)
{
    BIND_FOR_STD::bind_process();

    bind_process();
}
