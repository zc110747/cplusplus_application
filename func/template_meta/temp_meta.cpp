/**********************************************************
 * 说明: 模板元编程
************************************************************/
#include <iostream>
#include <type_traits>

template<bool cond, typename First, typename Second>
struct IF;

template<typename First, typename Second>
struct IF<true, First, Second>
{
    typedef First result;
};

template<typename First, typename Second>
struct IF<false, First, Second>
{
    typedef Second result;
};

//add
template<int First, int... rest>
struct add_all_;

template <int First>
struct add_all_<First>:std::integral_constant<int, First>
{
};

template <int First, int Second, int... Rest>
struct add_all_<First, Second, Rest...>
    :std::integral_constant<int, add_all_<First>::value + add_all_<Second, Rest...>::value>
{
};

//sub(第一个值减去后面之和)
template<int First, int... Rest>
struct sub_all_;

template <int arg>
struct sub_all_<arg>:std::integral_constant<int, arg>
{
};

template <int First, int Second, int... Rest>
struct sub_all_<First, Second, Rest...>
    :std::integral_constant<int, sub_all_<First>::value - add_all_<Second, Rest...>::value>
{
};

template<bool cond, int arg, int... rest>
struct add_sub_all
{
    static const int res_value = 
        IF<cond, add_all_<arg, rest...>, sub_all_<arg, rest...>>::result::value;
};

int main(int argc, char *argv[])
{
    std::cout<<"add_sub_all true:"<<add_sub_all<true, 5, 4, 3, 2>::res_value<<std::endl;
    std::cout<<"add_sub_all false:"<<add_sub_all<false, 25, 4, 1>::res_value<<std::endl;
    std::cout<<"add_all:"<<add_all_<5, 4, 3, 2>::value<<std::endl;
    std::cout<<"sub_all:"<<sub_all_<25, 4, 2, 4>::value<<std::endl;
    return 0;
}
