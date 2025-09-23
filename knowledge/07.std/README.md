# STD标准库函数

C++标准库是C++编程语言的一部分，它提供了一组通用的类和函数，用于处理各种常见的编程任务。标准库被组织成多个头文件，每个头文件包含一组相关的功能。

- [any](#any)
  - [any_cast](#any_cast)
- [bitset](#bitset)
  - [bit_cast](#bit_cast)
- [filesystem](#filesystem)
- [functional](#functional)
  - [function](#function)
  - [bind](#bind)
  - [invoke](#invoke)
- [locale](#locale)
- [new](#new)
- [optional](#optional)
  - [std::nullopt](#nullopt)
- [random](#random)
- [ratio](#ratio)
- [regex](#regex)
- [stream](#stream)
- [string](#string)
- [string_view](#string_view)
- [tuple](#tuple)
- [type_traits](#type_traits)
- [utility](#utility)
- [variant](#variant)

## any

std::any是C++17标准库中引入的一个类型安全的通用容器，它可以存储任意类型的值。

- 主要特点：

1. 类型安全：std::any确保存储的值的类型是已知的，并且在运行时进行类型检查。
2. 动态类型：std::any可以存储任何类型的值，包括基本类型、自定义类型、甚至是其他std::any对象。
3. 轻量级：std::any的实现通常是基于类型擦除技术，这使得它在存储和访问值时具有较低的开销。

- 成员函数

| 成员函数 | 说明 |
| --- | --- |
| operator= | 赋值运算符，用于将一个std::any对象赋值给另一个std::any对象。 |
| emplace | 用给定的参数构造一个新的值，并将其存储在std::any中。 |
| has_value | 检查std::any是否包含一个值。 |
| reset | 清除std::any中的值。 |
| swap | 交换两个std::any对象的值。 |
| type | 返回std::any中存储的值的类型。 |

### any_cast

any_cast是C++17标准库中引入的一个函数模板，用于从std::any中提取值，并将其转换为指定的类型。如果类型不匹配，会抛出std::bad_any_cast异常。

## bitset

std::bitset是C++标准库中的一个类模板，用于表示固定大小的位序列。它提供了一种方便的方式来处理二进制数据，例如位操作、位计数、位测试等。

- 格式

```cpp
// N: 位序列的大小
template <size_t N> class bitset;
```

相关网址: https://www.cplusplus.com/reference/bitset/bitset/

- 主要特点

1. 固定大小：std::bitset的大小在编译时确定，因此它适用于需要固定大小的位序列的场景。
2. 高效操作：std::bitset提供了高效的位操作，例如位设置、位清除、位翻转、位测试等。
3. 类型安全：std::bitset是类型安全的，它确保所有操作都是在位级别上进行的，避免了类型错误。
4. 易于使用：std::bitset提供了直观的接口，使得位操作变得简单易懂

- 成员函数

| 成员函数 | 说明 |
| --- | --- |
| operator[] | 访问位序列中的指定位。 |
| count | 计算位序列中设置为1的位数。 |
| size | 返回位序列的大小。 |
| test | 检查指定位是否被设置为1。 |
| any | 检查是否至少有一位被设置为1。 |
| none | 检查是否没有位被设置为1。 |
| all | 检查所有位是否都设置为1。 |
| set | 将指定位设置为1。 |
| reset | 将指定位设置为0。 |
| flip | 翻转指定位或位序列中的所有位。 |
| to_string | 将位序列转换为字符串。 |
| to_ulong | 将位序列转换为无符号整数。 |
| to_ullong | 将位序列转换为无符号长整数。 |

### bit_cast

std::bit_cast是C++20引入的一个函数模板，定义在<bit>头文件中，用于在不同类型之间进行位级别的转换。它提供了一种类型安全的方式来重新解释对象的位模式，避免了传统的类型转换（如 reinterpret_cast）可能带来的未定义行为。

std::bit_cast的特点如下所示。

1. 类型安全：确保转换的源对象和目标对象是兼容的，避免了类型转换可能带来的未定义行为。
2. 位级转换: 在位级别执行转换，将源类型的精确位表示复制到目标类型
3. 编译时安全性: 要求源类型和目标类型具有相同的大小

```cpp
#include <unistd.h>
#include <bitset>
#include <bit>
#include <iostream>

int main(int argc, char const *argv[])
{
    // bit_cast
    float f = 3.14159f;
    // 将 float 类型的 f 转换为 unsigned int 类型
    unsigned int u = std::bit_cast<unsigned int>(f);

    std::cout << "Float value: " << f << std::endl;
    std::cout << "Bit-casted unsigned int value: " << u << std::endl;

    // 将 unsigned int 类型的 u 转换回 float 类型
    float f_back = std::bit_cast<float>(u);
    std::cout << "Back to float value: " << f_back << std::endl;

    double d = 123.456;
    //unsigned int u2 = std::bit_cast<unsigned int>(d);             // 编译报错，需要长度一致
    unsigned long long u2 = std::bit_cast<unsigned long long>(d);

    return 0;
}
```

## filesystem

std::filesystem是C++17引入的标准库，提供了跨平台的文件系统操作功能，替代了传统的依赖于操作系统的文件操作函数，让文件和目录操作更加方便、安全和可移植。

std::filesystem支持一系列对于文件的操作，主要包含如下所示。

```cpp
// 路径类，用于表示目录或者文件路径
std::filesystem::path path("test_dir");

// 判断文件是否存在
std::filesystem::exists(path);

// 创建文件/目录
std::filesystem::create_directory(path);

// 删除文件/目录(无法删除非空目录)
std::filesystem::remove(path);

// 删除全部文件/目录
std::filesystem::remove_all(path);

// 遍历目录
for (auto& entry : std::filesystem::directory_iterator(path)) {
    std::cout << entry.path() << std::endl;
}

// 获取文件大小
std::filesystem::path file_path = "test_dir/test_file.txt";
std::filesystem::file_size(file_path);

// 重命名文件/目录
std::filesystem::rename(file_path, "test_dir/test_file2.txt");

// 复制文件/目录
std::filesystem::copy(file_path, "test_dir/test_file3.txt");

// 移动文件/目录
std::filesystem::rename(file_path, "test_dir2/test_file4.txt");

// 获取文件属性
std::cout << "File size: " << std::filesystem::file_size(file_path) << std::endl;
std::cout << "File type: " << std::filesystem::file_type(file_path) << std::endl;
std::cout << "File permissions: " << std::filesystem::permissions(file_path) << std::endl;
```

## functional

<functional>内部提供了一组用于函数对象相关的工具和函数。

相关网址: https://cplusplus.com/reference/functional/

其中支持一组用于管理的函数对象的工具函数，可以搭配STL库中的算法使用。

| 函数名 | 功能 |
| --- | --- |
| bit_and | 逻辑与 |
| bit_or | 逻辑或 |
| bit_xor | 逻辑异或 |
| divides | 除法 |
| equal_to | 等于 |
| greater | 大于 |
| greater_equal | 大于等于 |
| less | 小于 |
| less_equal | 小于等于 |
| logical_and | 逻辑与 |
| logical_not | 逻辑非 |
| logical_or | 逻辑或 |
| minus | 减法 |
| modulus | 取余 |
| multiplies | 乘法 |
| negate | 负数 |
| not_equal_to | 不等于 |
| plus | 加法 |

- 举例说明

```cpp
#include <iostream>
#include <functional>
#include <algorithm>

int main (int argc, char *argv[]) 
{
  int numbers[]={20,40,50,10,30};
  std::sort (numbers, numbers+5, std::greater<int>());
  for (int i=0; i<5; i++) {
    std::cout << numbers[i] << ' ';
  }
  std::cout << '\n';
  return 0;
}
```

### function

std::function是C++11引入的一个通用的多态函数包装器，定义在<functional>头文件中。它可以包装任何可调用的目标，包括普通函数、Lambda表达式、函数对象、成员函数等。

- 函数原型

```cpp
template <class R, class... Args>
class function<R(Args...)>;
```

- 举例说明

```cpp
#include <iostream>
#include <functional>

int add(int a, int b) {
    return a + b;
}

class func_obj {
public:
    int operator()(int a, int b) {
        return a + b;
    }
};

int main(int argc, char const *argv[]) 
{   
    // 函数指针
    std::function<int(int, int)> func = add;
    int ret = func(1, 2);
    std::cout << "func: " << ret << std::endl;

    // 函数对象
    std::function<int(int, int)> func2 = func_obj();
    ret = func2(1, 2);
    std::cout << "func2: " << ret << std::endl;

    // 匿名函数
    std::function<int(int, int)> func3 = [](int a, int b) {
        return a + b;
    };
    ret = func3(2, 3);
    std::cout << "func3: " << ret << std::endl;
}
```

### bind

std::bind是C++11引入的一个函数模板，定义在<functional>头文件中。它用于创建函数对象，通过将参数绑定到可调用对象来创建新的可调用对象。

- 函数原型

```cpp
template <class F, class... Args>
std::function<invoke_result_t<F, Args...>(Args...)> bind(F&& f, Args&&... args);
```

- 举例说明

```cpp
#include <iostream>
#include <functional>

int add(int a, int b) {
    return a + b;
}

void data_process(int &a, int &b) {
    a += 1;
    b += 1;
}

template <typename T>
class func_object
{
    public:
        T operator()(T a, T b) const {
            return a * b;
        }
};

void print_value(int a, int b) {
    std::cout << "a: " << a << " b: " << b << std::endl;
}

class object {
public:
    void method(int x) {
        std::cout << "Value: " << x << std::endl;
    }
};

int main(int argc, char const *argv[])
{
    // 绑定普通函数
    auto func1 = std::bind(add, std::placeholders::_1, std::placeholders::_2);
    auto ret = func1(1, 2);
    std::cout << "ret: " << ret << std::endl;

    // 绑定变量(引用绑定)
    int a = 1;
    int b = 2;
    std::cout << "a: " << a << " b: " << b << std::endl;
    auto func2 = std::bind(data_process, std::placeholders::_1, std::placeholders::_2);
    func2(a, b);
    std::cout << "a: " << a << " b: " << b << std::endl;
    func2(std::ref(a), std::ref(b));
    std::cout << "a: " << a << " b: " << b << std::endl;
    auto func3 = std::bind(data_process, std::ref(a), std::ref(b));
    func3();
    std::cout << "a: " << a << " b: " << b << std::endl;

    // 参数排序
    auto func4 = std::bind(print_value, std::placeholders::_2, std::placeholders::_1);
    func4(1, 2);
    std::cout << "a: " << a << " b: " << b << std::endl;

    // 绑定成员函数
    object obj;
    auto func5 = std::bind(&object::method, std::ref(obj), std::placeholders::_1);
    func5(10);
}
```

### invoke

std::invoke是C++17引入的一个标准库函数模板，位于 <functional> 头文件中。它提供了一种统一的方式来调用对象，包括函数、函数指针、成员函数指针、成员对象指针、lambda表达式等。

- 函数原型

```cpp
template <class F, class... Args>
invoke_result_t<F, Args...> invoke(F&& f, Args&&... args);
```

- 举例说明

```cpp
#include <functional>
#include <iostream>

int add(int a, int b) {
    return a + b;
}

class object {
public:
    int value_{0};
    int *ptr_{nullptr};

    object(int value): value_(value) {
        ptr_ = new int(value_);
    }
    ~object() {
        delete ptr_;
    }
    int add(int a, int b) {
        return a + b;
    }
};

int main(int argc, char *argv[])
{
    // 函数
    std::cout << std::invoke(add, 1, 2) << std::endl;

    // lambda
    auto ret = std::invoke([](int a, int b) { return a + b; }, 1, 2);
    std::cout << ret << std::endl;

    // 对象成员函数
    object obj(1);
    std::cout << std::invoke(&object::add, obj, 1, 2) << std::endl;

    // 对象成员变量
    std::cout << std::invoke(&object::value_, obj) << std::endl;

    // 指针成员变量
    std::cout << *std::invoke(&object::ptr_, obj) << std::endl;
}
```

## locale

std::locale 是 C++ 标准库中的一个类，用于表示特定的本地化环境

locale -a查看支持的指令集

主要特点：
1. 本地化支持：std::locale 提供了对不同地区和语言的本地化支持，使得程序能够适应不同的用户群体。
2. 可定制性：可以通过创建自定义的 std::locale 对象来定制特定的本地化设置。
3. 全局和局部设置：可以在全局范围内设置默认的 std::locale，也可以在局部范围内使用特定的 std::locale。
4. 与标准库组件集成：std::locale 与许多标准库组件（如 std::cout、std::cin、std::string 等）集成，使得这些组件能够自动适应本地化设置

## new

new，delete, new[]，delete[]

new 操作符，它用于动态分配内存
delete 操作符，它用于释放动态分配的内存
new[] 操作符，它用于动态分配数组
delete[] 操作符，它用于释放动态分配的数组

new 操作符的语法如下：
pointer = new type;
pointer = new type(initializer);

delete 操作符的语法如下：
delete pointer;

new[] 操作符的语法如下：
pointer = new type[size];

delete[] 操作符的语法如下：
delete[] pointer;

## optional

std::optional是C++17引入的一个标准库类，用于表示一个可能包含值的对象。它类似于其他语言中的Maybe或Option类型。std::optional可以用来避免使用空指针或特殊值（如 -1 或 nullptr来表示缺失值。std::optional 提供了一种更安全、更清晰的方式来处理可能缺失的值，避免了使用特殊值或空指针带来的潜在问题。

- 成员函数

| 函数名 | 说明 |
| --- | --- |
| emplace | 在 std::optional 对象中构造一个值 |
| has_value | 判断 std::optional 对象是否包含值 |
| reset | 将 std::optional 对象中的值重置为未定义状态 |
| swap | 交换两个 std::optional 对象中的值 |
| value | 返回 std::optional 对象中包含的值 |
| value_or(default_value) | 返回std::optional对象中包含的值，如果std::optional对象不包含值，则返回默认值default_value |

### nullopt

std::nullopt是一个特殊常量，用于表示 std::optional对象不包含任何值（空状态）。

其具有以下特性：

1. 类型安全 - 避免使用空指针或特殊值
2. 明确语义 - 清晰地表达"可能有值，可能没有"的概念
3. 异常安全 - 提供安全的访问方法
4. 性能优化 - 避免不必要的对象构造

举例说明。

```cpp
#include <iostream>
#include <optional>

std::optional<int> over_zero(int value) {
    if (value > 0) {
        return value;
    } else {
        return std::nullopt;
    }
}

int main(int argc, char const *argv[])
{
    std::optional<int> result = over_zero(-1);
    if (result.has_value()) {
        std::cout << "result: " << result.value() << std::endl;
    } else {
        std::cout << "result is null" << std::endl;
    }
    std::cout << "result value_or: " << result.value_or(0) << std::endl;

    result = over_zero(1);
    if (result.has_value()) {
        std::cout << "result: " << result.value() << std::endl;
    } else {
        std::cout << "result is null" << std::endl;
    }
    std::cout << "result value_or: " << result.value_or(0) << std::endl;
}
```

## random

std::random是C++标准库中的一个组件，用于生成伪随机数。它提供了多种随机数生成器和分布，以满足不同场景的需求。

相关网址: https://cplusplus.com/reference/random/

对于随机数发生器，主要定义如下。

### random_device

std::random_device是C++ 标准库的一个类，用于生成非确定性的随机数。它通常用于初始化随机数生成器的种子，以确保每次程序运行时生成的随机数序列都是不同的。

支持函数如下。

| 函数名 | 说明 |
| --- | --- |
| operator() | 返回一个非确定性随机数 |
| min() | 返回生成器支持的最小值 |
| max() | 返回生成器支持的最大值 |
| entropy() | 返回生成器的熵值 |

具体示例如下所示。

```cpp
#include <iostream>
#include <random>

int main(int argc, char const *argv[])
{
    std::random_device rd;
    std::cout << "rd: " << rd() << std::endl;           // 生成一个非确定性随机数
    std::cout << "rd min: " << rd.min() << std::endl;   // 返回生成器支持的最小值
    std::cout << "rd max: " << rd.max() << std::endl;   // 返回生成器支持的最大值
    std::cout << "rd entropy: " << rd.entropy() << std::endl;   // 返回生成器的熵值

    return 0;
}
```

### pseudo_random

在随机数种子random_device种子得基础上，C++定义了一系列伪随机数发生器，通过算法来控制生成相应得随机数。主要得随机数发生器有：

1. std::minstd_rand: 线性同余生成器，速度快，但随机性较差
2. std::minstd_rand0: 线性同余生成器，与std::minstd_rand相同，但使用不同的初始化参数
3. std::mt19937: 32位Mersenne Twister随机数生成器, 周期长，随机性好。
4. std::mt19937_64: 64位Mersenne Twister随机数生成器。
5. std::ranlux24: 随机数发生器，速度慢，但随机性高。
6. std::ranlux24_base: 随机数发生器，基于ranlux24，但速度更快。
7. std::ranlux48: 随机数发生器，速度慢，但随机性高。
8. std::ranlux48_base: 随机数发生器，基于ranlux48，但速度更快。
9. std::knuth_b: 随机数发生器，基于Knuth算法，但速度慢。
10. std::default_random_engine: 默认的随机数生成器，通常基于Mersenne Twister算法。

随机数发生器的使用方法如下。

```cpp
#include <iostream>
#include <random>

int main(int argc, char const *argv[])
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::cout << "gen: " << gen() << std::endl;
    std::cout << "gen.min: " << gen().min() << std::endl;
    std::cout << "gen.max: " << gen().max() << std::endl;

    return 0;
}
```

### distribution

在获取随机数值得基础上，C++支持通过算法管理分布，以满足不同场景得需求。主要支持得随机数发生器如下所示。

1. std::uniform_int_distribution: 均匀分布，生成一个指定范围内的整数
2. std::uniform_real_distribution: 均匀分布，生成一个指定范围内的实数
3. std::normal_distribution: 正态分布，生成一个服从正态分布的实数
4. std::bernoulli_distribution: 伯努利分布，生成一个服从伯努利分布的布尔值
5. std::binomial_distribution: 二项分布，生成一个服从二项分布的整数
6. std::geometric_distribution: 几何分布，生成一个服从几何分布的整数
7. std::negative_binomial_distribution: 负二项分布，生成一个服从负二项分布的整数
8. std::poisson_distribution: 泊松分布，生成一个服从泊松分布的整数
9. std::exponential_distribution: 指数分布，生成一个服从指数分布的实数
10. std::gamma_distribution: 伽马分布，生成一个服从伽马分布的实数
11. std::weibull_distribution: 韦伯分布，生成一个服从韦伯分布的实数
12. std::extreme_value_distribution: 极值分布，生成一个服从极值分布的实数
13. std::lognormal_distribution: 对数正态分布，生成一个服从对数正态分布的实数
14. std::chi_squared_distribution: 卡方分布，生成一个服从卡方分布的实数
15. std::cauchy_distribution: 柯西分布，生成一个服从柯西分布的实数
16. std::fisher_f_distribution: 费舍尔-费雪分布，生成一个服从费舍尔-费雪分布的实数
17. std::student_t_distribution: 学生t分布，生成一个服从学生t分布的实数
18. std::discrete_distribution: 离散分布，生成一个服从离散分布的整数
19. std::piecewise_constant_distribution: 离散分布，生成一个分段常数分布的整数
20. std::piecewise_linear_distribution: 离散分布，生成一个分段线性分布的整数

具体示例如下所示。

```cpp
#include <iostream>
#include <random>

int main(int argc, char const *argv[])
{
    std::random_device rd;
    std::mt19937 gen(rd());

    // 创建一个均匀分布的整数分布(大部分情况只需要使用满足整数分布和浮点分布得即可)
    std::uniform_int_distribution<> dis(1, 100);
    std::cout << "uniform_int_distribution() = " << dis(gen) << std::endl;

    return 0;
}
```

## ratio

std::ratio是C++11 引入的一个模板类，定义在 <ratio> 头文件中，用于在编译时表示有理数。它主要用于处理编译时的分数运算，比如时间单位的转换、频率的计算等，能在编译阶段完成分数的化简和计算，提高程序的性能和安全性。

相关网址如下：https://cplusplus.com/reference/ratio/

具体原型如下所示。

```cpp
// N表示分子，D表示分母
template <intmax_t N, intmax_t D = 1> class ratio;
```

支持的操作如下。

| 函数 | 描述 |
| --- | --- |
| std::ratio_add | 计算两个比例的和 |
| std::ratio_subtract | 计算两个比例的差 |
| std::ratio_multiply | 计算两个比例的积 |
| std::ratio_divide | 计算两个比例的商 |
| std::ratio_equal | 比较两个比例是否相等 |
| std::ratio_not_equal | 比较两个比例是否不相等 |
| std::ratio_less | 比较两个比例是否小于 |
| std::ratio_less_equal | 比较两个比例是否小于等于 |
| std::ratio_greater | 比较两个比例是否大于 |
| std::ratio_greater_equal | 比较两个比例是否大于等于 |

具体示例如下。

```cpp
#include <iostream>
#include <ratio>

int main(int argc, char const *argv[])
{
    using r1 = std::ratio<1, 2>;
    using r2 = std::ratio<2, 3>;

    std::cout << "r1: " << r1::num << "/" << r1::den << std::endl;

    // 计算两个比例的和、差、积、商
    using sum = std::ratio_add<r1, r2>;
    using diff = std::ratio_subtract<r1, r2>;
    using prod = std::ratio_multiply<r1, r2>;
    using quot = std::ratio_divide<r1, r2>;

    std::cout << "sum: " << sum::num << "/" << sum::den << std::endl;
    std::cout << "diff: " << diff::num << "/" << diff::den << std::endl;
    std::cout << "prod: " << prod::num << "/" << prod::den << std::endl;
    std::cout << "quot: " << quot::num << "/" << quot::den << std::endl;

    using eq = std::ratio_equal<r1, r2>;
    std::cout << "eq: " << eq::value << std::endl;

    using neq = std::ratio_not_equal<r1, r2>;
    std::cout << "neq: " << neq::value << std::endl;

    using less = std::ratio_less<r1, r2>;
    std::cout << "less: " << less::value << std::endl;

    using less_equal = std::ratio_less_equal<r1, r2>;
    std::cout << "less_equal: " << less_equal::value << std::endl;

    using greater = std::ratio_greater<r1, r2>;
    std::cout << "greater: " << greater::value << std::endl;

    using greater_equal = std::ratio_greater_equal<r1, r2>;
    std::cout << "greater_equal: " << greater_equal::value << std::endl;

    return 0;
}
```

## regex

std::regex 是 C++ 标准库中的一个类，用于处理正则表达式。
正则表达式是一种强大的文本处理工具，用于匹配、查找和替换字符串中的模式。
std::regex 提供了一种类型安全且高效的方式来处理正则表达式。

## stream

输入输出流

C++ 中的输入输出流（I/O Streams）是处理数据输入输出的核心机制, 它允许程序从外部源读取数据, 并将数据写入到外部源。

std::istream是C++标准库中的输入流类，它是所有输入流类的基类。它提供了一组函数，用于从输入流中读取数据
std::ostream是C++标准库中的输出流类，它是所有输出流类的基类。它提供了一组函数，用于向输出流中写入数据
std::ofstream是std::ostream的子类，它用于向文件写入数据
std::ifstream是std::istream的子类，它用于从文件读取数据
std::stringstream是std::iostream的一个子类，它用于在内存中读写数据
std::cin是std::istream的实例，它是标准输入流，用于从标准输入读取数据
std::cout是std::ostream的实例，它是标准输出流，用于向标准输出写入数据
std::cerr是std::ostream的实例，它是标准错误输出流，用于向标准错误输出写入数据
std::clog是std::ostream的实例，它是标准日志输出流，用于向标准日志输出写入数据
std::ios::sync_with_stdio 设置 std::cin 和 std::cout 的同步状态，以提高输入输出的性能

## string

std::string 是 C++ 标准库中的一个类，用于表示和操作字符串。它提供了丰富的成员函数和操作符，使得字符串的处理变得更加方便和高效。

- 格式

```cpp
// basic_string
template < class charT,           
    class traits = char_traits<charT>,    // basic_string::traits_type           
    class Alloc = allocator<charT>>        // basic_string::allocator_type           
class basic_string;

// string
typedef basic_string<char> string;
```

相关网址： https://cplusplus.com/reference/string/string/

- 常用构造方法

```cpp
// 常用构造方法
std::string str1;

// 复制构造方法
std::string str2(str1);

// 带参数的构造方法
std::string str3("Hello, World!");

// 赋值构造函数
std::string str4 = "Hello, World!";

// 列表初始化
std::string str5 = {"Hello, World!"};
std::string str6{"Hello, World!"};
```

- 迭代器

| 迭代器 | 描述 |
| --- | --- |
| begin | 返回指向字符串开头的迭代器 |
| end | 返回指向字符串末尾的迭代器 |
| rbegin | 返回指向字符串末尾的逆向迭代器 |
| rend | 返回指向字符串开头的逆向迭代器 |
| cbegin | 返回指向字符串开头的常量迭代器 |
| cend | 返回指向字符串末尾的常量迭代器 |
| crbegin | 返回指向字符串开头的常量逆向迭代器 |
| crend | 返回指向字符串末尾的常量逆向迭代器 |

- 成员函数

| 函数 | 描述 |
| --- | --- |
| size | 返回字符串的长度 |
| length | 获取字符串的长度 |
| max_size | 返回字符串的最大长度 |
| resize | 设置字符串的长度 |
| capacity | 获取字符串的容量 |
| reserve | 预留字符串的容量 |
| clear | 清空字符串 |
| empty | 检查字符串是否为空 |
| shrink_to_fit | 缩小字符串的容量 |
| operator[] | 返回指定位置的字符 |
| at | 返回指定位置的元素，并进行边界检查 |
| back | 返回字符串的最后一个字符 |
| front | 获取字符串的第一个字符 |
| operator+ | 连接两个字符串 |
| append | 向字符串末尾添加另一个字符串或字符 |
| push_back | 向字符串末尾添加一个字符 |
| assign | 设置字符串的值 |
| insert | 在指定位置插入字符串或字符 |
| replace | 替换字符串的指定范围 |
| swap | 交换两个字符串 |
| pop_back | 删除字符串末尾的字符 |
| c_str | 返回指向字符串内部 C 样式字符串的指针 |
| data | 返回指向字符串内部字符数组的指针 |
| get_allocator | 获取字符串的分配器 |
| copy | 将字符串的一部分复制到另一个字符数组中 |
| find | 在字符串中查找指定字符或子字符串 |
| rfind | 在字符串中查找指定字符或子字符串 |
| find_first_of | 在字符串中查找指定字符或字符集 |
| find_last_of | 在字符串中查找指定字符或字符集 |
| find_first_not_of | 在字符串中查找第一个不包含指定字符或字符集的位置 |
| find_last_not_of | 在字符串中查找最后一个不包含指定字符或字符集的位置 |
| substr | 返回字符串的子字符串 |
| compare | 比较两个字符串的大小 |

## string_view

std::string_view是C++17引入的一个轻量级的非拥有性字符串视图类。它位于<string_view>头文件中，提供了一种高效的方式来引用字符串数据，而不需要复制字符串内容，这使得它在处理字符串时非常节省内存和时间。

- 格式

```cpp
template <class CharType, class Traits = char_traits<CharType>>
class basic_string_view;

typedef basic_string_view<char, char_traits<char>> string_view;
```

相关网址: https://learn.microsoft.com/zh-cn/cpp/standard-library/basic-string-view-class?view=msvc-170#basic_string_view

- 常用构造方法

```cpp
// 构造方法
std::string_view str1;

// 带参数的构造方法
std::string_view str2("Hello, World!");

// 带参数的构造方法
std::string_view str3("Hello, World!", strlen("Hello, World!"));

// 列表初始化
std::string_view str4 = {"Hello, World!"};
std::string_view str5{"Hello, World!"};
```
- 迭代器

| 迭代器 | 描述 |
| --- | --- |
| begin | 返回指向字符串视图开头的迭代器 |
| end | 返回指向字符串视图末尾的迭代器 |
| rbegin | 返回指向字符串视图末尾的逆向迭代器 |
| rend | 返回指向字符串视图开头的逆向迭代器 |
| cbegin | 返回指向字符串视图开头的常量迭代器 |
| cend | 返回指向字符串视图末尾的常量迭代器 |
| crbegin | 返回指向字符串视图开头的常量逆向迭代器 |
| crend | 返回指向字符串视图末尾的常量逆向迭代器 |

- 成员函数

| 函数 | 描述 |
| --- | --- |
| operator== | 比较两个字符串视图是否相等 |
| operator!= | 比较两个字符串视图是否不相等 |
| operator[] | 返回指定位置的元素 |
| at | 返回指定位置的元素 |
| back | 获取字符串视图的最后一个元素 |
| copy | 将字符串视图的内容复制到字符数组中(从视图中读出) |
| compare | 比较两个字符串视图的大小 |
| data | 返回指向字符串视图内部字符数组的指针 |
| empty | 检查字符串视图是否为空 |
| ends_with | 检查字符串视图是否以指定的后缀结尾(C++20) |
| find | 在字符串视图中查找子字符串 |
| find_first_not_of | 在字符串视图中查找第一个不包含指定字符的位置 |
| find_first_of | 在字符串视图中查找第一个包含指定字符的位置 |
| find_last_not_of | 在字符串视图中查找最后一个不包含指定字符的位置 |
| find_last_of | 在字符串视图中查找最后一个包含指定字符的位置 |
| front | 获取字符串视口的第一个元素 |
| length | 获取字符串视图的长度 |
| max_size | 获取字符串视图的最大长度 |
| remove_prefix | 从开头移除字符串视图的指定数目的元素 |
| remove_suffix | 从末尾移除字符串视图的指定数目的元素 |
| rfind | 在字符串视图中反向查找子字符串 |
| size | 获取字符串视图的长度 |
| starts_with | 检查字符串视图是否以指定的前缀开头(C++20) |
| substr | 返回字符串视图的子字符串视图 |
| swap | 交换两个字符串视图 |

## tuple

std::tuple 是 C++ 标准库中的一个模板类，用于将多个不同类型的值组合成一个单一的对象。它类似于结构体（struct）或类（class），但不需要定义成员变量和成员函数。std::tuple 可以包含任意数量和类型的元素，并且可以通过索引或类型来访问这些元素。

- 格式

```cpp
// 构造定义
template <class... Types> class tuple;
```

相关网址： https://cplusplus.com/reference/tuple/tuple/

- 常用构造方法

```cpp
// construct
std::tuple<int, double> t1;

// construct with parameters
std::tuple<int, double> t2(1, 2.5);

// copy construct
std::tuple<int, double> t3(t1);

// operator=
std::tuple<int, double> t4 = t1;

// make_tuple
std::tuple<int, double> t5 = std::make_tuple(1, 2.5);

// make pair
std::pair<int, double> t6 = std::make_pair(1, 2.5);

// list initialization
std::tuple<int, double> t7 = {1, 2.5};
std::tuple<int, double> t8{1, 2.5};
```

tuple中支持一系列函数来进行元组操作，主要如下所示。

| 函数 | 描述 |
| --- | --- |
| std::get<N>(t) | 返回元组t中第N个元素 |
| std::tuple_size<T> | 返回元组T的元素个数 |
| std::tuple_element<N, T> | 返回元组T中第N个元素的类型 |
| std::tuple_cat(t1, t2, ...) | 将多个元组连接成一个元组 |
| std::make_tuple(t1, t2, ...) | 创建一个元组，包含多个元素 |
| std::forward_as_tuple(t1, t2, ...) | 将多个参数转发为一个元组 |
| std::tie(t1, t2, ...) | 将多个参数绑定到一个元组中 |
| std::ignore | 一个占位符，用于忽略元组中的元素，配合std::tie()使用 |

## type_traits

std::type_traits 是 C++ 标准库中的一个模板库，它提供了一系列的模板类和模板函数，用于在编译时获取和操作类型的特性。
这些特性包括类型是否是整数、浮点数、指针、引用、类、联合体、枚举等，
以及类型的大小、对齐方式、是否有默认构造函数、是否有拷贝构造函数、是否有移动构造函数等。

std::type_traits 中的模板类和模板函数都是通过模板特化和模板元编程技术实现的，
它们可以在编译时进行类型检查和类型转换，从而提高代码的效率和安全性

std::type_traits 中常用的模板类和模板函数：
1. std::is_integral<T>：判断类型 T 是否是整数类型。
2. std::is_floating_point<T>：判断类型 T 是否是浮点数类型。
3. std::is_pointer<T>：判断类型 T 是否是指针类型。
4. std::is_reference<T>：判断类型 T 是否是引用类型。
5. std::is_class<T>：判断类型 T 是否是类类型。
6. std::is_union<T>：判断类型 T 是否是联合体类型。
7. std::is_enum<T>：判断类型 T 是否是枚举类型。
8. std::is_same<T, U>：判断类型 T 和 U 是否相同。
9. std::enable_if<B, T>：根据条件 B 来选择是否启用类型 T。
10. std::conditional<B, T, F>：根据条件 B 来选择类型 T 或类型 F。
11. std::integral_constant<T, v>：定义一个类型为 T，值为 v 的编译期常量。

## utility

## variant

std::variant是C++17 准库中引入的一个类型安全的联合体（union）替代品。它允许你在一个变量中存储多种不同类型的值，并且在运行时可以安全地访问和操作这些值。
与传统的联合体不同，std::variant会跟踪当前存储的是哪种类型的值，从而避免了类型不安全的问题。

相关网址： https://learn.microsoft.com/zh-cn/cpp/standard-library/variant-operators?view=msvc-170

std::variant 提供了以下功能：

1. 存储多种类型的值：std::variant 可以存储多个类型的值，这些类型可以是任意的，包括用户定义的类型。
2. 运行时类型检查：std::variant 会在运行时检查当前存储的是哪种类型的值，从而避免了类型不安全的问题。
3. 访问存储的值：你可以使用 std::get 函数来访问 std::variant 中存储的值，或者使用 std::visit 函数来对存储的值进行操作。
4. 异常安全：std::variant 是异常安全的，它会在构造和赋值时自动处理异常，避免了内存泄漏和未定义行为。

相关方法：

1. std::get<T>(var)：返回std::variant中类型为T的值。如果std::variant当前存储的类型不是T，则会抛出std::bad_variant_access异常。
2. std::get_if<T>(var)：返回std::variant中类型为T的值的指针。如果std::variant当前存储的类型不是T，则返回nullptr。
3. std::holds_alternative<T>(var)：判断std::variant是否存储了类型为T的值。如果是，则返回true；否则返回false。
4. std::swap(var1, var2)：交换两个std::variant的值。

```cpp
#include <iostream>
#include <variant>
#include <string>

int main(int argc, char const *argv[])
{
    std::variant<int, float, std::string> v;

    v = 42;
    std::cout << std::get<int>(v) << std::endl;

    v = 3.14f;
    std::cout << std::get<float>(v) << std::endl;

    if (std::holds_alternative<float>(v)){
        std::cout << "float: " << std::get<float>(v) << std::endl;
    } else if (std::holds_alternative<std::string>(v)) {
        std::cout << "string: " << std::get<std::string>(v) << std::endl;
    } else {
        std::cout << "unknown type" << std::endl;
    }

    v = "hello world";
    auto result = std::get_if<std::string>(&v);
    if (result) {
        std::cout << "string: " << *result << std::endl;
    } else {
        std::cout << "string: not found" << std::endl;
    }

    std::variant<int, float, std::string> v2 = "test";
    std::swap(v, v2);
    std::cout << "v: " << std::get<std::string>(v) << std::endl;
    std::cout << "v2: " << std::get<std::string>(v2) << std::endl;
    return 0;
}
```

### visit

std::visit是C++17标准库中引入的一个函数，用于对std::variant中的值进行操作。它允许你使用一个函数对象来处理std::variant中的值，并返回一个结果。

std::decay_t是C++17标准库中引入的一个类型别名模板，用于将一个类型转换为其非引用类型。它会去除类型的cv限定符和引用，并返回一个非引用类型(可用于在模板中获取原始类型）。

具体示例如下。

```cpp
#include <iostream>
#include <variant>

template <typename T>
constexpr auto print_type(T&& arg) {
    using T1 = std::decay_t<decltype(arg)>;
    if constexpr (std::is_same_v<T1, int>) {
        return 0;
    } else if constexpr (std::is_same_v<T1, double>) {
        return 1;
    } else if constexpr (std::is_same_v<T1, std::string>) {
        return 2;
    } else {
        return -1;
    }
}

int main(int argc, char const *argv[])
{
    std::variant<int, float, std::string> v;

    v = 42;
    
    auto func = [](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, int>) {
            std::cout << "int: " << arg << std::endl;
        } else if constexpr (std::is_same_v<T, float>) {
            std::cout << "float: " << arg << std::endl;
        } else if constexpr (std::is_same_v<T, std::string>) {
            std::cout << "string: " << arg << std::endl;
        } else {
            std::cout << "unknown type" << std::endl;
        }
    };
    std::visit(func, v);

    int i;
    double d;
    std::string s;
    std::cout << "print_type(i): " << print_type(i) << std::endl;
    std::cout << "print_type(d): " << print_type(d) << std::endl;
    std::cout << "print_type(s): " << print_type(s) << std::endl;
    return 0;
}
```
