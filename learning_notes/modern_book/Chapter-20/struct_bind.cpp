//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      struct_bind.cpp
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
#include "struct_bind.hpp"
#include "../includes.hpp"

using namespace std;

//20.4 实现一个类元组类型
#if __MORE_THAN_CPP17__
class BindBase4
{
public:
    int a = 42;
};
class BindTest4: public BindBase4
{
public:
    double b = 11.7;
};

namespace std
{
    template<>
    struct tuple_size<BindTest4> {
        static constexpr size_t value = 2;
    };

    template<>
    struct tuple_element<0, BindTest4>{
        using type = int;
    };

    template<>
    struct tuple_element<1, BindTest4>{
        using type = double;
    }; 
}

template<std::size_t Idx>
auto& get(BindTest4 &bt) = delete;

template<>
auto& get<0>(BindTest4 &bt) {
    return bt.a;
}

template<>
auto& get<1>(BindTest4 &bt) {
    return bt.b;
}
#endif

namespace STRUCT_BIND
{
    //20.1 使用结构化绑定
    template<typename T1, typename T2>
    struct BindTest1
    {
        T1 a;
        T2 str;
    };

    //20.3 结构化绑定的3种类型
    class BindBase3
    {
    public:
        int a = 42;
        double b = 11.7;
    };
    class BindTest3 : public BindBase3
    {};

    class BindBase3_1{};
    class BindTest3_1 : public BindBase3_1
    {
    public:
        int a = 21;
        double b = 1.5;
    };

    //20.5 绑定的访问权限问题
    #if __MORE_THAN_CPP20__
    class C5
    {
    friend void foo(const C5&);
    private:
        int a = 5;
        double b = 6.1;
    };

    void foo(const C5& other)
    {
        auto [x, y] = other;
        cout<<x<<" "<<y<<" | ";
    }
    #endif

    void test_run()
    {
        FUNCTION_START()

        //20.1 使用结构化绑定
        //auto是类型占位符，[x, y]是绑定标识符列表
        //结构化绑定能够直接绑定结构体
        cout<<"\nchapter20.1\n  ";
        auto val1 = std::make_tuple(11, 7);
        int x, y;
        std::tie(x, y) = val1;
        cout<<x<<" "<<y<<" | ";
        std::tie(x, std::ignore) = val1;
        cout<<x<<" | ";
        
        #if __MORE_THAN_CPP17__
            auto[a1, b1] = val1;
            cout<<a1<<" "<<b1<<" | ";

            BindTest1<int, std::string> bt1{1, "hello"};
            auto[x1, y1] = bt1;
            cout<<x1<<" "<<y1<<" | ";

            BindTest1<int, std::string> btest1[] =
            {
                {1, "hello"},
                {2, "world"}
            };
            for(const auto& [x, y] : btest1)
            {
                cout<<x<<" "<<y<<" | ";
            }
        #else
            cout<<"\n  ";
            __LOWRE_THAN_CPP17_RUN;
        #endif

        //20.2 深入理解结构化绑定
        //1.赋值结构化绑定绑定的是右边对象的匿名副本
        //  别名是单纯的别名，别名的类型和绑定目标对象的子对象类型相同
        //2.引用结构化绑定绑定的是右边对象，会进行同步修改
        cout<<"\nchapter20.2\n  ";
        #if __MORE_THAN_CPP17__
        BindTest1<int, std::string> bt2{1, "hello"};
        const auto[x2, y2] = bt2;
        cout<<&bt2.a<<" "<<&x2<<" | ";
        cout<<&bt2.str<<" "<<&y2<<" | ";

        auto& [a2, b2] = bt2;
        cout<<&bt2.a<<" "<<&a2<<" | ";
        cout<<&bt2.str<<" "<<&b2<<" | ";
        a2 = 5;
        b2 = "test";
        cout<<a2<<" "<<bt2.a<<" | ";
        cout<<b2<<" "<<bt2.str<<" | ";
        #else
            __LOWRE_THAN_CPP17_RUN;
        #endif

        //20.3 结构化绑定的3种类型
        //1.类或结构体中的非静态成员数目必须和结构化绑定列表中的标识符一致
        //2.数据成员必须是公有的(C++20之前)
        //3.数据成员必须在同一个类或者基类中
        //4.类和结构体中不能包含匿名联合体
        //绑定到原生数组
        cout<<"\nchapter20.3\n  ";
        #if __MORE_THAN_CPP17__
        int arr3[] = {1, 3, 5};
        auto[x3, y3, z3] = arr3;
        cout<<x3<<" "<<y3<<" "<<z3<<" | ";

        //绑定到结构体和类对象
        BindBase3 bb3{2, 1.5};
        BindTest3 bt3{1, 2.1};
        auto[a3, b3] = bb3;
        auto[c3, d3] = bt3;
        cout<<a3<<" "<<b3<<" | ";
        cout<<c3<<" "<<d3<<" | ";

        BindTest3_1 bt31;
        auto[vx3, vy3] = bt31;
        cout<<vx3<<" "<<vy3<<" | ";

        //绑定到元组和类元组的对象
        std::map<int, std::string> mapstr3 = {
            {1, "1"},
            {2, "2"}
        };
        for(const auto&[id, str]:mapstr3)
        {
            cout<<id<<" "<<str<<" | ";
        }
        #else
            __LOWRE_THAN_CPP17_RUN;
        #endif

        //20.4 实现一个类元组类型
         // 支持绑定到元组和类元组对象的条件
        // 1.满足std::tuple_size<T>::value是符合语法的表达式
        // 2.std::tuple_element<i, T>::type是符合语法的表达式，且其中每个i的值都小于std::tuple_size<T>::value
        // 3.类型T必须存在合法的模板函数get<i>()或者get(i)(t), 返回实例t中的每个元素
        cout<<"\nchapter20.4\n  ";
        #if __MORE_THAN_CPP17__
        BindTest4 bt4;
        auto& [x4, y4] = bt4;
        cout<<x4<<" "<<y4<<" | ";
        #else
            __LOWRE_THAN_CPP17_RUN;
        #endif

        //20.5 绑定的访问权限问题
        //C++20规定结构化绑定不在强调必须为公开数据成员 
        cout<<"\nchapter20.5\n  ";
        #if __MORE_THAN_CPP20__
        C5 c5;
        foo(c5);
        #else
            __LOWRE_THAN_CPP20_RUN;
        #endif
        FUNCTION_END()
    }
}