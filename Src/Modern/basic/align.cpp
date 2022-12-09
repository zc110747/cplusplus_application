
#include "align.hpp"
#include <iostream>

struct A
{
    char a;
    short b;
    double c;
};

using namespace std;

int align_process(void)
{
    {
        //长度
        cout<<sizeof(A::a)<<" "<<sizeof(A::b)<<" "<<sizeof(A::c)<<" "<<sizeof(A)<<" | ";
    
        //对齐 - 结构体对齐以最大值对齐
        cout<<alignof(A::a)<<" "<<alignof(A::b)<<" "<<alignof(A::c)<<" "<<alignof(A)<<" | ";

        //alignof支持类型和变量(GCC扩展)
        auto a = alignof(int);
        auto func = [](){};
        auto b = alignof(func);
        auto c = alignof(decltype(func));
        cout<<a<<" "<<b<<" "<<c<<" | ";
    }

    {
        //alignas设置变量的对齐方式
        alignas(8) int a = 0;
        cout<<alignof(a)<<" "<<alignof(decltype(a))<<" | ";

        struct alignas(8) B1{
            char a;
            int b;
        };
        //alignas的输入值必须时2的幂值
        struct alignas(1) B2{ 
            char a;
            int b;
        };
        struct B3{
            char a;
            alignas(8) int b;
            alignas(double) short c;
        };
        
        //8, 4, 对结构体的声明，当小于成员变量的对齐值时，会被忽略
        cout<<alignof(B1)<<" "<<alignof(B2)<<" "<<alignof(B3)<<" "<<sizeof(B3)<<" | ";
    }

    {
        cout<<alignment_of<int>::value<<" "<<alignment_of<int>()<<" | ";
        aligned_storage_t<256, 8> buffer;
        cout<<sizeof(buffer)<<" "<<alignof(buffer)<<" | ";
        aligned_union_t<16, int, char, double> union_b;
        cout<<sizeof(union_b)<<" "<<alignof(union_b)<<" | ";
    }

    {
        alignas(4) char *pChar = new char;
        char a, b;
        alignas(16) char *pChar1 = new char;

        cout<<"0x"<<std::hex<<(long long)pChar<<" | ";
        cout<<"0x"<<std::hex<<(long long)pChar1<<" | ";
        delete pChar;
        delete pChar1;
    }
    return 0;
}