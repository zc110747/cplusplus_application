# cpluspuls 2020~新特性

本节中主要包含C++20~之后引入的新特性，按照功能划分为四大类，分别是概念和约束、范围和视图、module导入和协程应用；除上述特性外，还包含增加的特殊的功能模块，如std::formtat、std::span等，都将在本节进行介绍，具体如下所示。

- [compare](#compare)
- [concepts](#concepts)
  - [requires](#requires)
  - [concept](#concept)
- [coroutines](#coroutines)
- [format](#format)
- [module](#module)
- [ranges](#ranges)

## compare

compare是C++20引入用于比较两个对象的大小关系的头文件库，它提供了`三向比较符<=>`来比较两个对象的大小关系，避免了传统的比较运算符（如 <、>、<=、>=）可能带来的未定义行为。

对于`三向比较符<=>`，支持三种不同的返回结果，负数表示小于、0表示等于、整数表示大于。

对于其返回类型包括std::strong_ordering、std::weak_ordering和std::partial_ordering三种排序方式。

- std::strong_ordering(强排序)，用于具有一致全序关系的值，例如整数
- std::weak_ordering(弱排序)，允许不同对象在比较上等价，但它们不一定在对象本身的意义上相等
- std::partial_ordering(部分排序)，允许某些值无法比较。例如IEEE浮点数中的NaN与普通浮点数比较时，会产生unordered

## concepts

concepts，又称概念，是C++20引入的重要特性，用于描述模板类型是否符合某些条件的特性。

关于概念的具体实现，又涉及两个关键字concept和requires。其中concept类似using，用于定义一个概念类型，可通过bool类型或者requires表达式来指定类型是否符合要求。复杂的概念，往往是requires和bool类型，一条或者多条的组合。

### requires

requires，又称约束，是用于模板或auto类型参数的一种语法，用于指定类型参数必须满足的条件。

约束可以是一个bool类型的表达式，也可以是一个或一组requires表达式，用于指定类型参数必须满足的条件，其格式如下所示。

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

1. 如果是bool型表达式，必须是初等表达式或者带括号的任意表达式
2. 使用||或&&连接多个表达式，分别表示或、与的关系
3. 对于单个类或者函数时，必须满足所有的要求，不然会按照顺序进行检查，直到找到第一个不符合要求的表达式，然后编译错误
4. 用于约束重载函数或类的构造函数时，约束作为对模板的特化，会拥有更优先的匹配度，优先执行

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

### concept

concept是一种用于描述类型要求的语法，可以将requires表达式命名为概念，在具有相同功能的基础上，实现了对于requires表达式的复用。

concept定义的变量也可以是bool类型或者requires表达式，具体格式如下所示。

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

Coroutines(协程)是C++20引入的重要特性。它允许一个函数暂停执行，保留当前状态，之后再从暂停的位置继续执行。

对于C++协程，主要由三个关键字实现，具体如下所示。

| 关键字 | 功能说明 | 详细说明 |
| --- | --- | --- |
| co_await | 等待一个操作 | 暂停当前协程，等待某个操作满足恢复条件，再继续执行 |
| co_yield | 产生一个值并暂停 | 常用于生成器（Generator），每次调用方请求下一个值时，协程继续运行直到产生下一个值。 |
| co_return | 结束协程 | 表示协程执行结束，可以通过协程的返回对象传递最终结果 | 

关于协程的应用示例如下所示。

```c
#include <coroutine>
#include <iostream>

struct Generator {
    struct promise_type {
        int value{};

        Generator get_return_object() {
            return Generator{
                std::coroutine_handle<promise_type>::from_promise(*this)
            };
        }

        std::suspend_always initial_suspend() noexcept {
            return {};
        }

        std::suspend_always final_suspend() noexcept {
            return {};
        }

        std::suspend_always yield_value(int v) noexcept {
            value = v;
            return {};
        }

        void return_void() noexcept {}

        void unhandled_exception() {
            std::terminate();
        }
    };

    std::coroutine_handle<promise_type> handle;

    explicit Generator(
        std::coroutine_handle<promise_type> h
    ) : handle(h) {}

    ~Generator() {
        if (handle) {
            handle.destroy();
        }
    }

    bool next() {
        if (!handle || handle.done()) {
            return false;
        }

        handle.resume();
        return !handle.done();
    }

    int value() const {
        return handle.promise().value;
    }
};

Generator numbers()
{
    co_yield 10;
    co_yield 20;
    co_yield 30;
}

int main()
{
    auto gen = numbers();

    while (gen.next()) {
        std::cout << gen.value() << '\n';
    }
}
```

## format

format是C++20引入的标准库头文件，提供了一种类型安全的字符串格式化方式，使用起来更加方便和简单。

format的应用主要是std::format库，实现类型安全且灵活的字符串格式化，替代传统的printf函数和std::stringstream。

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

`<ranges>`是C++20`引入的重要标准库组件，它将范围、迭代器、视图、约束算法和惰性求值结合起来，让序列处理更加直观、可组合。

ranges各版本能力说明。

| 标准版本 | 相关能力 |
| --- | --- |
| C++20 | 基础 ranges、views、范围算法和 concepts |
| C++23 | 增加 views::zip、views::enumerate 等实用功能，以及更多范围设施 |
| C++26 | 引入范围大小提示相关设施，例如 approximately_sized_range 和 ranges::reserve_hint |

Ranges包含功能说明如下所示。

- Ranges：范围，用统一的方式描述可以通过迭代器访问的元素序列，例如 vector、数组、字符串和视图。
- Views：视图，对已有数据进行过滤、转换、切片等操作。多数视图不拥有底层元素，并且可以惰性求值。
- Range algorithms：范围算法，提供std::ranges::sort、std::ranges::find等处理范围数据的算法
- Concepts：范围约束，提供std::ranges::range、std::ranges::input_range、std::ranges::random_access_range等用于描述类型具备哪些迭代能力。
- Pipeline：管道组合，使用 | 将视图操作连接起来，形成易于阅读的序列处理流程。

下面进行分块的详细说明。

### 范围（Ranges）

范围是一种能够通过 begin 和 end 迭代器或哨兵来表示元素序列的对象。

具体示例如下。

```cpp
#include <iostream>
#include <ranges>
#include <vector>

int main()
{
    std::vector<int> values{10, 20, 30, 40, 50};

    for (int value : values)
    {
        std::cout << value << '\n';
    }

    // A range can also be passed directly to a ranges algorithm.
    std::ranges::sort(values);
}
```

这里的 values 是一个范围。它可以被遍历，也可以直接传给 std::ranges::sort。

注意，范围不一定是容器：std::views::iota(1, 6) 生成的整数序列也是范围。

范围相关的访问接口如下。

| 接口 | 功能 | 说明 |
| --- | --- | --- |
| begin | 获取起始迭代器 | 通常由 std::ranges::begin 调用 |
| end | 获取结束迭代器或哨兵 | 结束位置不一定与起始迭代器同类型 |
| cbegin/cend | 常量迭代访问 | 常见于容器成员接口 |
| rbegin/rend | 反向迭代访问 | 需要底层类型支持反向迭代 |
| crbegin/crend | 常量反向迭代访问 | 常见于容器成员接口 |
| size | 获取元素数量 | 并非所有范围都支持常数时间求大小 |
| ssize | 获取有符号形式的大小 | C++20 的范围访问函数 |
| empty | 判断范围是否为空 | 适用于满足相应约束的范围 |
| data | 获取连续存储数据的指针 | 只有连续范围才适用 |
| cdata | 获取常量数据指针 | 适用于连续范围 |
| reserve_hint | 提供预计大小提示 | 不是C++20的通用范围访问接口 |

这里需要特别区分：std::ranges::begin 等是 C++20 的范围访问定制点，而cbegin、crbegin等通常是容器成员函数或其他相应接口，不能认为它们都是 std::ranges 下同名的 C++20 函数。

### 视图（Views）

视图是 Ranges 中最有代表性的特性之一。它可以在不必创建新的完整结果容器的情况下，对已有序列进行过滤、转换、截取和组合。

- 过滤视图：std::views::filter只保留符合条件的元素。

```cpp
#include <iostream>
#include <ranges>
#include <vector>

int main()
{
    std::vector<int> values{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    auto evenValues = values | std::views::filter(
        [](int value) {
            return value % 2 == 0;
        });

    for (int value : evenValues)
    {
        std::cout << value << ' ';
    }

    std::cout << '\n';
}
```

evenValues 并不是一个新建的 vector，而是一个过滤视图。遍历它时，会根据条件筛选原始数据。

- 转换视图：std::views::transform对每个元素执行转换操作。

```cpp
#include <iostream>
#include <ranges>
#include <vector>

int main()
{
    std::vector<int> values{1, 2, 3, 4, 5};

    auto squares = values | std::views::transform(
        [](int value) {
            return value * value;
        });

    for (int value : squares)
    {
        std::cout << value << ' ';
    }

    std::cout << '\n';
}
```

- 其他常见视图

| 视图 | 功能 | 示例 |
| --- | --- | --- |
| views::all | 将对象包装为视图或视图适配器 | views::all(values) |
| views::filter | 按条件过滤 | views::filter(pred) |
| views::transform | 转换元素 | views::transform(fn) |
| views::take | 取前 N 个元素 | views::take(5) |
| views::drop | 跳过前 N 个元素 | views::drop(2) |
| views::reverse | 反向遍历 | views::reverse |
| views::iota | 生成递增序列 | views::iota(1, 10) |
| views::join | 展平嵌套范围 | views::join |
| views::split | 按分隔符拆分范围 | views::split(',') |

其中 views::iota、filter、transform、take、drop 等适配器可用于构建惰性序列。

### 管道操作符`|`

管道可以把多个视图操作连接起来。前一个操作产生的范围，会作为后一个操作的输入。

例如：生成 1 到 20 的整数，筛选出偶数，计算平方，再取前三个结果。

```cpp
#include <iostream>
#include <ranges>

int main()
{
    auto result =
        std::views::iota(1, 21)
        | std::views::filter([](int n) {
              return n % 2 == 0;
          })
        | std::views::transform([](int n) {
              return n * n;
          })
        | std::views::take(3);

    for (int value : result)
    {
        std::cout << value << ' ';
    }

    std::cout << '\n';
}
```

需要注意，管道操作符主要用于支持管道组合的视图和适配器。普通的 std::ranges::sort 等算法通常是直接调用，而不是直接放到管道右侧。

### 范围算法（Ranges Algorithms）

C++20 在传统`<algorithm>`中已有算法的基础上，引入了 std::ranges 版本。它们可以直接接受范围，不必手动传入 begin() 和 end()。

- 传统算法与范围算法对比

```cpp
#include <algorithm>
#include <iostream>
#include <ranges>
#include <vector>

int main()
{
    std::vector<int> values{5, 2, 4, 1, 3};

    // Traditional algorithm
    std::sort(values.begin(), values.end());

    // Ranges algorithm
    std::ranges::sort(values);

    for (int value : values)
    {
        std::cout << value << ' ';
    }

    std::cout << '\n';
}
```

两种排序调用都能工作，但 std::ranges::sort(values) 更简洁。

- 常见范围算法

| 算法 | 功能 |
| --- | --- |
| std::ranges::sort | 排序
| std::ranges::find	| 查找元素
| std::ranges::count | 统计指定值的出现次数
| std::ranges::count_if | 统计符合条件的元素数
| std::ranges::copy | 复制元素
| std::ranges::transform | 将转换结果写入目标范围
| std::ranges::for_each | 对每个元素执行操作
| std::ranges::all_of | 判断所有元素是否满足条件
| std::ranges::any_of | 判断是否存在满足条件的元素
| std::ranges::none_of | 判断是否没有元素满足条件
| std::ranges::remove_if |将符合条件的元素移到逻辑移除区域
| std::ranges::lower_bound | 在有序范围中查找下界

注意：std::ranges::transform 是算法，不等同于 std::views::transform。前者通常把结果写入目标位置，后者则创建一个按需计算的视图。

### Ranges Concepts：范围概念约束

Ranges 不仅提供算法，也引入了 C++20 Concepts，用于在编译期描述一个类型支持哪些操作。

例如，模板函数可以明确要求参数是一个范围，而不必等到函数体内使用迭代器时才发现类型不符合要求。

```cpp
#include <iostream>
#include <ranges>
#include <vector>

template<std::ranges::input_range Range>
void print_range(const Range& values)
{
    for (const auto& value : values)
    {
        std::cout << value << ' ';
    }

    std::cout << '\n';
}

int main()
{
    std::vector<int> values{10, 20, 30};
    print_range(values);

    auto sequence = std::views::iota(1, 4);
    print_range(sequence);
}
```

常见Range Concepts主要含义.

- std::ranges::range: 支持 begin 和 end
- std::ranges::input_range: 支持输入式遍历
- std::ranges::forward_range: 支持多次向前遍历
- std::ranges::bidirectional_range: 支持向前和向后遍历
- std::ranges::random_access_range: 支持随机访问
- std::ranges::contiguous_range: 元素在内存中连续存储
- std::ranges::sized_range: 可以获得范围大小
- std::ranges::view: 满足视图概念要求

这些概念描述的是能力，而不是具体容器类型。例如，std::vector 通常满足 random_access_range 和 contiguous_range，而 std::list 支持双向迭代，但不支持随机访问。

### 惰性求值与性能

这是理解 Views 时最重要的部分之一。

考虑下面的代码：

auto result = values
    | std::views::filter(is_valid)
    | std::views::transform(convert);


通常不会立即把所有结果计算出来并存储到新容器中。创建视图主要是在构建一套遍历规则，实际筛选和转换通常发生在遍历时。

优点：

- 减少不必要的中间容器和内存分配。
- 可以组合多种数据处理步骤。
- 对大型序列或按需生成的序列很有用。

但惰性求值不代表一定更快。反复遍历视图时，过滤和转换操作可能重复执行；某些视图还会缓存部分状态，因此不能一概而论，需要特别注意视图的生命周期。

```cpp
auto make_view()
{
    std::vector<int> values{1, 2, 3};

    return values | std::views::filter(
        [](int n) { return n > 1; });
}
```

这段代码存在生命周期问题：返回的过滤视图引用局部 values，而 values 在函数返回时已经销毁。随后遍历该视图会产生未定义行为。

因此，使用视图时要确认底层数据仍然有效。某些适配器可以拥有或保存底层范围，但不能假定所有视图都会自动延长数据的生命周期。
