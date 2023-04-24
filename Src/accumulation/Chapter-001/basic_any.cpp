//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      any.cpp
//
//  Purpose:
//         
//
//  Author:
//     	@听心跳的声音
//
//  Assumptions:
//
//  Revision History:
//      4/19/2023   Create New Version
/////////////////////////////////////////////////////////////////////////////
#include "basic_any.hpp"
#include "../includes.hpp"

using namespace std;

namespace BASIC_ANY
{
    struct Base
    {
        virtual Base* copy() = 0;
    };
    
    template<typename T>
    struct Derived:public Base
    {
        T data_;
        Derived(const T& val):data_{val}
        {
        }
        virtual Base* copy() 
        {
            return new Derived<T>(data_);
        }
    };

    struct basic_any
    {
    public:
        //默认构造函数
        basic_any() = default;

        //带参数构造函数
        template<typename T>
        basic_any(const T& data):ptr_{new Derived<T>(data)}
        {
        }

        //复制构造函数
        basic_any(const basic_any& rhs)
        {   
            if(rhs.ptr_)
            {
                ptr_ = rhs.ptr_->copy();
            }
        }

        //赋值函数
        basic_any& operator=(const basic_any& rhs)
        {
            if(ptr_) delete ptr_;
            if(rhs.ptr_)
            {
                ptr_ = rhs.ptr_->copy();
            }
            else 
            {
                ptr_ = rhs.ptr_;
            }
            return *this;
        }

        virtual ~basic_any()
        {
            if(!ptr_)
            {
                delete ptr_;
                ptr_ = nullptr;
            }
        }

        template<typename T>
        T &get()
        {
            return static_cast<Derived<T> *>(ptr_)->data_;
        }
    private:
        Base *ptr_{nullptr};
    };

    struct A
    {
        int x{1};
        int y{5};
    };

    void test_study()
    {
        FUNCTION_START()
        basic_any def;
        def = 's';
        cout<<def.get<char>()<<" | ";

        basic_any x = 3;
        x = 3.141;

        A a1;
        basic_any y = a1;

        cout<<x.get<double>()<<" | ";
        cout<<y.get<A>().x<<" | ";
        cout<<y.get<A>().y<<" | ";

        basic_any str = std::string{"hello"};
        cout<<str.get<string>()<<" | ";
        str = 16;
        cout<<str.get<int>()<<" | ";
  

        FUNCTION_END()
    }
}
