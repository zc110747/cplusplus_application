/*
c++兼容c语言的实现，主要包含基础数据类型，循环，指针，sizeof等, 
这部分可以参考《C Primer Plus》学习。

基础类型
- char，字符类型，长度1字节
- int，整型，支持关键字short, long, long long, unsigned、signed具有不同含义
- long long，长整型，至少为64位，可在数字后加LL(ll)标识变量为长整型
- float，单精度浮点类型，长度4字节
- double，双精度浮点类型，长度8字节

新字符类型(Unicode)

1. C++20:char8_t，Unicode字符(1字节)
2. C++17:char16_t, char32_t；Unicode字符(2字节，4字节)

格式化输出

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
| %e/E | 科学计数法输出的浮点型 | float/double |
| %% | 打印一个百分号 | - |
| %p | 打印一个指针 | - |

内存

最小存储单位是位(bit)，存储0、1，是计算机存储的基本构造位
字节(byte)是常用的计算机存储单位，字节的定义一般为8位
字(word)是计算机给定的自然存储单位，一般等于计算机的位宽，如32位的计算机，字长为4字节

sizeof 返回数据/类型的长度, 如果是指针，返回的是指针占用的地址长度，而不是指针类型的长度。
strlen 返回字符串或者字符串数组的长度，以'\0'截止，不包含此位

计算机按照存储地址分为栈，堆，自由存储区，全局/静态存储区和常量存储区。

1.栈，由编译器自动管理得，用于存储局部变量和函数参数的地址。
2.堆，由new，new[]申请的，由用户管理的地址，需要通过手动调用delete，delete[]释放。
3.自由存储区，由malloc申请的内存块，和堆使用方法一致，需要手动free释放。
4.全局/静态存储区，全局变量或者静态的局部变量，C中分为初始化(RW)和未初始化(ZI)区域，C++则占用同一块区域。
5.常量存储区，定义为只读的数据存储区，一般不允许修改。

c++函数
- 函数声明，函数定义，函数调用
- 函数的默认参数允许你为函数的一个或多个参数提供默认值
  - 在C++中，函数的默认参数允许你为函数的一个或多个参数提供默认值。当调用该函数时，如果没有为这些参数提供值，那么将使用默认值

strlen
说明: 用于统计字符串的长度。
函数原型: size_t strlen(const char *string);

strncat
说明: 在字符串的结尾追加n个字符(覆盖\0)，实现字符串拼接
函数原型: char* strncat(char* dest, const char *src, size_t n);

strcmp
说明：比较两个字符串，并根据结果返回
函数原型: int strcmp(const char *str1,const char *str2)

strncmp
说明：比较两个字符串的前n个字节，并根据结果返回
函数原型: int strncmp(const char *str1, const char *str2, size_t n)

strcpy
说明：将源地址的数据复制到目的地址
函数原型: char * strcpy ( char * destination, const char * source );

strncpy
说明：将源地址的前n个数据复制到目的地址
函数原型: char * strncpy ( char * destination, const char * source, size_t num );

strchr
说明：返回指向 C 字符串 str 中第一个出现的字符的指针。
函数原型:  char * strchr (char * str, int character );

sprintf
说明：格式化字符串保存到目的地址中
函数原型: int sprintf ( char * str, const char * format, ... );

sscanf
说明：从字符串中格式化读取数据
函数原型:  int sscanf ( const char * s, const char * format, ...);

__func__: 获取函数名称
__VA_ARGS__: 获取可变参数
__FILE__: 获取文件名称
__LINE__: 获取行号
__DATE__: 获取编译日期
*/

#include <iostream>
#include <cstdio>
#include <climits>
#include <stdlib.h>
#include <stdarg.h>

using namespace std;

void standard_improve(void);

int main(int argc, char **argv)
{
    // 1. 基础数据类型
    {
        char a = '0';
        short b = 1;
        int c = 1;
        long d = 1;
        float e = 1.0;
        double f = 1.0;
        char *p = &a;
        int *q = &c;

        printf("a = %c, b = %hd, c = %d, d = %ld, e = %f, f = %lf\n", a, b, c, d, e, f);
        printf("p = %p, q = %p\n", p, q);
        printf("sizeof(a) = %ld, sizeof(b) = %ld, sizeof(c) = %ld, sizeof(d) = %ld, sizeof(e) = %ld, sizeof(f) = %ld\n", 
            sizeof(a), sizeof(b), sizeof(c), sizeof(d), sizeof(e), sizeof(f));
    }

    // 2.数组, 指针
    {
        int a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        int b[2][5] = {
            {1, 2, 3, 4, 5},
            {6, 7, 8, 9, 10}
        };
        int i, j;

        for(i = 0; i < 10; i++) {
            printf("a[%d] = %d, ", i, a[i]); 
        }
        printf("\n");

        for(i = 0; i < 2; i++) {
            for(j = 0; j < 5; j++) {
                printf("b[%d][%d] = %d, ", i, j, b[i][j]);
            }
            printf("\n");
        }

        int *pa = a;
        int(*pb)[5] = b;

        for(i = 0; i < 10; i++) {
            printf("a[%d] = %d, ", i, pa[i]); 
        }
        printf("\n");

        for(i = 0; i < 2; i++) {
            for(j = 0; j < 5; j++) {
                printf("b[%d][%d] = %d, ", i, j, pb[i][j]);
            }
            printf("\n");
        }

    }

    // 3.循环
    {
        int sum = 0;
        int i;
        
        for (i = 0; i < 10; i++) {
            sum += i;
        }
        printf("sum = %d\n", sum);

        i = 0;
        sum = 0;
        while(i < 10) {
            sum += i;
            i++;
        }
        printf("sum = %d\n", sum);

        i = 0;
        sum = 0;
        do
        {
            sum += i;
            i++;
        } while (i < 10);
        printf("sum = %d\n", sum);
    }

    //预定义宏
    {
        printf("__STDC__ = %d\n", __STDC__);
        printf("__DATE__ = %s\n", __DATE__);
        printf("__TIME__ = %s\n", __TIME__);
        printf("__FILE__ = %s\n", __FILE__);
    }

    standard_improve();
    return 0;
}

struct struct_test
{
    struct_test():name_(__func__) {
    }
    const char *name_;
};

#define ToCharArr(x)            #x
#define PR(...)                 printf(__VA_ARGS__);
#define LOG(...)                do { \
    fprintf(stderr, "%s: Line %d:\t", __FILE__, __LINE__);\
    fprintf(stderr, __VA_ARGS__); \
    fprintf(stderr, "\n");  \
}while(0);

// 函数声明
void printMessage(const std::string& message = "Hello, World!");

// 函数定义
void printMessage(const std::string& message) {
    std::cout << message << std::endl;
}

double SumOfFloat(int cont, ...){
    va_list ap;
    double sum = 0;
    va_start(ap, cont);

    for(int i=0; i<cont; i++)
        sum += va_arg(ap, double);

    va_end(ap);
    return sum;
}

void standard_improve(void)
{
    std::cout<<"Standard Clib: "<< __STDC_HOSTED__<<std::endl;
    std::cout<<"Standard C: "<<__STDC__<<std::endl;
    
    // __func__获取函数的名称
    std::cout<<struct_test().name_<<std::endl;

    // __VA_ARGS__获取可变参数
    LOG("Hello World!");
    LOG("%s", "NO ERR!");
    PR("Hello World!\n");

    // long long int: 64位整型
    long long int lli = -90000000000LL;
    unsigned long long int ulli = 9000000000000ULL;
    cout<<"LLONG_MIN: "<<LLONG_MIN<<endl;    //LLONG_MIN: -9223372036854775808
    cout<<"LLONG_MAX: "<<LLONG_MAX<<endl;    //LLONG_MAX: 9223372036854775807
    cout<<"ULLONG_MAX: "<<ULLONG_MAX<<endl;  //ULLONG_MAX: 18446744073709551615
    cout<<"ULLONG_INT: "<<ulli<<endl;

    printMessage();
    printMessage("Hello, C++!");

    cout<<SumOfFloat(3, 1.2f, 3.4, 5.6)<<endl;
}