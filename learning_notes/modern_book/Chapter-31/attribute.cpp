//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      attribute.cpp
//
//  Purpose:
//
//  Author:
//     	@公众号：<嵌入式技术总结>
//
//  Assumptions:
//
//  Revision History:
//      4/19/2023   Create New Version
/////////////////////////////////////////////////////////////////////////////
#include "attribute.hpp"
#include "../includes.hpp"

using namespace std;

namespace ATTRIBUTE
{
  //31.1 GCC的属性语法
    #ifdef __GNUC__
    class X1 {int i;};
    __attribute__((aligned(16))) X1 x1;
    int a1 __attribute__((aligned(8)));
    #endif

    //31.4 使用using打开属性的命名空间
    #ifdef __GNUC__
    [[gnu::always_inline, gnu::hot, gnu::const, nodiscard]]
    inline int f4()
    {
        return 4;
    }

    #if __MORE_THAN_CPP17__
    [[using gnu:always_inline, hot, const]] [[nodiscard]]
    inline int f41()
    {
        return 41;
    }
    #else
    inline int f41()
    {
        return 41;
    }
    #endif
    #endif

    //31.5 标准属性
    //[[noreturn]] 表示函数不会返回，函数体必须真的不返回（抛异常/abort/死循环），
    //否则编译器报 "'noreturn' function does return"，属性也就名不副实
    [[noreturn]]
    void foo5(void) {
        cout<<__func__<<" | ";
        throw std::runtime_error("foo5 is declared noreturn");
    }
    
    //[[carries_dependency]] 是 memory_order_consume 时代的依赖传播提示,
    //GCC 未实现该提示(挂上会报 attribute ignored), 这里保留函数本体作对照
    static void carries_func(void)
    {
        cout<<__func__<<" | ";
    }
  
    #if __MORE_THAN_CPP14__
    struct [[deprecated("need use new class")]] X5
    {
        int a;
    };
    [[deprecated]]
    void deprecated_func(void)
    {
        cout<<__func__<<" | ";
    }
    #endif

    #if __MORE_THAN_CPP17__
    void fallthrough_func(int a)
    {
        switch(a)
        {
        case 0:
            cout<<0<<",";
            [[fallthrough]];
        case 1:
            cout<<1<<" | ";
            break;
        default:
            break;
        }
    }

    [[nodiscard]] 
    int nodiscard_func()
    {
        return 2;
    }
    #endif

    [[maybe_unused]]
    int may_unused_func()
    {
        return 0;
    }

#if __MORE_THAN_CPP20__
    int likely_func(int i)
    {
        switch(i)
        {
            [[likely]] case 1: return 1;
            [[unlikely]] case 2: return 2;
            default: return 3;
        }
    }

    struct empty8
    {
    };

    struct x8
    {
        int i;
        [[no_unique_address]] empty8 e;
    };

    void nouniqueaddress(void)
    {
        x8 x;
        cout<<sizeof(x)<<" | ";
        cout<<"addr:0x"<<std::hex<<(long long)&(x.i);
        cout<<"addr:0x"<<std::hex<<(long long)&(x.e);
        cout<<" | ";
    }
#endif

    void test_run()
    {
        FUNCTION_START()

        //31.1 GCC的属性语法
        //__attribute__((attribute-list))
        cout<<"\nchapter31.1\n  ";
        #ifdef __GNUC__
        cout<<alignof(x1)<<" | ";
        cout<<alignof(a1)<<" | ";
        #endif
        
        //31.2 MSVC的属性语法
        //__declspec(attribute-list)
        //属性描述的是对象本身，而不是类型
        cout<<"\nchapter31.2\n  ";
        #ifdef _MSC_VER
        __declspec(align(32)) int a2;
        cout<<alignof(a2)<<" | ";
        #endif

        //31.3 标准属性说明符语法
        //单个属性 [[attr]]
        //多个属性 [[attr1, attr2, attr3(args)]]
        //属性一般在声明中使用，除了fallthrough属性可以用于switch语句
        cout<<"\nchapter31.3\n  ";

        //31.4 使用using打开属性的命名空间
        cout<<"\nchapter31.4\n  ";
        #ifdef __GNUC__
        cout<<f4()<<" | ";
        cout<<f41()<<" | ";
        #endif

        //31.5 标准属性
        //noreturn 声明为noreturn的函数，执行流会被中断
        //carries_dependency
        //1.作为函数或者lambda表达式参数的属性出现，表示调用者不用担心内存顺序,编译器不会生成内存栅栏指令
        //2.作为函数的属性出现，表示返回值已经处理好内存顺序，不需要函数返回前插入内存指令
        //deprecated 表示函数或者类以及被启用
        //fallthrough 提示编译器可以穿透，不需要警告
        //nodiscard 提升函数的返回值不应被舍弃
        //maybe_unused 消除警告
        //likely和unlikely 该属性所在的执行路径相对于其它执行路径进行优化
        //no_unique_address 指定该数据不需要唯一地址(对象必须非静态成员)
        cout<<"\nchapter31.5\n  ";
        carries_func();
        #if __MORE_THAN_CPP14__
        // 故意调用被 [[deprecated]] 标记的函数，演示的就是编译器给出的弃用告警
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wdeprecated-declarations"
        deprecated_func();
        #pragma GCC diagnostic pop
        #endif
        
        #if __MORE_THAN_CPP17__
        fallthrough_func(0);
        cout<<nodiscard_func()<<" | ";
        #endif

        cout<<may_unused_func()<<" | ";

        #if __MORE_THAN_CPP20__
        cout<<likely_func(1)<<" | ";
        nouniqueaddress();
        #else
        cout<<"\n  ";
        __LOWRE_THAN_CPP20_RUN;   
        #endif
        FUNCTION_END()
    }
}
