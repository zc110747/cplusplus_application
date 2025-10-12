//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. std::function
//      2. std::bind
//      3. std::invoke
//      4. 函数对象和仿函数
//
// Author:
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <ostream>
#include <thread>
#include <functional>

namespace FUNCTIONAL_PROCESS
{
    int func_add(int a, int b) 
    {
        return a + b;
    }

    class object
    {
        public:
            object(int value) : value_(value) {}
            ~object() {}

            auto operator-(const object &other) {
                return object(value_ - other.value_);
            }

        public:
            int value_;
    };

    std::ostream& operator<<(std::ostream &os, const object &obj) {
        os << obj.value_;
        return os;
    }
    template <typename T>
    T func_sub(T a, T b) 
    {
        return a - b;
    }

    template <typename T>
    class func_object
    {
        public:
            T operator()(T a, T b) const {
                return a * b;
            }
    };

    int test(void) 
    {
        std::cout << "============= FUNCTIONAL_PROCESS =============" << std::endl;

        std::function<int(int, int)> func = func_add;
        int ret = func(1, 2);
        std::cout << "func: " << ret << std::endl;

        // function和模板函数
        std::function<int(int, int)> func2 = func_sub<int>;
        ret = func2(1, 2);
        std::cout << "func2: " << ret << std::endl;

        std::function<object(object, object)> func3 = func_sub<object>;
        object obj1(1);
        object obj2(2);
        auto&& obj3 = func3(obj1, obj2);
        std::cout << "obj3: " << obj3 << std::endl;

        // function和lambda表达式
        std::function<int(int, int)> func4 = [](int a, int b) -> int {
            return a * b;
        };
        ret = func4(1, 2);
        std::cout << "func4: " << ret << std::endl;

        // function和函数对象
        std::function<int(int, int)> func5 = func_object<int>();
        ret = func5(2, 3);
        std::cout << "func5: " << ret << std::endl;

        return 0;
    }
}

namespace BIND_PROCESS
{
    int func_add(int a, int b) 
    {
        return a + b;
    }

    void data_process(int& a, int& b)
    {
        a += 1;
        b += 1;
    }

    class object
    {
        public:
            object(int value) : value_(value) {}
            ~object() {}

            int func_multiply(int a, int b)
            {
                return a * b * value_;
            }

            void run(void) 
            {
                // 在类中绑定对象实例
                auto func = std::bind(&object::func_multiply, this, std::placeholders::_1, std::placeholders::_2);
                auto ret = func(1, 2);
                std::cout << "ret: " << ret << std::endl;
            }
        public:
            int value_{0};
    };

    template <typename T>
    class func_object
    {
        public:
            T operator()(T a, T b) const {
                return a * b;
            }
    };

    void test(void)
    {
        std::cout << "============= BIND_PROCESS =============" << std::endl;

        auto func = std::bind(func_add, std::placeholders::_1, std::placeholders::_2);
        auto ret = func(1, 2);
        std::cout << "ret: " << ret << std::endl;

        auto func2 = std::bind(func_add, 1, std::placeholders::_1);
        ret = func2(2);
        std::cout << "ret: " << ret << std::endl;

        // cref实现引用绑定
        int a = 1, b = 2;
        std::cout << "a: " << a << " b: " << b << std::endl;
        auto func3 = std::bind(data_process, a, b);
        func3();
        std::cout << "a: " << a << ", b: " << b << std::endl;
        auto func4 = std::bind(data_process, std::ref(a), std::ref(b));
        func4();
        std::cout << "a: " << a << ", b: " << b << std::endl;

        // 绑定类对象实例
        object obj(2);
        auto func5 = std::bind(&object::func_multiply, obj, std::placeholders::_1, std::placeholders::_2);
        ret = func5(2, 3);
        std::cout << "ret: " << ret << std::endl;   //12
        std::cout << "obj.value_: " << obj.value_ << std::endl;   //2

        obj.run();

        // 绑定函数对象
        auto func6 = std::bind(func_object<int>(), std::placeholders::_1, std::placeholders::_2);
        ret = func6(2, 3);
        std::cout << "func6: " << ret << std::endl;
    }
}

namespace INVOKE_PROCESS
{
    int func_add(int a, int b) 
    {
        return a + b;
    }

    class object
    {
    public:
        object(int value) : value_(value) {}
        int func_multiply(int a, int b)
        {
            return a * b;
        }
    public:
        int value_;
    };

    void test(void)
    {
        std::cout << "============= INVOKE_PROCESS =============" << std::endl;

        // 普通函数调用
        auto ret = std::invoke(func_add, 1, 2);
        std::cout << "ret: " << ret << std::endl;

        // lambda调用
        int result = std::invoke([](int a, int b) { return a + b; }, 1, 2);
        std::cout << "result: " << result << std::endl;

        // 类成员函数调用
        object obj(2);
        ret = std::invoke(&object::func_multiply, obj, 2, 3);
        std::cout << "ret: " << ret << std::endl;

        // 类成员变量调用(获取成员变量值)
        ret = std::invoke(&object::value_, obj);
        std::cout << "ret: " << ret << std::endl;
    }
}

int main(int argc, char* argv[])
{
    FUNCTIONAL_PROCESS::test();

    BIND_PROCESS::test();

    INVOKE_PROCESS::test();
    return 0;
}
