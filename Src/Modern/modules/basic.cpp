/******************************************************************
 * 1.基本数据类型
 * 2.支持初始化语句的if和switch
 * 3.nullptr
 * 4.确定的表达式顺序
 * 5.字面量优化
********************************************************************/
#include <memory.h>
#include <climits>
#include <stdio.h>
#include <iostream>

using namespace std;

#define FUNCTION_START()  {cout<<__func__<<":\n  ";}{
#define FUNCTION_END()    }{cout<<"\n\n";  } 

struct TestStruct {
    TestStruct(): name(__func__) {}
    const char *name;
};

template <typename T, typename U>
int bit_copy(T &a, U &b)
{
    static_assert(sizeof(T) == sizeof(U), "can't copy with no equal size!");
    memcpy(&a, &b, sizeof(a));    
    return 0;
}

#define LOG(...)    printf(__VA_ARGS__)

static void literal_improve(void);

//noexcept 表示函数不会抛出异常
int basic_process(void) noexcept
{
    FUNCTION_START() 

    //显示标准类型
    cout<<"Standard Clib:"<<__STDC_HOSTED__<<" | ";
    cout<<"Standard C:"<<__STDC__<<" | ";
    cout<<"Version:"<<__cplusplus<<" | ";   

    //文件信息
    TestStruct ts;
    cout<<__FILE__<<" "<<__func__<<" "<<__LINE__<<" | ";
    cout<<ts.name<<" | ";

    //__VA_ARGS__变长参数的宏定义
    int x = 5;
    LOG("x = %d", 5);

    //wchar_t
    wchar_t arr[] = L"测试";
    cout<<sizeof(arr[0])<<" "<<sizeof(arr)<<" "<<sizeof(arr)/sizeof(wchar_t)<<" | ";
   
    long long ll_val;
    cout<<"Long Long:"<<sizeof(ll_val)<<" ";
    ll_val = LLONG_MIN;
    cout<<ll_val<<" ";
    ll_val = LLONG_MAX;
    cout<<ll_val<<" | ";

    int a = 4;
    int b;
    bit_copy(b, a);
    cout<<b<<" | ";
    
    //error: static assertion failed: can't copy with no equal size!
    //double c;
    //bit_copy(c, a);

    //if(init; condition) c++17
    {
        int x = 5;
        bool fb = false;
        if(fb=(x>3); fb){
            cout<<std::boolalpha<<fb<<" max"<<" | ";
        }else if(fb=(x<1); fb){
            cout<<std::boolalpha<<fb<<" min"<<" | ";
        }else
        {
            cout<<std::boolalpha<<fb<<" nul"<<" | ";
        }
    }

    //switch(init; condition) c++17
    {
        switch(int a=1; a)
        {
            case 1:
                cout<<1<<" | ";
                break;
            default:
                cout<<"default"<<" | ";
                break;
        }
    }   

    //nullptr
    {
        char *ptr = nullptr;
        if(ptr == nullptr)
        {
            cout<<"nullptr"<<" | ";
        }
        
        nullptr_t nul1;
        char *pnull = nul1;
        if(ptr == nul1)
        {
            cout<<"null1,"<<typeid(nul1).name()<<" | ";
        }
    }

    {
        //三元运算符目前不支持
        //bool b = (7 <=> 10) < 0;
        //cout<<boolalpha<<b<<" | ";
    }

    /*
    对于后缀表达式和移位操作符，表达式总是从左到右
    对于赋值表达式，表达式求值总是从右到左
    */
    {
        auto func = [](int i){
            cout<<i<<" | ";
        };
        int a = 5;
        func(++a);
        cout<<a<<" | ";
        int b = (++a);
        cout<<b<<" | ";
    }
    cout<<"\n";

    literal_improve();

    FUNCTION_END()
    return 0;
}

static void literal_improve(void)
{
    double farr[] = {1.5, 35.35, 600.25};
    for(auto &val:farr)
    {
        cout<<hexfloat<<val<<"="<<defaultfloat<<val<<" | ";
    }

    //2进制，8进制，10进制，16进制
    unsigned int xarr[] = {0b111L, 063L, 25L, 0xf1L}; 
    for(auto &val:xarr)
    {
        cout<<val<<" | ";
    }

    //单引号作为界定符，不影响实际数值，仅方便查看(C++14)
    unsigned int sarr[] = {0b111'111L, 02'163L, 25'000L, 0x23'1f1L}; 
    for(auto &val:sarr)
    {
        cout<<val<<" | ";
    }

    //原生字符串
    char hello_str[] = R"(
        "this is hello html"
        "for test.")";
    cout<<hello_str<<" | ";
}