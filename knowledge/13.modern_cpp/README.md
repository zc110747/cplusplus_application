# 现代C++功能扩展

- [attribute](#attribute)
- [auto](#auto)
  - [decltype](#decltype)
  - [declval](#declval)
- [constexpr](#constexpr)
- [extend](#extend)
- [lambda](#lambda)
- [literal](#literal)
- [static_assert](#static_assert)

## attribute

## auto

auto关键字用于自动类型推导，它可以根据变量的初始化表达式来推断变量的类型。auto通常用于简化复杂类型的声明，特别是在使用模板或迭代器时。

对于auto来说也有些限制，主要如下所示。

1. 对于结构体来说，非静态成员不能用auto。
2. auto不能用作模板参数 vector<auto> demo{1, 2, 3, 4}。
3. auto推导忽略引用和cv限定符(const和volatile)，需要自行添加。

具体示例如下所示。

```cpp
auto val0 = 1;                          //int
auto val1 = {1, 2, 3};                  //std::initializer_list<int>
auto val2 = std::vector<int>{1, 2, 3};  //std::vector<int>
auto val3 = val2.begin();               //std::vector<int>::iterator
auto val4 = std::move(val2);            //std::vector<int>
```

### decltype

decltype关键字用于获取表达式的类型，而不是变量的类型。它可以用于声明函数返回值的类型，或者在模板中推导类型参数的类型。

对于decltype(e)来获取类型时，编译器将依序判断以下4个规则，这里假设e的类型为T。

1. 如果e时一个没有带括号的标记符表达式或者类成员访问表达式，那么decltype就是e所命名的实体的类型，另外，如果e是一个被重载的函数，则会导致编译时错误。
2. 如果e是一个将亡值(xvalue), 那么decltype(e)为T&&
3. 如果e是一个左值，则decltype(e)为T&
4. 其余情况decltype(e)为T

```cpp
int a1 = 1;
int &a2 = a1;
int &&a3 = std::move(a1);

decltype(a1) a4 = 1;                //int
decltype(a2) a5 = a1;               //int&
decltype(a3) a6 = std::move(a1);    //int&&
decltype(auto) a7 = std::move(a1);  //int&&
```

基于auto和decltype的类型推导，可以实现函数返回值类型后置和自动推导函数。

```cpp
// 函数返回类型后置语法
auto func1(int a, int b) -> decltype(a + b) {
    return a + b;
}

// 函数自动推导
auto func2(auto a, auto b) {
    return a + b;
}
```

decltype(auto)是C++14引入的类型说明符，结合了decltype和auto的特性，用于类型推导。auto推导时会忽略引用和cv限定符(const 和 volatile），而decltype能精确地返回表达式的类型，decltype(auto)综合二者，按decltype的规则进行类型推导。

```cpp
int a1 = 1;
int &a2 = a1;
int &&a3 = std::move(a1);
decltype(auto) a4 = a2;                 //int&
decltype(auto) a5 = std::move(a1);      //int&&
```

### declval

declval 是 C++11 引入的一个工具函数，定义在 <utility> 头文件中。它的主要作用是在不创建对象实例的情况下返回T&&类型。这在模板元编程中非常有用，特别是在需要处理类型的默认构造函数不可用或不希望创建对象实例的情况下。对于declval来说，有如下两个作用。

1. 不必经过构造函数就能使用该类型的成员函数。
2. 返回T&&类型，用于实现完美转发。

具体示例如下所示。

```cpp
#include <utility>
#include <type_traits>
#include <iostream>

template <typename T>
class A {
public:
    T t;
    T func() {
        return t;
    }
};

int main(int argc, char const *argv[]) 
{
    int b = 1;
    decltype(std::declval<A<int>>().func()) a = b;
    std::cout << typeid(a).name() << std::endl;             //int
    std::cout << std::is_rvalue_reference<decltype(a)>::value << std::endl;  // 0

    return 0;
}
```

## constexpr

constexpr是C++11 引入的一个关键字，用于声明常量表达式。常量表达式是指在编译时就能计算出结果的表达式。使用constexpr关键字可以让编译器在编译时就对表达式进行求值，从而提高程序的性能。一个函数要想成为常量表达式函数（constexpr)，必须满足如下4个条件。

1. 该函数必须有返回值，即函数的返回值类型不能是void。
2. 整个函数的函数体中，除了可以包含using指令、typedef语句以及static_assert断言外，只能包含一条return返回语句。
3. return返回的表达式必须是常量表达式。
4. 函数在使用之前，必须有对应的定义语句。

constexpr和const 的区别

1. const可以用于声明运行时和编译时的常量，而constexpr 只能用于声明编译时的常量。
2. const变量的值可以在运行时改变，而constexpr变量的值必须在编译时确定。
4. constexpr函数必须满足一定的条件才能被编译器识别为常量表达式函数，而const函数没有这样的限制。

在后续版本，放开了对constexpr函数的限制，主要如下所示。

1. C++14改进：允许局部变量和赋值语句、支持if和switch条件语句、支持修改对象的非const成员。
2. C++17改进：支持constexpr的编译时lambda表达式、增加if constexpr条件语句。

示例如下所示。

```cpp
#include <iostream>
#include <type_traits>

constexpr int func1(int a) {
    return a;
}

constexpr int func2(int a)
{
    if (a > 0) {
        return 2;
    } else {
        return 1;
    }
}

int main(int argc, char const *argv[]) 
{ 
    constexpr int a = 1;
    std::cout << "a: " << a << std::endl;

    int arr1[func1(1)];
    std::cout << "arr size: " << sizeof(arr1)/sizeof(int) << std::endl;

    constexpr auto add = [](int a, int b) constexpr {
        return a + b;
    };
    int arr2[add(1, 2)];
    std::cout << "arr size: " << sizeof(arr2)/sizeof(int) << std::endl;

    int arr3[func2(1)];
    std::cout << "arr size: " << sizeof(arr3)/sizeof(int) << std::endl;
    return 0;
}
```

## extend

扩展的C++语法糖，包含if/switch语句带条件初始化、nullptr、结构化绑定、扩展的for表达式等。

### if_switch_extend

在C++17中，if和switch语句支持在条件判断之前进行初始化。这是通过在if或switch关键字后面直接添加一个初始化语句来实现的。

```cpp
#include <iostream>

int main(int argc, char const *argv[]) 
{
    int b = 1;
    
    // if语句带条件初始化
    if (int a = b; a > 0) {
        std::cout << "a is greater than 0" << std::endl;
    }

    // switch语句带条件初始化
    switch (int a = b; a) {
        case 1:
            std::cout << "a is 1" << std::endl;
            break;
        default:
            std::cout << "a is not 1" << std::endl;
            break;
    }
    return 0;
}
```

### nullptr

在C++中，nullptr是一个关键字，用于表示空指针。它是在C++11标准中引入的，用于替代 NULL 宏，以提供更安全和类型安全的空指针表示。

nullptr的类型是std::nullptr_t，它是一个空指针类型，用于表示空指针。可以用nullptr_t来声明空指针变量。

```cpp
#include <iostream>

int main(int argc, char const *argv[]) 
{
    int *a = nullptr;
    if (a == nullptr) {
        std::cout << "a is nullptr" << std::endl;
    }

    decltype(nullptr) b;
    if (b == nullptr) {
        std::cout << "b is nullptr" << std::endl;
    }

    std::nullptr_t c;
    if (c == nullptr) {
        std::cout << "c is nullptr" << std::endl;
    }
    return 0;
}
```

### structured_binding

在C++17中，引入了结构化绑定（Structured Binding）的概念。它允许你将一个复杂的表达式分解成多个独立的变量，从而更方便地访问和操作表达式的各个部分。

支持结构化绑定的类型。

1. 结构体、位域格式的结构体和仅有公有成员变量的类
2. 原生数组
3. std::pair
4. std::tuple
5. std::array

```cpp
#include <iostream>
#include <tuple>
#include <array>

struct point {
    int x;
    int y;
};

class my_class {
public:
    my_class(int x, int y) : x(x), y(y) {}
    int x;
    int y;
};

union my_union {
    int x;

    struct {
        int x1:4;
        int y1:4;
        int z1:8;
    } p;
};

int main(int argc, char const *argv[]) 
{
    int arr1[3] = {1, 2, 3};

    // 原生数组支持结构化绑定
    auto [x1, y1, z1] = arr1;
    std::cout << "x1: " << x1 << ", y1: " << y1 << ", z1: " << z1 << std::endl;

    // std::tuple支持结构化绑定
    std::tuple<int, int, int> t1 = std::make_tuple(1, 2, 3);
    auto [x2, y2, z2] = t1;
    std::cout << "x2: " << x2 << ", y2: " << y2 << ", z2: " << z2 << std::endl;

    // std::array支持结构化绑定
    std::array<int, 3> arr2 = {4, 5, 6};
    auto [x3, y3, z3] = arr2;
    std::cout << "x3: " << x3 << ", y3: " << y3 << ", z3: " << z3 << std::endl;

    // std::pair支持结构化绑定
    auto [x4, y4] = std::make_pair(7, 8);
    std::cout << "x4: " << x4 << ", y4: " << y4 << std::endl;

    // 结构体支持结构化绑定
    point p1{1, 2};
    auto [x5, y5] = p1;
    std::cout << "x5: " << x5 << ", y5: " << y5 << std::endl;

    // 只有公有成员变量的类支持结构化绑定
    my_class p2{1, 2};
    auto [x6, y6] = p2;
    std::cout << "x6: " << x6 << ", y6: " << y6 << std::endl;

    // 位域支持结构化绑定
    my_union u1;
    u1.x = 0x1234;
    auto [x7, y7, z7] = u1.p;
    std::cout << "x7: " << x7 << ", y7: " << y7 << ", z7: " << z7 << std::endl;
    return 0;
}
```
### for_extend

扩展的for表达式（也称为范围-based for循环）是一种简化遍历容器（如数组、向量、列表等）的语法，其格式如下所示。

```cpp
for (declaration : range_expression) {
    // 循环体
}
```

相应代码如下所示。

## lambda

## literal

## static_assert
