# 现代C++功能扩展

- [attribute](#attribute)
- [auto](#auto)
  - [decltype](#decltype)
- [constexpr](#constexpr)
- [extend](#extend)
- [lambda](#lambda)
- [literal](#literal)
- [static_assert](#static_assert)

## attribute

一直以来，微软和GCC都支持一类特殊的属性语法，用于扩展语言的功能。指定的语法格式如下所示。

```cpp
// GCC支持的属性语法
// 指定变量的属性
__attribute__((attribute_list))

// 指定数据的对齐机制
int __attribute__((aligned(16))) arr[10];
struct __attribute__((aligned(16))) MyClass {
    int data[10];
};

// MSVC支持的属性语法
__declspec(attribute_list)

// 指定8字节对齐
__declspec(align(16)) int arr[10];
```

C++11 引入了属性语法，用于扩展语言的功能。属性语法的格式如下所示。

1. [[noreturn]]: 表示函数不会返回，通常用于标记函数不会正常返回
2. [[carries_dependency]]: 用于在多线程编程中优化内存访问顺序
3. [[deprecated]]: 显示的表示一个函数或变量已弃用，通常用于标记未来会被移除的功能
4. [[fallthrough]]: 表示在switch语句中，当一个case语句执行完后，会继续执行下一个case语句，而不是跳出switch语句
5. [[nodiscard]]: 表示函数的返回值可能会被忽略
6. [[maybe_unused]]: 表示变量可能会被忽略
7. [[likely]]: 表示某个条件很可能为真
8. [[unlikely]]: 表示某个条件很可能为假
9. [[no_unique_address]]: 表示变量的地址可能会被重复使用

## auto

auto关键字用于自动类型推导，它可以根据变量的初始化表达式来推断变量的类型。auto通常用于简化复杂类型的声明，特别是在使用模板或迭代器时。

auto的推导具有如下特点。

- 当一个auto关键字声明多个变量时，编译器遵循从左到右的顺序推导，以最左边的表达式推导auto具体类型。另外推导类型需要相同，否则会导致编译错误。
- 使用条件表达式初始化auto声明的变量时，编译器总是使用类型更宽的类型推导，如能够同时推导double或者int，那么就会推导为double类型。

对于auto来说也有些限制，主要如下所示。

- 对于结构体来说，非静态成员不能用auto推导；对于静态成员，C++17后支持非const变量用auto推导。
- auto不能用作模板参数，如vector<auto> demo{1, 2, 3, 4}。不过C++17后可以作为非类型模板形参的占位符，配合decltype(auto)可支持cv推导，另外推导出的类型必须为允许的模板参数类型。
- auto推导忽略引用、cv限定符(const和volatile)，如果有需要可以额外添加。
- auto和&&结合使用时，对于左值会将其推导为引用类型。
- auto推导数组或者函数时，会被推导成对应的指针类型。
- auto关键字配合初始化列表时(C++17后支持)，直接列表初始化(只能为一个元素)，推导类型为对应内部元素类型；等号列表初始化，推导类型std::initializer_list<T>类型。列表内需要为单一元素类型。

auto的另一个用处是实现返回类型后置和返回类型推导。通过使用auto作为占位符，可以声明返回类型。

具体示例如下所示。

```cpp
#include <iostream>
#include <vector>
#include <type_traits>

// 返回类型后置
auto add(auto a, auto b) -> decltype(a + b) {
    return a + b;
}

// 返回类型推导
auto mul(auto a, auto b){
    return a * b;
}

auto get_limit(int val) {
    if (val <= 0) {
        return 0;
    } else {
        return val;
    }
}

// C++14支持, 非类型模板参数支持使用auto作为占位符
template <auto N>
class Limit {
public:
    int arr[N];

    int size(void) {
        return N;
    }
};

template <decltype(auto) N>
void func(void) 
{
    std::cout << "N:" << N << std::endl;
    std::cout << "is_reference_v:" << std::is_reference_v<decltype(N)> << std::endl;
}

int main(int argc, const char *argv[]) 
{
    auto val0 = 1;                          //int
    auto val1{1};                           //int
    auto val2 = std::vector<int>{1, 2, 3};  //std::vector<int>
    auto val3 = val2.begin();               //std::vector<int>::iterator
    auto val4 = std::move(val2);            //std::vector<int>
    auto val5 = {1, 2, 3};                  //std::initializer_list<int>
    auto val6 = []() -> int { return 1; };  //lambda

    std::cout << " val0 type:" << typeid(val0).name() << std::endl;
    std::cout << " val1 type:" << typeid(val1).name() << std::endl;
    std::cout << " val2 size:" << val2.size() << std::endl;
    std::cout << " val3 type:" << typeid(val3).name() << std::endl;
    std::cout << " val4 size:" << val4.size() << std::endl;
    std::cout << " val5 size:" << val5.size() << std::endl;
    std::cout << val6() << std::endl;

    std::cout << add(1, 2) << std::endl;
    std::cout << mul(1, 2) << std::endl;
    std::cout << get_limit(-1) << std::endl;

    Limit<5> limit;
    std::cout << " limit size:" << limit.size() << std::endl;

    static const int NUM = 5;
    constexpr int NUM1 = 1;

    func<5>();      // const int
    func<NUM>();    // const int
    func<(NUM)>();  // const int&
    func<NUM1>();   // const int
    //func<(NUM1)>(); // 不支持, NUM1没有地址
}
```

### decltype

decltype关键字用于获取表达式的类型，而不是变量的类型。它可以用于声明函数返回值的类型，或者在模板中推导类型参数的类型。

对于decltype(e)来获取类型时，编译器将依序判断以下规则，这里假设e的类型为T。

1. 如果e时一个没有带括号的标记符表达式或者类成员访问表达式，那么decltype就是e所命名的实体的类型，另外，如果e是一个被重载的函数，则会导致编译时错误。
2. 如果e是一个类型为T的将亡值(xvalue), 那么类型为T&&。
3. 如果e是一个类型为T的左值，类型为T&。
4. 如果e是函数、仿函数调用，类型为调用后返回值类型。
5. 其余情况decltype(e)，类型为T。

当然decltype还有其它特性，如下所示。

1. decltype的推导会同步cv限定符，如果未加括号时，不会同步其父对象的cv限定符。
2. 基于auto和decltype的类型推导，可以实现函数返回值类型后置和自动推导函数。
3. decltype(auto)是C++14引入的类型说明符，结合了decltype和auto的特性，用于类型推导。auto推导时会忽略引用和cv限定符(const 和 volatile），而decltype能精确地返回表达式的类型，decltype(auto)综合二者，按decltype的规则进行类型推导。

具体示例如下所示。

```cpp
#include <iostream>
#include <typeinfo>
#include <type_traits>

// 函数返回类型后置语法
auto add(int a, int b) -> decltype(a + b) {
    return a + b;
}

// 函数自动推导
// 使用decltype，函数返回值类型后置推导
template <typename R1, typename R2>
auto mul(R1 a, R2 b) -> decltype(a * b) {
    return a * b;
}

// c++14，auto占位符推导
auto mul1(auto a, auto b) {
    return a + b;
}

struct Base {
    int val{1};
};

int main(int argc, const char *argv[]) 
{
    int a1 = 1;
    int &a2 = a1;
    int &&a3 = std::move(a1);

    decltype(a1) a4 = 1;                // int
    decltype(a2) a5 = a1;               // int&
    decltype(a3) a6 = std::move(a1);    // int&&

    const int a7 = 1;
    decltype(a7) a8 = 1;                // const int

    volatile int a9 = 1;
    decltype(a9) a10 = 1;                // volatile int

    std::cout << std::boolalpha;
    std::cout << "a1: " << std::is_reference<decltype(a1)>::value << std::endl; 
    std::cout << "a2: " << std::is_lvalue_reference<decltype(a2)>::value << std::endl;
    std::cout << "a3: " << std::is_rvalue_reference<decltype(a3)>::value << std::endl;

    std::cout << "a4: " << std::is_reference<decltype(a4)>::value << std::endl;
    std::cout << "a5: " << std::is_lvalue_reference<decltype(a5)>::value << std::endl;
    std::cout << "a6: " << std::is_rvalue_reference<decltype(a6)>::value << std::endl;

    std::cout << "a7: " << std::is_const<decltype(a7)>::value << std::endl;
    std::cout << "a8: " << std::is_const<decltype(a8)>::value << std::endl;

    std::cout << "a9: " << std::is_volatile<decltype(a9)>::value << std::endl;
    std::cout << "a10: " << std::is_volatile<decltype(a10)>::value << std::endl;

    auto val0 = mul(1, 2);
    std::cout << "val0: " << val0 << std::endl;

    auto val1 = mul1(1, 1.2);
    std::cout << "val1: " << val1 << std::endl;

    auto val2 = add(1, 2);
    std::cout << "val2: " << val2 << std::endl;

    decltype(auto) a11 = a10;
    std::cout << "a11: " << std::is_volatile<decltype(a11)>::value << std::endl;

    const Base a12;
    decltype(a12) a13 = a12;              // const Base
    decltype(a12.val) a14 = a12.val;      // double
    decltype((a12.val)) a15 = a12.val;    // const double& 

    a15 = 1;
    std::cout << "a12: " << std::is_const<decltype(a12)>::value << std::endl;
    std::cout << "a13: " << std::is_const<decltype(a13)>::value << std::endl;
    std::cout << "a14: " << std::is_const<decltype(a14)>::value << std::endl;;

    const Base* a16 = &a12;
    decltype(a16->val) a17 = a16->val;      // double
    decltype((a16->val)) a18 = a16->val;    // const double&
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

在C++中，lambda表达式是一种匿名函数，它允许你在需要函数对象的地方定义一个临时的函数。lambda表达式通常用于简化代码，特别是在需要传递函数作为参数的情况下。

```cpp
[capture list](parameters) -> return_type {
    // 函数体
} 

capture list: 捕获列表，用于捕获外部变量
    [] 无捕获的lambda表达式
    [=] 值捕获，lambda表达式通过值捕获所有外部变量
    [&] 通过引用捕获所有外部变量的lambda表达式
    [var] 只捕获变量var
    [&var] 只捕获变量var的引用
    [this] 捕获当前类中的this指针，按引用捕获
    [*this] 捕获当前类中的this指针，按值捕获，值捕获默认定义为const(C++17)
    [=, &var] 捕获所有外部变量的lambda表达式
    [&, var] 捕获所有外部变量的lambda表达式，但是var是通过值捕获的
```

可以使用std::function对象来存储lambda表达式，另外从原理上，lambda基于函数对象(仿函数)来实现。

泛型lambda表达式是C++14引入的特性，它允许在lambda表达式中使用auto占位符来表示参数类型，使得lambda可以接受不同类型的参数，实现类似函数模板的泛型功能。

Lambda捕获初始化器是 C++14 引入的特性，它允许你在lambda捕获列表中对捕获的变量进行自定义初始化，还能创建新的局部变量供lambda函数体使用，即使这些变量在 lambda 外部并不存在。

本节例程如下所示。

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

class lambda_class
{
public:
    lambda_class(int value) : m_value(value) {}

    int test(int a) 
    {
        auto v1 = [](){
            std::cout << "no capture" << std::endl;
        };
        auto v2 = [=, this](){
            std::cout << "copy m_value: " << m_value << std::endl;
        };
        auto v3 = [&](){
            std::cout << "reference m_value: " << m_value << std::endl;
        };
        auto v4 = [this](){
            std::cout << "reference this->m_value: " << m_value << std::endl;
        };
        auto v5 = [*this](){
            std::cout << "copy this->m_value: " << m_value << std::endl;
        };
        auto v6 = [this, a](){
            std::cout << "copy this->m_value: " << m_value + a << std::endl;
        };

        v1();
        v2();
        v3();
        v4();
        v5();
        v6();

        return 0;
    }
private:
    int m_value{0};
};

int main(int argc, char const *argv[]) 
{
    std::vector<int> vec = {1, 2, 3, 4, 5};

    // 使用lambda表达式对vec中的元素进行排序
    std::sort(vec.begin(), vec.end(), [](int a, int b) {
        return a > b;
    });
    for (const int& i : vec) {
        std::cout << i << std::endl;
    }

    // 在类中的capture捕获
    lambda_class lc(10);
    lc.test(1);

    std::vector<int> v1 = {1, 2, 3, 4, 5};

    // 泛型lambda函数
    auto lambdafunc = [&]<typename T>(T&& data) {
        std::cout << "泛型lambda函数: ";
        for (auto& x : data) {
            std::cout << x << " ";
        }
        std::cout << std::endl;
    };
    lambdafunc(v1);

    // 泛型lambda(auto)
    auto func = [](auto a, auto b) -> decltype(a+b) {
        std::cout<<"泛型lambda: ";
        return a+b;
    };
    std::cout<<func(2, 4)<<std::endl;

    // lambda捕获初始化器
    auto func1 = [moveVec = std::move(vec)](){
        std::cout<<"lambda捕获初始化器: ";
        for (auto x : moveVec) {
            std::cout << x << " ";
        }
        std::cout << std::endl;
    };
    func1();
    return 0;
}
```

## literal

1. 原生字符串字面量

原生字符串字面量（Raw String Literals）是C++11引入的一个特性，它允许你在字符串中包含特殊字符（如反斜杠和引号）而不需要进行转义。原生字符串字面量以 R"( 开始，以 )" 结束。在这两个符号之间的所有字符都被视为字符串的一部分，包括换行符和引号。

举例说明如下。

```cpp
#include <iostream>
#include <string>

int main(int argc, char const *argv[])
{
    const char* str1 = R"(Hello, world!)";
    std::cout << str1 << std::endl; 

    const char str2[] = R"(Hello,
        world!
    )";
    std::cout << str2 << std::endl;
    return 0;
}
```

2. 用户自定义字面量

用户可以自定义字面量（User-Defined Literals），这允许你为基本类型（如整数、浮点数、字符串等）定义新的后缀，从而创建自定义的字面量。
对于字符串字面量，你可以定义一个自定义后缀来创建一个特定类型的对象。

```cpp
#include <iostream>
#include <string>

class MyString: public std::string
{
public:
    using std::string::string;
    void print()
    {
        std::cout << *this << std::endl;
    }
};

MyString operator""_my(const char* str, size_t len)
{
    return MyString(str, len);
}

template<char ...c>
    MyString operator"" _my() {
        MyString str("");

        //折叠表达式
        using unused = int[];
        unused{(str.push_back(c), 0)...};
        return str;
    }

int main(int argc, char const *argv[])
{
    "Hello, world!"_my.print();

    auto val = 123_my;
    val.print();
    return 0;
}
```

3. 单引号作为整数分隔符

在C++14及以后的版本中，单引号（'）可以用作整数和浮点数字面量的分隔符，以提高代码的可读性。这些分隔符不会影响数值本身，它们只是为了让数字更易于阅读。

```cpp
#include <iostream>

int main(int argc, char const *argv[])
{
    // 单引号作为整数分隔符
    int billion = 1'000'000'000;
    int largeNumber = 123'456'789;

    double pi = 3.141'592'653;

    std::cout << "Billion: " << billion << std::endl;
    std::cout << "Large number: " << largeNumber << std::endl;
    std::cout << "Pi: " << pi << std::endl;
}
```

## static_assert

静态断言（static_assert）是C++11引入的一个编译时断言机制，用于在编译阶段检查某个条件是否为真。如果条件为假，编译器会产生一个错误信息，并且编译过程会失败。静
态断言通常用于在编译时验证模板参数、常量表达式或其他编译时可确定的条件。

静态断言的语法如下：

```cpp
static_assert(条件, 错误信息);
```

举例说明如下。

```cpp
#include <iostream>
#include <type_traits>

template <typename T>
void check_size() {
    static_assert(sizeof(T) <= 4, "Type size exceeds 4 bytes");
    std::cout << "Type size is within the limit." << std::endl;
}

int main(int argc, char const *argv[])
{
    static_assert(sizeof(int) == 4, "int must be 4 bytes");
    check_size<int>();    // 编译通过，int类型大小通常为4字节
    // check_size<double>();  // 编译失败，double类型大小通常为8字节
    check_size<char>();
    check_size<float>();
    
    return 0;
}
```
