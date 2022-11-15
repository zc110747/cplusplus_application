/******************************************************************
 * 第三十七章 模板参数优化推导
********************************************************************/

#include <map>
#include <vector>
#include <tuple>
#include <string>
#include <iostream>
#include <algorithm>
#include <memory>
#include <complex>
#include "temp_deduction.hpp"

using namespace std;

#define FUNCTION_START()  {cout<<__func__<<":\n  ";}{
#define FUNCTION_END()    }{cout<<"\n\n";  }  

namespace Template1
{
    template<class T, auto N>
    struct MyCountOf
    {
        MyCountOf(T(&)[N]) {}
        std::size_t value = N;
    };

    template<class T1, class T2>
    void foo(T1 t1, T2 t2)
    {
    }

    void temp_deduction_test(void)
    {
        FUNCTION_START()

        std::map<int,  std::string> mp1;
        mp1.insert(make_pair(1, "hello"));
        for(auto &x: mp1)
        {
            cout<<x.first<<" "<<x.second<<" | ";
        }

        auto tp1 = make_tuple(1, 2, 1.5, "hello");
        cout<<std::get<2>(tp1)<<" | ";

        //自动类型推导，map, vector
        auto vp1 = new vector{1, 2, 3};
        std::vector vp2{1, 2, 3};
        auto mp2 = new map{make_pair(1, "h"), make_pair(2, "w")};
        std::map mp3{make_pair(1, "h"), make_pair(2, "w")};
        std::complex c1{3.5, 1.0}; //类型推导需要参数类型一致

        cout<<(*vp1)[1]<<" "<<vp2[2]<<" | ";
        cout<<mp2->begin()->first<<" "<<mp3.begin()->second<<" | ";
        cout<<c1<<" | ";

        //类的类型推导
        MyCountOf c("hello"); 
        cout<<c.value<<" | ";

        //函数的类型推导
        foo(1, 1.5);
        foo<>(1, 1.5);
        foo<int>(1, 1.5);

        delete vp1;
        delete mp2;
        FUNCTION_END() 
    }
}

namespace Template2
{
    template<class T>
    struct LambdaWarp
    {   
        LambdaWarp(T t): func(t) {};

        template<class ...Args>
        void operator() (Args&& ... arg)
        {
            func(std::forward<Args>(arg) ...);
        }

        T func;
    };

    template<class T>
    struct S
    {
        T x;
        T y;
    };

    template<class T, class U>
    struct X
    {
        S<T> s1;
        U x;
        T y;    //必须由y推导整个类型，S<T>不参与推导
    };

    void temp_deduction_test(void)
    {
        FUNCTION_START()

        //拷贝初始化优先
        std::vector v1{1, 2, 3};   //vector<int>
        std::vector v2{v1};        //vector<int>
        for(auto &x: v2)
        {
            cout<<x<<" ";
        }
        cout<<" | ";

        auto v3 = std::vector{v2};
        for(auto &x: v3)
        {
            cout<<x<<" ";
        }
        cout<<" | ";

        LambdaWarp x([](int a, int b){
            cout<<a+b<< " | ";
        });
        x(1, 5);

        //别名模板的实参推导
        //C++20 future

        //聚合类型的类模板实参推导
        S s1{1, 2};
        cout<<s1.x<<" "<<s1.y<<" | ";

        X x1{{1, 2}, 3, 4};
        cout<<x1.s1.x<<" "<<x1.s1.y<<" | ";   
        FUNCTION_END() 
    }
}

void temp_deduction(void)
{
    Template1::temp_deduction_test();
    
    Template2::temp_deduction_test();
}
