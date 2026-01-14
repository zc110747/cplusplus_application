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
//      @公众号 <嵌入式技术总结>
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <ostream>
#include <thread>
#include <algorithm>
#include <numeric>
#include <functional>
#include <string>

namespace FUNCTIONAL_BASE
{
    int test(void) 
    {
        std::cout << "============= FUNCTIONAL_BASE =============" << std::endl;
        std::vector<int> v1 = {1, 2, 3, 4, 5};

        // plus、minus
        std::plus<int> plus;
        int ret = plus(1, 2);
        std::cout << "plus: " << ret << std::endl;

        std::minus<int> minus;
        ret = minus(1, 2);
        std::cout << "minus: " << ret << std::endl;

        ret = std::reduce(std::begin(v1), std::end(v1), 0, std::plus<>());
        std::cout << "reduce: " << ret << std::endl;

        std::modulus<int> modulus;
        ret = modulus(1, 2);
        std::cout << "modulus: " << ret << std::endl;

        std::equal_to<int> equal_to;
        ret = equal_to(1, 2);
        std::cout << "equal_to: " << ret << std::endl;

        std::logical_or<int> logical_or;
        std::cout << std::boolalpha;
        std::cout << "logical_or: " << logical_or(1, 2) << std::endl;

        // not_fn
        std::cout << "not_fn(equal_to): " << std::not_fn(logical_or)(1, 2) << std::endl;

        // identity
        std::vector<int> numbers = {1, 2, 3, 4, 5};
        auto num_identity = std::identity{}(numbers);
        for (auto &n : num_identity) {
            std::cout << n << " ";
        }
        std::cout << std::endl;

        // hash
        ret = std::hash<std::string>{}("hello");
        std::cout << "hash: " << ret << std::endl;

        // bind
        // std::placeholders::_1 表示占位符，用于指定在调用
        auto func = [](int a, int &b, int &c) {
            b = 1;
            c = 2;
            std::cout << a << " " << b << " " << c << std::endl;
        };
        int val1 = 0, val2 = 0;
        auto bind_func = std::bind(func, val1, std::ref(val2), std::placeholders::_1);

        bind_func(val1);
        std::cout << "val1: " << val1 << std::endl;
        std::cout << "val2: " << val2 << std::endl;

        return 0; 
    }
}

namespace STD_FUNCTION
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
        std::cout << "============= STD_FUNCTION =============" << std::endl;

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
    FUNCTIONAL_BASE::test();

    // STD_FUNCTION::test();

    // BIND_PROCESS::test();

    // INVOKE_PROCESS::test();
    return 0;
}
