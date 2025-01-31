/*
1. 命名空间说明

命名空间（namespace）是C++中用于解决命名冲突的一种机制。它允许你将全局作用域划分为不同的子作用域，
每个子作用域都有自己的名字，这样就可以在不同的命名空间中定义相同名字的变量、函数或类，而不会产生冲突。

命名空间的定义格式为：
namespace namespace_name {
    // 声明或定义变量、函数、类等
}
命名空间的访问方式为：
namespace_name::variable_name;
namespace_name::function_name();
namespace_name::ClassName object_name;

使用using关键字可以控制命名空间的访问权限，有两种形式：
using namespace namespace_name;：导入整个命名空间，使得该命名空间中的所有成员都可以直接使用，无需加上命名空间前缀。
using namespace_name::member_name;：只导入命名空间中的特定成员，使得该成员可以直接使用，无需加上命名空间前缀。

2. 嵌套命名空间

嵌套命名空间是指在一个命名空间内部再定义一个命名空间，格式为：

namespace namespace_name {
    namespace nested_namespace_name {
        // 声明或定义变量、函数、类等
    } 
}
访问嵌套命名空间中的成员时，需要使用多个::来指定完整的路径：
namespace_name::nested_namespace_name::variable_name;

3. 内联命名空间

内联命名空间是指在C++17及以上版本中，可以使用inline关键字来定义一个内联命名空间，格式为：

namespace namespace_name {
    inline namespace nested_namespace_name {
        // 声明或定义变量、函数、类等
    } 
}
访问内联命名空间中的成员时，可以直接使用外层命名空间的名字：
namespace_name::variable_name;

嵌套命名空间和内联命名空间可以组合使用（需要C++20支持），格式为：
namespace outer_namespace::inline inner_namespace {
    // 声明或定义变量、函数、类等
}
*/
#include <iostream>

using namespace std;

/*
FUNCTION_START()
FUNCTION_END()
*/
#define FUNCTION_START()  {cout<<__func__<<":\n";}{
#define FUNCTION_END()    }{cout<<"\n\n";  }   

//命名空间
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