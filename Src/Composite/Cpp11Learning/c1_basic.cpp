

#include "include.hpp"
#include <memory.h>
#include <climits>

using namespace std;

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

//noexcept 表示函数不会抛出异常
int basic_test(void) noexcept
{
    //显示标准类型
    cout<<"Standard Clib:"<<__STDC_HOSTED__<<endl;
    cout<<"Standard C:"<<__STDC__<<endl;
    cout<<"Version:"<<__cplusplus<<endl;   

    //文件信息
    TestStruct ts;
    cout<<__FILE__<<" "<<__func__<<" "<<__LINE__<<endl;
    cout<<ts.name<<endl;

    //__VA_ARGS__变长参数的宏定义
    int x = 5;
    LOG("x = %d", 5);

    //wchar_t
    wchar_t arr[] = L"测试";
    cout<<sizeof(arr[0])<<" "<<sizeof(arr)<<" "<<sizeof(arr)/sizeof(wchar_t)<<endl;
   
    long long ll_val;
    cout<<"Long Long:"<<sizeof(ll_val)<<" ";
    ll_val = LLONG_MIN;
    cout<<ll_val<<" ";
    ll_val = LLONG_MAX;
    cout<<ll_val<<endl;

    int a = 4;
    int b;
    bit_copy(b, a);
    cout<<b<<endl;
    
    //error: static assertion failed: can't copy with no equal size!
    //double c;
    //bit_copy(c, a);

    return 0;
}

