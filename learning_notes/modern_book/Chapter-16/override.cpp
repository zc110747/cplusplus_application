//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      override.cpp
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
#include "override.hpp"
#include "../includes.hpp"

using namespace std;

namespace OVERRIDE
{
    //16.1 重写，重载和隐藏
    class A_Base
    {
    public:
        virtual void f() {
            cout<<"A_Base::f"<<" | ";
        }
        void f(int a){  
            cout<<"A_Base::f(int)"<<" | ";
        }
        void f1(){
            cout<<"A_Base::f1"<<" | ";
        }

        virtual void f2() final {
            cout<<"A_Base::f2"<<" | ";
        }
    };

    class A_Derive final:public A_Base
    {
    public:
        //using A_Base::f; //通过using，可以将所有相关函数引入派生类
        void f() override {
            cout<<"A_Derive::f"<<" | ";
        }
        void f1(){
            cout<<"A_Derive::f1"<<" | ";
        }
    };

    //error, 声明为final的类不允许被继承
    // class A1_Derive:A_Derive{
    // };

    //16.2 重写引发的问题
    class Base2
    {
    public:
        virtual void some_func() {
            cout<<"Base2::some_func"<<" | ";
        }
        virtual void foo(int x) {
            cout<<"Base2::foo"<<" | ";
        }
        virtual void bar() const {
            cout<<"Base2::bar"<<" | ";
        }
        virtual void baz() {
            cout<<"Base2::baz"<<" | ";
        }
    };

    class Derived2: public Base2 {
    public:
        virtual void sone_func() {
            cout<<"Base2::some_func"<<" | ";
        }
        virtual void foo(int &x) {
            cout<<"Base2::foo"<<" | ";
        }
        virtual void bar() {
            cout<<"Base2::bar"<<" | ";
        }
        virtual void baz() {
            cout<<"Base2::baz"<<" | ";
        }
    };

    //16.3 使用override说明符
    class Derived3: public Base2 {
    public:
        virtual void some_func() override{
            cout<<"Base2::some_func"<<" | ";
        }
        virtual void foo(int x) override {
            cout<<"Base2::foo"<<" | ";
        }
        virtual void bar() const override{
            cout<<"Base2::bar"<<" | ";
        }
        void baz() override {
            cout<<"Base2::baz"<<" | ";
        }
    };

    //16.4 使用final说明符
    class Base4{
    public:
        virtual void log(const char *) const {
            cout<<"Base4 log"<<" | ";
        }
    };
    class BaseWithFileLog: public Base4{
    public:
        virtual void log(const char *) const override final{
            cout<<"BaseWithFileLog log"<<" | ";
        } 
    };

    void test_run()
    {
        FUNCTION_START()

        //16.1 重写，重载和隐藏
        //重写(override), 派生类覆盖了基类的虚函数(具有相同的函数签名和类型,且基类需要是虚函数)
        //重载(overload), 一个类有两个或者两个以上函数，函数名相同，签名不同   
        //隐藏(overwrite), 派生类出现基类的同名函数，如果签名不同，则基类函数无论是否为虚函数，都将隐藏。
        //如果签名相同，则需要确定是否为虚函数，是虚函数则重写，不是则为隐藏
        //(隐藏后，将无法调用基类的函数，如果需要使用，需要通过using引入派生类)
        cout<<"\nchapter16.1\n  ";
        A_Derive a1;
        a1.f();         //A_Derive::f,基类带virtual, 发生overwrite, 访问派生类
        //a1.f(1);      //error, 派生类出现基类同名函数，发生隐藏, 只能调用派生类函数
        a1.f1();        //A_Derive::f1,
        
        A_Base *a_ptr = static_cast<A_Base *>(&a1);
        a_ptr->f();     //A_Derive::f，函数名和签名相同，但是基类带virtual, overwrite, 会优先访问派生类
        a_ptr->f(2);    //A_Base::f(int), 直接调用基类的函数
        a_ptr->f1();    //A_Base::f1, 不在虚表中，调用基类的函数
        a_ptr->f2();    //A_Base::f2

        //16.2 重写引发的问题
        cout<<"\nchapter16.2\n  ";
        Derived2 d2;
        d2.bar();
        d2.baz();
        d2.some_func();

        //16.3 使用override说明符
        //放置在虚函数的尾部，明确告诉编译器这个虚函数需要覆盖基类的虚函数
        cout<<"\nchapter16.3\n  ";
        Derived3 d3;
        d3.bar();
        d3.baz();
        d3.foo(3);
        d3.some_func();

        //16.4 使用final说明符
        //final说明符告诉该虚函数不能被派生类重写
        cout<<"\nchapter16.4\n  ";    
        BaseWithFileLog log4;
        log4.log("hello");

        //16.5 override和final说明符的特别之处
        //override和final仍然能用作标识符，但不适合使用
        cout<<"\nchapter16.5\n  ";   
        FUNCTION_END()
    }
}