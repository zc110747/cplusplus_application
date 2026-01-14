//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      other_optimize.cpp
//
//  Purpose:
//
//  Author:
//     	@公众号 <嵌入式技术总结>
//
//  Assumptions:
//
//  Revision History:
//      4/19/2023   Create New Version
/////////////////////////////////////////////////////////////////////////////
#include "other_optimize.hpp"
#include "../includes.hpp"

using namespace std;

namespace OTHER_OPTIMIZE
{
    //34.1 显示自定义类型转换运算符
    //显示声明在某些期待上下文为bool的语境，仍然可以执行隐式转换
    //1.if, while, for的控制表达式
    //2.内建逻辑运算符!、&&和||的操作数
    //3.条件运算符?:的首个操作数
    //4.static_assert声明中的bool常量表达式
    //5.noexcept说明符中的表达式
    template<class T>
    class SomeStorage
    {
    public:
        SomeStorage() = default;
        SomeStorage(std::initializer_list<T> l) : data_(l) {};
        explicit operator bool() const { return !data_.empty(); }
    private:
        std::vector<T> data_;
    };

    //34.2 关于std::launder()
    struct X2
    {
        const int n;
    };
    union U2
    {
        X2 x;
        float f;
    };

    //34.3 返回值优化
    class X3
    {
    public:
        X3() {cout<<"x3 ctor"<<" | ";}
        X3(const X3 &x) {cout<<"copy x3 ctor"<<" | ";}
        ~X3() {cout<<"x3 dtor"<<" | "; }
    };

    X3 make_x()
    {
        X3 x1;
        return x1;
    }

    //34.4 允许按值进行默认比较
    #if __MORE_THAN_CPP20__
    struct C4 {
        int i;
        friend bool operator==(C4, C4) = default;
    };
    #endif

    //34.6 允许数组转换为未知范围的数组
    #if __MORE_THAN_CPP20__
    void f(int(&)[]) {
        cout<<"call f(int(&)[])";
    }
    void f(int(&)[1]) {
        cout<<"call f(int(&)[1])";
    }
    #endif

    //34.7 在delete运算符函数中析构对象
    #if __MORE_THAN_CPP20__
    struct X7
    {
        X7() {
        }
        ~X7() 
        {
            cout<<"call dtor"<<" | ";
        }
        void* operator new(size_t s)
        {
            cout<<"call ctor"<<" | ";
            return ::operator new(s);
        }
        void operator delete(X7 *ptr, std::destroying_delete_t)
        {
            ptr->~X7();
            cout<<"call delete"<<" | ";
            ::operator delete(ptr);
        }
    };
    #endif

    //34.9 修复const和默认复制构造不匹配造成无法编译问题
    #if __MORE_THAN_CPP20__
    struct MyType{
        MyType() = default;
        MyType(MyType &) {}
        int i{2};
    };

    template <typename T>
    struct Wrapper{
        Wrapper() = default;
        Wrapper(const Wrapper&) = default;
        T t;
    };
    #endif
    void test_run()
    {
        FUNCTION_START()

        //34.1 显示自定义类型转换运算符
        cout<<"\nchapter34.1\n  ";
        SomeStorage<int> s1{1, 2, 3};
        cout<<static_cast<bool>(s1)<<" | ";
        if(s1)
        {
            cout<<"not empty!"<<" | ";
        }

        //34.2 关于std::launder()
        cout<<"\nchapter34.2\n  ";
        #if __MORE_THAN_CPP17__
        U2 u2 = {{1}};
        X2 *p2 = new(&u2.x) X2{2};
        cout<<((*std::launder(&u2.x.n)) == 2)<<" | ";
        #else
        __LOWRE_THAN_CPP17_RUN;
        #endif

        //34.3 返回值优化
        //返回值优化是C++中的一种编译优化技术，它允许编译器将
        //函数返回的对象直接构造到它们本来要存储的变量空间中而不产生临时对象.
        cout<<"\nchapter34.3\n  ";
        {
            X3 x3 = make_x();
        }

        //34.4 允许按值进行默认比较
        cout<<"\nchapter34.4\n  ";
        #if __MORE_THAN_CPP20__
        C4 a41{1}, a42{2};
        cout<<(a41 == a42)<<" | ";
        #else
        __LOWRE_THAN_CPP20_RUN
        #endif 

        //34.5 支持new表达式推导数组长度
        cout<<"\nchapter34.5\n  ";
        #if __MORE_THAN_CPP20__
        int *x5 = new int[]{1, 2, 3};
        cout<<x5[0]<<" | "<<x5[1]<<" | ";
        #else
        __LOWRE_THAN_CPP20_RUN
        #endif   

        //34.6 允许数组转换为未知范围的数组
        cout<<"\nchapter34.6\n  ";
        #if __MORE_THAN_CPP20__   
        {
            int arr6[1];
            f(arr6);
        }
        #else
        __LOWRE_THAN_CPP20_RUN
        #endif

        //34.7 在delete运算符函数中析构对象
        cout<<"\nchapter34.7\n  ";
        #if __MORE_THAN_CPP20__   
        {
            X7 *ptr7 = new X7;
            delete ptr7;
        }
        #else
        __LOWRE_THAN_CPP20_RUN
        #endif

        //34.8 调用伪析构函数结束对象声明周期
        //伪析构函数的调用总是会结束对象的生命周期
        cout<<"\nchapter34.8\n  ";

        //34.9 修复const和默认复制构造不匹配造成无法编译问题
        cout<<"\nchapter34.9\n  ";
        #if __MORE_THAN_CPP20__   
        {
            Wrapper<MyType> var;
            cout<<var.t.i<<" | ";
        }
        #else
        __LOWRE_THAN_CPP20_RUN
        #endif

        //34.10 不推荐使用volatile的情况
        //1.不推荐算数类型的后缀++和--表达式使用volatile限定符
        //2.不推荐非类类型左操作数的赋值使用volatile限定符
        //3.不推荐函数形参和返回类型使用volatile限定符
        //4.不推荐结构化绑定使用volatile限定符
        cout<<"\nchapter34.10\n  ";

        //34.11 不推荐在下标表达式中使用逗号运算符
        cout<<"\nchapter34.11\n  ";
        int a11[]{1, 2, 3};
        int x = 1, y = 2;
        cout<<a11[x, y]<<" | ";

        //34.12 模块
        //import module
        cout<<"\nchapter34.12\n  ";

        FUNCTION_END()
    }
}
