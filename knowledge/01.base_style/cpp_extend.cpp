//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      cpp_extend.cpp
//
//  Purpose:
//     1. 新字符类型(Unicode)
//     2. C++函数扩展
//     3. 包含文件信息的定义
//
// Author:
//      @公众号：<嵌入式技术总结>
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <cstdlib>
#include <cstdio>
#include <climits>
#include <iostream>
#include <cstdarg>
#include <limits>

#if __cplusplus >= 202007L
#include <version>
#include <numbers>
#endif

#if __cplusplus >= 202302L
#include <stdfloat>
#endif

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

// 使用va_list获取可变参数，并处理
double SumOfFloat(int count, ...){
    va_list ap;
    double sum = 0;
    va_start(ap, count);

    for(int i=0; i<count; i++) {
        sum += va_arg(ap, double);
    }

    va_end(ap);
    return sum;
}

void standard_improve(void)
{
    printf("============= %s ==============\n", __func__);

    std::cout << "Standard Clib: "<< __STDC_HOSTED__ << std::endl;
    std::cout << "Standard C: "<<__STDC__ << std::endl;
    
    // __func__获取函数的名称
    std::cout << struct_test().name_ << std::endl;

    // __VA_ARGS__获取可变参数
    LOG("Hello World!");
    LOG("%s", "NO ERR!");
    PR("Hello World!\n");

    // long long int: 64位整型
    long long int lli = -90000000000LL;
    unsigned long long int ulli = 9000000000000ULL;
    std::cout <<"LLONG_MIN: "<< LLONG_MIN << std::endl;    //LLONG_MIN: -9223372036854775808
    std::cout <<"LLONG_MAX: "<< LLONG_MAX << std::endl;    //LLONG_MAX: 9223372036854775807
    std::cout <<"ULLONG_MAX: "<< ULLONG_MAX << std::endl;  //ULLONG_MAX: 18446744073709551615
    std::cout <<"ULLONG_INT: "<< ulli << std::endl;

    printMessage();
    printMessage("Hello, C++!");

    std::cout << "SumOfFloat: " << SumOfFloat(3, 1.2f, 3.4, 5.6) << std::endl;

    std::cout << "max: " << std::numeric_limits<int>::max() << std::endl;
    std::cout << "min: " << std::numeric_limits<int>::min() << std::endl;
    std::cout << "lowest: " << std::numeric_limits<int>::lowest() << std::endl;
    std::cout << "epsilon: " << std::numeric_limits<int>::epsilon() << std::endl;
    std::cout << "round_error: " << std::numeric_limits<int>::round_error() << std::endl;
    std::cout << "infinity: " << std::numeric_limits<int>::infinity() << std::endl;
    std::cout << "quiet_NaN: " << std::numeric_limits<int>::quiet_NaN() << std::endl;
    std::cout << "signaling_NaN: " << std::numeric_limits<int>::signaling_NaN() << std::endl;
    std::cout << "denorm_min: " << std::numeric_limits<int>::denorm_min() << std::endl;

    // numbers
#if __cplusplus >= 202007L
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
#else
    std::cout << "warn: numbers support in 202007L, current: " << __cplusplus << std::endl;
#endif

    // stdfloat
#if __cplusplus >= 202302L
    std::float16_t f16 = 0.1f16;
    std::cout << "f16: " << f16 << std::endl;

    std::float32_t f32 = 0.1f32;
    std::cout << "f32: " << f32 << std::endl;

    std::float64_t f64 = 0.1f64;
    std::cout << "f64: " << f64 << std::endl;
#else
    std::cout << "warn: stdfloat support in 202302L, current: " << __cplusplus << std::endl;
#endif

    // version
#if __cplusplus >= 202007L
    std::cout << "__cpp_lib_any: " << __cpp_lib_any << std::endl;
    std::cout << "__cpp_lib_apply: " << __cpp_lib_apply << std::endl;
    std::cout << "__cpp_lib_as_const: " << __cpp_lib_as_const << std::endl;
    std::cout << "__cpp_lib_chrono: " << __cpp_lib_chrono << std::endl;
    std::cout << "__cpp_lib_variant: " << __cpp_lib_variant << std::endl;
    std::cout << "__cpp_lib_void_t: " << __cpp_lib_void_t << std::endl;
#else
    std::cout << "warn: version support in 202007L, current: " << __cplusplus << std::endl;
#endif
}
