
#include "attribute.hpp"
#include <iostream>

using namespace std;

static void no_return_func(void);
static void carries_func(void);
static void deprecated_func(void);
static void fallthrough_func(void);
static int nodiscard_func(void);
class [[nodiscard]] X1{
};
static X1 nodiscard_returnX();
static int foo(int a, int b);
static int f(int i);
static void nouniqueaddress(void);

int attribute_process(void)
{
    carries_func();

    deprecated_func(); 

    fallthrough_func(); 

    nodiscard_func();

    nodiscard_returnX();

    foo(1, 2);

    f(2);

    nouniqueaddress();

    no_return_func();
    return 0;
}

//指定程序会做好内存屏障，不需要编译器参与
[[carries_dependency]]
static void carries_func(void)
{
    cout<<__FILE__<<__func__<<" | ";
}

[[noreturn]]
static void no_return_func(void)
{
    cout<<"no return"<<" | ";
}

//声明被弃用， 可带弃用原因
struct [[deprecated("need use new class")]] X
{
    int a;
};

[[deprecated]]
static void deprecated_func(void)
{
    X x;
    cout<<__FILE__<<__func__<<" | ";
}

//指定switch能够穿透，不在报警告
static void fallthrough_func(void)
{
    int a = 2;

    switch(a)
    {
        case 0:
            [[fallthrough]]
        case 2:
            cout<<"right"<<" | ";
            break;
        default:
            cout<<"default"<<" | ";
            break;
    }
}

//指定函数的返回值不可被忽略, 忽略会报警告或者错误
static int [[nodiscard]] nodiscard_func(void)
{
    return 1;
}

static X1 nodiscard_returnX()
{
    X1 x;
    return x;
}

//指定参数可以被忽略
static int foo(int a [[maybe_unused]], int b  [[maybe_unused]])
{
    return a;
}

//指定优化和不优化
static int f(int i)
{
    switch(i)
    {
        [[likely]] case 1: return 1;
        [[unlikely]] case 2: return 2;
        default: return 3;
    }
}

struct Empty{
};
struct X2{
    int i;
    [[no_unique_address]] Empty e;
};

static void nouniqueaddress(void)
{
    X2 x;
    cout<<sizeof(X)<<" | ";
    cout<<"addr:0x"<<std::hex<<(long long)&(x.i);
    cout<<"addr:0x"<<std::hex<<(long long)&(x.e);
    cout<<" | ";
}