/**********************************************************
 * 说明: constexpr
 * const 支持运行时和编译期的只读声明
 * constexpr 仅支持编译器的常量
************************************************************/
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


int main(int argc, char *argv[])
{
    char char_val0[add_sum<2, 4, 6>()];
    char char_val1[ADD_SUM];
    std::cout<<"char_val0 size:"<<sizeof(char_val0)<<std::endl;
    std::cout<<"char_val1 size:"<<sizeof(char_val1)<<std::endl;
    std::cout<<add_sum<1, 2, 3>()<<std::endl;
    
    char char_val2[int_val2];
    std::cout<<"char_val2 size:"<<sizeof(char_val2)<<std::endl;

    char char_val3[mul_all_<2, 3, 4, 5>::value];
    std::cout<<"char_val3 size:"<<sizeof(char_val3)<<std::endl;

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
    return 0;
}


