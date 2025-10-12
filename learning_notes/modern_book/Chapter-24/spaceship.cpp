//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      spaceship.cpp
//
//  Purpose:
//
//  Author:
//     	@听心跳的声音
//
//  Assumptions:
//
//  Revision History:
//      4/19/2023   Create New Version
/////////////////////////////////////////////////////////////////////////////
#include "spaceship.hpp"
#include "../includes.hpp"

using namespace std;

namespace SPACESHIP
{
    //24.2 三向类型的返回类型
    #if __MORE_THAN_CPP20__
    struct B
    {
        int a;
        auto operator <=> (const B&) const = default;
    };
    
    class weakCompare
    {
    public:
        weakCompare(int n):a_(n){
        }

        std::weak_ordering operator<=>(const weakCompare& b) const{
            return (b.a_ - a_)<=>0;
        }
    private:
        int a_;
    };

    enum color{
        red = 10
    };
    #endif
    
    class TCompare
    {
    public:
        TCompare(int n):a_(n){
        }

        bool operator== (const TCompare& b) const{
            return a_ == b.a_;
        }
        bool operator< (const TCompare& b) const{
            return a_ < b.a_;
        }
    private:
        int a_;
    };

    #if __MORE_THAN_CPP20__
    class GlobalCompare
    {
    public:
        TCompare t{5};
        std::strong_ordering operator<=>(const GlobalCompare& ) const = default;
    };
    #endif

    using namespace std::rel_ops;

    void test_run()
    {
        FUNCTION_START()

        //24.1 "太空飞船运算符"
        //lhs <=> rhs, 
        //lhs<rhs 返回小于0
        //lhs=rhs 返回等于0
        //lhs>rhs 返回大于0
        //返回结果只能和0比较
        cout<<"\nchapter24.1\n  ";
        #if __MORE_THAN_CPP20__
            bool b = 7 <=> 11 < 0;
            cout<<std::boolalpha<<b<<" | "; 
        #else
            __LOWRE_THAN_CPP20_RUN;
        #endif

        //24.2 三向类型的返回类型
        //std::strong_ordering: strong_ordering::less, strong_ordering::equal, strong_ordering::greater
        //std::weak_ordering: weak_ordering::less, weak_ordering::equal, weak_ordering::greater
        //std::partial_ordering: 
        //  partial_ordering::less, partial_ordering::equivlent, partial_ordering::greater
        //  partial_ordering::unordered = 当两个操作数没有关系给出partial_ordering::unordered 
        cout<<"\nchapter24.2\n  ";
        #if __MORE_THAN_CPP20__
        std::cout<<typeid(decltype(7 <=> 11)).name()<<" | ";    //strong_ordering
        B b1, b2;
        std::cout<<typeid(decltype(b1 <=> b2)).name()<<" | ";   //strong_ordering
        weakCompare w1(2), w2(3);
        std::cout<<typeid(decltype(w1 <=> w2)).name()<<" | ";   //weak_ordering
        std::cout<<typeid(decltype(2.5 <=> 2.7)).name()<<" | "; //partial_ordering
        #else
            __LOWRE_THAN_CPP20_RUN;
        #endif

        //24.3 对基础类型的支持
        //1.对两个算数类型的操作数一般进行算术转换, 整型结果是strong_ordering， 浮点是partial_ordering
        //2.无作用域枚举类型和整型操作数比较，枚举转换成整型比较，不能和浮点类型比较
        //3.不同枚举类型的值，不能互相比较
        //4.bool类型互相比较，结果是std::strong_ordering
        //5.不支持数组的比较
        //6.指针可以进行相互比较，结果是std::strong_ordering
        cout<<"\nchapter24.3\n  ";
        #if __MORE_THAN_CPP20__
        cout<<typeid(decltype(1.5<=>5)).name()<<" | ";
        cout<<typeid(decltype(red<=>10)).name()<<" | ";
        auto val3 = true <=> false;
        cout<<typeid(decltype(val3)).name()<<" | ";
        cout<<(val3<0)<<" | ";
        int num3 = 10;
        auto ptr3 = &num3;
        auto ptrx3 = &num3;
        cout<<(ptr3<=>ptrx3 == 0)<<" | ";
        #else
        __LOWRE_THAN_CPP20_RUN;
        #endif

        //24.4 自动生成比较运算符
        //标准库提供std::rel_ops的命令空间
        //在用户自定义类型提供==和<运算符的情况下
        //生成!=, >, <=和>=
        //C++20后，如果有三向运算符，自动生成>=, <=, >, <
        cout<<"\nchapter24.4\n  ";
        TCompare t4{1}, f4{2};
        cout<<(t4 == f4)<<" | ";
        cout<<(t4 < f4)<<" | ";
        cout<<(t4 != f4)<<" | ";
        cout<<(t4 > f4)<<" | ";
        cout<<(t4 <= f4)<<" | ";
        cout<<(t4 >= f4)<<" | ";
        #if __MORE_THAN_CPP20__
        weakCompare w4(2), l4(3);
        cout<<(w4 < l4)<<" | ";
        cout<<(w4 > l4)<<" | ";
        cout<<(w4 <= l4)<<" | ";
        cout<<(w4 >= l4)<<" | ";
        #else
        cout<<"\n  ";
        __LOWRE_THAN_CPP20_RUN;
        #endif

        //24.5 兼容旧代码
        //如果用户自定义类型实现了<, ==运算符函数，生成三向比较符的时候会使用这些运算符
        cout<<"\nchapter24.5\n  ";
        #if __MORE_THAN_CPP20__
        GlobalCompare g5, s5;
        cout<<(g5 <=> s5 < 0)<<" | ";
        #else
        __LOWRE_THAN_CPP20_RUN;
        #endif
        FUNCTION_END()
    }
}