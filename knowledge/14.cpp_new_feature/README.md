<<<<<<< HEAD
# cpluspuls 2020~新特性

本节中主要包含C++20~之后引入的新特性，按照功能划分为四大类，分别是概念和约束、范围和视图、module导入和协程应用；除上述特性外，还包含增加的特殊的功能模块，如std::formtat、std::span等，都将在本节进行介绍，具体如下所示。

- [barrier](#barrier)
- [concepts](#concepts)
  - [requires](#requires)
  - [concept](#concept)
- [coroutines](#coroutines)
- [format](#format)
- [latch](#latch)
- [module](#module)
- [ranges](#ranges)
- [source_location](#source_location)
- [span](#span)

## barrier

std::barrier是C++20引入的同步原语，定义在<barrier>头文件，用于多线程同步，确保一组线程在特定点同步，所有线程到达该点后才能继续执行后续操作。

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
=======
# cpluspuls 2020~2023新特性

关于g++的新特性，使用gcc 15.2版本进行测试。

## concepts

concepts和reguires是C++20引入的重要特性，用于描述模板类型是否符合某些条件的特性。其中concepts是一种用于描述类型要求的语法，而requires则是用于在模板定义中指定这些要求的关键字。

## barrier

std::barrier是C++23引入的标准库类，位于<barrier>头文件，用于在多线程环境中同步多个线程的执行。
>>>>>>> 30b84540516477c29747dece63510e1e03010626

## format

std::format是C++20 引入的标准库函数，位于<format>头文件，用于实现类型安全且灵活的字符串格式化，替代传统的printf系列函数和std::stringstream。
<<<<<<< HEAD

## latch

std::latch是C++20引入的同步原语，定义在<latch>头文件中，用于多线程同步，可让一组线程等待直到某个事件完成。它的工作模式是一次性的，一旦计数器减到0，就不能再重置。

具体网址: https://en.cppreference.com/w/cpp/thread/latch.html

其构造函数如下所示。

```cpp
// 构造函数，声明latch对象
// expected: 初始化计数器的数值()
constexpr explicit latch( std::ptrdiff_t expected );
```

支持方法如下。

| 方法 | 说明 |
| --- | --- |
| count_down | 减少计数器的值 |
| wait | 阻塞当前线程，直到计数器减到0 |
| try_wait | 尝试等待计数器减到0，不阻塞线程 |
| arrive_and_wait | 减少计数器的值并阻塞当前线程，直到计数器减到0 |
| max | 返回计数器的最大值 |

具体示例如下所示。

```cpp
int main(int argc, const char* argv[])
{
    constexpr int num_threads = 4;

    std::latch work_done(num_threads);
    std::vector<std::thread> threads;
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([&work_done]() {
            std::cout << "线程 " << std::this_thread::get_id() << " 开始工作..." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "线程 " << std::this_thread::get_id() << " 完成工作，计数器减 1。" << std::endl;
            work_done.count_down();
        });
    }

    work_done.wait();
    std::cout << "所有线程工作完成，主线程继续执行。" << std::endl;

    for (auto& t : threads) {
        t.join();
    }

    return 0;
}
```

## module

## ranges

在C++20中，ranges是一个重要的特性，它提供了一种更简洁、更强大的方式来处理容器和序列。ranges库引入了一系列新的概念和算法，使得代码更加直观和易于理解

1. 范围(Ranges)

ranges 库的核心是范围（Ranges）的概念。范围是一个可以迭代的对象，它可以是一个容器（如std::vector、std::list等），也可以是一个视图（View）。视图是一种轻量级的对象，它不拥有数据，而是对现有数据的一种引用或转换。

2. 视图(Views)

视图是 ranges 库中的一个重要概念。
视图是一种轻量级的对象，它不拥有数据，而是对现有数据的一种引用或转换。
视图可以通过各种算法和操作符进行组合，形成复杂的操作链。

3. 算法(Algorithms)
ranges 库还引入了一系列新的算法，这些算法可以直接作用于范围，而不需要显式地指定迭代器。
这些算法的名称与标准库中的算法类似，但它们接受范围作为参数，而不是迭代器。

4. ranges 库引入了管道操作符（|），它允许你将多个视图和算法组合在一起，形成一个操作链。管道操作符使得代码更加简洁和易于理解。

## source_location

## span

span是C++20引入的一个容器视图类，它提供了一种轻量级的、非拥有的方式来访问连续的对象序列，用于处理动态大小和静态大小数组。span提供了与数组相关的操作，如访问元素、获取大小和范围。span与原始的数组结构具有相同的内存地址，因此可以通过修改span来修改原始数据，这样就扩展了数组的访问。

其结构原型如下所示。

```cpp
// span的结构原型
// T: 元素类型
// Extent: 元素数量，默认值为std::dynamic_extent，表示动态大小
template<
    class T,
    std::size_t Extent = std::dynamic_extent
> class span;
```

具体网址: https://en.cppreference.com/w/cpp/container/span.html

常见构造方法:

```cpp
int arr[]{1, 2, 3};

// 通过数组创建span(默认动态长度)
std::span<int> span1(arr);

// 通过数组创建指定大小的span
std::span<int, 3> span2(arr);
```

主要特点：

1. 非拥有性：std::span 不拥有它所指向的数据，它只是提供了对数据的访问接口。这意味着std::span不会分配或释放内存，它只是引用了已经存在的内存。这表示具有两个特点，一是对于span的修改会同步修改原始数据，二是不能够改变span所引用数据的大小和内存，否则可能导致运行错误。
2. 连续内存访问：std::span 要求它所指向的数据是连续存储的，这使得它可以高效地进行随机访问和迭代。
3. 大小和范围：std::span 可以在编译时或运行时指定其大小，这使得它可以灵活地适应不同的使用场景。
4. 与标准库的集成：std::span 可以与标准库中的其他容器和算法无缝集成，使得代码更加简洁和高效。

- 迭代器

| 方法 | 说明 |
| --- | --- |
| begin | 返回指向第一个元素的迭代器 |
| cbegin | 创建一个只读迭代器，指向第一个元素(C++23) |
| end | 返回指向最后一个元素的迭代器 |
| cend | 创建一个只读迭代器，指向最后一个元素(C++23) |
| rbegin | 创建一个逆向迭代器，指向最后一个元素 |
| crbegin | 创建一个逆向只读迭代器，指向最后一个元素(C++23) |
| rend | 创建一个逆向迭代器，指向第一个元素 |
| crend | 创建一个逆向只读迭代器，指向第一个元素(C++23) |

- 方法

| 方法 | 说明 |
| --- | --- |
| front | 返回第一个元素 |
| back | 返回最后一个元素 |
| at | 返回指定索引的元素(C++26) |
| operator[] | 返回指定索引的元素 |
| data | 返回指向内容的元素指针 |
| size | 返回元素数量 |
| size_bytes | 返回元素占用的字节数 |
| empty | 判断是否为空 |
| first | 从起始索引截取指定数量的元素 |
| last | 从末尾索引截取指定数量的元素 |
| subspan | 获取子span |

std::span可以配合其它函数使用，具体如下。

| 函数 | 说明 |
| --- | --- |
| std::as_bytes | 将span转换为字节span |
| std::as_writable_bytes | 将span转换为可写字节span |
=======
>>>>>>> 30b84540516477c29747dece63510e1e03010626
