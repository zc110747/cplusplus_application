//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      base.cpp
//
//  Purpose:
//     1. 基础数据类型
//     2. 数组和指针
//     3. 宏定义和扩展
//
// Author:
//      @公众号：<嵌入式技术总结>
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <cstdio>
#include <iostream>

#define START_STR   "hello"
#define END_NUM     64

// #：将参数转换为字符串
#define TO_STRING(x) #x
#define CONCAT_STR(x, y) START_STR "_" TO_STRING(y)

// ##: 连接符，将两个参数连接起来
#define TEST_STRING "hello world"
#define CONCAT_STR2(x, y) x##y

int type_process(void)
{
    std::cout << "======================= type_process =======================" << std::endl;
    char a1 = '0';
    short a2 = 1;
    int a3 = 1;
    long a4 = 1;
    long long a5 = 1;
    float a6 = 1.2;
    double a7 = 1.2;

    unsigned int a8 = 1;
    unsigned long a9 = 1;
    
    short *pa2 = &a2;
    int *pa3 = &a3;

    std::cout << "a1: " << a1 << " | ";
    std::cout << "a2: " << a2 << " | ";
    std::cout << "a3: " << a3 << " | ";
    std::cout << "a4: " << a4 << " | ";
    std::cout << "a5: " << a5 << " | ";
    std::cout << "a6: " << a6 << " | ";
    std::cout << "a7: " << a7 << " | ";
    std::cout << "a8: " << a8 << " | ";
    std::cout << "a9: " << a9 << std::endl;
    std::cout << "pa2: " <<*pa2 << std::endl;
    std::cout << "pa3: " << *pa3 << std::endl;

    printf("a1 size: %d\n", sizeof(a1));
    printf("a1: %d\n", a1);
    printf("a2: %d\n", a2);
    printf("a3: %d\n", a3);
    printf("a4: %ld\n", a4);
    printf("a5: %lld\n", a5);
    printf("a6: %f\n", a6);
    printf("a7: %lf\n", a7);
    printf("a8: %u\n", a8);
    printf("a9: %lu\n", a9);

    {
        // 隐式类型转换
        int a1 = 1;
        long long int a2 = 2;
        auto a3 = a1 + a2;
        std::cout << "a1 type: " << typeid(a1).name() << " | ";         //i
        std::cout << "a2 type: " << typeid(a2).name() << " | ";         //x
        std::cout << "a3 type: " << typeid(a3).name() << std::endl;     //x

        unsigned int a4 = 1;
        auto a5 = a4 + a1;
        std::cout << "a4 type: " << typeid(a4).name() << " | ";         //j
        std::cout << "a5 type: " << typeid(a5).name() << std::endl;     //j

        short a6 = 1;
        auto a7 = a6 + a1;
        std::cout << "a6 type: " << typeid(a6).name() << " | ";         //s
        std::cout << "a7 type: " << typeid(a7).name() << std::endl;     //i

        __int32_t a8 = 1;
        auto a9 = a8 + a1;
        std::cout << "a8 type: " << typeid(a8).name() << " | ";         //i
        std::cout << "a9 type: " << typeid(a9).name() << std::endl;     //i
    }
    return 0;
}

int arrar_process(void)
{
    int a[5] = {1, 2, 3, 4, 5};
    size_t nums = sizeof(a)/sizeof(a[0]);

    std::cout << "======================= array_process =======================" << std::endl;
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

    return 0;
}

typedef void (*func_ptr)(int);
void func(int a)
{
    std::cout << "func a: " << a << std::endl;
}

int pointer_process(void)
{
    std::cout << "======================= pointer_process =======================" << std::endl;

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

    // 指针数组
    int *p6[5];
    for (int i = 0; i < 5; i++) {
        p6[i] = new int(i);
    }
    for (int i = 0; i < 5; i++) {
        std::cout << "p6[" << i << "]: " << *p6[i] << " | ";
    }
    std::cout << std::endl;
    for (int i = 0; i < 5; i++) {
        delete p6[i];
    }

    // 数组指针
    int (*p7)[5] = &arr;
    for (int i = 0; i < 5; i++) {
        std::cout << "p7[" << i << "]: " << *(*p7 + i) << " | ";
    }
    std::cout << std::endl;

    // 函数指针
    func_ptr p5 = func;
    p5(10);

    return 0;
}

int controlflow_process(void)
{
    std::cout << "======================= controlflow_process =======================" << std::endl;
    int i = 0;
    int sum = 0;

    // for循环
    for (i = 0; i < 10; i++) {
        sum += i;
    }
    std::cout << "for sum: " << sum << std::endl;

    // continue跳出当前循环，进入下一次循环
    // break跳出当前循环, 结束循环
    sum = 0;
    for (i = 0; i < 10; i++) {
        if (i == 5) {
            continue;
        }

        if (sum > 30) {
            break;
        }
        sum += i;
    }
    std::cout << "for continue sum: " << sum << std::endl;

    // 循环while
    sum = 0;
    i = 0;
    while(1) {
        if (i >= 10) {
            break;
        }
        sum += i;
        i++;
    }
    std::cout << "while sum: " << sum << std::endl;

    // 循环do-while
    sum = 0;
    i = 0;
    do {
        sum += i;
        i++;
    } while (i < 10);
    std::cout << "do-while sum: " << sum << std::endl;

    // switch-case
    i = 0;
    switch (i) {
        case 0:
            std::cout << "i = 0" << std::endl;
            break;
        case 1:
            std::cout << "i = 1" << std::endl;
            break;
        default:
            std::cout << "i = " << i << std::endl;
            break;
    }

    // if-else
    if (sum < 45) {
        std::cout << "sum < 45" << std::endl;
    } else if (sum > 45) {
        std::cout << "sum > 45" << std::endl;
    } else {
        std::cout << "sum = 45" << std::endl;
    }
    return 0;
}

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

int struct_process(void)
{
    std::cout << "======================= struct_process =======================" << std::endl;
    
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

union union_type
{
    int a1;
    short a2;
    float f1;
};

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

int union_process(void)
{
    std::cout << "======================= union_process =======================" << std::endl; 
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

enum enum_type
{
    ENUM_TYPE_A = 0,
    ENUM_TYPE_B,
    ENUM_TYPE_C,
};

int enum_process(void)
{
    std::cout << "======================= enum_process =======================" << std::endl;
    
    enum_type e1 = ENUM_TYPE_A;
    std::cout << "e1: " << e1 << std::endl;

    if (e1 == ENUM_TYPE_A) {
        std::cout << "e1 is ENUM_TYPE_A" << std::endl;
    } else {
        std::cout << "e1 is not ENUM_TYPE_A" << std::endl;
    }

    return 0;
}

int define_process(void)
{
    std::cout << "======================= define_process =======================" << std::endl;

    std::cout << "__STDC__: " << __STDC__ << std::endl;
    std::cout << "__DATE__: " << __DATE__ << std::endl;
    std::cout << "__TIME__: " << __TIME__ << std::endl;
    std::cout << "__FILE__: " << __FILE__ << std::endl;

    std::cout << "CONCAT_STR(START_STR, END_NUM): " << CONCAT_STR(START_STR, END_NUM) << std::endl;
    std::cout << "CONCAT_STR2(TEST_, STRING): " << CONCAT_STR2(TEST_, STRING) << std::endl;
    return 0;
}

int basic_type_process(void)
{
    type_process();

    arrar_process();

    pointer_process();

    controlflow_process();

    struct_process();

    union_process();

    enum_process();

    define_process();

    return 0;
}