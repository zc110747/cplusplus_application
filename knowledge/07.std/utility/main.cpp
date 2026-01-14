//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. 相关组件类，std::move, std::forward，std::rel_ops
//      2. 相关组件方法, std::swap, std::make_pair
//      3. std::declval
// Author:
//      @公众号 <嵌入式技术总结>
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <utility>
#include <tuple>
#include <vector>
#include <string>

namespace UTILITY_BASE
{
    void over_func1 (const int& x) 
    {
        std::cout << "[lvalue]" << std::endl;
    }

    void over_func1 (int&& x) 
    {
        std::cout << "[rvalue]" << std::endl;
    }

    template <class T> 
    void func (T&& x) {
        over_func1 (x);                   // always an lvalue
        over_func1 (std::forward<T>(x));  // rvalue if argument is rvalue
    }

    template <class T>
    class A {
    public:
        A() = default;
        T x;
        T y;
        A(T x, T y) : x(x), y(y) {}
        bool operator==(const A& other) const {
            return x == other.x && y == other.y;
        }
        bool operator<(const A& other) const {
            return x < other.x && y < other.y;
        }
    };

    void foo(int& arg) {
        std::cout << "lvalue" << std::endl;
    }
    
    void foo(int&& arg) {
        std::cout << "rvalue" << std::endl;
    }

    template<class T>
    void wrapper(T&& arg)
    {
        // arg is always lvalue
        foo(std::forward<T>(arg));
    }

    int test(void) 
    {
        std::cout << "===================== UTILITY_BASE =====================" << std::endl;

        std::vector<int> v1 = {1, 2, 3, 4, 5};
        std::vector<int> v2 = {6, 7, 8, 9, 10};

        // swap
        std::swap(v1, v2);
        for (const auto& val : v1) {
            std::cout << val << ", ";
        }
        std::cout << std::endl;

        // exchange
        int a = 0;
        int b = std::exchange(a, 10);
        std::cout << "a: " << a << ", b: " << b << std::endl;
        // move
        auto&& v3 = std::move(v2);
        v3.push_back(11);
        for (const auto& val : v3) {
            std::cout << val << ", ";
        }
        std::cout << std::endl;

        // move, std::forward
        int a1 = 0;
        
        func(a1);
        func(0);

        using namespace std::rel_ops;
        A<int> ac1(1, 2), ac2(3, 4);
        std::cout << "ac1 == ac2: " << (ac1 == ac2) << std::endl;
        std::cout << "ac1 < ac2: " << (ac1 < ac2) << std::endl;

        // 通过rel_ops支持
        std::cout << "ac1 != ac2: " << (ac1 != ac2) << std::endl;
        std::cout << "ac1 > ac2: " << (ac1 > ac2) << std::endl;
        std::cout << "ac1 <= ac2: " << (ac1 <= ac2) << std::endl;
        std::cout << "ac1 >= ac2: " << (ac1 >= ac2) << std::endl;

        // std::forward 转发引用
        int x1 = 0;
        int& x2 = x1;

        wrapper(x1);            // lvalue
        wrapper(x2);            // lvalue
        wrapper(std::move(x1)); // rvalue

        // as_const
        std::string s1 = "hello";
        auto && s2 = std::as_const(s1);

        std::cout << s2 << std::endl;

        // cmp_equal
        std::cout << "cmp_equal: " << std::cmp_equal(1, 2) << std::endl;
        
        // in_range
        std::cout << std::boolalpha;
        std::cout << "in_range: " << std::in_range<std::size_t>(23) << std::endl;
        std::cout << "in_range: " << std::in_range<short>(655360) << std::endl;

        // make_pair
        std::pair<int, int> p = std::make_pair(1, 2);
        std::cout << "make_pair: " << p.first << ", " << p.second << std::endl;
        std::cout << "make_pair: " << std::get<0>(p) << ", " << std::get<1>(p) << std::endl;
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

int main() 
{
    UTILITY_BASE::test();

    DECLVAL::test();

    return 0;
}