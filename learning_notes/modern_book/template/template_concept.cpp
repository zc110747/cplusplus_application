/******************************************************************
 * 第四十一章 概念和约束
********************************************************************/
#include "template_concept.hpp"
#include <type_traits>
#include <string>
#include <iostream>

using namespace std;

namespace CONCEPT_1
{
    #if __MORE_THAN_CPP20__
    template <class T, class U = std::enable_if_t<std::is_integral_v<T>>>
    struct X{
        T val;
    };

    template <class T>
    requires std::is_integral_v<T>
    struct Y{
        T val;
    };

    template <class T>
    requires std::is_integral_v<T> || std::is_floating_point_v<T>
    struct Z{
        T val;
    };

    constexpr bool bar() {
        return true;
    }
    template <class T>
    requires (bar())
    struct X1{
        T val;
    };
    #endif
    
    void test(void)
    {
        #if __MORE_THAN_CPP20__
        X<int> x1{1};
        //X<std::string> x2;
        std::cout<<x1.val<<std::endl;

        Y<int> y1{1};
        //Y<std::string> y2;
        std::cout<<y1.val<<std::endl;

        Z<int> z1{1};
        Z<float> z2{1.2};
        std::cout<<z1.val<<std::endl;
        std::cout<<z2.val<<std::endl;

        X1<int> x_1{1};
        std::cout<<x_1.val<<std::endl;
        #endif
    }
}


void template_concept(void)
{
    CONCEPT_1::test();
}