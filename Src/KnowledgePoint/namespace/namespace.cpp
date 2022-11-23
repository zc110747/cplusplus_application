/************************************************************************************
 * namespace
 * 命名空间允许类、对象和函数分组在一个子作用域下、从而避免发生命名冲突
 * 命名空间的定义格式为 [nsp]{...}
 * 命名空间的访问为 [nsp]::[var/func/class]
 * 使用using可以控制命名空间的访问权限，支持
 * using namespace [nsp]或 using [nsp]::[var/func/class]两种形式，分别导入全部
 * 内容和指定信息
 * 嵌套命名空间:支持[nsp]::[nsp]::...[nsp]{...}的结构之间声明嵌套的命名空间
 * 内联命名空间:主要为inline [nsp]的结构，声明为内联的命名空间会默认引入到上层空间
 * 嵌套命名空间和内联命名可以组合使用(需要C++20支持), 类似格式为
 * [nsp]::inline [nsp]::[nsp]{...}
***********************************************************************************/
#include <iostream>

using namespace std;

/*
FUNCTION_START()
FUNCTION_END()
*/
#define FUNCTION_START()  {cout<<__func__<<":\n";}{
#define FUNCTION_END()    }{cout<<"\n\n";  }   

//命名空间的结构和访问
namespace BASIC
{
    namespace T1
    {
        int a = 0;
        int b = 1;
        struct A 
        {
            void print(){
                cout<<"T1::A"<<endl;
            }
        };
    }

    namespace T2
    {
        int a = 2;
        int b = 3;

        struct A 
        {
            void print(){
                cout<<"T2::A"<<endl;
            }
        };
    }

    void namespace_test(void)
    {
        FUNCTION_START()

        cout<<T1::a<<" "<<T1::b<<" | ";
        T1::A a1;
        a1.print();

        cout<<T2::a<<" "<<T2::b<<" | ";
        T2::A a2;
        a2.print();

        FUNCTION_END()
    }
}

namespace NESTED
{
    namespace T0::T1
    {
        int a = 0;
        int b = 1;
        struct A 
        {
            void print(){
                cout<<"T0::T1::A"<<endl;
            }
        };
    }

    namespace T0::T1::T2
    {
        int a = 2;
        int b = 3;
        struct A 
        {
            void print(){
                cout<<"T0::T1::T2::A"<<endl;
            }
        };
    }

    void namespace_test(void)
    {
        FUNCTION_START()

        cout<<T0::T1::a<<" "<<T0::T1::b<<" | ";
        T0::T1::A a1;
        a1.print();

        cout<<T0::T1::T2::a<<" "<<T0::T1::T2::b<<" | ";
        T0::T1::T2::A a2;
        a2.print();

        using namespace T0::T1;
        cout<<a<<" "<<b<<" | ";
        A a3;
        a3.print();

        cout<<T2::a<<" "<<T2::b<<" | ";
        T2::A a4;
        a4.print();

        FUNCTION_END()
    }
}

namespace INLINE
{
    namespace T
    {
        namespace V1
        {
            void print(void)
            {
                cout<<"V1"<<" | ";
            }
        }  

        inline namespace V2
        {
            void print(void)
            {
                cout<<"V2"<<" | ";
            }
        }
    } 

#if __cplusplus > 202002L
    namespace T0::V1
    {
        void print(void)
        {
            cout<<"V1"<<" | ";
        }
    }

    namespace T0::inline V2
    {
        void print(void)
        {
            cout<<"V2"<<" | ";
        }
    }
#endif

    void namespace_test(void)
    {
        FUNCTION_START()

        T::print();
        T::V2::print();
        T::V1::print();

#if __cplusplus > 202002L
        T0::print();
        T0::V2::print();
        T0::V1::print();
#endif 
        FUNCTION_END()
    }
}

int main(void)
{
    BASIC::namespace_test();

    NESTED::namespace_test();

    INLINE::namespace_test();
    return 0;
}