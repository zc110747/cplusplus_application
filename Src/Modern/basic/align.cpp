/******************************************************************
 * 第三十章   alignas和alignof 
********************************************************************/
#include "align.hpp"
#include <iostream>
#include <vector>

using namespace std;
#define FUNCTION_START()  {cout<<__func__<<":\n  ";}{
#define FUNCTION_END()    }{cout<<"\n\n";  } 

namespace ALIGN_OF
{
    struct A
    {
        char a;
        short b;
        double c;
    };

    //alignof获取类型或者变量的对齐值
    void process(void)
    {
        FUNCTION_START()

        //sizeof获取结构体对象的长度
        cout<<sizeof(A::a)<<" "<<sizeof(A::b)<<" "<<sizeof(A::c)<<" "<<sizeof(A)<<" | ";
    
        //alignof获取对象的对齐长度
        cout<<alignof(A::a)<<" "<<alignof(A::b)<<" "<<alignof(A::c)<<" "<<alignof(A)<<" | ";

        //alignof支持类型和变量(GCC扩展)
        vector<size_t> size_vec;
        auto func = [](){};
        size_vec.push_back(alignof(int));
        size_vec.push_back(alignof(int *));
        size_vec.push_back(alignof(func));
        size_vec.push_back(alignof(decltype(func)));
        for(const auto&size : size_vec)
        {
            cout<<size<<" ";
        }

        FUNCTION_END()
    }
} 

namespace ALIGN_AS
{
    //ALIGN_AS设置变量的对齐值
    //1.alignas输入的必须是2的幂值，如1，2，4...
    //2.alignas对结构体进行声明时，如果alignas设置值小于自然对齐值,则被忽略，如果大于，将结构体以对齐值进行填充完整结构
    struct A{
        char a;
        short b;
    };

    struct alignas(8) A1{
        char a;
        short b;
    };

    struct alignas(1) A2{
        char a;
        short b;
    };

    struct A3{
        char a;
        alignas(4) short b;
    };

    void process(void)
    {
        FUNCTION_START()

        //声明对齐的变量，变量按照对齐值定义，类型还是原始对齐
        int a1;
        alignas(8) int a2 = 0;
        alignas(1) int a3 = 0;
        cout<<alignof(a1)<<" "<<alignof(decltype(a1))<<" | ";   //4, 4
        cout<<alignof(a2)<<" "<<alignof(decltype(a2))<<" | ";   //8, 4
        cout<<alignof(a3)<<" "<<alignof(decltype(a3))<<"\n  ";  //1, 4

        //alignas声明结构体
        alignas(1) A a4;
        cout<<alignof(A)<<" "<<alignof(a4)<<" "<<sizeof(a4)<<" | ";  //2, 1, 4
        A1 a5;
        cout<<alignof(A1)<<" "<<alignof(a5)<<" "<<sizeof(a5)<<" | "; //8, 8, 8
        A2 a6;
        cout<<alignof(A2)<<" "<<alignof(a6)<<" "<<sizeof(a6)<<" | "; //2, 2, 4
        alignas(1) A3 a7;
        cout<<alignof(a7)<<" "<<alignof(a7.b)<<" "<<sizeof(a7)<<"\n  "; //1, 4, 8

        //alignment_of和alignof功能基本一致
        //aligned_storage_t用来分配指定对齐长度和大小的一块内存
        //aligned_union_t用来分配指定对齐长度的内存，长度则选取对齐最大的变量
        cout<<alignment_of<int>::value<<" "<<alignment_of<int>()<<" | ";
        aligned_storage_t<256, 8> buffer;
        cout<<sizeof(buffer)<<" "<<alignof(buffer)<<" | ";
        aligned_union_t<64, int, char, double> union_b;
        cout<<sizeof(union_b)<<" "<<alignof(union_b)<<" | ";

        FUNCTION_END()
    }
}

int align_process(void)
{
    ALIGN_OF::process();

    ALIGN_AS::process();
    
    return 0;
}