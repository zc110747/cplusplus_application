//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      namespace.cpp
//
//  Purpose:
//     1. 命名空间说明
//     2. 嵌套命名空间
//     3. 内联命名空间
//     4. 命名空间别名
//
// Author:
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>

using namespace std;

/*
FUNCTION_START()
FUNCTION_END()
*/
#define FUNCTION_START()  {cout<<__func__<<":\n";}{
#define FUNCTION_END()    }{cout<<"\n\n";  }   

// 命名空间
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

//嵌套命名空间
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

//内联命名空间
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

    namespace TV2 = T::V2;

    void namespace_test(void)
    {
        FUNCTION_START()

        T::print();         //V2
        T::V2::print();     //V2
        T::V1::print();     //V1
        TV2::print();       //V2

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