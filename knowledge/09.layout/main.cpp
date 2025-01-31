/**********************************************************
 * 说明: struct_layout.cpp
 * 
 * ****** 平凡可拷贝类型(trivial copyable)*****
 * 它指的是那些可以通过简单的内存拷贝（如memcpy）进行复制，并且在复制过程中不会有任何特殊行为（如调用构造函数或析构函数）的类型
 * 1. 没有虚函数和虚基类
 * 2. 类和其基类本身满足平凡可拷贝类型
 * 3. 所有非静态成员都是平凡可拷贝类型
 * TrivalCopyable类型可以通过简单的内存拷贝（如memcpy）进行复制，并且在复制过程中不会有任何特殊行为
 * 可通过is_trivially_copyable_v判断
 * 
 * ***** 平凡类型(trivial)*****
 * 平凡类型（Trivial Type）是指那些可以通过简单的内存拷贝（如memcpy）进行复制，并且在复制过程中不会有任何特殊行为（如调用构造函数或析构函数）的类型
 * 1.所有构造函数和析构函数均为默认(不能存在用户自定义的构造，拷贝构造，移动构造，移动赋值，析构函数等)
 * 2.拷贝赋值运算符和移动赋值运算符均为默认(不能存在用户自定义的赋值函数) 
 * 3.平凡类型的所有基类和非静态成员也必须是平凡类型。
 * 可通过is_trivial_v判断
 * 
 * ***** 标准布局类型(standard-layout)*****
 * 与C语言兼容的数据类型，并且其内存布局是标准的、可预测的
 * 1.所有非静态成员具有相同的访问控制
 * 2.没有虚函数和虚基类
 * 3.基类和派生类的非静态成员具不能有相同的名称
 * 4.第一个非静态成员类型不能是当前类类型
 * 5.所有非静态成员都是标准布局类型
 * 可通过is_trivially_copyable_v判断
 * 
 * 标准布局类型可以直接进行内存拷贝，但不一定是平凡类型，因为它可能有用户自定义的构造函数或其他特殊成员函数
 * 平凡类型可以通过简单的内存拷贝进行复制，并且在复制过程中不会有任何特殊行为，但不一定是标准布局类型，
 * 因为它可能有不同的访问控制或其他不符合标准布局类型要求的特性。
 * 
 ***** 聚合类型 *****
 聚合类型（Aggregate Type）是指那些可以进行聚合初始化（Aggregate Initialization）的类型(C++20允许带基类的聚合类型)
 * 1.没有用户声明，提高或者继承的构造函数(包含default)
 * 2.没有用户声明，提高或者继承的赋值运算符
 * 3.没有虚函数
 * 4.没有私有或保护的非静态成员
 * 5.没有私有，保护或虚继承的基类
 * 聚合类型支持默认的列表初始化, 可通过is_aggregate_v判断
 * 
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
#include <cstring>
#include <iostream>
#include <type_traits>
#include <cstdint>

using namespace std;

#define FUNCTION_START()  {cout<<__func__<<":\n  ";}{
#define FUNCTION_END()    }{cout<<"\n\n";  }   

//聚合类型
namespace AGGREGATE_TYPE
{
    struct A {
        int a;
        int b;
        void print() {
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

    void test(void)
    {
        FUNCTION_START()

        //true, true, false
        //有private变量, 非聚合类型
        cout<<"aggregate:"<<std::boolalpha<<std::is_aggregate<A>::value<<" | ";
        cout<<"aggregate:"<<std::boolalpha<<std::is_aggregate<B>::value<<" | ";
        cout<<"aggregate:"<<std::boolalpha<<std::is_aggregate<C>::value<<" | ";

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

//平凡类型
namespace TRIVIAL
{
    struct A
    {
        A(){}
        A(int v): a(v){}
        int a{0};
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

    void test(void)
    {
        FUNCTION_START()

        //true false
        //有用户自定义构造函数, 不是trivial类型
        A a1{1}, b1;

        cout<<"trivivally able:"<<std::boolalpha<<std::is_trivially_copyable_v<A><<" | ";
        cout<<"trivivally:"<<std::boolalpha<<std::is_trivial_v<A><<"\n  ";
        
        memcpy(&b1, &a1, sizeof(A));
        cout<<"b1 value:"<<b1.a<<"\n  ";

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
    void test(void)
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

    void test(void)
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

        delete pDerive;
        pDerive = nullptr;
        delete pBase;
        pBase = nullptr;

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

        {
            class U1{
            public:
                int a{-1};
                int get_a(){
                    return a;
                }
                virtual int get_v_a(){
                    return a;
                }
            };
            class U2:public U1{
            public:
                int a{2};
                int get_a(){
                    return a;
                }

                int get_v_a(){
                    return a;
                }
            };
            cout<<sizeof(U1)<<" "<<sizeof(U2)<<" | ";
            U2 u;
            U1 *pu = &u;

            //基类指针访问派生类的接口
            cout<<pu->get_a()<<" | ";   
            cout<<pu->get_v_a()<<" | ";
        }
        FUNCTION_END()
    }
}

namespace ALIGNMENT
{
struct alignas(16) MyStruct {
    char a;     // 1字节
    int b;      // 4字节
    double c;   // 8字节
};

void test(void)
{
    std::cout << "Size of MyStruct: " << sizeof(MyStruct) << std::endl;
    std::cout << "Alignment of MyStruct: " << alignof(MyStruct) << std::endl;
}
}
int main(void)
{
    AGGREGATE_TYPE::test();

    TRIVIAL::test();

    STANDARD::test();

    CLASS_LAYOUT::test();

    ALIGNMENT::test();

    return 0;
}