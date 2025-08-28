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
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <cstdlib>
#include <cstdio>
#include <climits>
#include <iostream>
#include <cstdarg>
#include <cstdbool>

using namespace std;

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
    printf("============= %s ==============\n", __func__);

    std::cout<<"Standard Clib: "<< __STDC_HOSTED__<<std::endl;
    std::cout<<"Standard C: "<<__STDC__<<std::endl;
    
    // __func__获取函数的名称
    std::cout<<struct_test().name_<<std::endl;

    _Bool flag = true;
    printf("flag = %d\n", flag);

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
