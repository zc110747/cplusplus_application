//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      delegation.cpp
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
#include "delegation.hpp"
#include "../includes.hpp"

using namespace std;

namespace DELEGATION
{
    //12.1 冗余的构造函数
    class X1
    {
    public:
        X1():a_(0), b_(0.) {common_init();}
        X1(int a):a_(a), b_(0.) {common_init();}
        X1(double b):a_(0), b_(b) {common_init();}
        X1(int a, double b):a_(a), b_(b) {common_init();}
        void show()
        {
            cout<<a_<<" | "<<b_<<" | ";
        }
    private:
        void common_init() {}
        int a_;
        double b_;
    };
    
    //12.2 委托构造函数
    class X2
    {
    public:
        X2():X2(0, 0.) {}
        X2(int a):X2(a, 0.) {
            cout<<"int struct"<<" | ";
        }
        X2(double b):X2(0, b) {
            cout<<"double struct"<<" | ";
        }
        X2(int a, double b):a_(a), b_(b) {
            common_init();
        }
        X2(const X2& other): X2(other.a_, other.b_) {
        }
        
        void show()
        {
            cout<<a_<<" | "<<b_<<" | ";
        }
    private:
        void common_init() {
            cout<<"basic struct"<<" | ";
        }
        int a_;
        double b_;
    };

    //12.3 委托模板构造函数
    class X3
    {
    public:
        X3(std::vector<short>& v): X3(v.begin(), v.end()) {};
        X3(std::deque<int>& v): X3(v.begin(), v.end()) {};
        void show()
        {
            for(const auto& val: l_)
            {
                cout<<val<<" | ";
            }
        }

    private:
        template<class T> X3(T first, T last): l_(first, last) {}
        std::list<int> l_;
    };

    //12.4 捕获委托构造函数的异常
    class X4
    {
    public:
        X4() try: X4(0) {}
        catch (int e)
        {
            cout<<"catch:"<<e<<" | ";
            throw 3;
        }
        X4(int a) try: X4(a, 0.) {}
        catch (int e)
        {
            cout<<"catch:"<<e<<" | ";
            throw 2;
        }
        X4(double b): X4(0, b) {}
        X4(int a, double b) : a_(a), b_(b){
            throw 1;
        }
    private:
        int a_;
        double b_;
    };

    void test_run()
    {
        FUNCTION_START()

        //12.1 冗余的构造函数
        cout<<"\nchapter12.1\n  ";
        X1 x1(1);
        x1.show();
        
        X1 x12(1, 1.5);
        x12.show();

        //12.2 委托构造函数
        //1.每个构造函数都可以委托另一个构造函数为代理，每个构造函数
        //既是委托构造函数也是代理函数
        //2.不要递归循环委托
        //3.如果一个构造函数为委托构造函数，那么其初始化列表里就不能
        //对数据成员和基类进行初始化
        //4.委托构造函数的执行顺序是先执行代理构造函数的初始化列表，然后
        //执行代理构造函数的主体，最后执行委托构造函数的主体
        //5.如果在代理构造函数执行完成后，委托构造函数主体抛出了异常，
        //则自动调用该类型的析构函数
        cout<<"\nchapter12.2\n  ";
        X2 x2(1.2);
        x2.show();

        X2 x21(x2);
        x21.show();

        //12.3 委托模板构造函数
        //当使用Function-try-block去捕获委托构造函数异常时，其过程和捕获初始化列表异常如出一辙
        cout<<"\nchapter12.3\n  ";
        std::vector<short> a{1, 2, 3, 4, 5};
        std::deque<int> b{1, 2, 3, 4, 5};
        X3 x3(a);
        X3 x31(b);
        x3.show();
        x31.show();

        //12.4 捕获委托构造函数的异常
        cout<<"\nchapter12.4\n  ";
        try
        {
            X4 x;
        }
        catch(int e)
        {
            cout<<"catch:"<<e<<" | ";
        }
        
        //12.5 委托参数较少的构造函数
        //将参数较少的构造函数委托给参数较多的构造函数
        cout<<"\nchapter12.5\n  ";
        
        FUNCTION_END()
    }
}
