/*
declval 是 C++11 引入的一个工具函数，定义在 <utility> 头文件中。
它的主要作用是在不创建对象实例的情况下获取类型的右值引用。
这在模板元编程中非常有用，特别是在需要处理类型的默认构造函数不可用或不希望创建对象实例的情况下。

1. 将任意一个类型转换成右值引用类型
2. 不必经过构造函数就能使用该类型的成员函数
*/
#include <iostream>
#include <utility>

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

int main(int argc, char* argv[]) 
{
    decltype(std::declval<A<int>>().t()) a{1}; // int a;
    decltype(std::declval<Base<int>>().t()) b{2}; // int b;
    std::cout <<  a << ',' << b << '\n';

    decltype(std::declval<B>().t()) c = 3; // = int c;
    std::cout << c << '\n';

    std::cout << get_value<NonDefaultConstructible>(1) << std::endl;
    return 0;
}