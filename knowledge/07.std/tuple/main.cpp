/*
std::tuple 是 C++ 标准库中的一个模板类，用于将多个不同类型的值组合成一个单一的对象。
它类似于结构体（struct）或类（class），但不需要定义成员变量和成员函数。
std::tuple 可以包含任意数量和类型的元素，并且可以通过索引或类型来访问这些元素。


*/
#include <tuple>
#include <iostream>
#include <algorithm>
#include <string>
#include <unistd.h>
#include <vector>

using std::cout;
using std::endl;

auto getValues() -> std::tuple<int, double> {
    return std::make_tuple(42, 3.14);
}

int main(int argc, char* argv[])
{
    // forward_as_tuple
    auto usr_tp = std::forward_as_tuple("hello", 24, 'a');
    cout<<"forward_as_tuple;"<<std::get<0>(usr_tp)<<" "<<std::get<1>(usr_tp)<<" ";
    cout<<std::get<2>(usr_tp)<<endl;

    std::get<1>(usr_tp) = 100; 
    cout<<"usr_tp[1]:"<<std::get<1>(usr_tp)<<endl;

    // size
    cout<<"size:"<<std::tuple_size<decltype(usr_tp)>::value<<endl;

    // make_tuple
    std::tuple<std::string, char, int>  maketuple = std::make_tuple("hello", 'm', 24);
    cout<<"make tuple:"<<std::get<0>(maketuple)<<" "<<std::get<1>(maketuple)<<" ";
    cout<<std::get<2>(maketuple)<<endl;

    // tie
    std::string tiestr;
    char tiechar;
    int tieint;
    std::tie(tiestr, tiechar, tieint) = maketuple;
    cout<<"tie:"<<tiestr<<" "<<tiechar<<" "<<tieint<<endl;

    // tuple_cat
    std::pair<int,char> mypair (10,'a');
    auto cattuple = std::tuple_cat(maketuple, std::tuple<int, char>(mypair));
    cout<<"tuple_cat:"<<std::get<0>(cattuple)<<" "<<std::get<1>(cattuple)<<" ";
    cout<<std::get<2>(cattuple)<<" "<<std::get<3>(cattuple)<<" ";
    cout<<std::get<4>(cattuple)<<endl;

    // 比较tuple
    std::tuple<int, double, std::string> tuple1(1, 2.0, "Hello");
    std::tuple<int, double, std::string> tuple2(1, 2.0, "World");
    bool equal = (tuple1 == tuple2); // 比较两个 tuple 是否相等
    cout<<equal<<endl;

    // 获取tuple元素
    auto val = getValues();
    cout<<std::get<0>(val)<<" "<<std::get<1>(val)<<endl;

    // 结构化绑定解析tuples
    auto [a, b] = getValues();
    cout<<a<<" "<<b<<endl;
    return 0;
} 
