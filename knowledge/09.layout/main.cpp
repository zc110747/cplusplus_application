<<<<<<< HEAD
//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. 聚合类型
//      2. 平凡可拷贝类型和平凡类型
//      3. 标准布局类型
//      4. POD类型
//      5. 类的内存布局
// 
//
// Author:
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <cstring>
#include <iostream>
#include <type_traits>

namespace TRIVIAL_COPYABLE_TYPE
{
    // 没有虚函数和虚基类
    class MyClass {
    public:
        MyClass(int x, double y) : x_(x), y_(y) {
        }
    public:
        int x_;
        double y_;
    };

    class MyClass2:public MyClass {  
    };

    class MyClass3 {
    public:
        MyClass3() {
            ptr_ = new int(10);
        }
        ~MyClass3() {
            delete ptr_;
            ptr_ = nullptr;
        }
    private:
        int *ptr_{nullptr};
    };

    int process(void) 
    {
        std::cout << "============ TRIVIAL COPYABLE ============" << std::endl;

        std::cout << std::boolalpha;
        std::cout << "MyClass is trivially copyable: " << std::is_trivially_copyable_v<MyClass> << std::endl;       //true
        std::cout << "MyClass2 is trivially copyable: " << std::is_trivially_copyable_v<MyClass2> << std::endl;     //true

        MyClass obj1(1, 2);
        obj1.x_ = 10;

        MyClass obj2(3, 4);

        // 平凡可拷贝类型，可以直接使用memcpy进行复制
        memcpy(&obj2, &obj1, sizeof(MyClass));
        std::cout << "obj2.x: " << obj2.x_ << std::endl;

        MyClass3 obj3;

        std::cout << "MyClass is trivially copyable: " << std::is_trivially_copyable_v<MyClass3> << std::endl;      //false

        return 0;
    }
}

namespace TRIVIAL_TYPE
{
    // 没有虚函数和虚基类
    class MyClass {
    public:
        MyClass(int x, double y) : x_(x), y_(y) {
        }
    public:
        int x_;
        double y_;
    };

    class MyClass1 {
    public:
        MyClass1() = default;
    public:
        int x_;
        double y_;
    };

    class MyClass2: public MyClass1 {
    public:
        MyClass2() = default;
    };

    int process(void)
    {
        std::cout << "============ TRIVIAL ============" << std::endl;

        std::cout << std::boolalpha;
        std::cout << "MyClass is trivial: " << std::is_trivial_v<MyClass> << std::endl;                         //false
        std::cout << "MyClass1 is trivial: " << std::is_trivial_v<MyClass1> << std::endl;                       //true
        std::cout << "MyClass2 is trivial: " << std::is_trivial_v<MyClass2> << std::endl;                       //true

        MyClass obj1(1, 2);
        obj1.x_ = 10;

        MyClass obj2(3, 4);

        // 平凡类型，可以直接使用memcpy进行复制
        memcpy(&obj2, &obj1, sizeof(MyClass));
        std::cout << "obj2.x: " << obj2.x_ << std::endl;

        return 0;
    }    
}

namespace STANDARD_LAYOUT_TYPE
{
    class MyClass {
    public:
        MyClass() = default;

        int x_;
    };

    class MyClass1 {
    public:
        MyClass1() = default;

    public:
        int x_;

    // 非静态成员具有不同的权限
    private:
        int y_;
    };

    class MyClass2: public MyClass {
    public:
        MyClass2() = default;

        int z_;
    };

    class MyClass3 {
    public:
        MyClass3(int x): x_(x) {
        }
        ~MyClass3() {
        }
        int x_;
    };

    int process(void)
    { 
        std::cout << "============ STANDARD LAYOUT ============" << std::endl;

        std::cout << std::boolalpha;
        std::cout << "MyClass is standard layout: " << std::is_standard_layout_v<MyClass> << std::endl;     //true 
        std::cout << "MyClass1 is standard layout: " << std::is_standard_layout_v<MyClass1> << std::endl;   //false
        std::cout << "MyClass2 is standard layout: " << std::is_standard_layout_v<MyClass2> << std::endl;   //false
        std::cout << "MyClass3 is standard layout: " << std::is_standard_layout_v<MyClass3> << std::endl;   //true

        MyClass3 obj3(10);
        std::cout << "obj3.x: " << obj3.x_ << std::endl;
        MyClass3 obj4(0);
        memcpy(&obj4, &obj3, sizeof(MyClass3));
        std::cout << "obj4.x: " << obj4.x_ << std::endl;

        return 0;
    }
}

namespace AGGREGATE_TYPE
{
    class MyStruct {
    public:
        int x_;
        double y_;
    };

    class MyStruct1: public MyStruct {
    public:
        int z_;
    };

    class MyStruct2 {
    private:
        int w_;
    };

    int process(void)
    {
        std::cout << "============ AGGREGATE ============" << std::endl;

        MyStruct s = {1, 2.0}; // 聚合初始化
        std::cout << s.x_ << " " << s.y_ << std::endl;

        std::cout << std::boolalpha;
        std::cout << std::is_aggregate_v<MyStruct> << std::endl;            //true

        MyStruct1 s1 = {1, 2.0, 3};
        std::cout << s1.x_ << " " << s1.y_ << " " << s1.z_ << std::endl;

        std::cout << std::is_aggregate_v<MyStruct1> << std::endl;           //true
        std::cout << std::is_aggregate_v<MyStruct2> << std::endl;           //false

        return 0;
    }
}

namespace POD_TYPE
{
    class MyStruct {
    public:
        int x_;
        double y_;
    };

    class MyStruct1: public MyStruct {
    public:
        int z_;
    };

    template<typename T>
    constexpr auto pod_detect = std::is_trivial_v<T> && std::is_standard_layout_v<T> ;

    int process(void) 
    {
        std::cout << "============ POD ============" << std::endl;

        MyStruct s = {1, 2.0};
        std::cout << s.x_ << " " << s.y_ << std::endl;

        std::cout << std::boolalpha;
        std::cout << std::is_pod_v<MyStruct> << std::endl;            //true

        MyStruct1 s1 = {1, 2.0, 3};
        std::cout << s1.x_ << " " << s1.y_ << " " << s1.z_ << std::endl;

        std::cout << std::is_pod_v<MyStruct1> << std::endl;           //false

        std::cout << pod_detect<MyStruct> << std::endl;               //true
        std::cout << pod_detect<MyStruct1> << std::endl;              //false

        MyStruct s2;
        memcpy(&s2, &s, sizeof(MyStruct));
        std::cout << s2.x_ << " " << s2.y_ << std::endl;

        return 0;
    }
}

namespace CLASS_MEMORY
{
    class MyClass {
    };

    class MyClass1: public MyClass {
    public:
        virtual void func1() {
        }
    };

    class MyClass2 {
        static inline int x_{1};
    };

    class MyClass3 {
    public:
        int x_{1};
    };

    int process(void) 
    {
        std::cout << "============ CLASS MEMORY ============" << std::endl;

        std::cout << sizeof(MyClass) << std::endl;      // 1
        std::cout << sizeof(MyClass1) << std::endl;     // 8
        std::cout << sizeof(MyClass2) << std::endl;     // 1
        std::cout << sizeof(MyClass3) << std::endl;     // 4

        return 0;
    }    
}
int main(int argc, char *argv[])
{
    TRIVIAL_COPYABLE_TYPE::process();

    TRIVIAL_TYPE::process();

    STANDARD_LAYOUT_TYPE::process();

    AGGREGATE_TYPE::process();

    POD_TYPE::process();

    CLASS_MEMORY::process();

    return 0;
}
=======
/*

*/
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
>>>>>>> 62558f2a9fe27a1dd64315cc3ed8ad9fadd71fcb
