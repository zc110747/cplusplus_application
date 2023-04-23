/******************************************************************
 * 第一章 新基础类型
 * 第十八章 支持初始化语句的if和switch
 * 第十九章 静态断言(static_assert)
 * 第二十三章 指针字面量nullptr 
 * 第二十八章 确定的表达式求值顺序 
 * 第二十九章 字面量优化 
 * 第三十二章 新增预处理器和宏
********************************************************************/
#include <memory.h>
#include <climits>
#include <limits>
#include <stdio.h>
#include <string>
#include <iostream>

#include "../includes.hpp"

using namespace std;

namespace BASIC_TYPE
{
    //long long 
    void process()
    {
        FUNCTION_START() 

        //long long表示至少为64位的整型
        //unsigned long long 表示无符号类型
        long long LL0 = 1256LL;
        long long LL1 = 65536<<16;        
        long long LL2 = 65536LL<<16;
        cout<<LL0<<" "<<LL1<<" "<<LL2<<" | ";

        //支持通过宏和模板读取类型的最大范围
        cout<<LLONG_MIN<<" "<<LLONG_MAX<<" "<<ULLONG_MAX<<" | ";
        cout<<std::numeric_limits<long long>::min()<<" | ";
        cout<<std::numeric_limits<long long>::max()<<" | ";
        cout<<std::numeric_limits<unsigned long long>::max()<<" | ";
        cout<<"\n  ";

        //char, char16_t, char32_t
        //u8,   u,         U        --C++17
        //多字节字符串和宽字符串的转换C++17不推荐使用
        // char ch0 = u8'a';       //c++20可使用char8_t
        // char16_t ch1 = u'好';
        // char32_t ch2 = U'好';
        // u16string str1 = {u"测试"};
        // u32string str2 = {U"测试"};

        
        FUNCTION_END()
    }
}

//扩展的if, switch表达式
namespace IF_SWITCH_INIT
{
    void process()
    {
        FUNCTION_START()

        //if语句中支持初始化语句, 结构为if(init, condition)
        //if中的初始化变量贯穿整个生命周期，else if中初始化变量只存在于else if和后续的语句
        int x = 5;
        bool fb = false;
        if(fb=(x>3); fb)
        {
            cout<<std::boolalpha<<fb<<" max"<<" | ";
        }
        else if(fb=(x<1); fb)
        {
            cout<<std::boolalpha<<fb<<" min"<<" | ";
        }
        else
        {
            cout<<std::boolalpha<<fb<<" nul"<<" | ";
        }

        //switch语句中支持初始化语句, 结构为switch(init, condition)
        switch(int a=1; a)
        {
            case 1:
                cout<<1<<" | ";
                break;
            default:
                cout<<"default"<<" | ";
                break;
        }

        FUNCTION_END()
    }
}

namespace NULLPTR
{
    template<class T>
    class Widget
    {
    public:
        Widget(){
            cout<<"Widget"<<" | ";
        }
    };

    template<>
    class Widget<std::nullptr_t>
    {
    public:
        Widget(){
            cout<<"nullptr Widget"<<" | ";
        }
    };

    template<class T>
    Widget<T> *make_widget(T)
    {
        return new Widget<T>();
    }

    void process()
    {
        FUNCTION_START()

        char *p = nullptr;
        if(!p)
        {
            cout<<"nullptr"<<" | ";
        }

        char *p1 = NULL;
        if(p1 == nullptr)  //可以将NULL和nullptr比较
        {
            cout<<"nullptr"<<" | ";
        }

        //nullptr是关键字
        //null1, null2, nullptr可以进行比较，相同
        //nullptr是纯右值,null1, null2是左值，可以取地址
        decltype(nullptr) null1;
        std::nullptr_t null2;
        char *p2 = null1;
        char *p3 = null2;
        if(p2 == nullptr)
        {
            cout<<"nullptr"<<" | ";
        }
        if(p3 == nullptr)
        {
            cout<<"nullptr"<<" | ";
        }

        //NULL定义为0,默认会执行int类型，nullptr可以正确执行特化
        auto w1 = make_widget(NULL);
        auto w2 = make_widget(nullptr);
        delete w1;
        delete w2;
        FUNCTION_END()
    }
}

namespace DEFINED
{
    #define LOG(msg, ...) printf("[" __FILE__ ":%d]" msg, __LINE__, __VA_ARGS__)
    #define LOG_M(msg, ...) printf("[" __FILE__ ":%d]" msg, __LINE__ __VA_OPT__(,) __VA_ARGS__)

    void process()
    {
        FUNCTION_START() 

        cout<<__func__<<" | ";          //打印函数名
        cout<<__cplusplus<<" | ";       //打印当前C++版本
        cout<<__STDC_HOSTED__<<" | ";   //判断是否支持完整C库
        cout<<__STDC__<<" | ";          //表示和C是否一致
        cout<<std::dec<<__LINE__<<" | ";          //打印当前的行

        //判断文件是否能被包含, __has_include
        #if __has_include(<string>)
        cout<<"can include"<<" | ";
        #endif

        //用于判断atrribute是否支持, __has_cpp_attribute
        //如果支持返回加如时间，否则返回0
        // cout<<__has_cpp_attribute(fallthrough)<<" | ";
        // cout<<__cpp_aggregate_bases<<" | ";

        //通过__VA_ARGS实现可变变量
        LOG("Hello %d | ", 2022);
        LOG_M("This is test");

        FUNCTION_END() 
    }
}

//在编译期能够执行，且能够提供诊断信息的断言
namespace ASSERT
{
    class X
    {
    public:
        int a;
    };

    class VX
    {
        virtual void func(){
        }
    };

    void process(void)
    {
        FUNCTION_START() 

        static_assert(true, "Not Right!");
        static_assert(sizeof(X) == sizeof(X::a), "Not Equal!");
        static_assert(sizeof(VX) == sizeof(void *), "not equal ptr");

        FUNCTION_END()
    }
}

int basic_process(void)
{
    BASIC_TYPE::process();

    IF_SWITCH_INIT::process();

    NULLPTR::process();

    DEFINED::process();

    ASSERT::process();
    
    return 0;
}