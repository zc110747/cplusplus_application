/*
1. auto类型推导占位符
auto关键字用于自动类型推导，它可以根据变量的初始化表达式来推断变量的类型。auto通常用于简化复杂类型的声明，特别是在使用模板或迭代器时
不过对于auto来说也有些限制:
- 不能作函数的形参推导 void fun(auto i = 1){}
- 对于结构体来说，非静态成员不能用auto. struct str{auto var = 10;}
- 不能声明auto数组 auto d[3] = {1, 2, 3}; C++17后支持
- auto不能用作模板参数 vector<auto> demo{1, 2, 3, 4};

函数返回类型后置语法
在C++11之前，函数的返回类型必须在函数名之前声明。C++11引入了函数返回类型后置语法，允许在函数参数列表之后声明返回类型。
这种语法通常与auto和decltype结合使用，特别是在模板编程中

返回类型推导，允许编译器自动推导函数的返回类型，简化代码编写

2. decltype类型推导表达式
decltype关键字用于获取表达式的类型，而不是变量的类型。它可以用于声明函数返回值的类型，或者在模板中推导类型参数的类型
对于decltype(e)来获取类型时，编译器将依序判断以下4个规则：
- 如果e时一个没有带括号的标记符表达式或者类成员访问表达式，那么decltype就是e所命名的实体的类型，另外，如果e是一个被重载的函数，则会导致编译时错误。
- 否则，假如e的类型是T，如果e是一个将亡值(xvalue), 那么decltype(e)为T&&
- 否则，假如e的类型是T，如果e是一个左值，则decltype(e)为T&
- 否则， 假如e的类型是T，则decltype(e)为T

decltype(auto)表达式
decltype(auto)是C++14引入的类型说明符，结合了decltype和auto的特性，用于类型推导。auto推导时会忽略引用和cv限定符(const 和 volatile），而decltype能精确地返回表达式的类型，decltype(auto)综合二者，按decltype的规则进行类型推导。

3. declval 是 C++11 引入的一个工具函数，定义在 <utility> 头文件中。
它的主要作用是在不创建对象实例的情况下获取类型的右值引用。
这在模板元编程中非常有用，特别是在需要处理类型的默认构造函数不可用或不希望创建对象实例的情况下。

1. 将任意一个类型转换成右值引用类型
2. 不必经过构造函数就能使用该类型的成员函数
*/
#include <iostream>
#include <vector>
#include <utility>
#include <type_traits>
 
template<typename T>
struct is_volatile : std::false_type {};
 
template<typename T>
struct is_volatile<volatile T> : std::true_type {};

// 返回类型后置
auto add(int a, int b) -> int {
    return a + b;
}

// 返回类型推导
auto add2(auto a, auto b) {
    return a+b;
}

// 使用decltype推导返回类型
template <typename T, typename U>
auto multiply(T t, U u) -> decltype(t * u) {
    return t * u;
}

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
    // auto类型推导占位符
    auto x = 42; // x的类型被推导为int
    auto y = 3.14; // y的类型被推导为double
    auto z = "hello"; // z的类型被推导为const char*

    std::cout << "================ auto ================" << std::endl;
    std::cout << "x: " << x << std::endl;
    std::cout << "y: " << y << std::endl;
    std::cout << "z: " << z << std::endl;

    auto x1{1};
    std::cout << typeid(x1).name() << std::endl;        //int
    auto x2 = {1, 2, 3, 4, 5};
    std::cout << typeid(x2).name() << std::endl;        //std::initializer_list<int>

    std::vector<int> vec = {1, 2, 3, 4, 5};

    for (auto it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    auto sum_val = add(1, 2); // sum_val的类型被推导为int
    std::cout << "sum_val: " << sum_val << std::endl;

    sum_val = add2(2, 3);
    std::cout << "sum_val: " << sum_val << std::endl;

    // decltype类型推导表达式
    std::cout << "================ decltype ================" << std::endl;;
    {
        int a = 10;
        double b = 3.14;
        decltype(a + b) c = a + b; // c的类型被推导为double
        std::cout << "c: " << c << std::endl;

        auto product = multiply(3, 4.5); // product的类型被推导为double
        std::cout << "product: " << product << std::endl;

        const volatile int val = 5;
        auto val1 = val;
        decltype(auto) val2 = val;

        std::cout << "is_const val:" << std::is_const<decltype(val)>::value<< std::endl;
        std::cout << "is_const val1:" << std::is_const<decltype(val1)>::value<< std::endl;
        std::cout << "is_const val2:" << std::is_const<decltype(val2)>::value<< std::endl;
        std::cout << "is_volatile val:" << is_volatile<decltype(val)>::value<< std::endl;
        std::cout << "is_volatile val1:" << is_volatile<decltype(val1)>::value<< std::endl;
    }

    // declval获取右值引用
    std::cout << "================ declval ================" << std::endl;;
    {
        decltype(std::declval<A<int>>().t()) a{1}; // int a;
        decltype(std::declval<Base<int>>().t()) b{2}; // int b;
        std::cout <<  a << ',' << b << '\n';

        decltype(std::declval<B>().t()) c = 3; // = int c;
        std::cout << c << '\n';

        std::cout << get_value<NonDefaultConstructible>(1) << std::endl;
    }

    return 0;
}
