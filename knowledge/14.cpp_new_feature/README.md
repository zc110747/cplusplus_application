# cpluspuls 2020~新特性

本节中主要包含C++20~之后引入的新特性，按照功能划分为四大类，分别是概念和约束、范围和视图、module导入和协程应用；除上述特性外，还包含增加的特殊的功能模块，如std::formtat、std::span等，都将在本节进行介绍，具体如下所示。

- [compare](#compare)
- [concepts](#concepts)
  - [requires](#requires)
  - [concept](#concept)
- [coroutines](#coroutines)
- [format](#format)
- [latch](#latch)
- [module](#module)
- [ranges](#ranges)

## compare

compare是C++20引入用于比较两个对象的大小关系的头文件库，它提供了三向比较符<=>来比较两个对象的大小关系，避免了传统的比较运算符（如 <、>、<=、>=）可能带来的未定义行为，并提供了std::strong_ordering、std::weak_ordering和std::partial_ordering三种排序方式。

- std::strong_ordering 强排序
  - 定义：如果a < b，那么a的排序值小于b的排序值；如果a > b，那么a的排序值大于b的排序值；如果a == b，那么a的排序值等于b的排序值。
  - 性质：强排序是一种全序关系，即任意两个元素之间都存在排序关系。
  - 应用场景：当需要对不同类型的对象进行排序时，使用强排序可以确保排序的结果是可预测的，避免了未定义行为。
- std::weak_ordering 弱排序
  - 定义：如果a < b，那么a的排序值小于b的排序值；如果a > b，那么a的排序值大于b的排序值；如果a == b，那么a的排序值等于b的排序值。
  - 性质：弱排序是一种偏序关系，即任意两个元素之间都可能存在排序关系。
  - 应用场景：当需要对不同类型的对象进行排序时，使用弱排序可以确保排序的结果是可预测的，避免了未定义行为。
- std::partial_ordering 部分排序
  - 定义：如果a < b，那么a的排序值小于b的排序值；如果a > b，那么a的排序值大于b的排序值；如果a == b，那么a的排序值等于b的排序值；如果a和b之间不存在排序关系，那么a的排序值和b的排序值都不存在。
  - 性质：部分排序是一种偏序关系，即任意两个元素之间都可能存在排序关系。
  - 应用场景：当需要对不同类型的对象进行排序时，使用部分排序可以确保排序的结果是可预测的，避免了未定义行为。

## concepts

concepts，又称概念，是C++20引入的重要特性，用于描述模板类型是否符合某些条件的特性。关于概念的具体实现，又涉及两个关键字concept和requires。其中concept类似using，用于定义一个概念类型，可通过bool类型或者requires表达式来指定类型是否符合要求。复杂的概念，往往是requires和bool类型，一条或者多条的组合，因此在理解概念，一定要建立对于约束的认知，下面开始进行说明。

### requires

requires，又称约束，是用于模板或auto类型参数的一种语法，用于指定类型参数必须满足的条件。约束可以是一个bool类型的表达式，也可以是一个或一组requires表达式，用于指定类型参数必须满足的条件，其格式如下所示。

```cpp
// 约束表达式为bool类型
requires (bool表达式)

// 举例说明
requires (N > 0 && N <= 10)

// 约束表达式为一个或一组requires表达式
requires requires(类型参数) {
    // requires表达式
}

// 举例说明
requires requires(T t) {
    // 类型要求
    typename T::iterator;
    typename T::value_type;
    
    // 表达式要求(方法要求，返回值类型要求)
    { t.size() } -> std::convertible_to<std::size_t>;
    { t.begin() } -> std::convertible_to<typename T::iterator>;
    { t.end() } -> std::convertible_to<typename T::iterator>;
    { t.empty() } -> std::convertible_to<bool>;

    // 嵌套要求
    requires std::copy_constructible<T>;
    requires std::move_constructible<T>;
    requires sizeof(typename T::value_type) > 1;
}
```

可以看到，在requires表达式中，我们可以定义类型要求、表达式要求(方法要求，返回值类型要求)、嵌套要求等，这些要求都必须满足，才会认为类型参数符合要求。对于requires为bool型表达式的情况，要注意如下特征。

1. 如果是bool型表达式，必须是初等表达式或者带括号的任意表达式。
2. 使用||或&&连接多个表达式，分别表示或、与的关系。
3. 对于单个类或者函数时，必须满足所有的要求，不然会按照顺序进行检查，直到找到第一个不符合要求的表达式，然后编译错误。
4. 用于约束重载函数或类的构造函数时，约束作为对模板的特化，会拥有更优先的匹配度，优先执行。

具体示例如下所示。

```cpp
#include <iostream>
#include <type_traits>
#include <concepts>
#include <vector>

// 约束表达式为bool类型
template <typename T>
requires std::is_integral_v<T>
void print_int(T t)
{
    std::cout << "print_int: " << t << std::endl;
}

constexpr bool is_odd(int t) {
    return t >= 0 && t % 2 == 1;
}

// 非初始表达式需要加括号
template <int N>
requires (is_odd(N))
void print_n()
{
    std::cout << "print_n: " << N << std::endl;
}

// && 表示与的关系，必须同时满足多个条件
// || 表示或的关系，只要满足其中一个条件即可
template <typename T>
requires std::is_integral_v<T> && std::is_unsigned_v<T>
void print_uint(T t)
{
    std::cout << "print_uint: " << t << std::endl;
}

// 支持通过reguires定义组合约束，并通过requires来进行管理函数和类
template <typename T>
requires requires(T a, typename T::value_type v) {
    { a.push_back(v) } -> std::convertible_to<void>;
    { a.size() } -> std::convertible_to<std::size_t>;
    { a.begin() } -> std::convertible_to<typename T::iterator>;
    { a.end() } -> std::convertible_to<typename T::iterator>;
}
void process_container(T &container) {
    for (auto con_iter = container.begin(); con_iter != container.end(); ++con_iter) {
        std::cout << *con_iter << " ";
    }
    std::cout << std::endl;
}

// 对于支持重载的函数或类的构造函数，约束作为特化，匹配时会优先执行
template <typename T>
class demo
{
public:
    demo(T t): m_t(t) {
        std::cout << "demo: " << m_t << std::endl;
    }

    demo(T t) requires requires(T t) {
        { t.size() } -> std::convertible_to<std::size_t>;
    }: m_t(t) {
        std::cout << "demo requires"<< std::endl;
    }
private:
    T m_t;
};

int main(int argc, char const *argv[])
{
    print_int(10);

    print_uint(static_cast<unsigned int>(10));

    print_n<5>();

    std::vector<int> vec = {1, 2, 3};
    process_container(vec);

    demo<int> d1(10);
    demo<std::vector<int>> d2(vec);
    return 0;
}
```

### concepts

concept是一种用于描述类型要求的语法，可以将requires表达式命名为概念，在具有相同功能的基础上，实现了对于requires表达式的复用。同理，concept定义的变量也可以是bool类型或者requires表达式。

concept的格式如下所示。

```cpp
// 通过concept定义一个概念
template <typename T>
concept [variable] = [requires表达式];

// 在模板类中使用concept
template <[variable] T>
class {};

// 在模板函数中使用concept
template <[variable] T>
func()
```

具体示例如下所示。

```cpp
#include <iostream>
#include <type_traits>
#include <concepts>
#include <vector>
#include <initializer_list>

template <typename T>
concept is_for_extend = requires(T t) {
    { t.begin() } -> std::same_as<typename T::iterator>;
    { t.end() } -> std::same_as<typename T::iterator>;

    // 添加operator++和++operator
    ++std::declval<typename T::iterator>(); // 前置operator++
    std::declval<typename T::iterator>()++; // 后置operator++
    
    // 支持解引用operator*
    *std::declval<typename T::iterator>(); // 解引用operator*
};

template <is_for_extend T>
void print_container(T t) {
    for (const auto& e : t) {
        std::cout << e << " ";
    }
    std::cout << std::endl;
}

template <typename T>
concept is_printable = requires(T t) {
    { std::cout << t } -> std::same_as<std::ostream&>;
};

template <is_printable T>
class print_class {
public:
    print_class(T t): m_t(t) {}
    void print() {
        std::cout << m_t << std::endl;
    }
private:
    T m_t;
};

int main(int argc, char const *argv[]) 
{
    std::vector<int> v = {1, 2, 3, 4, 5};
    print_container(v);

    print_class<int> val(3);
    val.print();
    return 0;
}
```

## coroutines

## format

format是C++20引入的标准库头文件，提供了一种类型安全的字符串格式化方式，使用起来更加方便和简单。主要是std::format库，主要实现类型安全且灵活的字符串格式化，替代传统的printf系列函数和std::stringstream。

format参考网址： https://en.cppreference.com/w/cpp/header/format.html

其支持的主要方法如下。

| 方法 | 描述 |
| --- | --- |
| format | 格式化字符串，返回一个std::string对象 |
| format_to | 格式化字符串并写入到指定的输出迭代器|
| format_to_n | 格式化字符串并写入到指定的输出迭代器，最多写入n个字符，返回一个std::format_to_n_result对象 |
| formatted_size | 获取格式化字符串的长度 |
| runtime_format | 运行时格式化字符串(C++26) |
| vformat | 格式化字符串并返回一个std::string对象 |
| vformat_to | 运行时格式化字符串并写入到指定的输出迭代器，最多写入n个字符 |
| visit_format_args | 访问格式化字符串中的参数(C++26 deprecated) |
| make_format_args/make_wformat_args | 创建一个std::format_args对象 |

其中format_to_n返回一个std::format_to_n_result对象，包含了写入的字符数和指向输出迭代器的指针。

```cpp
// 返回格式化字符串写入的字符数和指向输出迭代器的指针
// out: 指向输出迭代器的指针
// size: 写入的字符数
template< class OutputIt >
struct format_to_n_result {
    OutputIt out;
    std::iter_difference_t<OutputIt> size;
};
```

具体示例如下所示。

```cpp
#include <iostream>
#include <format>

int main(int argc, char const *argv[])
{
    // 格式化字符串
    std::string s = std::format("{} {}", 1, 2);
    std::cout << s << std::endl;

    // 格式化字符串并写入到指定的输出迭代器
    std::string buffer;
    std::format_to(std::back_inserter(buffer), "{} {}", 1, 2);
    std::cout << buffer << std::endl;

    // 获取格式化字符串的长度
    std::cout << "formated_size: " << std::formatted_size(std::locale("en_US.UTF-8"), "formated_size: {}", "test") << std::endl;

    // // 运行时格式化字符串
    // std::string s2 = std::runtime_format("{} {}", 1, 2);
    // std::cout << s2 << std::endl;

    // 格式化字符串并写入到指定的输出迭代器，最多写入n个字符
    std::string buffer2;
    std::vformat_to(std::back_inserter(buffer2), "vformat_to: {}, {}", std::make_format_args("test", "hello"));
    std::cout << buffer2 << std::endl;
    return 0;
}
```

## module

## ranges

在C++20中，ranges是一个重要的特性，它提供了一种更简洁、更强大的方式来处理容器和序列。ranges库引入了一系列新的概念和算法，使得代码更加直观和易于理解

ranges参考网址： <https://en.cppreference.com/w/cpp/header/ranges.html>

### 范围(Ranges)

ranges 库的核心是范围（Ranges）的概念。范围是一个可以迭代的对象，它可以是一个容器（如std::vector、std::list等），也可以是一个视图（View）。视图是一种轻量级的对象，它不拥有数据，而是对现有数据的一种引用或转换。

ranges支持的方法如下。

| 方法 | 描述 |
| --- | --- |
| begin | 返回范围的开始迭代器 |
| end | 返回范围的结束迭代器 |
| cbegin | 返回范围的常量开始迭代器 |
| cend | 返回范围的常量结束迭代器 |
| rbegin | 返回范围的反向开始迭代器 |
| rend | 返回范围的反向结束迭代器 |
| crbegin | 返回范围的常量反向开始迭代器 |
| crend | 返回范围的常量反向结束迭代器 |
| reserve_hint | 为范围保留足够的空间，以避免重新分配 |
| size | 返回范围的元素数量 |
| ssize | 返回范围的元素数量（有符号整数） |
| empty | 判断范围是否为空 |
| data | 返回范围的底层数据指针 |
| cdata | 返回范围的常量底层数据指针 |

2. 视图(Views)

视图是 ranges 库中的一个重要概念。
视图是一种轻量级的对象，它不拥有数据，而是对现有数据的一种引用或转换。
视图可以通过各种算法和操作符进行组合，形成复杂的操作链。

3. 算法(Algorithms)
ranges 库还引入了一系列新的算法，这些算法可以直接作用于范围，而不需要显式地指定迭代器。
这些算法的名称与标准库中的算法类似，但它们接受范围作为参数，而不是迭代器。

4. ranges 库引入了管道操作符（|），它允许你将多个视图和算法组合在一起，形成一个操作链。管道操作符使得代码更加简洁和易于理解。
