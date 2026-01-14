# CPP基础语法

- [基本类型](#base_type)
- [复合类型](#compound_type)
  - [array](#array)
  - [pointer](#pointer)
  - [struct](#string)
  - [union](#union)
  - [enum](#enum)
- [格式化](#stdio_format)
- [内存布局](#layout)
- [C基础库](#c_lib)
- [C++扩展语法](#cpp_extend)
  - [function](#function)
  - [pre_processor](#pre_processor)
  - [limits](#limits)
  - [numbers](#numbers)
  - [stdfloat](#stdfloat)
  - [version](#version)
- [返回主页](../../README.md)

## basic_grammar

## base_type

基础类型:

- char，字符类型，长度1字节
- int，整型，不小于2字节
- float，单精度浮点类型，一般长度4字节
- double，双精度浮点类型，一般长度8字节
- long double, 双精度长浮点型，一般长度为16字节

修饰符:

- short, 修饰符，可以int搭配，short int(可省略), 表示占用可能比int更少的存储空间（一般为2字节）
- long, 修饰符，可以int搭配，long int(可省略), 表示占用可能比int更多的存储空间（一般为4字节）
- long long, 修饰符，可以int搭配，long long int(可省略), 表示占用可能比int更多的存储空间（一般为8字节）
- unsigned/signed，有符号/无符号，和任意整型搭配(char, int, long)，unsigned int(可省略)表示无符号，signed int(可省略)表示有符号

扩展类型:

- chart8_t, C++20支持，8位字符类型，长度1字节
- char16_t, 16位字符类型，长度2字节
- char32_t, 32位字符类型，长度4字节
- bool, 布尔类型，长度1字节
- void, 空类型，长度0字节
- wchar_t, 宽字符类型，长度2字节
- size_t, 存储大小类型，长度为sizeof(size_t)字节，unsigned int(可省略)
- float16_t, 16位浮点类型，长度2字节
- float32_t, 32位浮点类型，长度4字节
- float64_t, 64位浮点类型，长度8字节

类型后缀：

- u : 无符号整型，unsigned int类型。
- l: 长整型，long类型。
- ul : 无符号长整型，unsigned long类型。
- ll : 长长整型，long long类型。
- ull : 无符号长长整型，unsigned long long类型。
- f: 单精度浮点类型，float类型。
- f32: 32位浮点类型，float类型。
- f64: 64位浮点类型，double类型。

隐式整型转换。

- 隐式整型转换时，遵从从低等级到高等级的转换规则；有符号向无符号转换。
- 至于转换等级，长度越长的整型等级越高，例如short < int；长度相同时，标准整型高于扩展整型。

具体示例如下：

```c
#include <iostream>
#include <stdfloat>

int main(int argc, const char *argv[])
{
    int a1 = 10;
    unsigned int a2 = 10ul;
    long a3 = 10ll;

    std::cout << "a1: " << a1 << std::endl;
    std::cout << "a2: " << a2 << std::endl;
    std::cout << "a3: " << a3 << std::endl;

    float f1 = 10.0f;
    std::float16_t f2 = 10.0f16;
    std::float32_t f3 = 10.0f32;
    std::float64_t f4 = 10.0f64;
    std::float128_t f5 = 10.0f128;

    std::cout << "f1: " << f1 << std::endl;
    std::cout << "f2: " << f2 << std::endl;
    std::cout << "f3: " << f3 << std::endl;
    std::cout << "f4: " << f4 << std::endl;

    return 0;
}
```

## compound_type

复合类型是通过基本类型组合或者处理得到的数据类型。包含数组、指针、结构体、联合体、枚举等。

### array

数组是一组相同类型的元素，存储在一起，元素个数固定，元素类型固定，元素类型和数组类型一致。可通过sizeof运算符获取数组的长度，单位为字节。另外还支持多维数组，例如int a[2][3]表示一个2行3列的整型数组。

具体示例如下：

```c
#include <iostream>

int main(int argc, const char *argv[])
{
    // 一维数组
    int a[5] = {1, 2, 3, 4, 5};
    size_t nums = sizeof(a)/sizeof(a[0]);

    std::cout << "nums: " << nums << std::endl;

    for (int i = 0; i < nums; i++) {
        std::cout << a[i] << " ";
    }
    std::cout << std::endl;

    // 多维数组
    int arr[2][3] = {
        {1, 2, 3}, 
        {4, 5, 6}
    };
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            std::cout << arr[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    return 0;
}
```

### pointer

指针是一种数据类型，它保存一个内存地址，指针类型和被保存的地址类型一致。可以使用&运算符获取指针指向的内存地址，使用*运算符获取指针指向的值。另外对于指针可以使用C语言中的malloc，free来分配和释放内存，也可以使用C++中的new，delete/new，delete[]来分配和释放内存。

另外pointer也可以使用sizeof运算符获取指针的长度，这个是指针占用的地址，而非指针指向的内存地址或类型的长度，与平台以及使用的编译环境相关。一般32位平台为4字节，64位平台根据编译选项不同，可以为4字节或8字节。

具体示例如下：

```c
#include <iostream>

int main(int argc, const char *argv[])
{
    int a = 10;
    short b = 20;

    // 取得指针
    int *pa = &a;
    short *pb = &b;

    // 解指针、获取指针长度
    std::cout << "pa: " << *pa << std::endl;
    std::cout << "pb: " << *pb << std::endl;
    std::cout << "pa size: " << sizeof(pa) << std::endl;
    std::cout << "pb size: " << sizeof(pb) << std::endl;

    int arr[5] = {1, 2, 3, 4, 5};
    int *p1 = arr;

    // 指针的运算，加减偏移类型的长度
    p1 += 1;
    std::cout << "p1: " << *p1 << std::endl;
    std::cout << "offset: " << (uint64_t)p1 - (uint64_t)arr << std::endl;

    // malloc, free
    int *p2 = (int *)malloc(sizeof(int));
    if (p2 == nullptr) {
        std::cout << "malloc failed" << std::endl;
        return -1;
    }
    *p2 = 10;
    std::cout << "p2: " << *p2 << std::endl;
    free(p2);

    // new, delete/new, delete[]
    int *p3 = new int(10);               //申请单个变量并初始化
    std::cout << "p3: " << *p3 << std::endl;
    delete p3;

    // new[], delete[]
    int *p4 = new int[5]{1, 2, 3, 4, 5};   //申请数组并初始化
    std::cout << "p4: " << *p4 << std::endl;
    delete[] p4;

    return 0;
}
```

#### struct

结构体是一种数据类型，它保存一组变量，变量存储在一起，元素个数可变，元素类型可变，元素类型和结构体类型一致。在C++中，结构体的功能和类非常相似，但结构体和类之间有如下区别：

- 结构体默认所有成员都是public的，而类是private的。
- 结构体默认继承的访问权限也是public的，而类是private的。

具体示例如下所示。

```cpp
#include <iostream>

struct test_struct {
    int a;
    int b;
    int c;
    inline static int d{1};
    static int get_d() {
        return d;
    }
};

struct bit_field
{
    int a1: 2;
    int a2: 3;
    int a3: 2;
    int def: 25;
};

int main(int argc, const char *argv[])
{ 
    // 结构体初始化
    test_struct s1 = {1, 2, 3};
    std::cout << "s1.a: " << s1.a << std::endl;
    std::cout << "s1.b: " << s1.b << std::endl;
    std::cout << "s1.c: " << s1.c << std::endl;

    test_struct s2 = {.a = 1, .b = 2, .c = 3};
    std::cout << "s2.a: " << s2.a << std::endl;
    std::cout << "s2.b: " << s2.b << std::endl;
    std::cout << "s2.c: " << s2.c << std::endl;

    // 静态成员变量
    std::cout << "test_struct::d: " << test_struct::d << std::endl;
    std::cout << "s1.d: " << s1.d << std::endl;
    s1.d = 2;
    std::cout << "test_struct::get_d(): " << test_struct::get_d() << std::endl;
    std::cout << "s2.d: " << s2.get_d() << std::endl;

    // sizeof
    std::cout << "sizeof(s1): " << sizeof(s1) << std::endl;

    // bit_field 位域
    // 计算机中以补码存储，最高位为符号位，0为正，1为负，对于3bit, 范围为[-4, 3]，最大值011，最小值100
    bit_field bf1 = {.a1 = 1, .a2 = 7, .a3 = 3};
    std::cout << "sizeof(bf1): " << sizeof(bf1) << std::endl;
    std::cout << "bf1.a1: " << bf1.a1 << std::endl;     // 1
    std::cout << "bf1.a2: " << bf1.a2 << std::endl;     // -1
    std::cout << "bf1.a3: " << bf1.a3 << std::endl;     // -1
    std::cout << "bf1.def: " << bf1.def << std::endl;
    return 0;
}
```

### union

联合体是一种数据类型，它保存一组变量，变量存储在一起，元素个数可变，元素类型可变，元素类型和联合体类型一致。联合体中的所有变量共享同一块内存空间，因此联合体的大小等于其最大的成员的大小。联合体内部的变量默认访问权限和结构体默认访问权限一致，都是public。

具体示例如下所示。

```cpp
#include <iostream>

union union_struct
{
    uint32_t val_;

    struct {
        uint32_t a1_:1;
        uint32_t a2_:3;
        uint32_t a3_:4;
        uint32_t def_:24;
    } bits;
};

int main(int argc, const char *argv[])
{
    union_type u1;
    u1.a1 = 10;
    std::cout << "u1.a1: " << u1.a1 << std::endl;
    std::cout << "u1.a2: " << u1.a2 << std::endl;

    if (u1.a1 == u1.a2) {
        std::cout << "小端存储" << std::endl;
    } else {
        std::cout << "大端存储" << std::endl;
    }

    u1.a2 = 20;
    std::cout << "u1.a1: " << u1.a1 << std::endl;
    std::cout << "u1.a2: " << u1.a2 << std::endl;

    u1.f1 = 30.0f;
    std::cout << "u1.a1: " << u1.a1 << std::endl;
    std::cout << "u1.a2: " << u1.a2 << std::endl;
    std::cout << "u1.f1: " << u1.f1 << std::endl;

    // bit_field
    union_struct u2 = {.val_ = 0x12345678};
    std::cout << std::hex;
    std::cout << "u2.bits.a1_: 0x" << u2.bits.a1_ << std::endl;
    std::cout << "u2.bits.a2_: 0x" << u2.bits.a2_ << std::endl;
    std::cout << "u2.bits.a3_: 0x" << u2.bits.a3_ << std::endl;
    std::cout << "u2.bits.def_: 0x" << u2.bits.def_ << std::endl;
    return 0;
};
```

### stdio_format

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

### function

C++函数和C99标准类似，但有额外的功能支持，具体说明如下所示。

- 和C语言类型，包含函数声明、函数定义和函数调用，与C语言基本一致。 
- 对于C++函数来说，支持函数重载，即可以定义多个函数，函数名相同但参数不同。
- 在C++中，函数的默认参数允许你为函数的一个或多个参数提供默认值。当调用该函数时，如果没有为这些参数提供值，那么将使用默认值。

具体示例如下：

```cpp
#include <iostream>

// 函数声明，可以带默认值
int add(int a, int b = 1);
int add(int a, int b, int c);

int main(int argc, const char *argv[]) 
{
    std::cout << add(1) << std::endl;
    std::cout << add(1, 2) << std::endl;
    std::cout << add(1, 2, 3) << std::endl;
    return 0;
}

// 函数定义
// 函数重载
int add(int a, int b) {
    return a + b;
}

int add(int a, int b, int c) {
    return a + b + c;
}
```

### pre_processor

宏定义和预处理器是C语言支持的功能，具体如下所示。C语言提供了丰富的预处理机制，方便了跨平台的代码的实现，此外C语言通过宏机制实现的数据和代码块替换，字符串格式化，代码段切换，对于工程应用具有重要意义，下面按照功能需求，描述在C语言运用中的常用预处理机制。

`#include`：包含文件命令，在C语言中，它执行的效果是将包含文件中的所有内容插入到当前位置，这不只包含头文件，一些参数文件，配置文件，也可以使用该文件插入到当前代码的指定位置。其中<>和""分别表示从标准库路径还是用户自定义路径开始检索。

`#define`：宏定义，常见的用法包含定义常量或者代码段别名，当然某些情况下配合##格式化字符串，可以实现接口的统一化处理，实例如下：

```cpp
#define MAX_SIZE  10
#define MODULE_ON  1
#define ERROR_LOOP() do{ \
                     printf("error loop\n"); \
                   }while(0);
#define global(val) g_##val
int global(v) = 10;
int global(add)(int a, int b)
{
    return a+b;
}
```

`#if..#elif...#else...#endif`：条件选择判断，根据预定义的宏参数，选择不同的代码块执行。

`#ifdef..#endif, #ifndef...#endif`：条件选择判断，根据预定义的宏参数，选择不同的代码块执行。这种综合性项目和跨平台项目中为了满足多种情况下的需求往往会被使用。

`#undef`：取消定义的参数，避免重定义问题。

`#error，#warning`：用于用户自定义的告警信息，配合#if，#ifdef使用，可以限制错误的预定义配置。

`#pragma`：带参数的预定义处理，常见的`#pragma pack(1)`, 不过使用后会导致后续的整个文件都以设置的字节对齐，配合push和pop可以解决这种问题，代码如下。

```cpp
#pragma pack(push)
#pragma pack(1)
struct TestA
{
   char i;
   int b;
}A;
#pragma pack(pop); //注意要调用pop，否则会导致后续文件都以pack定义值对齐，执行不符合预期
```

等同于

```cpp
 struct _TestB{  
   char i;
   int b;
 }__attribute__((packed))A; 
```

- 常用宏定义

| 预处理器宏 | 说明 |
| --- | --- |
| `__func__` | 获取函数名称 |
| `__cplusplus` | 获取C++标准版本 |
| `__VA_ARGS__` | 获取可变参数 |
| `__FILE__` | 获取文件名称 |
| `__LINE__` | 获取行号 |
| `__DATE__` | 获取编译日期 |
| `__TIME__` | 获取编译时间 |
| `__STDC__` | 表示编译器的实现是否与C标准兼容 |
| `__STDC_VERSION__` | 获取C标准版本 |
| `__STDC_HOSTED__` | 编译器的目标系统环境是否包含完整的c库 |
| `__INT_MAX__` | 最大的有符号整数 |
| `__INT_MIN__` | 最小的有符号整数 |
| `__UINT_MAX__` | 最大的无符号整数 |
| `__LONG_MAX__` | 最大的有符号长整数 |
| `__LONG_MIN__` | 最小的有符号长整数 |
| `__ULONG_MAX__` | 最大的无符号长整数 |
| `__has_include__` | 检查是否包含头文件, C++17支持 |
| `__has_feature__` | 检查是否支持某个特性 |
| `__has_extension__` | 检查是否支持某个扩展 |
| `__has_builtin__` | 检查是否支持某个内建函数 |
| `__has_attribute__` | 检查是否支持某个属性 |

- `__Pragma`

`__Pragma`操作符是C++11引入的，用于在编译时执行一些操作，其格式如下所示。

```cpp
// 语法
__Pragma(字符串字面量)
```

目前主要实现的有`__Pragma("once")`，等同于`#pragma once`, 用于防止头文件被重复包含。

- `__VA_ARGS__`

`__VA_ARGS__` 是C99标准引入的，可以使用变长参数的宏定义，C++11同样也支持该功能，主要使用方法如下所示。

```cpp
// 配合printf使用
#define PR(...) printf(__VA_ARGS__)

#define ERR(...) do { \
                    fprintf(stderr, "%s:%d: ", __FILE__, __LINE__); \
                    fprintf(stderr, __VA_ARGS__); \
                    fprintf(stderr, "\n");
                } while (0)
```

### limits

limits头文件定义了C++标准库中各种类型的最大和最小值。

limits参考网址: <https://en.cppreference.com/w/cpp/types/numeric_limits.html>。

具体方法如下所示。

| 方法 | 说明 |
| --- | --- |
| is_specialized | 是否为特殊化类型 |
| is_signed | 是否为有符号类型 |
| is_integer | 是否为整数类型 |
| is_exact | 是否为精确类型 |
| has_infinity | 是否有无限大 |
| has_quiet_NaN | 是否有quite的NaN值 |
| has_signaling_NaN | 是否有signaling的NaN值 |
| has_denorm | 是否有非正规数 |
| has_denorm_loss | 是否有非正规数损失 |
| round_style | 四舍五入样式 |
| is_iec559 | 是否为IEC 599/IEEE 754浮点数 |
| is_bounded | 是否为有界类型 |
| is_modulo | 是否为模运算类型 |
| digits | 二进制位数 |
| digits10 | 十进制位数 |
| max_digits10 | 最大的十进制位数 |
| radix | 基数 |
| min_exponent | 最小的指数 |
| max_exponent | 最大的指数 |
| min_exponent10 | 最小的10进制指数 |
| max_exponent10 | 最大的10进制指数 |
| traps | 是否为陷阱类型 |
| tinyness_before | 是否在四舍五入之前检测非正规数损失 |
| min | 返回类型T的最小负值 |
| lowest | 返回类型T的最小负值（对于有符号类型）或最大负值（对于无符号类型） |   
| max | 返回类型T的最大正值 |
| epsilon | 类型T的最小正值 |
| round_error | 四舍五入误差 |
| infinity | 类型T的无限大 |
| quiet_NaN | 返回类型T的quite的NaN值 |
| signaling_NaN | 返回类型T的signaling的NaN值 |
| denorm_min | 类型T的非正规数最小值 |


具体示例如下所示。

```cpp
#include <climits>
#include <iostream>
#include <limits>

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

    std::cout << "max: " << std::numeric_limits<int>::max() << std::endl;
    std::cout << "min: " << std::numeric_limits<int>::min() << std::endl;
    std::cout << "lowest: " << std::numeric_limits<int>::lowest() << std::endl;
    std::cout << "epsilon: " << std::numeric_limits<int>::epsilon() << std::endl;
    std::cout << "round_error: " << std::numeric_limits<int>::round_error() << std::endl;
    std::cout << "infinity: " << std::numeric_limits<int>::infinity() << std::endl;
    std::cout << "quiet_NaN: " << std::numeric_limits<int>::quiet_NaN() << std::endl;
    std::cout << "signaling_NaN: " << std::numeric_limits<int>::signaling_NaN() << std::endl;
    std::cout << "denorm_min: " << std::numeric_limits<int>::denorm_min() << std::endl;

    return 0;
}
```

### numbers

numbers是C++20引入的用于提供一些常用的数学常量的库文件。它包含了一些常用的数学常量，例如圆周率π、自然对数的底数e等。

numbers参考网址：<https://en.cppreference.com/w/cpp/header/numbers.html>

支持的常用数学常量如下所示。

| 常量 | 描述 |
| --- | --- |
| e | 指数e的值 |
| log2e | 底数2对e的对数 |
| log10e | 底数2对10的对数 |
| pi | 圆周率π的值 |
| inv_pi | 圆周率pi的倒数 |
| inv_sqrtpi | 圆周率pi的倒数开方 |
| ln2 | 底数e对2的对数 |
| ln10 | 底数e对10的对数 |
| sqrt2 | 根号2的值 |
| sqrt3 | 根号3的值 |
| inv_sqrt3 | 根号3的倒数 |
| egamma | 欧拉常数（Euler's constant）的值 |
| phi | 黄金比例（Golden ratio）的值 |

具体示例如下所示。

```cpp
#include <numbers>
#include <iostream>

int main(int argc, const char* argv[])
{
    std::cout << "e: " << std::numbers::e << std::endl;
    std::cout << "log2e: " << std::numbers::log2e << std::endl;
    std::cout << "log10e: " << std::numbers::log10e << std::endl;
    std::cout << "pi: " << std::numbers::pi << std::endl;
    std::cout << "inv_pi: " << std::numbers::inv_pi << std::endl;
    std::cout << "inv_sqrtpi: " << std::numbers::inv_sqrtpi << std::endl;
    std::cout << "ln2: " << std::numbers::ln2 << std::endl;
    std::cout << "ln10: " << std::numbers::ln10 << std::endl;
    std::cout << "sqrt2: " << std::numbers::sqrt2 << std::endl;
    std::cout << "sqrt3: " << std::numbers::sqrt3 << std::endl;
    std::cout << "inv_sqrt3: " << std::numbers::inv_sqrt3 << std::endl;
    std::cout << "egamma: " << std::numbers::egamma << std::endl;
    std::cout << "phi: " << std::numbers::phi << std::endl;

    return 0;
}
```

### stdfloat

stdfloat是C++23实现的头文件，定义了指定长度的浮点数类型。

stdfloat参考网址: <https://en.cppreference.com/w/cpp/header/stdfloat.html>。

定义的类型如下。

| 类型 | 说明 |
| --- | --- |
| std::float16_t | 16位浮点数 |
| std::float32_t | 32位浮点数 |
| std::float64_t | 64位浮点数 |
| std::float128_t | 128位浮点数 |
| std::bfloat16_t | 16位浮点数 |

具体示例如下所示。

```cpp
#include <iostream>
#include <stdfloat>

#if __STDCPP_FLOAT64_T__ != 1
    #error "64-bit float type required"
#endif

int main(int argc, char const *argv[])
{
    std::float16_t f16 = 0.1f16;
    std::cout << "f16: " << f16 << std::endl;

    std::float32_t f32 = 0.1f32;
    std::cout << "f32: " << f32 << std::endl;

    std::float64_t f64 = 0.1f64;
    std::cout << "f64: " << f64 << std::endl;

    return 0;
}
```

### version

version是C++20引入的一个命名空间，用于提供一些常用的数学常量。它包含了一些常用的版本号常量，例如C++20的版本号为202002L。

version参考网址: <https://en.cppreference.com/w/cpp/header/version.html>。

其内部通过包含<ciso646>定义了大量了C++特征宏常量，例如__cpp_strong_enums、__cpp_ranges等。

这里以C++20为例，展示列表如下地址: <https://en.cppreference.com/w/cpp/utility/feature_test.html>。这里展示下关键的已经支持的宏常量。

| 宏常量 | 说明 |
| --- | --- |
| __cpp_lib_any | 支持std::any |
| __cpp_lib_apply | 支持std::apply | 
| __cpp_lib_as_const | 支持std::as_const |
| __cpp_lib_chrono | 支持std::chrono |
| __cpp_lib_variant | 支持std::variant |
| __cpp_lib_void_t | 支持std::void_t |

具体示例如下所示。

```cpp
#include <iostream>
#include <version>

int main(int argc, char const *argv[])
{
    std::cout << "__cpp_lib_any: " << __cpp_lib_any << std::endl;
    std::cout << "__cpp_lib_apply: " << __cpp_lib_apply << std::endl;
    std::cout << "__cpp_lib_as_const: " << __cpp_lib_as_const << std::endl;
    std::cout << "__cpp_lib_chrono: " << __cpp_lib_chrono << std::endl;
    std::cout << "__cpp_lib_variant: " << __cpp_lib_variant << std::endl;
    std::cout << "__cpp_lib_void_t: " << __cpp_lib_void_t << std::endl;

    return 0;
}
```
