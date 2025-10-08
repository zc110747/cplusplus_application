//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. std::tuple声明
//      2. std::tuple方法
//      3. std::apply应用
//
// Author:
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <tuple>
#include <iostream>
#include <algorithm>
#include <string>
#include <unistd.h>
#include <vector>

namespace TUPLE_CONSTURCT {

    int test(void) 
    {
        std::cout << "========================= TUPLE_CONSTURCT =========================" << std::endl;
        
        // construct
        std::tuple<int, double> t1;
        std::get<0>(t1) = 100;
        std::get<1>(t1) = 200.0;
        std::cout << "t1: " << std::get<0>(t1) << " " << std::get<1>(t1) << std::endl;

        // construct with parameters
        std::tuple<int, double> t2(1, 2.5);
        std::cout << "t2: " << std::get<0>(t2) << " " << std::get<1>(t2) << std::endl;

        // copy construct
        std::tuple<int, double> t3(t1);
        std::cout << "t3: " << std::get<0>(t3) << " " << std::get<1>(t3) << std::endl;

        // operator=
        std::tuple<int, double> t4 = t1;
        std::cout << "t4: " << std::get<0>(t4) << " " << std::get<1>(t4) << std::endl;

        // make_tuple
        std::tuple<int, double> t5 = std::make_tuple(1, 2.5);
        std::cout << "t5: " << std::get<0>(t5) << " " << std::get<1>(t5) << std::endl;

        // make pair
        std::pair<int, double> t6 = std::make_pair(1, 2.5);
        std::cout << "t6: " << t6.first << " " << t6.second << std::endl;

        // list initialization
        std::tuple<int, double> t7 = {1, 2.5};
        std::tuple<int, double> t8{1, 2.5};
        std::cout << "t7: " << std::get<0>(t7) << " " << std::get<1>(t7) << std::endl;
        std::cout << "t8: " << std::get<0>(t8) << " " << std::get<1>(t8) << std::endl;

        // forward_as_tuple
        std::tuple<std::string, int, char> t9 = std::forward_as_tuple("hello", 24, 'a');
        std::cout << "t9: " << std::get<0>(t9) << " " << std::get<1>(t9) << " " << std::get<2>(t9) << std::endl;
        return 0;
    }
}

namespace TUPLE_METHOD { 
    
    int test(void) 
    {
        // std::get
        std::cout << "========================= TUPLE_METHOD =========================" << std::endl;
        std::tuple<int, double, std::string> t1(1, 2.0, "hello");
        std::cout << "t1: " << std::get<0>(t1) << " " << std::get<1>(t1) << " " << std::get<2>(t1) << std::endl;

        std::get<0>(t1) = 100;
        std::cout << "t1: " << std::get<0>(t1) << " " << std::get<1>(t1) << " " << std::get<2>(t1) << std::endl;

        // size
        auto size = std::tuple_size<decltype(t1)>::value;
        std::cout << "tuple_size size: " << size << std::endl;

        // tuple_cat
        std::pair<int,char> pair0(10, 'a');
        auto t2 = std::tuple_cat(t1, std::tuple<int, char>(pair0));
        std::cout << "tuple_cat:" << std::get<0>(t2) << " " << std::get<1>(t2) << " ";
        std::cout << std::get<2>(t2) << " " << std::get<3>(t2) << " ";
        std::cout << std::get<4>(t2) << std::endl;

        // std::tie
        int val1;
        double val2;
        std::string val3;

        std::tie(val1, val2, val3) = t1;
        std::cout << "tie: " << val1 << " " << val2 << " " << val3 << std::endl;

        // equal
        std::tuple<int, double, std::string> t3(1, 2.0, "Hello");
        std::tuple<int, double, std::string> t4(1, 2.0, "World");
        bool equal = (t3 == t4); // 比较两个 tuple 是否相等
        std::cout << "equal: " << equal << std::endl;

        // structured bind
        // 结构化绑定解析tuples
        auto [val4, val5] = std::make_tuple(1, 2);
        std::cout << "structured bind: " << val4 << " " << val5 << std::endl;

        // tuple_element 获取元组中指定位置的元素类型
        std::tuple_element<0, decltype(t1)>::type val6 = std::get<0>(t1);
        std::cout << "tuple_element: " << val6 << std::endl;

        // ignore(配合std::tie使用的占位符，用于忽略元组中的元素)
        std::tie(val1, std::ignore, val3) = t1;
        std::cout << "ignore: " << val1 << " " << val3 << std::endl;
        return 0;
    }
}

namespace TUPLE_APPLY 
{
    int add(int a, int b) 
    {
        return a + b;
    }

    auto multiply = [](int a, int b)  {
        return a * b;
    };

    // 重载 << 运算符，用于打印 tuple
    template<typename... Ts>
    std::ostream& operator<<(std::ostream& os, std::tuple<Ts...> const& theTuple)
    {
        std::apply
        (
            [&os](Ts const&... tupleArgs)
            {
                os << '[';
                std::size_t n{0};
                ((os << tupleArgs << (++n != sizeof...(Ts) ? ", " : "")), ...);
                os << ']';
            }, theTuple
        );
        return os;
    }

    int test(void) 
    {
        // std::apply
        std::cout << "========================= TUPLE_APPLY =========================" << std::endl;
   
        auto args = std::make_tuple(3, 4);

        int sum = std::apply(add, args);
        std::cout << "Sum: " << sum << std::endl;

        int product = std::apply(multiply, args);
        std::cout << "Product: " << product << std::endl;

        auto tuple_args = std::forward_as_tuple("hello", 24, 'a');
        std::apply([](auto&&... args) {
            ((std::cout << args << " "), ...);
            std::cout << std::endl; 
        }, tuple_args);

        auto func = [](std::string s, int i, char c) {
            std::cout << "String: " << s;
            std::cout << " | Integer: " << i;
            std::cout << " | Character: " << c << std::endl;
        };
        std::apply(func, tuple_args);

        std::cout << "tuple_args: " << tuple_args << std::endl;
        return 0;
    }
}

int main(int argc, char* argv[])
{
    TUPLE_CONSTURCT::test();

    TUPLE_METHOD::test();

    TUPLE_APPLY::test();

    return 0;
} 
