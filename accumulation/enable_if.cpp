//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      enable_if.cpp
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
#include "enable_if.hpp"
#include "../includes.hpp"
#include <typeinfo>

using namespace std;

/*
enable_if是为了配合SFINAE存在的
SFINAE指的是在匹配重载的函数或者类时，如果没有匹配上，不会报错。
template<class T>
typename std::enable_if<is_integral<T>::value, bool>::type
---------------------------------------------------------------------------------
template<class T, typename = std::enable_if<is_floating_point_v<T>, void>::type>

enable_if的输出类型，可以作为函数的返回值以及
*/

namespace ENABLE_IF
{
    // 1. the return type (bool) is only valid if T is an integral type:
    template<class T>
    typename std::enable_if<is_integral<T>::value, bool>::type
    is_odd(T i)
    {
        return bool(i%2);
    }

    // 2. the second template argument is only valid if T is an integral type:
    template<class T, class = typename std::enable_if<is_integral<T>::value>::type>
    bool is_even(T i)
    {
        return !bool(i%2);
    } 

    template<typename T>
    struct is_test_integral : std::false_type {};

    template<>
    struct is_test_integral<int> : std::true_type {};

    template<class T>
    bool test_integral(T i) requires std::is_integral_v<T>
    {
        return is_integral<T>::value;
    }

    template<class T, typename L= std::enable_if<is_floating_point_v<T>, float>::type>
    bool test_float(T i)
    {
        L v;

        cout<<typeid(v).name()<<" | ";
        return is_floating_point_v<T>;
    }

    template<class T>
    class enable_if_test
    {
    public:
        enable_if_test(T a) requires std::is_integral_v<T> {
            cout<<"int"<<" | ";
        }
        enable_if_test(T a) requires std::is_floating_point_v<T> {
            cout<<"float"<<" | ";
        }     
    };

    template <typename T>
    struct is_std_vector: std::false_type { };

    template<typename ValueType>
    struct is_std_vector<std::vector<ValueType>>: std::true_type { };

    template<typename T>
    typename std::enable_if<is_std_vector<T>::value, T>::type
    propery(void)
    {
        T value;
        cout<<"vector"<<endl;
        return value;
    };

    template<typename T>
    typename std::enable_if<!is_std_vector<T>::value, T>::type
    propery(void)
    {
        T value;
        cout<<"normal"<<endl;
        return value;
    };

    void test_run()
    {
        FUNCTION_START()

        std::vector<int> a;
        propery<std::vector<int>>();
        propery<int>();

        cout<<"\nchapter3.1\n  ";
        cout<<std::boolalpha<<is_even(5)<<" | ";
        
        cout<<is_odd(6)<<" | ";
        
        cout<<is_test_integral<int>::value<<" | ";
        cout<<is_test_integral<float>::value<<" | ";

        cout<<test_integral(4)<<" | ";
        cout<<test_float(4.3)<<" | ";

        enable_if_test t1 = {1};
        enable_if_test t2 = {1.5};


        FUNCTION_END()
    }
}

