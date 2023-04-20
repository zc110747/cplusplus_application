//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      align.cpp
//
//  Purpose:
//      alignas 和 alignof
//
// Author:
//     	@听心跳的声音
//
//  Assumptions:
//
//  Revision History:
//      4/19/2023   Create New Version
/////////////////////////////////////////////////////////////////////////////

#include "align.hpp"
#include "../includes.hpp"

using std::cout;

namespace ALIGN
{
    struct a_default
    {
        char a;
        //reseved[2]
        short b;
        //reseved[4]
        double c;
    };

    template<class T> struct alignof_tick { char c; T member; };
    #define ALIGNOF(type) offsetof(alignof_tick<type>, member);

    //ALIGN_AS设置变量的对齐值
    //1.alignas输入的必须是2的幂值，如1，2，4...
    //2.alignas对结构体进行声明时，如果alignas设置值小于自然对齐值,则被忽略，如果大于，将结构体以对齐值进行填充完整结构
    struct b_default
    {
        char a;
        short b;
    };

    struct alignas(1) b_align_1{
        char a;
        short b;
    };

    struct alignas(8) b_align_8
    {
        char a;
        short b;
    };

    struct b_with_align{
        char a;
        alignas(4) short b;
    };

    void test_study()
    {
        FUNCTION_START()

        //30.1 不可忽视的数据对其问题
        //对于默认对齐的结构体，内部参数采用自然对齐的方式，如果有间隙，则自动偏移补足
        cout<<"chapter 30.1\n  ";
        cout<<sizeof(a_default::a)<<" "<<sizeof(a_default::b)<<" "<<sizeof(a_default::c)<<" | ";
        cout<<sizeof(a_default::a)+sizeof(a_default::b)+sizeof(a_default::c)<<" "<<sizeof(a_default)<<"\n";

        //30.2 c++11标准之前的控制数据对齐的方法
        //基于offsetof获取对象的地址
        cout<<"chapter 30.2\n  ";
        auto x = ALIGNOF(a_default);
        cout<<x<<"\n";

        //30.3 使用alignof运算符
        //alignof对于c++默认只支持类型，通过alignof(decltype(var))可扩展支持变量(通过此方式会丢失自定义对齐)
        //gcc则直接扩展支持变量的对齐值读取
        //所有的标量类型都满足max_align_t对齐机制，new和malloc返回需要以此对齐
        cout<<"chapter 30.3\n  ";        
        cout<<alignof(a_default::a)<<" "<<alignof(a_default::b)<<" "<<alignof(a_default::c)<<" "<<alignof(a_default)<<"\n  ";
        cout<<"int size:"<<alignof(int)<<"\n  ";
        cout<<"int* size:"<<alignof(int*)<<"\n  ";
        auto func = [&](int a){};
        cout<<"func size:"<<alignof(func)<<"\n  ";
        int *p = new int(1);
        cout<<"0x"<<std::hex<<(long long)p<<"\n  ";
        cout<<std::dec<<alignof(max_align_t)<<"\n";   

        //30.4 使用alignas说明符
        //alignof输入值必须是2的倍数，否则无法通过编译
        //对象的alignof输出，以结构体中最大的定义作为输出
        cout<<"chapter 30.4\n  ";
        int a1;
        alignas(8) int a2 = 0;
        cout<<alignof(a1)<<" "<<alignof(decltype(a1))<<" | ";   //4, 4
        cout<<alignof(a2)<<" "<<alignof(decltype(a2))<<" | ";   //8, 4

        //alignas声明结构体
        alignas(1) b_default a4;
        cout<<alignof(b_default)<<" "<<alignof(a4)<<" "<<sizeof(a4)<<" | ";     //2, 1, 4
        b_align_1 a5;
        cout<<alignof(b_align_1)<<" "<<alignof(a5)<<" "<<sizeof(a5)<<" | ";     //8, 8, 8
        b_align_8 a6;
        cout<<alignof(b_align_8)<<" "<<alignof(a6)<<" "<<sizeof(a6)<<" | ";     //2, 2, 4
        alignas(1) b_with_align a7;
        cout<<alignof(a7)<<" "<<alignof(a7.b)<<" "<<sizeof(a7)<<"\n";         //1, 4, 8

        //30.5其它关于对齐字节长度的支持
        //alignment_of和alignof功能基本一致
        //aligned_storage_t用来分配指定对齐长度和大小的一块内存
        //aligned_union_t用来分配指定对齐长度的内存，长度则选取对齐最大的变量
        cout<<"chapter 30.5\n  ";
        #if __MORE_THAN_CPP17__
        cout<<std::alignment_of<int>::value<<" "<<std::alignment_of<int>()<<" | ";  //4, 4
        std::aligned_storage_t<256, 8> buffer;
        cout<<sizeof(buffer)<<" "<<alignof(buffer)<<" | ";                          //256 8
        std::aligned_union_t<64, int, char, double> union_b;
        cout<<sizeof(union_b)<<" "<<alignof(union_b)<<" | ";                        //64 8
        #else
        __LOWRE_THAN_CPP17_RUN;
        #endif

        FUNCTION_END()
    }
}
