# 现代C++功能扩展

- [attribute](#attribute)
- [auto](#auto)
  - [decltype](#decltype)
- [constexpr](#constexpr)
  - [constexpr_extension](#constexpr_extension)
  - [consteval](#consteval)
  - [constinit](#constinit)
- [cpp_extension](#cpp_extension)
  - [if switch初始化](#if_switch_init)
  - [nullptr](#nullptr)
  - [结构化绑定](#structured_binding)
  - [扩展的for表达式](#for_range)
- [lambda](#lambda)
- [literal](#literal)
  - [law_string](#law_string)
  - [defined_literal](#defined_literal)
  - [single_quote](#single_quote)
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

- 当一个auto关键字声明多个变量时，编译器遵循从左到右的顺序推导，以最左边的表达式推导auto具体类型；另外推导类型需要相同，否则会导致编译错误。
- 使用条件表达式初始化auto声明的变量时，编译器总是使用类型更宽的类型推导，如能够同时推导double或者int，那么就会推导为double类型。

对于auto来说也有些限制，主要如下所示。

- 对于结构体来说，非静态成员不能用auto推导；对于静态成员，C++17后支持非const变量用auto推导。
- 不能够声明auto数组，即使使用中括号进行初始化(会被推导为std::initializer_list<T>类型，和数组不匹配)。
- auto不能用作模板参数，如`vector<auto> demo{1, 2, 3, 4}`。不过C++17后可以作为非类型模板形参的占位符，配合decltype(auto)可支持cv推导，另外推导出的类型必须为允许的模板参数类型。
- auto推导忽略引用、cv限定符(const和volatile)，如果有需要可以额外添加。
- auto和&&结合使用时，对于左值会将其推导为引用类型。
- auto推导数组或者函数时，会被推导成对应的指针类型。
- auto关键字配合初始化列表时(C++17后支持)，直接列表初始化(只能为一个元素)，推导类型为对应内部元素类型；等号列表初始化，推导类型std::initializer_list<T>类型，其中列表内需要为单一元素类型。

```cpp
// 返回类型后置
auto add(auto a, auto b) -> decltype(a + b) {
    return a + b;
}

// 返回类型推导
auto mul(auto a, auto b){
    return a * b;
}
```

非类型模板参数也可以使用auto作为占位符，要求推导出的类型必须为允许的模板参数类型。

```cpp
// 非类型模板参数的auto作为占位符
template <auto N>
void func(void) {
    std::cout << "N:" << N << std::endl;
}
```

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

auto的另一个用处是实现返回类型后置和返回类型推导。通过使用auto作为占位符，可以声明返回类型。

具体示例如下所示。

```cpp
#include <iostream>

class outer_type
{
private:
    struct inner_type { int a; };

public:
    inner_type get_inner(void);
};

// 返回类型后置，用于声明返回类型，可以省略外部结构体的名称
auto outer_type::get_inner(void) -> inner_type {
    return inner_type{10};
}

template <typename T1, typename T2>
auto multiply(T1 &&a, T2 &&b) -> decltype(a * b) {
    return a * b;
}

int main(int argc, const char *argv[]) 
{
    outer_type outer;

    std::cout << outer.get_inner().a << std::endl;  // 10
    std::cout << multiply(2, 3.1) << std::endl;     // 6.2
    return 0;
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

1. decltype的推导会同步cv限定符，包括const和volatile；不过对于父对象的cv限定符(如结构体中的某个成员变量)，decltype不会同步，可通过添加括号来实现同步其父对象的cv限定符。
2. 基于auto和decltype的类型推导，可以实现函数返回值类型后置和自动推导函数。
3. decltype(auto)是C++14引入的类型说明符，结合了decltype和auto的特性，用于类型推导。auto推导时会忽略引用和cv限定符(const和volatile），而decltype能精确地返回表达式的类型，decltype(auto)综合二者，按decltype的规则进行类型推导。

另外可以使用`using type = decltype(e)`来声明类型，并在后面使用此类型声明变量。

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

    // decltype(nullptr)类型
    using type_nullptr = decltype(nullptr);

    type_nullptr pn;
    std::cout << "pn is nullptr: " << (pn == nullptr) << std::endl; // true
}
```

## constexpr

constexpr是C++11 引入的一个关键字，用于声明常量表达式。常量表达式是指在编译时就能计算出结果的表达式。使用constexpr关键字可以让编译器在编译时就对表达式进行求值，从而提高程序的性能。一个函数要想成为常量表达式函数（constexpr)，必须满足以下条件。

- 该函数必须有返回值，即函数的返回值类型不能是void，且必须是常量表达式(C++14开始，void 类型的函数也可以是常量表达式函数)。
- 整个函数的函数体中，除了可以包含using指令、typedef语句以及static_assert断言外，只能包含一条return返回语句。
- 函数在使用之前，必须有对应的定义语句，且使用constexpr修饰。

constexpr和const的区别如下。

- const可以用于声明运行时和编译时的常量，而constexpr 只能用于声明编译时的常量。
- const变量的值可以在运行时改变，而constexpr变量的值必须在编译时确定。
- constexpr函数必须满足一定的条件才能被编译器识别为常量表达式函数，而const函数没有这样的限制。

在C+14版本，放开了对constexpr函数的限制，具体如下所示。

1. 函数体允许声明变量，除了没有初始化、static和thread_local变量。
2．函数允许出现if和switch语句，不能使用go语句。
3．函数允许所有的循环语句，包括for、while、do-while。
4．函数可以修改生命周期和常量表达式相同的对象。
5．函数的返回值可以声明为void。
6．constexpr声明的成员函数不再具有const属性。

具体示例如下所示。

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

constexpr int func3(int a)
{
    int b = 1, sum = 0;
    for (int i = 0; i < a; ++i) {
        sum += b;
    }
    return sum;
}

constexpr double val = 5;

// constexpr 类
// 构造函数需要是constexpr函数
class demo
{
public:
    constexpr demo() : a_(1) {}
    constexpr demo(int i) : a_(0) {
        if (i > 0) {
            a_ = i;
        } else {
            a_ = -i;
        }
    }
    constexpr int get_a() const { return a_; }
    constexpr void set_a(int a) { a_ = a; }
    
    // constexpr包含内联属性(C++17)
    static constexpr int b_ = 1;
private:
    int a_{0};
};

constexpr demo make_demo(int i) {
    demo d;
    d.set_a(i);
    return d;
}

int main(int argc, char const *argv[]) 
{ 
    constexpr int a = 1;
    std::cout << "a: " << a << std::endl;

    int arr1[func1(1)];
    std::cout << "arr1 size: " << sizeof(arr1)/sizeof(int) << std::endl;

    constexpr auto add = [](int a, int b) constexpr {
        return a + b;
    };
    int arr2[add(1, 2)];
    std::cout << "arr2 size: " << sizeof(arr2)/sizeof(int) << std::endl;

    int arr3[func2(1)];
    std::cout << "arr3 size: " << sizeof(arr3)/sizeof(int) << std::endl;

    int arr4[func3(3)];
    std::cout << "arr4 size: " << sizeof(arr4)/sizeof(int) << std::endl;

    constexpr demo d2 = make_demo(2);
    int arr5[d2.get_a()];
    std::cout << "arr5 size: " << sizeof(arr5)/sizeof(int) << std::endl;
    
    static_assert(val > 0, "val must be greater than 0");
    static_assert(demo::b_ > 0, "demo::b_ must greater than 0");
    return 0;
}
```

### constexpr_extension

在C++17中，增加了constexpr lambda表达式，即可以在编译时对lambda表达式进行求值。对于所有lambda表达式，在条件允许的情况下，都会隐式声明为constexpr。另外constexpr对于类的静态对象也被赋予内联属性，不需要额外添加inline关键字。

另外，对于C++17，扩展支持if constexpr语句，即可以在编译时对if语句进行求值。对于if constexpr，具有满足以下特点。

1．if constexpr的条件必须是编译期能确定结果的常量表达式。
2. 条件结果一旦确定，编译器将只编译符合条件的代码块。
3. if constexpr不支持短路原则，也就是所有条件都必须是合法的常量表达式，否则无法编译通过。这时可以使用嵌套的if constexpr实现。

在C++20中，允许在constexpr函数中修改联合体的有效成员。

具体示例如下所示。

```cpp
#include <iostream>
#include <string>
#include <type_traits>

class demo
{
public:
    demo() = default;

    static constexpr int b_ = 1;
};

// double类型特殊处理
template <typename T>
bool is_same_value(T a, T b)
{
    if constexpr (std::is_same_v<T, double>) {
        if (std::abs(a - b) < 0.0001) {
            return true;
        } else {
            return false;
        }
    } else {
        return a == b;
    }
}

template <typename T>
auto any2int(T a)
{
    if constexpr (std::is_same_v<T, std::string>) {
        if constexpr (T::npos != -1) {
            return std::stoi(a.c_str());
        } else {
            return 0;
        }
    } else {
        return a;
    }
}

union u_type
{
    int i_;
    double d_;
};

constexpr int ut_func() 
{
    u_type u;
    u.i_ = 100;
    u.d_ = 100.0;
    return u.i_;
}

int main(int argc, char const *argv[]) 
{
    // lambda允许情况下隐式声明为constexpr，不满足直接退回普通函数
    auto add_func = [](int a, int b) {
        return a + b;
    };
    int arr1[add_func(1, 2)];
    std::cout << "arr1 size: " << sizeof(arr1)/sizeof(int) << std::endl;

    // lambda使用捕获对象或者全局变量时，需要对应对象也为constexpr，否则会退化成普通函数
    constexpr int c = 1;
    auto mul_func = [c](int a, int b) constexpr {
        return a * b * c;
    };
    int arr2[mul_func(1, 2)];
    std::cout << "arr2 size: " << sizeof(arr2)/sizeof(int) << std::endl;

    static const int c1 = 1;
    auto mul_func1 = [](int a, int b) constexpr {
        return a * b * c1;
    };
    int arr3[mul_func1(1, 2)];
    std::cout << "arr3 size: " << sizeof(arr3)/sizeof(int) << std::endl;

    static_assert(demo::b_ > 0, "demo::b_ must greater than 0");

    std::cout << std::boolalpha;

    // double类型特殊处理
    double a = 1.0, b = 1.00001;
    std::cout << "a == b: " << is_same_value(a, b) << std::endl;

    int a1 = 1, b1 = 2;
    std::cout << "a1 == b1: " << is_same_value(a1, b1) << std::endl;

    std::cout << "any2int(\"123\"): " << any2int("123") << std::endl;
    std::cout << "any2int(\"123.456\"): " << any2int(2) << std::endl;

    std::cout << "ut_func(): " << ut_func() << std::endl;
    return 0;
}
```

### consteval

consteval是C++20引入的关键字，用于声明一个函数只能在编译时调用，不能在运行时调用，这种函数被称为立即函数。

立即函数的主要特点如下。

1. 立即函数只能在编译时调用，不能在运行时调用。
2. 立即函数的参数必须是编译时能确定结果的常量表达式。
3. 立即函数的返回值必须是一个常量表达式。

立即函数不会退化成普通函数，运行时调用时会触发编译错误，lambda函数也支持定义为consteval。

具体示例如下所示。

```cpp
#include <iostream>

consteval int func_0() 
{
    return 100;
}

consteval int func_1(int a) 
{
    return a * a;
}

constexpr int const_data = func_1(10);

int main(int argc, char const *argv[]) 
{
    std::cout << "func_0(): " << func_0() << std::endl;
    std::cout << "func_1(10): " << func_1(10) << std::endl;
    std::cout << "const_data: " << const_data << std::endl;

    int arr1[func_1(10)];
    std::cout << "arr1 size: " << sizeof(arr1)/sizeof(int) << std::endl;

    // lambda也支持定义为consteval
    auto func2 = [](int a, int b) consteval {
        return a + b;
    };
    std::cout << "func2(1, 2): " << func2(1, 2) << std::endl;
    return 0;
}
```

### constinit

constinit是C++20引入的关键字，用于声明一个变量在编译时必须初始化，不能在运行时初始化。

constinit变量的主要特点如下。

1. constinit变量在编译时必须初始化，不能在运行时初始化。
2. constinit变量的初始化值必须是一个常量表达式。
3. constinit变量并不要求对象具有常量属性。

具体示例如下所示。

```cpp
#include <iostream>
#include <string>

constinit int const_data = 100;

constexpr const char *get_str() 
{
    return "hello world";
}
constinit const char* const_str = get_str();

constexpr int get_val(int a) 
{
    return a * a;
}
constinit int const_val = get_val(10);

int main(int argc, char const *argv[]) 
{
    static constinit int const_data1 = 200;

    std::cout << "const_data: " << const_data << std::endl;
    std::cout << "const_data1: " << const_data1 << std::endl;
    std::cout << "const_str: " << const_str << std::endl;
    std::cout << "const_val: " << const_val << std::endl;
    return 0;
}
```

## cpp_extension

扩展的C++语法糖，包含if/switch语句带条件初始化、nullptr、结构化绑定、扩展的for表达式等。

### if_switch_init

在C++17中，if和switch语句支持在条件判断之前进行初始化。这是通过在if或switch关键字后面直接添加一个初始化语句来实现的。使用初始化语句的变量，其生命周期贯穿整个if/switch作用域。

其格式如下。

```cpp
// if语句带条件初始化
// init: 初始化语句，如int a = 1;
// condition: 条件判断语句，如a > 0;
// body: 满足条件时执行的语句块
if (init; condition) {
    body
}

// switch语句带条件初始化
// init: 初始化语句，如int a = 1;
// condition: 分支条件的变量;
// body: 满足条件时执行的语句块
switch(init; condition) {
    case value:
        body
        break;
    ...
    default:
        body
        break;
}
```

具体示例如下所示。

```cpp
#include <iostream>

int func1() {
    return 1;
}

int func2() {
    return 2;
}

int main(int argc, char const *argv[]) 
{
    int b = 1;
    
    // if语句带条件初始化
    if (int a = b; a > 0) {
        std::cout << "a is greater than 0" << std::endl;
    }

    // if/else if语句都支持条件初始化
    if (int a = func1(); a < 1) {
        std::cout << "a is 1"  << std::endl;
    } else if (int a = func2(); a == 2) {
        std::cout << "a is 2" << std::endl;
    } else {
        std::cout << "a is not 1 or 2" << std::endl;
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

在C++17中，引入了结构化绑定（Structured Binding）的概念。它允许你将一个复杂的表达式分解成多个独立的变量，从而更方便地访问和操作表达式的各个部分。支持结构化绑定的类型如下所示。

- 结构体、位域格式结构体及特殊类。
- 原生数组。
- std::tuple元组类型或者类元组类型。
- std::pair对类型。
- std::array数组。

其中结构体、位域格式结构体、类满足结构化绑定，需要满足只有公有成员变量、且数据成员必须在同一个类或者基类中。对于类元组类型，需要满足以下条件。

1. 需要满足std::tuple_size<T>::value是一个符合语法的表达式，并且该表达式获得的整数值与标识符列表中的别名个数相同。
2. 类型T还需要保证std::tuple_element<i, T>::type也是一个符合语法的表达式，其中i是小于std::tuple_size<T>::value的整数，表达式代表了类型T中第i个元素的类型。
3. 类型T必须存在合法的成员函数模板get<i>()或者函数模板get<i>(t)，其中i是小于std::tuple_size<T>::value的整数，t是类型T的实例，get<i>()和get<i>(t)返回的是实例t中第i个元素的值。

另外C++20后，只要结构化绑定作用域支持直接访问类中的全部变量即可，不需要全部位公有成员变量；扩展支持场景如友元类、友元函数，继承类访问父类protected成员变量。

具体示例如下所示。

```cpp
#include <iostream>
#include <tuple>
#include <array>
#include <vector>
#include <map>

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

class my_class2:public my_class {
    using my_class::my_class;
};

union my_union {
    int x;

    struct {
        int x1:4;
        int y1:4;
        int z1:8;
    } p;
};

class tuple_demo {
public:
    int x_{1};
};

// 基类和类中都有参数，默认不支持结构化绑定
// 通过自定义扩展支持
class tuple_demo2: public tuple_demo {
public:
    int y_{2};

    // 成员函数get扩展，支持结构化绑定
    template <std::size_t Idx> auto &get() {
        if constexpr (Idx == 0) {
            return x_;
        } else if constexpr (Idx == 1) {
            return y_;
        } else {
            static_assert(Idx < 2, "Idx Must Small Than 2");
            return 0;
        }
    }
};

// 重定义全局的get函数，支持结构化绑定
// template<std::size_t Idx>
// auto &get(tuple_demo2 &t) {
//     if constexpr (Idx == 0) {
//         return t.x_;
//     } else if constexpr (Idx == 1) {
//         return t.y_;
//     } else {
//         return 0;
//     }
// }


namespace std {
    template<>
    struct tuple_size<tuple_demo2> {
        static constexpr std::size_t value = 2;
    };

    template<>
    struct tuple_element<0, tuple_demo2> {
        using type = int;
    };

    template<>
    struct tuple_element<1, tuple_demo2> {
        using type = int;
    };
}

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

    // 支持继承的类，但所有变量需要在同一个类或者基类中
    my_class2 p3{1, 2};
    auto [x8, y8] = p3;
    std::cout << "x8: " << x8 << ", y8: " << y8 << std::endl;

    // 位域支持结构化绑定
    my_union u1;
    u1.x = 0x1234;
    auto [x7, y7, z7] = u1.p;
    std::cout << "x7: " << x7 << ", y7: " << y7 << ", z7: " << z7 << std::endl;

    // 通过结构化绑定遍历容器
    std::vector<point> vec = {{1, 2}, {3, 4}, {5, 6}};
    // 遍历vec容器，将每个元素的x和y打印出来
    for (const auto& [x, y] : vec) {
        std::cout << "x: " << x << ", y: " << y << " ";
    }
    std::cout << std::endl;

    // std::pair支持结构化绑定
    auto pair1 = std::make_pair(1, 2);
    auto [key1, value1] = pair1;
    std::cout << "key1: " << key1 << ", value1: " << value1 << std::endl;

    std::map<int, int> map = {{1, 2}, {3, 4}, {5, 6}};
    for (const auto& [key, value] : map) {
        std::cout << "key: " << key << ", value: " << value << " ";
    }
    std::cout << std::endl;

    tuple_demo2 t5;
    auto& [x9, y9] = t5;
    std::cout << "x: " << x9 << ", y: " << y9 << std::endl;
    std::cout << "get<0>: " << t5.get<0>() << std::endl;
    return 0;
}
```

### for_range

扩展的for表达式（也称为范围-based for循环）是一种简化遍历容器（如数组、向量、列表等）的语法。for表达式在C++20后，和if、switch语句一致，扩展对于初始化的支持。

扩展for表达式格式如下所示。

```cpp
for (declaration : range_expression) {
    loop_body
}
```

范围表达式可以是数组或对象，对象必须满足以下2个条件中的任意一个。

1. 对象为数组，如int arr[10]。
2. 对象实现了begin()和end()成员函数的对象，并且这组对象支持"operator++、operator!=、operator*"操作(可以是指针或者迭代器)；如标准模板库中的std::vector、std::list，另外满足上述需求的自定义类型也支持for表达式。

具体示例如下所示。

```cpp
#include <iostream>
#include <vector>

template <typename T, int N>
class my_array{
public:
    my_array() = default;
    my_array(std::initializer_list<T> list) {
        std::copy(list.begin(), list.end(), arr);
    }

    int* begin() {
        return &arr[0];
    }

    int* end() {
        return &arr[N];
    }

private:
    T arr[N]{0};
};

std::vector<int> get_vec()
{
    auto vec = std::vector<int>{1, 2, 3, 4, 5};
    return vec;
}

int main(int argc, char const *argv[]) 
{
    std::vector<int> vec = {1, 2, 3, 4, 5};

    // 遍历vec容器，将每个元素打印出来
    for (const int& i : vec) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    // for支持初始化
    for(auto foo = get_vec(); const auto& bar : foo) {
        std::cout << bar << " ";
    }
    std::cout << std::endl;

    my_array<int, 5> arr = {1, 2, 3, 4, 5};
    for (const int& i : arr) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    return 0;
}
```

## lambda

在C++中，lambda表达式是一种匿名函数，它允许你在需要函数对象的地方定义一个临时的函数。lambda表达式通常用于简化代码，特别是在需要传递函数作为参数的情况下。

```cpp
[capture list](parameters) specifiers exception -> return_type {
    body
} 
```

各部分功能说明如下：

- capture list: 捕获列表，用于捕获外部局部变量。
  - [] 无捕获的lambda表达式。
  - [=] 值捕获，lambda表达式通过值捕获所有外部变量，在类方法环境中包含this指针(C++20废弃，需要单独捕获， [=, this]。
  - [&] 通过引用捕获所有外部变量的lambda表达式。
  - [var] 只捕获变量var。
  - [&var] 只捕获变量var的引用。
  - [this] 捕获当前类中的this指针，按引用捕获。
  - [*this] 捕获当前类中的this指针，按值捕获，值捕获默认定义为const(C++17)，可通过mutable修饰符取消const属性。
  - [=, &var] 捕获所有外部变量的lambda表达式，但是var是通过引用捕获的。
  - [&, var] 捕获所有外部变量的lambda表达式，但是var是通过值捕获的。
- parameters: 可选参数列表
- specifiers: 可选函数修饰符, 如mutable等，表示可以允许修改值捕获值。
- exception: 可选抛出异常的声明，如noexcept、throw()等。表示lambda表达式是否会抛出异常。
- return_type: 可选返回值类型，如void、int、double等。表示lambda表达式的返回值类型。
- body: 函数体, 包含在大括号{}中，用于定义lambda表达式的行为。

注意1：**对于lambda来说，捕获的是作用域内的非静态变量；对于静态变量，不需要捕获，可以在函数体内直接使用。**

注意2：**引用捕获和值捕获的区别是，引用捕获的变量是外部变量的引用，而值捕获的变量是外部变量的副本。也就是说引用捕获可以直接修改，且会改变外部变量的值。而值捕获的变量修改需要加mutable修饰符，且不会改变外部变量的值。**

注意3：**无状态lambda表达式(无捕获信息)，可以隐式转换为函数指针。**

示例代码如下：

```cpp
#include <iostream>

static int a1 = 1;

int callback(int(*func)(int, int)) 
{
   return func(1, 2);
}

int main(int argc, char const *argv[]) 
{
    int a = 1;
    static int a2 = 1;

    // 值捕获
    auto func1 = [a](int b) -> int {
        return a + b;
    };
    std::cout << "func1(2): " << func1(2) << std::endl;

    // 引用捕获
    auto func2 = [&a](int b) -> int {
        a += b;
        return a + b;
    };
    std::cout << "func2(2): " << func2(2) << std::endl;
    std::cout << "a: " << a << std::endl;

    // mutable 表示可以修改值捕获的值
    // noexcept 表示lambda表达式不会抛出异常
    auto func3 = [a]() mutable noexcept -> int {
        a += 1;
        return a;
    };
    std::cout << "func3(): " << func3() << std::endl;

    // 简单lambda表达式，无捕获和参数
    // 静态变量不需要捕获，可以在函数体内直接使用
    auto func4 = [](){
        return a1 + a2;
    };
    std::cout << "func4(): " << func4() << std::endl;

    // 调用回调函数
    auto func5 = [](int a, int b){
        return a + b;
    };
    int ret = callback(func5);
    std::cout << "callback(func4): " << ret << std::endl;
    return 0;
}
```

可以使用std::function对象来存储lambda表达式，另外从原理上，lambda基于函数对象(仿函数)来实现。对于Lambda还有以下重要特性，其中C++14支持泛型lambda，Lambda捕获初始化器，C++20则支持lambda模板。

1. 泛型lambda表达式是C++14引入的特性，它允许在lambda表达式中使用auto占位符来表示参数类型，使得lambda可以接受不同类型的参数，实现类似函数模板的泛型功能。
2. Lambda捕获初始化器是C++14引入的特性，它允许你在lambda捕获列表中对捕获的变量进行自定义初始化，还能创建新的局部变量供lambda函数体使用，即使这些变量在lambda外部并不存在。
3. lambda模板是C++20引入的特性，它允许在lambda表达式中使用模板参数，使得lambda可以接受任意类型的参数，实现类似模板函数的功能。

关于上述说明，具体例程如下所示。

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

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
   
    // C++20支持无状态lambda构造或者赋值
    auto greator = [](auto a, auto b) -> bool {
        return a > b;
    };
    std::map<std::string, int, decltype(greator)> map = {
        {"a", 1},
        {"b", 2},
        {"c", 3},
    };
    for (const auto& [key, value] : map) {
        std::cout << key << ": " << value << std::endl;
    }

    return 0;
}
```

## literal

### law_string

原生字符串字面量（Raw String Literals）是C++11引入的一个特性，它允许你在字符串中包含特殊字符（如反斜杠和引号）而不需要进行转义。原生字符串字面量以R"( 开始，以 )"结束。在这两个符号之间的所有字符都被视为字符串的一部分，包括换行符和引号。

在C++20中，R"()"也支持通过前缀L、u、U和u8指定原生字符串字面量的类型。

1. L前缀：表示宽字符串字面量（wchar_t类型）。
2. u前缀：表示UTF-8编码的字符串字面量（char16_t类型）。
3. U前缀：表示UTF-32编码的字符串字面量（char32_t类型）。
4. u8前缀：表示UTF-8编码的字符串字面量（char类型）。

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

    char8_t char_1[] = u8"Hello, world!";
    char16_t char_2[] = u"Hello, world!";
    char32_t char_3[] = U"Hello, world!";
    wchar_t char_4[] = L"Hello, world!";

    std::u8string str_u8(char_1);
    std::cout << std::string_view(reinterpret_cast<const char *>(str_u8.c_str()), str_u8.length()) << std::endl;

    std::u16string str_u16(char_2);
    std::u32string str_u32(char_3);
    
    std::cout << str_u16.length() << std::endl;
    std::cout << str_u32.length() << std::endl;

    std::wcout << char_4 << std::endl;
    return 0;
}
```

### defined_literal

用户可以自定义字面量（User-Defined Literals），这允许你为基本类型（如整数、浮点数、字符串等）定义新的后缀，从而创建自定义的字面量。
对于字符串字面量，你可以定义一个自定义后缀来创建一个特定类型的对象。

```cpp
#include <iostream>
#include <string>
#include <type_traits>

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
MyString operator""_my() {
    MyString str("");

    //折叠表达式
    using unused = int[];
    unused{(str.push_back(c), 0)...};
    return str;
}

template <int scale, char... unit_char>
struct weight_unit {
    constexpr static int value = scale;
    constexpr static char unit_char_value[sizeof...(unit_char) + 1] = {unit_char..., '\0'};
};

template <class T>
requires requires { 
    T::value; 
    T::unit_char_value;
}
class weight_with_unit {
public:
    weight_with_unit() : length_(0) {}
    weight_with_unit(unsigned long long length) : length_(length*T::value) {}

    template<class U>
    weight_with_unit<std::conditional_t<(T::value > U::value), U, T>> operator+ (const weight_with_unit<U>& other) const {
        using unit_type = std::conditional_t<(T::value > U::value), U, T>;
        return weight_with_unit<unit_type>((length_ + other.get_length()) / unit_type::value);
    }

    unsigned long long get_length() const { return length_; }
    constexpr static const char* get_unit_str() {return T::unit_char_value;}

private:
    unsigned long long length_;
};

template <class T>
std::ostream& operator<<(std::ostream& os, const weight_with_unit<T>& wu) {
    os << wu.get_length() / T::value << weight_with_unit<T>::get_unit_str();
    return os;
}

using kg_unit = weight_unit<1000, 'k', 'g'>;
using g_unit = weight_unit<1, 'g'>;

using weight_with_kg = weight_with_unit<kg_unit>;
using weight_with_g = weight_with_unit<g_unit>;

weight_with_kg operator""_kg(unsigned long long val) {
    return weight_with_kg(val);
}

weight_with_g operator""_g(unsigned long long val) {
    return weight_with_g(val);
}

int main(int argc, char const *argv[])
{
    "Hello, world!"_my.print();

    auto val = 123_my;
    val.print();

    auto val_kg = weight_with_kg(64);
    std::cout << val_kg.get_length() << std::endl;
    std::cout << val_kg << std::endl;

    auto val_g = weight_with_g(123);
    std::cout << val_g << std::endl;

    auto val_kg_g = val_kg + val_g;
    std::cout << val_kg_g << std::endl;

    auto val_kg_g_2 = 64_kg + 123_g;
    std::cout << val_kg_g_2 << std::endl;
    return 0;
}
```

### single_quote

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

断言是C++本身提供的，用于检查代码中的逻辑错误的机制。断言就是将一个返回值总是需要为真的判别式放在语句中，用于排除在设计的逻辑上不应该产生的情况。基础的断言语法如下：

```cpp
// 运行时断言，检查到逻辑错误中止执行
assert(条件);

// 举例说明，限制n的输入必须大于0
int run_func(int n) {
    assert(n > 0);
    return n;
}
```

运行时断言在运行时检查，引入性能损失。且无法携带额外的错误信息，只是直接中止程序，不利于后续分析，另外对于模板实例化的实现是在编译阶段完成，运行时断言也无法处理。

C++引入静态断言（static_assert）这一编译时断言机制，用于在编译阶段检查某个条件是否为真。如果条件为假，编译器会产生一个错误信息，并且编译过程会失败。静态断言通常用于在编译时验证模板参数、常量表达式或其他编译时可确定的条件。在C++中，可以在包含`#include <cassert>`头文件前使用`#define NDEBUG`宏来关闭静态断言。

静态断言的语法如下：

```cpp
// 静态断言
static_assert(常量表达式, 可选的错误信息);
```

静态断言在编译阶段检查，第一个参数必须是常量表达式，需要编译阶段确定值；第二个参数是可选的错误信息，用于在断言失败时提供更多的上下文信息，不存在则直接显示静态断言错误的位置。

举例说明如下。

```cpp
#include <iostream>
#include <type_traits>

template <typename T>
void check_size() {
    static_assert(sizeof(T) <= 4, "Type size exceeds 4 bytes");
    std::cout << "Type size is within the limit." << std::endl;
}

class A {};
class B : public A {};

template <typename R1, typename R2>
class E {
    static_assert(std::is_base_of<R1, R2>::value, "R2 must be derived from R1");
};

int main(int argc, char const *argv[])
{
    static_assert(sizeof(int) == 4, "int must be 4 bytes");
    check_size<int>();    // 编译通过，int类型大小通常为4字节
    // check_size<double>();  // 编译失败，double类型大小通常为8字节
    check_size<char>();
    check_size<float>();
    
    E<A, B> e;
    // E<B, A> e1;         // 编译失败，R2必须继承自R1
    return 0;
}
```
