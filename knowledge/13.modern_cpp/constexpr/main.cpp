/*
const和constexpr
constexpr 是 C++11 引入的一个关键字，用于声明常量表达式。
常量表达式是指在编译时就能计算出结果的表达式。
使用 constexpr 关键字可以让编译器在编译时就对表达式进行求值，从而提高程序的性能
const 支持运行时和编译期的只读声明

constexpr 和 const 的区别
1. const可以用于声明运行时和编译时的常量，而constexpr 只能用于声明编译时的常量。
2. const变量的值可以在运行时改变，而constexpr变量的值必须在编译时确定。
4. constexpr函数必须满足一定的条件才能被编译器识别为常量表达式函数，而const函数没有这样的限制。

一个函数要想成为常量表达式函数（constexpr)，必须满足如下4个条件
1. 该函数必须有返回值，即函数的返回值类型不能是 void
2. 整个函数的函数体中，除了可以包含 using 指令、typedef 语句以及 static_assert 断言外，只能包含一条 return 返回语句
3. return 返回的表达式必须是常量表达式
4. 函数在使用之前，必须有对应的定义语句
*/
#include <iostream>
#include <type_traits>
#include <memory>
#include <string>
#include <vector>

//模板函数和constexpr
template<int First>
constexpr int add_sum(void)
{
    return First;
}
template<int First, int Second, int... Rest>
constexpr int add_sum(void)
{
   return add_sum<First>() + add_sum<Second, Rest...>();
}
constexpr int ADD_SUM = add_sum<1, 2, 3>();

//constexpr
//const值包含运行时和编译时，constexpr只支持编译时
const int int_val0 = 1;
const int int_val1 = int_val0;
constexpr int int_val2 = 2;
//constexpr int int_val3 = int_val2;  //出错，不允许

//constexpr, 结构体和类
struct const_type
{
    static constexpr int number = 2;
    //c++17 内联, constexpr默认外部可见
    //static const inline number = 2;
};

template<int First, int... rest>
struct mul_all_;

template <int First>
struct mul_all_<First>:std::integral_constant<int, First>
{
};

template <int First, int Second, int... Rest>
struct mul_all_<First, Second, Rest...>
    :std::integral_constant<int, mul_all_<First>::value * mul_all_<Second, Rest...>::value>
{
};

template<class T>
constexpr auto add(T x, T y) -> decltype(x+y)
{
    return x+y;
}

int main(int argc, char *argv[])
{
    char val0[add_sum<2, 4, 6>()];
    char val1[ADD_SUM];
    std::cout<<"val0: "<<sizeof(val0)<<std::endl;
    std::cout<<"val1: "<<sizeof(val1)<<std::endl;
    std::cout<<"add sum: "<<add_sum<1, 2, 3>()<<std::endl;
    
    char val2[int_val2];
    std::cout<<"val2 size:"<<sizeof(val2)<<std::endl;

    char val3[mul_all_<2, 3, 4, 5>::value];
    std::cout<<"val3 size:"<<sizeof(val3)<<std::endl;

    std::unique_ptr<char[]> unique_ptr_val0{new char[add_sum<5, 3>()]};
    unique_ptr_val0[0] = 'h';
    unique_ptr_val0[1] = 'e';
    unique_ptr_val0[2] = '1';
    unique_ptr_val0[3] = '1';
    unique_ptr_val0[4] = 'o';
    unique_ptr_val0[5] = '\0';
    std::cout<<std::string(unique_ptr_val0.get())<<std::endl;

    std::cout<<const_type::number<<std::endl;
    std::vector<int> vector_val0;
    vector_val0.push_back(const_type::number);
    std::cout<<vector_val0[0]<<std::endl;

    constexpr auto result = add(1, 2);
    std::cout<<"reuslt: "<<result<<std::endl;
    static_assert(result == 3, "result should be 3!");

    int val_list[add(2, 3)] = {1, 2, 3, 4, 5};
    for (const auto &val : val_list) {
        std::cout<<val<<" ";
    }
    std::cout<<std::endl;
    
    return 0;
}
