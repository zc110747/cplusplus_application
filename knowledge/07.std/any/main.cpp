/*
std::any 是 C++17 标准库中引入的一个类型安全的通用容器，它可以存储任意类型的值。

主要特点：
1. 类型安全：std::any 确保存储的值的类型是已知的，并且在运行时进行类型检查。
2. 动态类型：std::any 可以存储任何类型的值，包括基本类型、自定义类型、甚至是其他std::any对象。
3. 轻量级：std::any 的实现通常是基于类型擦除技术，这使得它在存储和访问值时具有较低的开销。

成员函数:
emplace: 用给定的参数构造一个新的值，并将其存储在std::any中。
has_value: 检查std::any是否包含一个值。
reset: 清除std::any中的值。
swap: 交换两个std::any对象的值。
type: 返回std::any中存储的值的类型。

any_cast: 从std::any中提取值，并将其转换为指定的类型。
*/
#include <any>
#include <string>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <complex>

using std::cout;
using std::endl;

int main(int argc, char* argv[])
{
    std::any a = "a";
    std::any b = 1;
    std::any c{std::in_place_type<std::string>, "c"};

    cout<<std::boolalpha;

    //any cast
    cout<<"a: "<<std::any_cast<const char *>(a)<<endl;
    cout<<"b: "<<std::any_cast<int>(b)<<endl;
    cout<<"c: "<<std::any_cast<std::string>(c)<<endl;

    //type
    cout<<"type: "<<a.type().name()<<endl;

    //has_value
    cout<<"has_value: "<<b.has_value()<<endl;

     //reset   
    b.reset();
    cout<<"reset: "<<b.has_value()<<endl;

    //operator =
    std::any a1;
    a1 = a;
    cout<<std::any_cast<const char *>(a1)<<endl;
    a1 = c;
    cout<<std::any_cast<std::string>(c)<<endl;

    //swap
    std::any a2;
    a2.swap(a1);
    cout<<std::any_cast<std::string>(a2)<<endl; 

    std::any c2{std::vector{1, 2, 3}};
    auto v2 = std::any_cast<std::vector<int>>(c2);
    for (auto val:v2) {
        cout<<val<<" ";
    }
    cout<<endl;
    auto pv2 = std::any_cast<std::vector<int>>(&c2);
    if (pv2 != nullptr) {
        for (auto val:*pv2) {
            cout<<val<<" ";
        }
        cout<<endl;
    }
    
    //make any
    auto c1 = std::make_any<std::complex<double>>(0.1, 0.2);
    cout<<std::any_cast<std::complex<double>>(c1)<<endl;

    //emplace
    c1.emplace<std::complex<double>>(0.1, 0.3);
    cout<<std::any_cast<std::complex<double>>(c1)<<endl;

    //bad any_cast
    try
    {
        /* code */
        std::any errval = 1;
        std::cout<<std::any_cast<std::string>(errval)<<endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
} 
