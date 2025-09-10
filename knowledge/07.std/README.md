# STD标准库函数

C++标准库是C++编程语言的一部分，它提供了一组通用的类和函数，用于处理各种常见的编程任务。标准库被组织成多个头文件，每个头文件包含一组相关的功能。

- [any](#any)
- [bitset](#bitset)
- [byte](#byte)
- [filesystem](#filesystem)
- [functional](#functional)
- [locale](#locale)
- [new](#new)
- [optional](#optional)
- [std::reference_wrapper](#reference_wrapper)
- [std::span](#span)
- [std::string_view](#string_view)
- [std::variant](#variant)

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

any_cast: 从std::any中提取值，并将其转换为指定的类型。如果类型不匹配，会抛出std::bad_any_cast异常。

详细测试代码见: [std::any验证代码](./any/main.cpp)

## bitset

std::bitset是 C++ 标准库中的一个类模板，用于表示固定大小的位序列。
它提供了一种方便的方式来处理二进制数据，例如位操作、位计数、位测试等

主要特点：
1. 固定大小：std::bitset的大小在编译时确定，因此它适用于需要固定大小的位序列的场景。
2. 高效操作：std::bitset提供了高效的位操作，例如位设置、位清除、位翻转、位测试等。
3. 类型安全：std::bitset是类型安全的，它确保所有操作都是在位级别上进行的，避免了类型错误。
4. 易于使用：std::bitset提供了直观的接口，使得位操作变得简单易懂

成员函数：
all: 检查所有位是否都设置为1。
any: 检查是否至少有一位被设置为1。
count: 计算位序列中设置为1的位数。
flip: 翻转位序列中的所有位或指定位。
none: 检查是否没有位被设置为1。
operator[]: 访问位序列中的指定位。
reset: 将位序列中的所有位设置为0。
set: 将位序列中的所有位设置为1或指定位。
size: 返回位序列的大小。
test: 检查指定位是否被设置为1。
to_string: 将位序列转换为字符串。
to_ullong: 将位序列转换为无符号长整数。
to_ulong: 将位序列转换为无符号整数。

std::bitcast
std::bit_cast 是 C++20 引入的一个函数模板，定义在 <bit> 头文件中，用于在不同类型之间进行位级别的转换。它提供了一种类型安全的方式来重新解释对象的位模式，避免了传统的类型转换（如 reinterpret_cast）可能带来的未定义行为

## byte

std::byte 
std::byte 是 C++17 引入的标准库类型，定义在 <cstddef> 头文件中，用于表示最小的可寻址内存单元，也就是字节。它是一个枚举类型，用于更安全、更明确地处理字节数据，避免使用 char 或 unsigned char 时可能出现的混淆。

类型安全：std::byte 不能隐式转换为整数类型，必须显式转换，这有助于避免意外的类型转换错误。
位操作：支持常见的位操作符，如 &、|、^、~、<< 和 >>。
无符号性：std::byte 是无符号的，这意味着它的取值范围是从 0 到 255

## filesystem

std::filesystem
C++17 引入的标准库，定义在 <filesystem> 头文件中，提供了跨平台的文件系统操作功能，替代了传统的依赖于操作系统的文件操作函数，让文件和目录操作更加方便、安全和可移植

## functional

std::invoke
std::invoke 是 C++17 引入的一个标准库函数模板，位于 <functional> 头文件中。它提供了一种统一的方式来调用可调用对象，包括函数、函数指针、成员函数指针、成员对象指针、lambda 表达式等。

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

std::optional 是 C++17 引入的一个标准库类，用于表示一个可能包含值的对象。它类似于其他语言中的Maybe或Option类型。
std::optional 可以用来避免使用空指针或特殊值（如 -1 或 nullptr）来表示缺失值

std::optional 提供了一种更安全、更清晰的方式来处理可能缺失的值，避免了使用特殊值或空指针带来的潜在问题。

成员函数：
emplace：在 std::optional 对象中构造一个值。
has_value：判断 std::optional 对象是否包含值。
reset：将 std::optional 对象中的值重置为未定义状态。
swap：交换两个 std::optional 对象中的值。
value：返回 std::optional 对象中包含的值。如果 std::optional 对象不包含值，则会抛出 std::bad_optional_access 异常。
value_or(default_value)：返回 std::optional 对象中包含的值，如果 std::optional 对象不包含值，则返回默认值 default_value。

## random

std::random 是 C++ 标准库中的一个组件，用于生成伪随机数。它提供了多种随机数生成器和分布，以满足不同的需求
随机数生成器
std::minstd_rand：线性同余生成器，速度快，但随机性较差
std::default_random_engine：默认的随机数生成器，通常基于Mersenne Twister算法。
std::mt19937：32位Mersenne Twister随机数生成器, 周期长，随机性好。
std::mt19937_64：64位Mersenne Twister随机数生成器。
分布
std::uniform_int_distribution：生成均匀分布的整数。
std::uniform_real_distribution：生成均匀分布的浮点数。
std::normal_distribution：生成正态分布的浮点数。
std::bernoulli_distribution：生成伯努利分布的布尔值。

std::random_device：是 C++ 标准库中的一个类，用于生成非确定性的随机数。它通常用于初始化随机数生成器的种子，以确保每次程序运行时生成的随机数序列都是不同的。

## ratio

std::ratio C++11 引入的一个模板类，定义在 <ratio> 头文件中，用于在编译时表示有理数。
它主要用于处理编译时的分数运算，比如时间单位的转换、频率的计算等，能在编译阶段完成分数的化简和计算，提高程序的性能和安全性。

std::ratio_add：计算两个比例的和。
std::ratio_subtract：计算两个比例的差。
std::ratio_multiply：计算两个比例的积。
std::ratio_divide：计算两个比例的商

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

std::string 是 C++ 标准库中的一个类，用于表示和操作字符串。
它提供了丰富的成员函数和操作符，使得字符串的处理变得更加方便和高效

构造函数：
std::string()：默认构造函数，创建一个空字符串。
std::string(const char* s)：使用 C 风格的字符串 s 构造字符串。
std::string(const std::string& str)：使用另一个字符串 str 构造字符串。

成员函数：
append：向字符串末尾添加另一个字符串或字符。
assign：将字符串设置为另一个字符串或字符。
at：返回指定位置的字符，并进行边界检查。
back：返回字符串的最后一个字符。
begin：返回指向字符串开头的迭代器。
c_str：返回指向字符串内部 C 风格字符串的指针。
capacity：返回字符串的容量。
cbegin：返回指向字符串开头的常量迭代器。
cend：返回指向字符串末尾的常量迭代器。
clear：清空字符串。
compare：比较两个字符串的大小。
copy：将字符串的一部分复制到另一个字符数组中。
crbegin：返回指向字符串开头的常量反向迭代器。
crend：返回指向字符串末尾的常量反向迭代器。
data：返回指向字符串内部字符数组的指针。
empty：检查字符串是否为空。
end：返回指向字符串末尾的迭代器。
ends_with(c++20)：检查字符串是否以指定的后缀结尾。
erase：删除字符串中的一部分。
find：查找子字符串的位置。
find_first_of：查找第一个匹配的字符或子字符串的位置。
find_last_of：查找最后一个匹配的字符或子字符串的位置。
find_first_not_of：查找第一个不匹配的字符或子字符串的位置。
find_last_not_of：查找最后一个不匹配的字符或子字符串的位置。
front：返回字符串的第一个字符。
get_allocator：返回字符串的分配器。
insert：在指定位置插入另一个字符串或字符。
length：返回字符串的长度。
max_size：返回字符串的最大可能长度。
pop_back：删除字符串的最后一个字符。
push_back：在字符串末尾添加一个字符。
rbegin：返回指向字符串开头的反向迭代器。
rend：返回指向字符串末尾的反向迭代器。
replace：替换字符串中的一部分。
reserve：为字符串预留存储空间。
resize：调整字符串的大小。
rfind：反向查找子字符串的位置。
shrink_to_fit：将字符串的容量缩小到与长度相同。
size：返回字符串的长度。
starts_with(c++20)：检查字符串是否以指定的前缀开头。
substr：返回字符串的子串。
swap：交换两个字符串的内容。
operator+=: 字符串拼接操作符。
operator[]：访问字符串中的字符。
operator!=：不等于操作符。
operator==：等于操作符。

## string_view

std::string_view 是 C++17 引入的一个轻量级的非拥有性字符串视图类，它位于 <string_view> 头文件中。std::string_view 提供了一种高效的方式来引用字符串数据，而不需要复制字符串内容，这使得它在处理字符串时非常节省内存和时间

成员函数：
compare: 比较字符串视图与另一个字符串视图或 C 风格字符串
length：返回字符串视图的长度
size: 返回字符串视图的长度
substr: 返回字符串视图的子字符串视图
starts_with: 检查字符串视图是否以指定的前缀开头
ends_with: 检查字符串视图是否以指定的后缀结尾
find: 在字符串视图中查找子字符串
rfind: 在字符串视图中反向查找子字符串
at: 返回指定位置的字符
operator[]: 返回指定位置的字符

## tuple

std::tuple 是 C++ 标准库中的一个模板类，用于将多个不同类型的值组合成一个单一的对象。
它类似于结构体（struct）或类（class），但不需要定义成员变量和成员函数。
std::tuple 可以包含任意数量和类型的元素，并且可以通过索引或类型来访问这些元素。

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

## utility

## variant

std::variant 是 C++17 标准库中引入的一个类型安全的联合体（union）替代品。
它允许你在一个变量中存储多种不同类型的值，并且在运行时可以安全地访问和操作这些值。
与传统的联合体不同，std::variant 会跟踪当前存储的是哪种类型的值，从而避免了类型不安全的问题。
