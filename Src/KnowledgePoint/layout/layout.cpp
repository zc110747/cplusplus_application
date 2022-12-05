/**********************************************************
 * 说明: struct_layout.cpp
 ***** 聚合类型 *****
 * 1.没有用户声明或者继承的构造函数(包含default)
 * 2.没有用户显示提供的继承的或显示的构造函数
 * 3.没有virtual成员函数
 * 4.没有private, protected的成员变量
 * 5.没有private, protected或virtual继承的基类
 * 聚合类型支持默认的列表初始化, 可通过is_aggregate_v判断
 
 ***** TrivalCopyable类型 *****
 * TrivalCopyable类型支持memory和流当作二进制处理(memcpy, memcmp, memmove)
 * 1.不能包含virtual，包含虚基类和虚函数
 * 2.构造函数需要符合平凡类型(使用default或者隐式的复制和移动构造函数，复制和移动赋值函数以及析构函数)
 * 3.基类也要满足该要求
 * 可通过is_trivially_copyable_v判断

 ***** trivial类型 *****
 * 1.满足TrivalCopyable类型
 * 2.所有构造函数均为默认构造函数(不能用户自定义构造函数)
 * 3.要求所有基类满足此要求
 * 可通过is_trivial_v判断

 ***** is_standard_layout类型 *****
 * 标准布局的类型可以和其它语言进行互相访问
 * 1.所有非静态类型具有相同的访问控制
 * 2.没有虚函数或虚基类
 * 3.(派生类没有非静态成员同时具有非静态数据成员的基类不超过一个)或(没有含非静态数据成员的基类)
 * 可通过is_trivially_copyable_v判断

 ***** POD类型(Plain Old Data) *****
 * 兼容C语言的类型，C++可以直接调用POD类型的数据, 拥有POD特征的类
 * 直接字节拷贝或二进制拷贝
 * 1.满足平凡类型(trivial)
 * 2.是标准类型布局(standard-layout)
 * 3.所有非静态成员都是POD类型
 * 可通过is_pod_v判断
 * 
 ***** 对象内存分布 *****
 * 1.空类占用1个字节，以使对象占用有效地址
 * 2.虚继承，类或者其基类有虚函数，会让类增加虚表指针
 * 3.虚表指针的个数在基类或者单继承中最多只有一个，多重继承则跟基类中本身带虚指针对象的数目一致。
 * 4.静态成员变量和函数不占用对象的空间
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

    struct B:public A
    {
        int c;
    };

    struct C:public A
    {
    private:
        int d;
    };

    void layout_process(void)
    {
        FUNCTION_START()

        //true, true, false
        //有private变量, 非聚合类型
        cout<<std::boolalpha<<std::is_aggregate<A>::value<<" | ";
        cout<<std::boolalpha<<std::is_aggregate<B>::value<<" | ";
        cout<<std::boolalpha<<std::is_aggregate<C>::value<<" | ";

        //聚合类型支持默认列表初始化
        A a1 = {1, 2};
        a1.print();

        A a2{3, 4};
        a2.print();

        B b1{2, 5, 7};
        b1.print();

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

    struct B 
    {
        B() = default;
        B(B const&) = default;
    };

    struct C
    {
    public:
        virtual void f(){}
    };

    struct D
    {
    public:
        D(D const&&){};
    };

    void layout_process(void)
    {
        FUNCTION_START()

        //true false
        //有用户自定义构造函数,不是trivial类型 
        cout<<std::boolalpha<<std::is_trivially_copyable_v<A><<" | ";
        cout<<std::boolalpha<<std::is_trivial_v<A><<"\n  ";

        //true true
        //默认构造函数，拷贝构造函数，是trivial类型 
        cout<<std::boolalpha<<std::is_trivially_copyable_v<B><<" | ";
        cout<<std::boolalpha<<std::is_trivial_v<B><<"\n  ";

        //false false
        //带有虚函数，非trival类型，非trival copyable类型
        cout<<std::boolalpha<<std::is_trivially_copyable_v<C><<" | ";
        cout<<std::boolalpha<<std::is_trivial_v<C><<"\n  ";

        //false false
        //使用非默认的移动、拷贝构造函数，移动、拷贝赋值函数，以及析构函数，会带来不能copyable
        cout<<std::boolalpha<<std::is_trivially_copyable_v<D><<" | ";
        cout<<std::boolalpha<<std::is_trivial_v<D>;
        FUNCTION_END()
    }
}

namespace STANDARD
{
    struct A
    {
        int a;
    };

    struct B
    {
        virtual void func(){}
    };

    struct C
    {
    private:
        int a;
    public:
        int b;
    };

    struct D:public A
    {
    };

    //c++20 deprecated, use is_standard_layout_v && is_trivial_v repalce of is_pod_v
    void layout_process(void)
    {
        FUNCTION_START()

        //true true true
        cout<<std::is_standard_layout_v<A><<" | ";
        cout<<std::is_trivial_v<A><<" | ";
        cout<<std::is_pod_v<A><<"\n  ";

        //false, false, false
        //有虚函数,既不是平凡类型，也不是标准布局类型
        cout<<std::is_standard_layout_v<B><<" | ";
        cout<<std::is_trivial_v<B><<" | ";
        cout<<std::is_pod_v<B><<"\n  ";

        //false, true, false
        //访问控制不一致
        cout<<std::is_standard_layout_v<C><<" | ";
        cout<<std::is_trivial_v<C><<" | ";
        cout<<std::is_pod_v<C><<"\n  ";

        //true true true
        //派生类没有静态成员
        cout<<std::is_standard_layout_v<D><<" | ";
        cout<<std::is_trivial_v<D><<" | ";
        cout<<std::is_pod_v<D>;

        FUNCTION_END()
    }
}

namespace CLASS_LAYOUT
{
    class Base
    {
    public:
        virtual void print() {
            cout<<"Base"<<" | ";
        }  
        virtual void print1(){
            cout<<"Base1"<<" | ";
        }
    };

    class Derive:public Base
    {
    public:
        virtual void print() {
            cout<<"Derive"<<" | ";
        }   
    };

    using func_ptr = void(*)(void);

    class A {};
    class B {};
    class A1:public A, public B {};
    class A2:public A{};
    class A3:public virtual A{};
    class A4:public virtual A{};
    class A5:public A3, public A4 {};
    class A6:public virtual A, public A3 {};
    class A7:public B, public A3 {};

    class C{
        int a;
    };
    class C1{
        int a;
        inline static int b = 0;
    };
    class C2{
        int a;
        inline static int b = 0;
        void func(){
        }
    };

    void layout_process(void)
    {
        FUNCTION_START()
        
        //父类指针调用基类的函数
        Base *pBase = new Base();
        Base *pDerive = static_cast<Base *>(new Derive());
        pBase->print();
        pDerive->print();

        //虚表函数，占据地址范围与sizeof(func_ptr)一致
        auto *vptr_base = reinterpret_cast<uint64_t *>(pBase);
        cout<<vptr_base<<" | ";
        auto vfunc = (func_ptr *)(*(uint64_t *)(vptr_base));
        (*vfunc)();
        (*(vfunc+1))();
        cout<<"\n  ";

        cout<<sizeof(A)<<" | ";  //空类占1字节，使对象占地址
        cout<<sizeof(B)<<" | ";  //空类占1字节，使对象占地址
        cout<<sizeof(A1)<<" | "; //空类占1字节
        cout<<sizeof(A2)<<" | "; //空类占1字节
        cout<<sizeof(A3)<<" | "; //虚继承，安插虚表指针(与指针占用空间一致)
        cout<<sizeof(A4)<<" | "; //虚继承，安插虚表指针(与指针占用空间一致)
        cout<<sizeof(A5)<<" | "; //多重继承，每个带虚表的继承安插虚表指针，*n
        cout<<sizeof(A6)<<" | "; //多重继承, 虚继承可以避免重复包含，只有单个指针
        cout<<sizeof(A7)<<"\n  "; //多重继承

        //object
        cout<<sizeof(C)<<" | "; 
        cout<<sizeof(C1)<<" | ";  
        cout<<sizeof(C2)<<" | ";  
        FUNCTION_END()
    }
}

int main(void)
{
    AGGREGATE_TYPE::layout_process();

    TRIVIAL::layout_process();

    STANDARD::layout_process();

    CLASS_LAYOUT::layout_process();
}