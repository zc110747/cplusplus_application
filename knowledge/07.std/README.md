# STD标准库函数

C++标准库是C++编程语言的一部分，它提供了一组通用的类和函数，用于处理各种常见的编程任务。标准库被组织成多个头文件，每个头文件包含一组相关的功能。

- [any](#any)
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
  - [iostream](#iostream)
  - [fstream](#fstream)
  - [sstream](#sstream)
- [string](#string)
- [string_view](#string_view)
- [tuple](#tuple)
  - [std::apply](#apply)
- [type_traits](#type_traits)
- [utility](#utility)
  - [std::declval](#declval)
- [variant](#variant)
  - [std::visit](#visit)

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

另外，std::any支持一些函数用于创建、访问对象，具体如下所示。

1. std::in_place_type是C++17标准库中引入的一个类型，用于在std::any中存储值时指定值的类型。
2. std::make_any是C++17标准库中引入的一个函数模板，用于创建一个std::any对象，并使用给定的参数构造一个值。
3. std::any_cast是C++17标准库中引入的一个函数模板，用于从std::any中提取值，并将其转换为指定的类型。如果类型不匹配，会抛出std::bad_any_cast异常。

举例说明如下。

```cpp
#include <any>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char const *argv[]) 
{
    std::any a = 42;
    std::cout << std::any_cast<int>(a) << std::endl;

    a = "hello world";
    std::cout << std::any_cast<std::string>(a) << std::endl;

    a = 3.14;
    std::cout << std::any_cast<double>(a) << std::endl;

    a = std::vector<int>{1, 2, 3};
    const auto&vec = std::any_cast<std::vector<int>>(a);
    for (const auto& i : vec) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    a = std::make_any<std::string>("hello world");
    std::cout << std::any_cast<std::string>(a) << std::endl;
}
```

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

std::locale 是 C++ 标准库中的一个类，用于表示特定的本地化环境。

具体网址：https://cplusplus.com/reference/locale/locale/

```shell
# 查看支持的指令集
locale -a
```

其支持的方法如下所示。

| 方法 | 功能描述 |
| --- | --- |
| classic | 返回一个std::locale对象，该对象表示使用 C 语言的默认本地化环境。 |
| combine | 返回一个 std::locale对象，该对象表示使用指定本地化环境进行组合。 |
| global | 设置全局 std::locale 对象, 该对象表示使用指定本地化环境。 |
| name | 返回当前 std::locale 对象的名称。 |
| operator= | 设置当前 std::locale 对象，该对象表示使用指定本地化环境。 |
| operator== | 比较两个 std::locale 对象是否相等。 |
| operator!= | 比较两个 std::locale 对象是否不相等。|
| operator() | 使用本地locale参数进行本地化比较 |

主要特点：

1. 本地化支持，std::locale提供了对不同地区和语言的本地化支持，使得程序能够适应不同的用户群体。
2. 可定制性，可以通过创建自定义的 std::locale 对象来定制特定的本地化设置。
3. 全局和局部设置，可以在全局范围内设置默认的 std::locale，也可以在局部范围内使用特定的 std::locale。
4. 与标准库组件集成，std::locale 与许多标准库组件（如 std::cout、std::cin、std::string 等）集成，使得这些组件能够自动适应本地化设置。

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

对于C++中，支持一系列流操作来处理输入输出机制；按照功能分为输入/输出流、字符串流和文件流，用于满足不同场景的需求。

### iostream

iostream全称输入/输出流，主要处理键盘鼠标输入输出，并显示到界面中的功能，包含的主要内容如下。

相关网址：https://cplusplus.com/reference/iostream/

| 输入输出流 | 描述 |
| --- | --- |
| std::cin | 标准输入流，用于从标准输入设备（键盘）读取数据 |
| std::cout | 标准输出流，用于向标准输出设备（屏幕）写入数据 |
| std::cerr | 标准错误输出流，用于向标准错误输出写入数据 |
| std::clog | 标准日志输出流，用于向标准日志写入数据 |
| std::wcin | 标准宽字符输入流，用于从标准输入设备（键盘）读取宽字符数据 |
| std::wcout | 标准宽字符输出流，用于向标准输出写入宽字符数据 |
| std::wcerr | 标准宽字符错误输出流，用于向标准错误输出写入宽字符数据 |
| std::wclog | 标准宽字符日志输出流，用于向标准日志写入宽字符数据 |
| std::istream | 输入流，用于从输入设备读取数据 |
| std::ostream | 输出流，用于向输出设备写入数据 |

另外，ios是iostream的基础类，提供了一些通用的功能，如流的状态、异常处理、格式化输出等。指定输出格式如下所示。

| 格式符 | 描述 |
| --- | --- |
| boolalpha | 将输入的数字转换为布尔值 |
| showbase | 显示数值的进制前缀 |
| showpoint | 显示小数点 |
| showpos | 显示正数 |
| skipws | 忽略空白字符 |
| unitbuf | 每次写入数据时刷新缓冲区 |
| uppercase | 将输出的数字转换为大写 |
| noboolalpha | 取消布尔值转换 |
| noshowbase | 取消显示数值的进制前缀 |
| noshowpoint | 取消显示小数点 |
| noshowpos | 取消显示正数 |
| noskipws | 取消忽略空白字符 |
| nounitbuf | 取消每次写入数据时刷新缓冲区 |
| nouppercase | 取消将输出的数字转换为大写 |
| dec | 十进制 |
| hex | 十六进制 |
| oct | 八进制 |
| fixed | 固定小数位数 |
| scientific | 科学计数法 |
| internal | 默认小数位数 |
| left | 左对齐 |
| right | 右对齐 |

另外，std::ios::sync_with_stdio(bool sync)可以改变C++和C语言的输入输出同步方式。设置为true可以混用printf和cout，而不会出现缓冲区冲突(不过在实际产品中建议统一接口，不要混用)。

具体示例如下所示。

```cpp
#include <cstdio>
#include <iostream>

class object {
public:
    int a;
};

std::ostream& operator<<(std::ostream& os, const object& obj) {
    os << "object: " << obj.a;
    return os;
}

int main(int argc, char const *argv[])
{   
    // std::cin 输入流，用于从标准输入设备（键盘）读取数据
    int num;
    std::cout << "请输入一个数字：";
    std::cin >> num;
    std::cout << "您输入的数字是：" << num << std::endl;

    // std::cout 标准输出流，用于向标准输出写入数据
    std::cout << "Hello World!" << std::endl;
    std::cerr << "Error!" << std::endl;
    std::clog << "Log!" << std::endl;

    std::ios::sync_with_stdio(true);
    for(int i=0; i<3; i++) {
        printf("c out:%d\n", i);
        std::cout << "c++ out:" << i << std::endl;
    }

    // 显示布尔值
    std::cout << std::boolalpha;
    std::cout << true << std::endl;
    std::cout << std::noboolalpha;
    std::cout << false << std::endl;

    // 显示进制
    int val = 10;
    std::cout << std::hex;
    std::cout << "val: 0x" << val << std::endl;
    std::cout << std::dec;
    std::cout << "val: " << val << std::endl;

    object obj;
    std::cout << obj << std::endl;
    return 0;
}
```

### fstream

std::fstream是C++标准库中的一个类，其位于<fstream>头文件中，用于处理文件输入输出。另外支持两个子类，std::ifstream和std::ofstream，分别用于从文件读取数据、向文件写入数据。

fstream支持的方法操作如下所示。

| 方法 | 描述 |
| --- | --- |
| open | 打开文件 |
| is_open | 检查文件是否已打开 |
| close | 关闭文件 |
| rdbuf | 从文件读取数据 |
| gcount | 获取当前位置 |
| get | 获取一个字符 |
| getline | 获取一行数据 |
| read | 按照块读取数据 |
| readsome | 读取数据到缓冲区 |
| putback | 添加一个字符到缓冲区 |
| put | 添加一个字符到缓冲区 |
| write | 按照块写入数据 |
| tellp | 获取文件写入位置 |
| seekp | 设置文件写入位置 |
| tellg | 获取文件读取位置 |
| seekg | 设置文件读取位置 |
| flush | 刷新缓冲区 |
| sync | 同步文件 |
| good | 检查文件是否正常 |
| eof | 检查文件是否已结束 |
| fail | 检查文件是否失败 |
| bad | 检查文件是否出错 |
| clear | 清空错误状态 |

另外支持通过ios设置文件的打开模式、位置等信息，具体示例如下所示。

| 模式 | 描述 |
| --- | --- |
| std::ios::app | 追加模式 |
| std::ios::ate | 打开文件后定位到文件末尾 |
| std::ios::binary | 二进制模式 |
| std::ios::in | 输入模式 |
| std::ios::out | 输出模式 |
| std::ios::trunc | 截断模式 |
| std::ios::beg | 定位到文件开头 |
| std::ios::cur | 定位到当前位置 |
| std::ios::end | 定位到文件末尾 |

具体示例如下所示。

```cpp
#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char const *argv[])
{
    std::fstream file("test.txt", std::ios::out | std::ios::in);
    if(file.is_open()) {
        file << "Hello, World!" << std::endl;

        // 设置文件读取位置
        file.seekg(0, std::ios::beg);
        std::string str;
        file >> str;
        std::cout << str << std::endl;
        file.close();
    }

    // 读取文件
    std::ifstream ifile("test.txt", std::ios::in);
    if(ifile.is_open()) {
        std::string str;
        ifile >> str;
        std::cout << str << std::endl;
        ifile.close();
    }

    // 追加写入文件
    std::ofstream ofile("test.txt", std::ios::out | std::ios::app);
    if(ofile.is_open()) {
        ofile << "append, World!" << std::endl;
        ofile.close();
    }
    return 0;
}
```
### sstream

std::sstream是C++标准库中的一个类，用于处理字符串输入输出。它提供了丰富的成员函数和操作符，使得字符串的处理变得更加方便和高效。sstream默认以空格作为分界符进行字符串分割，如果使用其它分隔符，使用std::getline()函数指定。

sstream支持的方法操作如下所示。

| 方法 | 描述 |
| --- | --- |
| str | 获取字符串 |
| operator<< | 向字符串流中写入数据 |
| operator>> | 从字符串流中读取数据 |
| getcount | 获取流中的字符数 |
| get | 获取一个字符 |
| getline | 从字符串流中读取一行数据 |
| read | 从字符串流中读取数据 |
| write | 向字符串流中写入数据 |
| seekg | 设置字符串流的读取位置 |
| tellg | 获取字符串流的读取位置 |
| seekp | 设置字符串流的写入位置 |
| tellp | 获取字符串流的写入位置 |
| flush | 刷新字符串流 |
| sync | 同步字符串流 |
| good | 检查字符串流是否正常 |
| eof | 检查字符串流是否已结束 |
| fail | 检查字符串流是否失败 |
| bad | 检查字符串流是否出错 |
| clear | 清空字符串流的错误状态 |

具体示例如下所示。

```cpp
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char const *argv[])
{
    std::stringstream ss;
    
    // 向字符串流中写入数据
    ss << "Hello, World!";
    std::cout << ss.str() << std::endl;
    std::cout << "good:" << ss.good() << std::endl;
    std::cout << "eof:" << ss.eof() << std::endl;
    std::cout << "fail:" << ss.fail() << std::endl;
    std::cout << "bad:" << ss.bad() << std::endl;

    // 从字符串流中读取数据(默认以空格作为分隔符)
    std::string str;
    ss >> str;
    std::cout << str << std::endl;

    std::cout << "tellg:" << ss.tellg() << std::endl;
    std::cout << "tellp:" << ss.tellp() << std::endl;

    std::vector<std::string> vec;
    std::stringstream ss1;

    ss1 << "Hello, World!";

    // 使用getline()函数指定分隔符进行字符串分割
    while(std::getline(ss1, str, ',')) {
        vec.push_back(str);
    }
    for(const auto& item : vec) {
        std::cout << item << ";";
    }
    std::cout << std::endl;
    return 0;
}
```

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

std::tuple是C++标准库中的一个模板类，用于将多个不同类型的值组合成一个单一的对象。它类似于结构体（struct）或类（class），但不需要定义成员变量和成员函数；tuple可以包含任意数量和类型的元素，并且可以通过索引或类型来访问这些元素。

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

## apply

std::apply是C++17引入的一个模板函数，用于将一个元组中的元素应用到一个函数或lambda表达式中。

参考地址：https://en.cppreference.com/w/cpp/utility/apply.html

其原型如下。

```cpp
//C++17 ~ C++23
template< class F, class Tuple >
constexpr decltype(auto) apply( F&& f, Tuple&& t );

// (since C++23)
template< class F, tuple-like Tuple >
constexpr decltype(auto) apply( F&& f, Tuple&& t ) noexcept(/* see below */);
```

使用方法。

```cpp
#include <tuple>
#include <iostream>

int add(int a, int b) 
{
    return a + b;
}

auto multiply = [](int a, int b)  {
    return a * b;
};

int main(int argc, char const *argv[]) 
{
    // std::apply
    std::cout << "========================= TUPLE_APPLY =========================" << std::endl;

    auto args = std::make_tuple(3, 4);

    int sum = std::apply(add, args);
    std::cout << "Sum: " << sum << std::endl;

    int product = std::apply(multiply, args);
    std::cout << "Product: " << product << std::endl;

    auto tuple_args = std::forward_as_tuple("hello", 24, 'a');
    std::apply([](auto&&... args) {
        ((std::cout << args << " "), ...);
        std::cout << std::endl; 
    }, tuple_args);

    auto func = [](std::string s, int i, char c) {
        std::cout << "String: " << s;
        std::cout << " | Integer: " << i;
        std::cout << " | Character: " << c << std::endl;
    };
    std::apply(func, tuple_args);

    return 0;
}
```

## type_traits

std::type_traits是C++标准库中的一个模板库，它提供了一系列的模板类和模板函数，用于在编译时获取和操作类型的特性。这些特性包括类型是否是整数、浮点数、指针、引用、类、联合体、枚举等，以及类型的大小、对齐方式、是否有默认构造函数、是否有拷贝构造函数、是否有移动构造函数等。

std::type_traits 中的模板类和模板函数都是通过模板特化和模板元编程技术实现的，它们可以在编译时进行类型检查和类型转换，从而提高代码的效率和安全性。

相关网址： https://cplusplus.com/reference/type_traits/

std::type_traits中常用的模板类和模板函数分类如下所示。

- 主要类型类别

| 模板类 | 描述 |
| --- | --- |
| std::is_array<T> | 判断类型T是否是数组类型 |
| std::is_class<T> | 判断类型T是否是类类型 |
| std::is_enum<T> | 判断类型T是否是枚举类型 |
| std::is_floating_point<T> | 判断类型T是否是浮点数类型 |
| std::is_function<T> | 判断类型T是否是函数类型 |
| std::is_integral<T> | 判断类型T是否是整数类型 |
| std::is_lvalue_reference<T> | 判断类型T是否是左值引用类型 |
| std::is_member_function_pointer<T> | 判断类型T是否是成员函数指针类型 |
| std::is_member_object_pointer<T> | 判断类型T是否是成员对象指针类型 |
| std::is_pointer<T> | 判断类型T是否是指针类型 |
| std::is_rvalue_reference<T> | 判断类型T是否是右值引用类型 |
| std::is_union<T> | 判断类型T是否是联合体类型 |
| std::is_void<T> | 判断类型T是否是 void 类型 |

- 复合类型类别

| 模板类 | 描述 |
| --- | --- |
| std::is_arithmetic<T> | 判断类型T是否是算术类型 |
| std::is_compound<T> | 判断类型T是否是复合类型 |
| std::is_fundamental<T> | 判断类型T是否是基础类型 |
| std::is_member_pointer<T> | 判断类型T是否是成员指针类型 |
| std::is_object<T> | 判断类型T是否是对象类型 |
| std::is_reference<T> | 判断类型T是否是引用类型 |
| std::is_scalar<T> | 判断类型T是否是标量类型 |

- 类型属性

| 模板类 | 描述 |
| --- | --- |
| std::is_abstract<T> | 判断类型T是否是抽象类型 |
| std::is_const<T> | 判断类型T是否是const类型 |
| std::is_empty<T> | 判断类型T是否是空类型 |
| std::is_literal_type<T> | 判断类型T是否是字面量类型 |
| std::is_pod<T> | 判断类型T是否是POD类型 |
| std::is_polymorphic<T> | 判断类型T是否是多态类型 |
| std::is_signed<T> | 判断类型T是否是signed类型 |
| std::is_standard_layout<T> | 判断类型T是否是标准布局类型 |
| std::is_trivial<T> | 判断类型T是否是trivial类型 |
| std::is_trivially_copyable<T> | 判断类型T是否是trivially copyable类型 |
| std::is_unsigned<T> | 判断类型T是否是unsigned类型 |
| std::is_volatile<T> | 判断类型T是否是volatile类型 |

- 类型特征

| 模板类 | 描述 |
| --- | --- |
| has_virtual_destructor<T> | 检测类型T是否具有虚析构函数 |
| std::is_assignable<T, U> | 检测类型T是否可赋值给类型U |
| std::is_constructible<T, Args...> | 检测类型T是否可构造 |
| std::is_copy_assignable<T> | 检测类型T是否可复制赋值 |
| std::is_copy_constructible<T> | 检测类型T是否可复制构造 |
| std::is_destructible<T> | 检测类型T是否可析构 |
| std::is_default_constructible<T> | 检测类型T是否可默认构造 |
| std::is_move_assignable<T> | 检测类型T是否可移动赋值 |
| std::is_move_constructible<T> | 检测类型T是否可移动构造 |
| std::is_trivially_assignable<T, U> | 检测类型T是否可 trivially 赋值给类型U |
| std::is_triaially_constructible<T, Args...> | 检测类型T是否可 trivially 构造 |
| std::is_trivially_copy_assignable<T> | 检测类型T是否可 trivially 复制赋值 |
| std::is_trivially_copy_constructible<T> | 检测类型T是否可 trivially 复制构造 |
| std::is_trivially_destructible<T> | 检测类型T是否可 trivially 析构 |
| std::is_trivially_default_constructible<T> | 检测类型T是否可 trivially 默认构造 |
| std::is_trivially_move_assignable<T> | 检测类型T是否可 trivially 移动赋值 |
| std::is_trivially_move_constructible<T> | 检测类型T是否可 trivially 移动构造 |
| std::is_nothrow_assignable<T, U> | 检测类型T是否可 noexcept 赋值给类型U |
| std::is_nothrow_constructible<T, Args...> | 检测类型T是否可 noexcept 构造 |
| std::is_nothrow_copy_assignable<T> | 检测类型T是否可 noexcept 复制赋值 |
| std::is_nothrow_copy_constructible<T> | 检测类型T是否可 noexcept 复制构造 |
| std::is_nothrow_destructible<T> | 检测类型T是否可 noexcept 析构 |
| std::is_nothrow_default_constructible<T> | 检测类型T是否可 noexcept 默认构造 |
| std::is_nothrow_move_assignable<T> | 检测类型T是否可 noexcept 移动赋值 |
| std::is_nothrow_move_constructible<T> | 检测类型T是否可 noexcept 移动构造 |

- 类型关系

| 模板类 | 描述 |
| --- | --- |
| std::is_base_of<Base, Derived> | 检测类型Derived是否是类型Base的基类 |
| std::is_convertible<From, To> | 检测类型From是否可以转换为类型To |
| std::is_same<T, U> | 检测类型T是否与类型U相同 |

- 所有查询

| 模板类 | 描述 |
| --- | --- |
| std::alignment_of<T> | 获取类型T的对齐字节数 |
| std::extent<T> | 获取类型T的元素数量 |
| std::rank<T> | 获取类型T的维数 |

- 去除符号

| 模板类 | 描述 |
| --- | --- |
| std::add_const<T> | 添加const修饰符 |
| std::add_cv<T> | 添加const和volatile修饰符 |
| std::add_volatile<T> | 添加volatile修饰符 |
| std::remove_const<T> | 移除const修饰符 |
| std::remove_cv<T> | 移除const和volatile 修饰符 |
| std::remove_volatile<T> | 移除volatile修饰符 |

- 复合型变化

| 模板类 | 描述 |
| --- | --- |
| std::add_pointer<T> | 添加指针修饰符 |
| std::add_lvalue_reference<T> | 添加左值引用修饰符 |
| std::add_rvalue_reference<T> | 添加右值引用修饰符 |
| std::decay<T> | 移除cv 和 引用修饰符 |
| std::make_signed<T> | 创建一个与类型T对应的有符号类型 |
| std::make_unsigned<T> | 创建一个与类型T对应无符号类型 |
| std::remove_all_extents<T> | 移除数组的维数 |
| std::remove_extent<T> | 移除数组的维数 |
| std::remove_pointer<T> | 移除指针修饰符 |
| std::remove_reference<T> | 移除引用修饰符 |
| std::underlying_type<T> | 获取类型T的底层类型 |

- 其它类型转换

| 模板类 | 描述 |
| --- | --- |
| std::aligned_storage<Len, Align> | 创建一个长度为Len且对齐字节数为Align的未初始化对象 |
| std::aligned_union<Len, T1, T2, ...> | 创建一个长度为Len且对齐字节数为std::max(alignof(T1), alignof(T2), ...)的未初始化对象 |
| std::common_type<T1, T2> | 获取两个类型T1和T2的公共类型 |
| std::conditional<Cond, T1, T2> | 根据条件Cond来选择 T1 或 T2 |
| std::enable_if<Cond, T> | 根据条件Cond来选择 T |
| std::result_of<F(Args...)> | 获取函数F的返回类型 |

## utility

utility提供了一些处理STL和STD的工具函数，主要包含如下所示。

1. std::swap: 交换两个参数的值。
2. std::make_pair: 创建一个pair对象。
3. std::forward: 将参数传递给函数。
4. std::move: 将参数传递给函数，并转换为右值引用。
5. std::move_if_noexcept: 将参数传递给函数，并转换为右值引用，如果函数有 noexcept 声明。
6. std::declval: 创建一个无类型的对象，用于在模板参数推导中。
6. std::rel_ops: 定义运算符相关的操作函数，支持==, <操作符的对象，扩展支持!=, >, <=, >=操作符(C++20废弃，建议使用<=>操作符)。

```cpp
#include <utility>
#include <iostream>
#include <vector>

void over_func1 (const int& x) 
{
    std::cout << "[lvalue]" << std::endl;
}

void over_func1 (int&& x) 
{
    std::cout << "[rvalue]" << std::endl;
}

template <class T> 
void func (T&& x) {
    over_func1 (x);                   // always an lvalue
    over_func1 (std::forward<T>(x));  // rvalue if argument is rvalue
}

template <class T>
class A {
public:
    A() = default;
    T x;
    T y;
    A(T x, T y) : x(x), y(y) {}
    bool operator==(const A& other) const {
        return x == other.x && y == other.y;
    }
    bool operator<(const A& other) const {
        return x < other.x && y < other.y;
    }
};

int main(int argc, char const *argv[]) 
{
    std::vector<int> v1 = {1, 2, 3, 4, 5};
    std::vector<int> v2 = {6, 7, 8, 9, 10};

    // swap
    std::swap(v1, v2);
    for (const auto& val : v1) {
        std::cout << val << ", ";
    }
    std::cout << std::endl;

    // make_pair
    std::pair<int, int> p = std::make_pair(1, 2);
    std::cout << p.first << ", " << p.second << std::endl;

    // move
    auto&& v3 = std::move(v2);
    v3.push_back(11);
    for (const auto& val : v3) {
        std::cout << val << ", ";
    }
    std::cout << std::endl;

    // move, std::forward
    int a1 = 0;
    
    func(a1);
    func(0);

    using namespace std::rel_ops;
    A<int> ac1(1, 2), ac2(3, 4);
    std::cout << "ac1 == ac2: " << (ac1 == ac2) << std::endl;
    std::cout << "ac1 < ac2: " << (ac1 < ac2) << std::endl;

    // 通过rel_ops支持
    std::cout << "ac1 != ac2: " << (ac1 != ac2) << std::endl;
    std::cout << "ac1 > ac2: " << (ac1 > ac2) << std::endl;
    std::cout << "ac1 <= ac2: " << (ac1 <= ac2) << std::endl;
    std::cout << "ac1 >= ac2: " << (ac1 >= ac2) << std::endl;
    return 0;
}
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

## variant

std::variant是C++17标准库中引入的一个类型安全的联合体（union）替代品。它允许你在一个变量中存储多种不同类型的值，并且在运行时可以安全地访问和操作这些值。
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
