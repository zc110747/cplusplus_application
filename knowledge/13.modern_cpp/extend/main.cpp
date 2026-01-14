//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2025-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1.  带初始化的条件语句if, switch
//      2.  nullptr
//      3.  结构化绑定
//      4.  扩展的for表达式
//
// Author:
//      @公众号 <嵌入式技术总结>
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include <algorithm>
#include <condition_variable>
#include <mutex>
#include <chrono>
#include <map>
#include <tuple>
#include <array>

using namespace std::chrono_literals;

namespace INIT_CONDITION
{
    std::condition_variable cv;
    std::mutex cv_m;

    int test(void) 
    {
        std::cout << "=============== INIT_CONDITION =============== \n";
        
        std::vector<int> vcon_0 = {1, 2, 3, 4, 5};

        // 带初始化的条件语句if, switch
        if (auto it = std::find(vcon_0.begin(), vcon_0.end(), 3); it != vcon_0.end()) {
            std::cout << "Found 3 at position: " << std::distance(vcon_0.begin(), it) << std::endl;
        } else {
            std::cout << "3 not found in the vector." << std::endl;
        }
        
        // 带初始化的条件语句switch
        switch(std::unique_lock<std::mutex> lk(cv_m); cv.wait_for(lk, 100ms)) {
            case std::cv_status::timeout:
            std::cout << "timeout" << std::endl;
            break;
            case std::cv_status::no_timeout:
            std::cout << "no timeout" << std::endl;
            break;
        }

        // switch语句的选择因子必须为整数类型
        switch (auto it = std::find(vcon_0.begin(), vcon_0.end(), 3); it != vcon_0.end()) {
            case false:
                std::cout << "3 not found in the vector." << std::endl;
                break;
            default:
                std::cout << "Found 3 at position: " << std::distance(vcon_0.begin(), it) << std::endl;
                break;
        }

        return 0;
    }
}

namespace NULLPTR
{
    template<class T>
    class widget {
    public:
        widget() {
            std::cout << "widget" << std::endl;
        }
    };

    template<>
    class widget<std::nullptr_t> {
    public:
        widget() {
            std::cout << "nullptr widget" << std::endl;
        }
    };

    template<class T>
    widget<T> *make_widget(T) {
        return new widget<T>();
    }

    int test(void) 
    {
        std::cout << "=============== NULLPTR =============== \n";
        
        int* ptr_1 = nullptr;
        if (ptr_1 == nullptr) {
            std::cout << "ptr_1 is null" << std::endl;
        }

        // NULL比较时自动转换为nullptr
        char *ptr_2 = NULL;
        if (ptr_2 == nullptr) {
            std::cout << "ptr_2 is null" << std::endl;
        }

        // 声明为std::nullptr_t类型, 与nullptr一致
        decltype(nullptr) ptr_3;
        std::nullptr_t ptr_4;
        if (ptr_3 == nullptr) {
            std::cout << "ptr_3 is null" << std::endl;
        }
        if (ptr_4 == nullptr) {
            std::cout << "ptr_4 is null" << std::endl;
        }

        // nullptr可以执行正确的构造函数
        auto ptr_5 = make_widget(nullptr);
        delete ptr_5;
        auto ptr_6 = make_widget(NULL);
        delete ptr_6;
        return 0;
    }
}

namespace STRUCT_BINDING
{
    struct Point {
        int x;
        int y;
    };

    class MyClass {
    public:
        MyClass(int x, int y) : x(x), y(y) {}
        int x;
        int y;
    };

    union MyUnion {
        int x;
        
        struct BitField {
            int x : 4;
            int y : 4;
            int z : 8;
        }bits;
    };



    std::tuple<int, double, std::string> get_data() {
        return std::make_tuple(42, 3.14, "Hello, Structured Binding!");
    }

    int test(void) 
    {
        std::cout << "=============== STRUCT_BINDING =============== \n";

        Point p1{1, 2};

        auto [x1, y1] = p1;
        std::cout << "p1: " << p1.x << ", " << p1.y << std::endl;
        std::cout << "x1: " << x1 << ", y1: " << y1 << std::endl;

        auto [x2, y2] = std::make_pair(3, 4);
        std::cout << "x2: " << x2 << ", y2: " << y2 << std::endl;

        auto [x3, y3, z3] = get_data();
        std::cout << "x3: " << x3 << ", y3: " << y3 << ", z3: " << z3 << std::endl;
        
        int arr[] = {1, 2, 3, 4};
        auto [x4, y4, z4, w4] = arr;
        std::cout << "Array: " << x4 << ", " << y4 << ", " << z4 << ", " << w4 << std::endl;

        std::map<std::string, int> scores = {
            {"Alice", 90},
            {"Bob", 85},
            {"Charlie", 95}
        };
        for (const auto& [name, score] : scores) {
            std::cout << name << ": " << score << std::endl;
        }

        auto [x5, y5] = std::make_tuple(5, std::string("test"));
        std::cout << "x5: " << x5 << ", y5: " << y5 << std::endl;

        std::tuple<int, double, std::string> t = std::make_tuple(1, 2.0, "test");
        auto& [x6, y6, z6] = t;
        std::cout << "x6: " << x6 << ", y6: " << y6 << ", z6: " << z6 << std::endl;

        // std::array支持结构化绑定
        std::array<int, 3> arr1 = {1, 2, 3};
        auto& [x7, y7, z7] = arr1;
        std::cout << "x7: " << x7 << ", y7: " << y7 << ", z7: " << z7 << std::endl;
        x7 = 100;
        std::cout << "arr1[0]: " << arr1[0] << std::endl;

        // 只有public变量的类支持结构化绑定
        MyClass obj(1, 2);
        auto& [x8, y8] = obj;
        std::cout << "x8: " << x8 << ", y8: " << y8 << std::endl;

        // 位域支持结构化绑定
        MyUnion u;
        u.x = 0x1234;
        auto& [x9, y9, z9] = u.bits;
        std::cout << "u.x: " << u.x << ", x9: " << x9 << ", y9: " << y9 << ", z9: " << z9 << std::endl;
        return 0;
    }
}

namespace FOR_EXTENSION
{   
    int test(void) 
    {
        std::cout << "=============== FOR_EXTENSION =============== \n";
        
        int arr[] = {1, 2, 3, 4, 5};
        for (auto i : arr) {
            std::cout << i << " ";
        }
        std::cout << std::endl;

        std::vector<int> v = {1, 2, 3, 4, 5};
        for (auto& i : v) {
            std::cout << i << " ";
        }
        std::cout << std::endl;

        std::map<std::string, int> m = {
            {"one", 1},
            {"two", 2},
            {"three", 3}
        };
        for (const auto& val : m) {
            std::cout << val.first << ": " << val.second << std::endl;
        }

        for (const auto& [key, val] : m) {
            std::cout << key << ": " << val << std::endl;
        }

        return 0;
    }
}

int main(int argc, char* argv[]) 
{
    // 带初始化的条件语句if, switch
    INIT_CONDITION::test();

    // nullptr
    NULLPTR::test();

    // 结构化绑定
    STRUCT_BINDING::test();

    // for 循环扩展
    FOR_EXTENSION::test();
    return 0;
}
