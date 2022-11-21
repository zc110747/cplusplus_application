/**********************************************************
 * 说明: struct_layout.cpp
 *** 聚合类型 ***
 * 1.没有用户声明或者继承的构造函数(包含default)
 * 2.没有用户显示提供的继承的或显示的构造函数
 * 3.没有virtual成员函数
 * 4.没有private, protected的成员变量
 * 5.没有private, protected或virtual继承的基类
 * 聚合类型支持默认的列表初始化
 *** POD类型(Plain Old Data) ***
 * 兼容C语言的类型，C++可以直接调用POD类型的数据, 拥有POD特征的类
 * 直接字节拷贝或二进制拷贝
 * 1.满足平凡类型(trivial)
 * 2.是标准类型布局(standard-layout)
 * 3.所有非静态成员都是POD类型
 *** trivial类型 ***
 * 1.满足TrivalCopyable类型
 * 2.具有默认构造函数(不能用户自定义构造函数)
 *** TrivalCopyable类型 ***
 TrivalCopyable类型支持memory和流当作二进制处理(memcpy, memcmp, memmove)
* 1.不能包含virtual，包含虚基类和虚函数
* 2.构造函数需要符合平凡类型
 *** is_standard_layout类型 ***
* 1.所有非静态类型具有相同的访问控制
* 2.没有虚函数或虚基类
* 3.没有引用类型的非静态成员
* 4.派生类没有非静态成员 或 具有非静态成员的基类不超过一个，没有含非静态数据成员的基类
************************************************************/

#include <iostream>
#include <type_traits>

using namespace std;

#define FUNCTION_START()  {cout<<__func__<<":\n  ";}{
#define FUNCTION_END()    }{cout<<"\n\n";  }   

namespace AGGREGATE_TYPE
{
    struct A
    {
        int a;
        int b;
        void print()
        {
            cout<<a<<" "<<b<<" | ";
        }
    };  

    void layout_process(void)
    {
        FUNCTION_START()

        A a1 = {1, 2};
        a1.print();

        A a2{3, 4};
        a2.print();

        FUNCTION_END()
    }
}

namespace TRIVIAL
{
    struct A
    {
        A(){}
        A(int v): a(v){}
        int a;
    };

    struct B {};

    struct C{
    public:
        virtual void f(){}
    };

    struct D{
    public:
        D(D const&) {}
    };

    void layout_process(void)
    {
        FUNCTION_START()

        cout<<std::boolalpha<<std::is_trivially_copyable_v<A><<" | ";
        cout<<std::boolalpha<<std::is_trivially_copyable_v<B><<" | ";
        cout<<std::boolalpha<<std::is_trivially_copyable_v<C><<" | ";
        cout<<std::boolalpha<<std::is_trivially_copyable_v<D><<"\n  ";

        
        cout<<std::boolalpha<<std::is_trivial_v<A><<" | ";
        cout<<std::boolalpha<<std::is_trivial_v<B><<" | ";
        cout<<std::boolalpha<<std::is_trivial_v<C><<" | ";
        cout<<std::boolalpha<<std::is_trivial_v<D><<" | ";
        FUNCTION_END()
    }
}

namespace STANDARD
{
    struct A{
        int a;
    };
    struct B:public A{};
    struct C:public A{
        int b;
    };

    void layout_process(void)
    {
        FUNCTION_START()

        cout<<std::is_standard_layout_v<A><<" | ";
        cout<<std::is_standard_layout_v<B><<" | ";
        cout<<std::is_standard_layout_v<C><<"\n  ";

        //c++20 deprecated, use is_standard_layout_v && is_trivial_v
        cout<<std::is_pod_v<A><<" | ";
        cout<<std::is_pod_v<B><<" | ";
        cout<<std::is_pod_v<C><<" | ";
        cout<<(std::is_standard_layout_v<A> && std::is_trivial_v<A>)<<" | ";

        FUNCTION_END()
    }
}
int main(void)
{
    AGGREGATE_TYPE::layout_process();

    TRIVIAL::layout_process();

    STANDARD::layout_process();
}