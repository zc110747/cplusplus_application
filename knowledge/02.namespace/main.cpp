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
                std::cout << "T1::A" << std::endl;
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
                std::cout << "T2::A" << std::endl;
            }
        };
    }

    void test(void)
    {
        std::cout << " =================== BASIC =================== " << std::endl;

        std::cout << T1::a << " "<<T1::b << " | ";
        T1::A a1;
        a1.print();

        std::cout << T2::a << " "<<T2::b << " | ";
        T2::A a2;
        a2.print();

        std::cout << std::endl;
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
                std::cout << "T0::T1::A" << std::endl;
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
                std::cout << "T0::T1::T2::A" << std::endl;
            }
        };
    }

    void test(void)
    {
        std::cout << " =================== NESTED =================== " << std::endl;

        std::cout << T0::T1::a << " "<<T0::T1::b << " | ";
        T0::T1::A a1;
        a1.print();

        std::cout << T0::T1::T2::a << " "<<T0::T1::T2::b << " | ";
        T0::T1::T2::A a2;
        a2.print();

        using namespace T0::T1;
        std::cout << a << " "<<b << " | ";
        A a3;
        a3.print();

        std::cout << T2::a << " "<<T2::b << " | ";
        T2::A a4;
        a4.print();

        std::cout << std::endl;
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
                std::cout << "V1" << " | ";
            }
        }  

        inline namespace V2
        {
            void print(void)
            {
                std::cout << "V2" << " | ";
            }
        }
    } 

#if __cplusplus > 202002L
    namespace T0::V1
    {
        void print(void)
        {
            std::cout << "V1" << " | ";
        }
    }

    namespace T0::inline V2
    {
        void print(void)
        {
            std::cout << "V2" << " | ";
        }
    }
#endif

    namespace TV2 = T::V2;

    void test(void)
    {
        std::cout << " =================== INLINE =================== " << std::endl;

        T::print();         //V2
        T::V2::print();     //V2
        T::V1::print();     //V1
        TV2::print();       //V2

#if __cplusplus > 202002L
        T0::print();
        T0::V2::print();
        T0::V1::print();
#endif 
     
        std::cout << std::endl;
    }
}

int main(int argc, char const *argv[])
{
    BASIC::test();

    NESTED::test();

    INLINE::test();
    return 0;
}