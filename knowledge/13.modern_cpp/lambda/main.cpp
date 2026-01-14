//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2025-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. lambda表达式和捕获(值捕获、引用捕获、隐式捕获、类中的this捕获)
//      2. 泛型lambda表达式
//      3. lambda捕获初始化器
//
// Author:
//      @公众号 <嵌入式技术总结>
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <vector>
#include <algorithm>
#include <iostream>
#include <ostream>
#include <functional>

namespace LAMBDA_CAPTURE
{
    class lambda_class
    {
    private:
        int x{0};
        int y{0};

    public:
        void test(int a, int b) {

            // no capture
            auto v1 = [] {
                std::cout << "no capture" << std::endl;
            };
            v1();

            // 值捕获，this指针
            auto v2 = [=, this] {
                x = a;
                std::cout << "x+y+a+b: " << x + y + a + b << std::endl;
            };
            v2();

            // 引用捕获
            auto v3 = [&] {
                y = a;
                std::cout << "x+y+a+b: " << x + y + a + b << std::endl;
            };
            v3();

            // this capture
            auto v4 = [this] {
                x = 1;
                std::cout << x << " " << y << std::endl;
            };
            v4();

            // this值捕获
            auto v5 = [this, a, b] {
                x = 5;
                y = 3;
                std::cout << x + y + a + b << std::endl;
            };
            v5();

            // *this值捕获
            auto v6 = [*this]() mutable {
                x = 10;
                y = 20;
                std::cout << x << " " << y << std::endl;
            };
            v6();
        }
    };

    // 仿函数，函数对象
    class func_obj
    {
    public:
        func_obj(int x = 0, int y = 0):m_x(x), m_y(y){}
        void operator()(int a) {
            std::cout << m_x << " " << m_y << " " << a << std::endl;
        }
    private:
        int m_x;
        int m_y;
    };

    
    void callback_func(int a, std::function<void(int a)> callback) 
    {
        if (callback) {
            callback(a);
        }
    }

    
    std::function<int(int, int)> add = [](int a, int b) {
        return a + b;
    };

    int test(void)
    {
        std::cout << "=============== LAMBDA_CAPTURE =============== \n";

        // lambda表达式和捕获
        std::vector<int> v1{1, 2, 3, 5, 7};
        std::for_each(v1.begin(), v1.end(), [](auto &ref){
            std::cout << ref << " ";
        });
        std::cout << std::endl;

        //lambda &reference value from
        auto func = [&v1](int size) -> int {
            auto min = size>v1.size()?v1.size():size;
            return min;
        };
        std::cout << func(3) << std::endl;

        // 引用捕获修改值
        int a = 10;
        auto func2 = [&a](int b) -> int {
            a = 20;
            return a + b;
        };
        std::cout << "func2(3): " << func2(3) << std::endl;
        std::cout << "a: " << a << std::endl;

        // 函数对象(仿函数)
        func_obj func_obj1(1, 2);
        std::cout << "func_obj1: " << std::endl;
        func_obj1(3);

        // lambda类中的捕获
        lambda_class lc1;
        lc1.test(1, 2);

        // 支持lambda回调的函数
        callback_func(5, [](int a) {
            std::cout << "callback func:" << a << std::endl;
        });

        // 绑定函数
        const auto& func1 = add;
        std::cout << "func1(1, 2): " << func1(1, 2) << std::endl;
        
        // 使用std::bind绑定成员参数
        auto addTwo = std::bind(add, 2, std::placeholders::_1);
        int result = addTwo(3);
        std::cout << "Result: " << result << std::endl;

        // 值捕获默认是const，不能修改捕获值
        // mutable修饰符可以修改捕获值
        auto func3 = [a](int x, int y) mutable -> int {
            a += 1;
            return a + x + y;
        };
        std::cout << "func3(1, 2): " << func3(1, 2) << std::endl;

        // noexcept表示函数不抛出异常
        auto func4 = [a](int x, int y) mutable noexcept(true) -> int {
            return a * x + y;
        };
        std::cout << "func4(1, 2): " << func4(1, 2) << std::endl;
        return 0;
    }
}

namespace LAMBDA_TEMPLATE {
    class Hello {
    public:
        void hello(const std::string& value) {
            std::cout << "Hello, " << value << "!" << std::endl;
        }
    };

    int test(void) 
    {
        std::cout << "====================== LAMBDA_TEMPLATE ====================" << std::endl;
        std::vector<int> v1 = {1, 2, 3, 4, 5};

        // 泛型lambda函数
        auto lambdafunc = [&]<typename T>(T&& data) {
            std::cout << "泛型lambda函数: ";
            for (auto& x : data) {
                std::cout << x << " ";
            }
            std::cout << std::endl;
        };
        lambdafunc(v1);

        // 泛型lambda(auto)
        auto func = [](auto a, auto b) -> decltype(a+b) {
            std::cout<<"泛型lambda: ";
            return a+b;
        };
        std::cout<<func(2, 4)<<std::endl;

        // std::bind绑定成员函数
        Hello hello_;
        std::function<void(const std::string&)> func2 = std::bind(&Hello::hello, hello_, std::placeholders::_1);
        func2("world");
        return 0;
    }
}

namespace LAMBDAT_CAPTURE_INIT
{
    class object
    {
    public:
        object(int a) : a_(a) {}
        int get_a() const { return a_; }

    private:
        int a_;
    };

    std::ostream& operator<<(std::ostream& os, const object& obj) {
        os << "object(" << obj.get_a() << ")";
        return os;
    }

    int test(void)
    {
        std::cout << "=============== LAMBDAT_CAPTURE_INIT =============== \n";
        std::vector<int> vec = {1, 2, 3, 4, 5};

        // lambda捕获初始化器
        auto func1 = [moveVec = std::move(vec)](){
            std::cout<<"lambda捕获初始化器: ";
            for (auto x : moveVec) {
                std::cout << x << " ";
            }
            std::cout << std::endl;
        };
        func1();

        object obj1(5);
        auto func2 = [obj = std::move(obj1)](){
            std::cout << "lambda捕获初始化器: " << obj << std::endl;
        };
        func2();
        return 0;
    }
}

int main(int argc, char* argv[])
{
    LAMBDA_CAPTURE::test();

    LAMBDA_TEMPLATE::test();

    LAMBDAT_CAPTURE_INIT::test();

    return 0;
} 
