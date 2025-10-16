# template

模板，是一种泛型编程的工具，它允许程序员编写通用的代码，而不是针对特定的数据类型。

- [模板基础说明](#template_basic)
  - [模板类型](#template_type)
  - [模板参数](#parameter_type)
  - [类型别名和别名模板](#alias)
- [模板用法](#template_usage)
  - [模板类型推导](#deduction)
  - [class和typename说明](#class_typename)
  - [模板特化和偏特化](#template_specialization)
  - [模板默认值](#template_default)
  - [可变参数模板和操作符解析](#template_variable_parameter)
- [模板规则](#template_rules)
  - [模板中的auto使用](#template_auto)
  - [ADL查找规则](#adl_search)
  - [用户自定义推导](#user_guides)
  - [SFINAE](#sfinae)
  - [外部模板](#explicit_template)
  - [模板元编程](#metaprogramming)
- [返回主页](../../README.md)

## template_basic

### template_type

模板按照类型分为函数模板、类模板、类型别名和别名模板。按照主体不同，分为函数模板和类模板；具体如下所示。

- 函数模板是一种通用的函数定义，它可以用于多种不同类型的参数。
- 类模板是一种通用的类定义，它可以用于多种不同类型的成员变量和成员函数。

具体示例如下所示。

```cpp
#include <iostream>

/// 函数模板
template <typename T>
T add(T a, T b) {
    return a + b;
}

/// 类模板
template <typename T>
class demo_type {
public:
    size_t get_size() {return sizeof(T);}
};

int main(int argc, char *argv[])
{
    // 函数模板
    std::cout << "add<int>(1, 2) = " << add<int>(1, 2) << std::endl;

    // 类模板
    demo_type<int> d1;
    std::cout << "d1.get_size() = " << d1.get_size() << std::endl;

    return 0;
}
```

### parameter_type

模板按照参数分为非类型模板和类型模板。

- 类型模板是最常见的模板类型，它允许你定义一个通用的类或函数，其中某些类型可以在实例化时指定，类型模板可以使用typename或class关键字来声明。
- 非类型模板参数允许你在模板中使用非类型的值，如整数、指针、引用和枚举。其中引用作为参数需要满足以下条件：必须是const引用，引用对象必须是全局或者静态变量。

具体示例如下所示。

```cpp
#include <iostream>

/// 类型模板
template <typename T>
class demo_type {
public:
    size_t get_size() {return sizeof(T);}
};

/// 非类型模板参数(整数)
template <int N>
class demo_non_type {
public:
    demo_non_type() = default;
    int get_size() {
        return N;
    }
};

int global_val = 10;
enum class demo_enum {
    red,
    green,
    blue
};

/// 非类型模板参数(引用、指针)
template <const int& N, int *ptr, demo_enum val = demo_enum::red>
class demo_non_type_ref {
public:
    demo_non_type_ref() = default;
    int get_size() {
        return N;
    }
    int* get_ptr() {
        return ptr;
    }
};

int main(int argc, char *argv[])
{
    demo_type<int> d1;
    std::cout << "d1.get_size() = " << d1.get_size() << std::endl;
    
    demo_non_type<10> d2;
    std::cout << "d2.get_size() = " << d2.get_size() << std::endl;

    demo_non_type_ref<global_val, &global_val> d3;
    std::cout << "d3.get_size() = " << d3.get_size() << std::endl;
    std::cout << "*d3.get_ptr() = " << *d3.get_ptr() << std::endl;
    return 0;
}
```

### alias

类型别名，一种使用using声明的通用类型别名定义，它可以用于多种不同类型的别名；在此基础上，带模板的类型别名则被称为别名模板; 类型别名和别名模板的格式如下所示。

```cpp
// 类型别名的格式
// identifier: 指定的类型别名
// template identifier：别名的类型，类似std::vector<int>，std::map<int, int>等
using identifier = type-identifier;

// 别名模板的格式
// identifier: 指定的类型别名
// template identifier：别名的类型，类似std::vector<T>，std::map<int, T>等
template <typename T>
using identifier = type-identifier;
```

具体示例如下所示。

```cpp
#include <iostream>
#include <vector>
#include <map>

using vec_int = std::vector<int>;
using map_int_int = std::map<int, int>;

template <typename T>
using map_int_T = std::map<int, T>;

int main(int argc, char *argv[])
{
    vec_int v1 = {1, 2, 3};
    for (const auto &i : v1) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    map_int_int m1 = {{1, 100}, {2, 200}};
    for (const auto &[key, value] : m1) {
        std::cout << key << " " << value << " ";
    }
    std::cout << std::endl;

    map_int_T<double> m2 = {{1, 100.0}, {2, 200.0}};
    for (const auto &[key, value] : m2) {
        std::cout << key << " " << value << " ";
    }
    std::cout << std::endl;
}
```

## template_usage

### deduction

对于类型模板，在调用时类型支持直接指定和自动推断；格式分别如下所示。

```cpp
// 直接指定模板类型
demo<int> d1;

// 自动推断模板类型
demo d2{2};
```

不过以下情况时，必须显示指定的模板类型。

- 模板函数调用时，参数无法提供足够信息。
- 模板类实例化时，类型无法提供足够信息或类型推导存在歧义。
- 模板类型推导存在歧义时，无法确定具体的模板类型。

具体示例如下所示。

```cpp
#include <iostream>

// 模板函数调用时，参数无法提供足够信息
template <typename T>
T create() {
    return T();
}

// 模板类实例化时，类型无法提供足够信息
template <typename T>
class Container {
public:
    Container() {}
    
    void set_val(T val) {val_ = val;}

    T get_val() {return val_;}

private:
    T val_;
};

// 必须显式指定模板类型，无法确定T的具体类型
template <typename T, typename U>
T add(U a, U b) {
    return static_cast<T>(a + b);
}

template <typename T>
class demo {
public:
    demo(T val) : val_(val) {}

public:
    T get_val() {return val_;}

private:
    T val_;
};

int main(int argc, char *argv[]) 
{
    auto x = create<int>();
    auto y = create<double>();

    Container<int> c1;
    c1.set_val(10);
    std::cout << "c1.get_val() = " << c1.get_val() << std::endl;

    int result = add<int, double>(1.5, 2.5);
    std::cout << result << std::endl;

    
    // 通过类型推导确定T的类型，不用显示指定
    demo d3{3};
    std::cout << "d3.get_val() = " << d3.get_val() << std::endl;
    return 0;
}
```

### class_typename

class和typename都可以用来声明类型模板参数。

- 声明模板参数时，class和typename都可以使用，没有区别。
- 在模板内部，如果一个名称是依赖于模板参数的，并且它是一个类型；那么在使用这个名称时，必须使用typename来告诉编译器这是一个类型。
- 声明模板模板参数，必须使用class, 不能使用typename。这里模板模板参数是一种特殊的模板参数，它允许你在模板中使用另一个模板类型作为参数，可以将模板的行为委托给另一个模板。

具体示例如下所示。

```cpp
#include <iostream>
#include <vector>

template <class T>
class demo_1 {
public:
    // 标识依赖名称为类型
    typename T::value_type val;  
};

// 模板模板参数Container，必须使用class, 不能使用typename
template <template <typename> class Container>
class  demo_2{
public:
    Container<int> container;
};

int main(int argc, char *argv[])
{
    demo_1<std::vector<int>> d1;
    demo_2<std::vector> d2;

    auto val_1 = d1.val;
    auto val_2 = d2.container;

    std::cout << typeid(val_1).name() << std::endl;     // 类型int
    std::cout << typeid(val_2).name() << std::endl;     // 类型std::vector<int>
    return 0;
}
```

### template_specialization

在 C++ 里，模板的特化和偏特化能让你为特定类型或特定值提供模板的特殊实现，以此增强代码的灵活性和适应性。

- 模板特化是指为特定类型或特定值提供模板的特殊实现；当使用特化类型实例化模板时，编译器会优先使用特化版本而非通用模板。
- 模板偏特化是指为特定类型或特定值提供模板的部分特殊实现，对于模板偏特化来说，仍然保留部分模板参数。

模板特化和偏特化的最大差别就是特化模板完全实例化，不存在模板类型；而偏特化模板仍然保留部分模板参数。另外按照匹配优先级来说，模板特化 > 偏特化 > 通用模板，即如果匹配特化模板，那么就会优先使用特化模板；如果匹配特化模板，那么就会匹配偏特化模板；如果不存在偏特化模板，才去匹配通用模板；如果通用模板也不匹配，才进行报错，这也是模板匹配的SFINAE机制之一，会在后面进行说明。

具体示例如下所示。

```cpp
#include <iostream>
#include <vector>

// 通用模板
template <typename T>
class demo {
public:
    // 通用实现
    demo() {
        std::cout << "demo" << std::endl;
    }
};

// 特化模板
template <>
class demo<int> {
public:
    // 特化实现
    demo() {
        std::cout << "demo<int>" << std::endl;
    }
};

// 偏特化模板
template <typename T>
class demo<T* > {
public:
    // 偏特化实现
    demo() {
        std::cout << "demo<T*>" << std::endl;
    }
};

// 特化模板
template <>
class demo<std::vector<int>> {
public:
    // 偏特化实现
    demo() {
        std::cout << "demo<std::vector<int>>" << std::endl;
    }
};

// 偏特化模板
template <typename T>
class demo<std::vector<T>> {
public:
    // 偏特化实现
    demo() {
        std::cout << "demo<std::vector<T>>" << std::endl;
    }
};

int main(int argc, char *argv[])
{
    demo<int> t1;                   // 调用特化模板，输出demo<int>
    demo<double> t2;                // 调用通用模板，输出demo
    demo<int*> t3;                  // 调用偏特化模板，输出demo<T*>
    demo<std::vector<int>> t4;      // 调用特化模板，输出demo<std::vector<int>>
    demo<std::vector<double>> t5;   // 调用偏特化模板，输出demo<std::vector<T>>

    return 0;
}
```

### template_default

默认模板参数主要是指在模板定义中为模板参数提供一个默认值，当调用模板时如果没有指定模板参数，那么就会使用默认值。在使用默认模板参数时，需要注意以下几点：

- 默认模板参数只能在模板定义中指定，不能在模板调用中指定。
- 默认模板参数只能指定从右向左的默认值，中间不能够跳过，否则会导致编译错误。
- 默认模板参数只能在模板定义中指定一次，不能在模板调用中指定多次。

具体示例如下所示。

```cpp
#include <iostream>

/// 默认模板参数
template <typename T = int>
class demo
{
public:
    demo() {
        std::cout << "demo" << std::endl;
    }
    ~demo() {
        std::cout << "~demo" << std::endl;
    }

    T get_val() {return val_;}
    
    void set_val(T val) {val_ = val;}

private:
    T val_;
};

template <typename T, int N1 = 1, int N2 = 3>
class demo_2
{
public:
    using value_type = T;

    int val1{N1};

    int val2{N2};
};

int main(int argc, char *argv[]) 
{
    demo d1;
    std::cout << d1.get_val() << std::endl;
    d1.set_val(1); 

    demo_2<int> d2;
    demo_2<double, 2> d3;
    demo_2<float, 4> d4;

    std::cout << d2.val1 << std::endl;
    std::cout << d2.val2 << std::endl;
    std::cout << d3.val1 << std::endl;
    std::cout << d3.val2 << std::endl;
    std::cout << d4.val1 << std::endl;
    std::cout << d4.val2 << std::endl;
}
```

### template_variable_parameter

可变参数模板是一种特殊的模板，它允许你在模板中使用可变数量的参数。

1. 可变参数模板是一种特殊的模板，它允许你在模板中使用可变数量的参数。
2. 可变参数模板的语法是在模板参数列表中使用省略号(...)来表示可变数量的参数。
3. 可变参数模板的参数可以是任意类型，包括基本类型、类类型、指针类型等。

对于可变参数模板，最重要的时如何展开参数包，展开参数包的方式有以下几种：

1. 递归展开参数包。
2. 初始化列表展开参数包。
3. 折叠表达式展开参数包。

本例中详细如下所示。

```cpp
#include <iostream>
#include <vector>

// 可变参数模板
template <typename T, typename... Args>
void print(T first, Args... args) {
    std::cout << first << std::endl;
    if constexpr (sizeof...(args) > 0) {
        print(args...);
    }
}

// 使用初始化列表展开参数包
template <typename... Args>
std::vector<int> createVector(Args... args) {
    std::vector<int> vec;
    (void)std::initializer_list<int>{(vec.push_back(args), 0)...};
    return vec;
}

int main(int argc, char *argv[])
{
    print(1, 2, 3, 4, 5);

    auto vec = createVector(1, 2, 3, 4, 5);

    std::cout << vec.size() << std::endl;
    return 0;
}
```

配合着可变参数，需要折叠表达式，用于对模板参数包在二元运算符上进行折叠操作。折叠表达式可以简化对可变参数模板的处理，尤其是在需要对参数包中的所有参数进行累积操作时，主要包含以下几种形式。

1. 一元左折叠: (pack op ...)
2. 一元右折叠: (... op pack)
3. 二元左折叠: (init op pack op ...)
4. 二元右折叠: (... op pack op init)

```cpp
#include <iostream>
#include <string>

// 一元左折叠实现拼接
template <typename... Args>
auto concat_left(Args... args) {
    return (args + ...);            // 展开形式为：((arg1 + arg2) + arg3) + ...
}

// 一元右折叠实现拼接
template <typename... Args>
auto concat_right(Args... args) {
    return (... + args);            // 展开形式为：arg1 + (arg2 + (arg3 + ...))
}

// 二元左折叠实现拼接
template <typename... Args>
auto dual_sum_left(std::string init, Args... args) {
    return (init + ... + args);     // 展开形式为：((init + arg1 + arg2) + arg3) + ...
}

// 二元右折叠实现拼接
template <typename... Args>
auto dual_concat_right(std::string init, Args... args) {
    return (args + ... + init);     // 展开形式为：arg1 + (arg2 + (arg3 +... + init))
}

int main(int argc, char *argv[])
{
    auto result1 = concat_left("Hello", " ", "World", std::string("!"));
    auto result2 = concat_right(std::string("Hello"), " ", "World", "!");
    auto result3 = dual_sum_left("Hello", " ", "World", "!");
    auto result4 = dual_concat_right("!", "Hello", " ", "World");

    std::cout << "result1: " << result1 << std::endl;
    std::cout << "result2: " << result2 << std::endl;
    std::cout << "result3: " << result3 << std::endl;
    std::cout << "result4: " << result4 << std::endl;
    return 0;
}
```

## template_rules

template支持其它规则和关键字，具体如下所示。

### template_auto

auto声明非类型模板参数，这允许你在模板定义中使用各种非类型参数，而不必明确指定参数的具体类型

```c
#include <iostream>

// 使用 auto 声明非类型模板参数的类模板
template <auto N>
class AutoTemplate {
public:
    void printValue() const {
        std::cout << "The value is: " << N << std::endl;
    }
};

int main(int argc, char *argv[]) 
{
    AutoTemplate<10> intTemplate;
    intTemplate.printValue();
    AutoTemplate<3.14> doubleTemplate;
    doubleTemplate.printValue();  
}
```

### adl_search

ADL查找规则，ADL（Argument-Dependent Lookup），也称为Koenig查找，是C++中用于查找函数名称的一种规则。ADL的主要目的是在调用函数时，不仅在当前作用域中查找函数定义，还会在函数参数的命名空间中查找。具体规则如下：

1. 当前作用域：首先在当前作用域中查找函数名称。
2. 参数类型的命名空间：如果在当前作用域中没有找到函数名称，则在函数参数的类型所在的命名空间中查找。
3. 关联命名空间：对于类类型的参数，还会在类的关联命名空间中查找。关联命名空间包括类本身的命名空间，以及类的基类的命名空间。

```cpp
#include <iostream>

namespace MyNamespace {
    /// class 
    class demo {};

    // 在命名空间中定义一个函数
    void print(demo obj) {
        std::cout << "Print function from MyNamespace" << std::endl;
    }

    std::ostream& operator<<(std::ostream& os, const demo& obj) {
        std::cout<< "obj demo" << std::endl;
        return os;
    }
}

int main(int argc, char *argv[]) 
{
    MyNamespace::demo obj;

    print(obj);         // 尽管没有显式指定命名空间，ADL会在MyClass所在的命名空间查找print函数

    std::cout << obj;   // 自动在MyClass所在的命名空间查找重载函数
    return 0;
}
```

### user_guides

用户自定义推导指引（User-Defined Deduction Guides）是C++17引入的一项特性，它允许程序员为类模板的构造函数提供自定义的类型推导规则。

```cpp
#include <iostream>
#include <type_traits>

/// wrapper类型
template <typename T>
class Wrapper {
public:
    T value;

    Wrapper(const T& val) : value(val) {}
};

template <typename T>
Wrapper(T) -> Wrapper<std::conditional_t<std::is_integral_v<T>, int, T>>;

int main() {
    Wrapper w1(10);      // 推导为 Wrapper<int>
    Wrapper w2(3.14);    // 推导为 Wrapper<double>

    std::cout << "w1 type: " << typeid(decltype(w1.value)).name() << std::endl;
    std::cout << "w2 type: " << typeid(decltype(w2.value)).name() << std::endl;
    return 0;
}
```

### sfinae

SFINAE（Substitution Failure Is Not An Error）是C++中的一个重要概念，它是模板元编程中的一种技术，用于在编译时根据模板参数的类型来选择不同的函数重载或模板特化.

C++中，函数模板与同名的非模板函数重载时，应遵循下列调用原则：

1. 寻找一个参数完全匹配的函数，若找到就调用它。若参数完全匹配的函数多于一个，则这个调用是一个错误的调用。
2. 寻找一个函数模板，若找到就将其实例化生成一个匹配的模板函数并调用它。
3. 若上面两条都失败，则使用函数重载的方法，通过类型转换产生参数匹配，若找到就调用它。
4. 若上面三条都失败，还没有找都匹配的函数，则这个调用是一个错误的调用。

具体示例如下所示：

```cpp
#include <iostream>
#include <vector>
#include <type_traits>

template <typename T, typename = void>
struct has_begin : std::false_type {};

template <typename T>
struct has_begin<T, std::void_t<decltype(std::declval<T>().begin())>> : std::true_type {};

// 有begin方法时调用此函数
template <typename T>
std::enable_if_t<has_begin<T>::value, void> print(T& obj) {
    for (auto it = obj.begin(); it != obj.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

// 没有begin方法时调用此函数
template <typename T>
std::enable_if_t<!has_begin<T>::value, void> print(T& obj) {
    std::cout << obj << std::endl;
}

int main(int argc, char* argv[]) {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    int num = 42;

    print(vec); // 调用有begin()方法的版本
    print(num); // 调用没有begin()方法的版本

    return 0;
}
```

### explicit_template

外部模板（Explicit Template Instantiation）是C++中的一个特性，它允许程序员在一个编译单元中显式地实例化一个模板，而在其他编译单元中使用这个实例化的模板，而不需要再次实例化。

```cpp
#include <iostream>

template <typename T>
class MyClass {
public:
    MyClass(T value) : value_(value) {}
    T getValue() const { return value_; }
private:
    T value_;
};

// 显式实例化
template class MyClass<int>;

int main(int argc, char* argv[]) 
{
    MyClass<int> obj(42);
    std::cout << obj.getValue() << std::endl;
    return 0;
}
```

### enable_if

enable_if/enable_if_t：用于在编译时根据条件选择是否启用某个函数或模板特化。通过std::enable_if_t可以在编译时根据条件选择是否启用某个函数或模板特化，而不是在运行时根据条件选择，配合SFINAE可以实现根据类型选择不同的函数重载或模板特化。

```cpp
#include <iostream>
#include <type_traits>

template <typename T>
std::enable_if_t<std::is_integral_v<T>, void> print(T value) {
    std::cout << "Integral value: " << value << std::endl;
}

template <typename T>
std::enable_if_t<!std::is_integral_v<T>, void> print(T value) {
    std::cout << "Non-integral value: " << value << std::endl;
}

int main(int argc, char* argv[]) {
    print(42);      // 调用第一个print函数
    print(3.14);    // 调用第二个print函数
    return 0;
}
```

### metaprogramming

模板元编程是一种编程范式，它使用模板来编写程序，在编译时进行计算和生成代码。模板元编程的主要目标是在编译时进行计算和优化，以提高程序的性能和可维护性。

模板元编程对于C++来说是比较复杂的内容，配合constexpr可以将各种复杂运算在编译期执行；这里只能列举单个示例来说明。

```cpp
#include <iostream>

// 辅助模板，用于判断是否为素数
template <int N, int Divisor = 2>
struct IsPrime {
    static constexpr bool value = (N % Divisor != 0) && IsPrime<N, Divisor + 1>::value;
};

// 特化版本，终止递归
template <int N>
struct IsPrime<N, N> {
    static constexpr bool value = true;
};

// 处理 N 小于 2 的情况
template <>
struct IsPrime<0> {
    static constexpr bool value = false;
};

template <>
struct IsPrime<1> {
    static constexpr bool value = false;
};

int main(int argc, char* argv[]) 
{
    // 计算值被递归次数限制，-ftemplate-depth=
    constexpr bool isPrime7 = IsPrime<556>::value;
    constexpr bool isPrime8 = IsPrime<999>::value;
    
    std::cout << "556 是素数: " << std::boolalpha << isPrime7 << std::endl;
    std::cout << "999 是素数: " << isPrime8 << std::endl;
    return 0;
}
```
