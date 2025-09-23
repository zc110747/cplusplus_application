//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2025-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. auto类型推导
//      2. decltype类型推导
//      3. declval获取右值引用
//
// Author:
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include <utility>
#include <type_traits>
 
namespace AUTO
{
    // 返回类型后置
    auto add(int a, int b) -> int {
        return a + b;
    }

    // 返回类型推导
    auto add2(auto a, auto b) {
        return a+b;
    }

    int test(void) {
        std::cout << "===================== Auto =====================" << std::endl;
        
        // auto类型推导占位符
        auto x = 42; // x的类型被推导为int
        auto y = 3.14; // y的类型被推导为double
        auto z = "hello"; // z的类型被推导为const char*

        std::cout << "x: " << x << std::endl;
        std::cout << "y: " << y << std::endl;
        std::cout << "z: " << z << std::endl;

        // auto类型推导初始化列表
        auto x1{1};
        std::cout << typeid(x1).name() << std::endl;            // int
        auto x2 = {1, 2, 3, 4, 5};
        std::cout << typeid(x2).name() << std::endl;            // std::initializer_list<int>

        // auto类型推导迭代器
        std::vector<int> vec = {1, 2, 3, 4, 5};
        for (auto it = vec.begin(); it != vec.end(); ++it) {    // std::vector<int>::iterator
            std::cout << *it << " ";
        }
        std::cout << std::endl;
        
        auto it1 = vec.cbegin();                                // std::vector<int>::const_iterator
        std::cout << *it1 << std::endl; 

        // 返回类型后置
        auto sum1 = add(1, 2); // sum1的类型被推导为int
        std::cout << "sum1: " << sum1 << std::endl;

        // 函数auto类型推导(参数类型推导)
        auto sum2 = add2(2, 3);
        std::cout << "sum2: " << sum2 << std::endl;

        // auto类型推导lambda
        auto func = [](auto a, auto b){
            return a + b;
        };
        auto sum3 = func(1, 2);
        std::cout << "sum3: " << sum3 << std::endl;

        // auto推导忽略引用和cv限定符
        auto&& val0 = std::move(sum3);
        std::cout << "val0: " << val0 << std::endl;
        sum3 = 1;
        std::cout << "val0: " << val0 << std::endl;
        return 0;
    }
}

namespace DECLTYPE
{
    // 使用decltype推导返回类型
    template <typename T, typename U>
    auto multiply(T t, U u) -> decltype(t * u) {
        return t * u;
    }

    template<typename T>
    struct is_volatile : std::false_type {};
    
    template<typename T>
    struct is_volatile<volatile T> : std::true_type {};

    int test(void) {
        std::cout << "===================== Decltype =====================" << std::endl;
        
        int x1 = 42;
        decltype(x1) y1 = x1;               // y的类型被推导为int
        std::cout << "y1: " << y1 << std::endl;

        double y2 = 3.14;
        decltype(x1 + y2) z1 = x1 + y2;     // z的类型被推导为double
        std::cout << "z1: " << typeid(z1).name() << std::endl;

        auto z2 = multiply(3, 4.5);         // z2的类型被推导为double
        std::cout << "z2: " << typeid(z2).name() << std::endl;
        
        // decltype(auto)、decltype保留引用和cv限定符
        volatile int x2 = 42;
        auto x3 = x2;
        decltype(x2) x4 = x2;
        decltype(auto) x5 = x2;

        std::cout << "x2 is volatile: " << is_volatile<decltype(x2)>::value << std::endl;
        std::cout << "x3 is volatile: " << is_volatile<decltype(x3)>::value << std::endl;
        std::cout << "x4 is volatile: " << is_volatile<decltype(x4)>::value << std::endl;
        std::cout << "x5 is volatile: " << is_volatile<decltype(x5)>::value << std::endl;

        const int x6 = 42;
        auto x7 = x6;
        decltype(x6) x8 = x6;
        decltype(auto) x9 = x6;
        std::cout << "x6 is const: " << std::is_const<decltype(x6)>::value << std::endl;
        std::cout << "x7 is const: " << std::is_const<decltype(x7)>::value << std::endl;
        std::cout << "x8 is const: " << std::is_const<decltype(x8)>::value << std::endl;
        std::cout << "x9 is const: " << std::is_const<decltype(x9)>::value << std::endl;

        int y3 = 10;
        int &y4 = y3;
        int &&y5 = std::move(y3);
        decltype(y4) y6 = y4;               // 左值int &
        decltype(y5) y7 = std::move(y5);    // 右值int &&
        decltype(auto) y8 = std::move(y5);
        std::cout << std::boolalpha;
        std::cout << "y6 is lvalue: " << std::is_lvalue_reference<decltype(y6)>::value << std::endl; // true
        std::cout << "y7 is rvalue: " << std::is_rvalue_reference<decltype(y7)>::value << std::endl; // true
        std::cout << "y8 is rvalue: " << std::is_rvalue_reference<decltype(y8)>::value << std::endl; // true
        
        return 0;
    } 
}

namespace DECLVAL
{
    struct base_t 
    { 
        virtual ~base_t(){} 
    };

    template<class T>
    struct Base : public base_t 
    {
        virtual T t() = 0;
    };

    template<class T>
    struct A : public Base<T> {
        ~A(){}
        virtual T t() override { std::cout << "A" << '\n'; return T{}; }
    };

    struct B {
        B() = delete;
        int t(){ return 1; }
    };

    
    // 一个简单的类，其默认构造函数被删除
    class NonDefaultConstructible {
    public:
        NonDefaultConstructible(int value) : value_(value) {}
        int value() const { return value_; }

    private:
        int value_;
        NonDefaultConstructible() = delete;
    };

    // 一个模板函数，用于获取类型的右值引用并调用其成员函数
    template <typename T>
    auto get_value(decltype(std::declval<T>().value()) val) -> decltype(std::declval<T>().value()) {
        return val;
    }

    int test(void)
    { 
        std::cout << "===================== Declval =====================" << std::endl;

        decltype(std::declval<A<int>>().t()) a{1};      // int a;
        decltype(std::declval<Base<int>>().t()) b{2};   // int b;
        std::cout << "a: " << a << ", b: " << b << '\n';

        decltype(std::declval<B>().t()) c = 3;          // int c;
        std::cout << "c: " << c << '\n';

        std::cout << "get_value<NonDefaultConstructible>(1): " << get_value<NonDefaultConstructible>(1) << std::endl;

        decltype(std::declval<A<int&>>().t()) a1 = a;      // int&;
        std::cout << "a1 is lvalue: " << std::is_lvalue_reference<decltype(a1)>::value << std::endl;  // true

        decltype(std::declval<A<int&&>>().t()) a2 = std::move(a);      // int&&;
        std::cout << "a2 is rvalue: " << std::is_rvalue_reference<decltype(a2)>::value << std::endl;  // true
        return 0;
    } 
}

int main(int argc, char* argv[]) 
{   
    AUTO::test();

    DECLTYPE::test();

    DECLVAL::test();

    return 0;
}
