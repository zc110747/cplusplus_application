/******************************************************************
 * 第四十一章 概念和约束
********************************************************************/
#include "template_concept.hpp"
#include <type_traits>
#include <string>

using namespace std;

namespace CONCEPT_1
{
    template <class T, class U = std::enable_if_t<std::is_integral_v<T>>>
    struct X{};

    template <class T>
    requires std::is_integral_v<T>
    struct Y{};

    template <class T>
    requires std::is_integral_v<T> || std::is_floating_point_v<T>
    struct Z{};

    constexpr bool bar() {
        return true;
    }
    template <class T>
    requires bar()
    struct X1{};

    
    void test(void)
    {
        X<int> x1;
        //X<std::string> x2;

        Y<int> y1;
        //Y<std::string> y2;

        Z<int> z1;
        Z<float> z2;

        X1<int> x_1;
    }
}


void template_concept(void)
{
    CONCEPT_1::test();
}