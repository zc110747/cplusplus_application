# C语言语法

- [基础语法](#basic_grammar)
  - [基本类型](#base_type)
  - [格式化](#format)
  - [内存布局](#layout)
- [C基础库](#c_lib)
- [C++扩展语法](#cpp_extend)
- [返回主页](../../README.md)

## basic_grammar

### base_type

- char，字符类型，长度1字节
- int，整型，不小于2字节
- float，单精度浮点类型，一般长度4字节
- double，双精度浮点类型，一般长度8字节
- long double, 双精度长浮点型，一般长度为16字节

修饰符
- short, 修饰符，可以int搭配，short int(可省略), 表示占用可能比int更少的存储空间（一般为2字节）
- long, 修饰符，可以int搭配，long int(可省略), 表示占用可能比int更多的存储空间（一般为4字节）
- long long, 修饰符，可以int搭配，long long int(可省略), 表示占用可能比int更多的存储空间（一般为8字节）
- unsigned/signed，有符号/无符号，和任意整型搭配(char, int, long)，unsigned int(可省略)表示无符号，signed int(可省略)表示有符号

扩展类型：
- _Bool, 布尔类型，长度1字节，取值true/false (#include <stdbool.h>)

### format

- printf：格式化输出
- scanf：格式化输入

| 格式化 | 格式化说明 | 类型 |
| --- | --- | --- |
| %d | 十进制整型 | int |
| %ld | 长整型 | long int |
| %lld | 64位整型 | long long int |
| %i | 有符号十进制整数，和%d相同 | int |
| %x/X | 十六进制整型 | int |
| %#x/#X | 带格式化0x/0X十六进制整型 | int |
| %u | 无符号十进制整数 | unsigned int |
| %o | 八进制整型 | int |
| %c | 字符类型 | char |
| %s | 字符串类型 | char * |
| %f | 浮点型,十进制计算型 | float/double |
| %lf | 长浮点型,十进制计算型 | float/double |
| %e/E | 科学计数法输出的浮点型 | float/double |
| %% | 打印一个百分号 | - |
| %p | 打印一个指针 | - |
   
### layout

最小存储单位是位(bit)，存储0、1，是计算机存储的基本构造位。

1. 字节(byte)是常用的计算机存储单位，字节的定义一般为8位。
2. 字(word)是计算机给定的自然存储单位，一般等于计算机的位宽；如32位的计算机，字长为4字节。

常用的计算长度的函数是sizeof和strlen，具体功能如下。

1. sizeof：返回数据/类型的长度, 如果是指针，返回的是指针占用的地址长度，而不是指针类型的长度。
2. strlen：返回字符串或者字符串数组的长度，以'\0'截止，不包含此位

计算机按照存储地址分为栈，堆，自由存储区，全局/静态存储区和常量存储区。

1. 栈，由编译器自动管理得，用于存储局部变量和函数参数的地址。
2. 堆，由new，new[]申请的，由用户管理的地址，需要通过手动调用delete，delete[]释放。
3. 自由存储区(动态存储区)，由malloc申请的内存块，和堆使用方法一致，需要手动free释放。
4. 全局/静态存储区，全局变量或者静态的局部变量，C中分为初始化(RW)和未初始化(ZI)区域，C++则占用同一块区域。
5. 常量存储区，定义为只读的数据存储区，一般不允许修改。

## c_lib

- **#include \<cassert\>** 包含assert语句处理
  - assert(expression) - 如果expression为假，则终止程序执行
- **#include \<cctype\>** 包含字符类型判断的一些处理
  - isalnum - 检查字符是否为字母或数字字符
  - isalpha - 检查字符是否为字母
  - isblank - 检查字符是否为空格或水平制表符
  - iscntrl - 检查字符是否为控制字符
  - isdigit - 检查字符是否为数字字符
  - isgraph - 检查字符是否为除空格字符外的可打印字符
  - islower - 检查字符是否为小写字母
  - isprint - 检查字符是否为可打印字符
  - ispunct - 检查字符是否为标点符号
  - isspace - 检查字符是否为空白字符
  - isupper - 检查字符是否为大写字母
  - isxdigit - 检查字符是否为16进制数字字符
  - tolower - 转换为小写字母
  - toupper - 转换为大写字母
- **#include \<cerrno\>** 错误处理
  - errno - 错误码
- **#include \<cfenv\>** 定义浮点处理模式
  - feclearexcept - 清除浮点异常标志
  - feraiseexcept - 触发浮点异常标志
  - fegetexceptflag: 获取当前浮点异常标志
  - fesetexceptflag: 设置浮点异常标志
  - fegetround: 获取当前浮点舍入模式
  - fesetround: 设置当前浮点舍入模式
  - fegetenv: 获取当前浮点环境
  - fesetenv: 设置当前浮点环境
  - feholdexcept: 保存当前浮点环境
  - feupdateenv: 更新当前浮点环境
  - fetestexcept: 测试浮点异常标志
- **#include \<cfloat\>** 定义浮点判断的一些宏定义
- **#include \<cinttypes\>** 标准库中定义的一种整数类型，系统支持的最大宽度的有符号整数类型
  - imaxabs - 获取取绝对值的intmax_t类型
  - imaxdiv - 获取商的imaxdiv_t类型
  - strtoimax - 字符串转换成intmax_t类型
  - strtoumax - 字符串转换成无符号intmax_t类型
  - wcstoimax - 字符串转换成intmax_t类型
  - wcstoumax - 字符串转换成无符号intmax_t类型
- **#include \<ciso646\>** 运算符重定义
- **#include \<climits\>** 获取系统支持的最大宽度的整数类型
  - INT_MAX - 最大的有符号整数
  - INT_MIN - 最小的有符号整数
  - UINT_MAX - 最大的无符号整数
- **#include \<clocale\>** 本地化环境处理
  - setlocale - 设置本地化环境
  - localeconv - 获取当前本地化环境
- **#include \<cmath\>** 数学处理
  - 三角函数：cos，sin，tan，acos，asin，atan，atan2
  - 双曲函数：cosh，sinh，tanh，acosh，asinh，atanh
  - 指数和对数函数：exp，frexp, ldexp，log, log10, modf, exp2，expm1，ilogb，log1p，log2，logb，scalbln，scalbn
  - 幂函数：pow，sqrt，cbrt, hypot
  - 误差函数：erf, erfc, lgamma, tgamma
  - 圆周函数: ceil, floor, fmod，trunc，round，lround，llround，rint，lrint，llrint，nearbyint，remainder，remquo
  - 浮点处理函数： copysign, nan, nextafter, nexttoward
  - 最大最小函数：fdim, fmax, fmin
  - 其它函数：fabs，abs，fma
  - 分类宏: fpclassify，isfinite, isinf, isnan, isnormal, signbit, isgreater, isgreaterequal, isless, islessequal, islessgreater, isunordered
- **#include \<csetjmp\>** 跳转处理
  - longjmp - 跳转处理
  - setjmp - 保存跳转环境
- **#include \<csignal\>** 信号处理，类型sig_atomic_t
  - signal - 信号处理
  - raise - 发送信号
- **#include \<cstdarg\>** 可变参数处理
  - va_list - 可变参数列表
  - va_start - 初始化可变参数列表
  - va_arg - 获取可变参数列表中的参数
  - va_end - 结束可变参数列表
  - va_copy - 复制可变参数列表
- **#include \<cstddef\>** 获取标准类型定义
  - std::byte是C++17引入的标准库类型，于表示最小的可寻址内存单元，也就是字节。它是一个枚举类型，用于更安全、更明确地处理字节数据。
- **#include \<cstdint\>** 获取标准整数类型定义
- **#include \<cstdio\>** 输入输出处理
  - 文件处理: remove，rename，tmpfile，tmpnam
  - 文件访问：fclose，fflush，fopen，freopen，setbuf，setvbuf
  - 格式化输入输出：fprintf，fscanf，printf，scanf，snprintf，sprintf，sscanf，vfprintf，vfscanf，vprintf，vscanf，vsnprintf，vsprintf，vsscanf
  - 字符输入输出: fgetc，fgets，fputc，fputs，getc，getchar，gets，putc，putchar，puts，ungetc
  - 直接输入输出: fread，fwrite
  - 文件位置：fgetpos，fseek，ftell，rewind
  - 异常处理：clearerr，feof，ferror，perror
- **#include \<cstdlib\>** 常见的C库函数
  - 类型转换: atof，atoi，atol，atoll，strtod，strtof，strtold, strtoll, strtoul, strtoull
  - 随机数: rand, srand
  - 内存申请: calloc, free, malloc, realloc
  - 环境变量: abort，exit，getenv，system，qsort
  - 运算方法: abs，div，labs，ldiv，llabs，lldiv
- **#include \<cstring\>** 包含字符串处理的一些函数
  - 字符串处理: strlen，strncat，strcmp，strncmp，strcpy，strncpy，strchr，sprintf，sscanf

## cpp_extend

- 新字符类型(Unicode)
  - C++20:char8_t，Unicode字符(1字节)
  - C++17:char16_t, char32_t；Unicode字符(2字节，4字节)

- c++函数
  - 函数声明，函数定义，函数调用 
- 函数的默认参数允许你为函数的一个或多个参数提供默认值
  - 在C++中，函数的默认参数允许你为函数的一个或多个参数提供默认值。当调用该函数时，如果没有为这些参数提供值，那么将使用默认值

- 预处理器

| 预处理器宏 | 说明 |
| --- | --- |
| \__func\__ | 获取函数名称 |
| \__VA_ARGS\__ | 获取可变参数 |
| \__FILE\__ | 获取文件名称 |
| \__LINE\__ | 获取行号 |
| \__DATE\__ | 获取编译日期 |
| \__TIME\__ | 获取编译时间 |
| \__STDC_VERSION\__ | 获取C++标准版本 |
| \__INT_MAX\__ | 最大的有符号整数 |
| \__INT_MIN\__ | 最小的有符号整数 |
| \__UINT_MAX\__ | 最大的无符号整数 |
| \__LONG_MAX\__ | 最大的有符号长整数 |
| \__LONG_MIN\__ | 最小的有符号长整数 |
| \__ULONG_MAX\__ | 最大的无符号长整数 |
| \__has_include\__ | 检查是否包含头文件, C++17支持 |
| \__has_feature\__ | 检查是否支持某个特性 |
| \__has_extension\__ | 检查是否支持某个扩展 |
| \__has_builtin\__ | 检查是否支持某个内建函数 |
| \__has_attribute\__ | 检查是否支持某个属性 |

具体示例如下所示。

```cpp
#include <climits>

// c++ 17
#if __has_include(<iostream>) 
#include <iostream>
#endif

int main(int argc, char const *argv[])
{
    std::cout << "func: " << __func__ << std::endl;
    std::cout << "file: " << __FILE__ << std::endl;
    std::cout << "line: " << __LINE__ << std::endl;
    std::cout << "date: " << __DATE__ << std::endl;
    std::cout << "time: " << __TIME__ << std::endl;
    std::cout << "__cplusplus: " << __cplusplus << std::endl;
    std::cout << "int_max: " << INT_MAX << std::endl;
    std::cout << "int_min: " << INT_MIN << std::endl;
    std::cout << "uint_max: " << UINT_MAX << std::endl;
    std::cout << "long_max: " << LONG_MAX << std::endl;
    std::cout << "long_min: " << LONG_MIN << std::endl;
    std::cout << "ulong_max: " << ULONG_MAX << std::endl;
    std::cout << "has_feature: " << __has_feature(cpp17) << std::endl;
    std::cout << "has_extension: " << __has_extension(cpp17) << std::endl;
    std::cout << "has_builtin: " << __has_builtin(__builtin_abs) << std::endl;
    std::cout << "has_attribute: " << __has_attribute(noreturn) << std::endl;

    return 0;
}
```


