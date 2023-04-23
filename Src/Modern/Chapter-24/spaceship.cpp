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
    #endif
    
    void test_study()
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

        FUNCTION_END()
    }
}

