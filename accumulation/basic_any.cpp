//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      basic_any.cpp
//
//  Purpose:
//         
//
//  Author:
//     	@公众号：<嵌入式技术总结>
//
//  Assumptions:
//
//  Revision History:
//      4/19/2023   Create New Version
/////////////////////////////////////////////////////////////////////////////
#include "basic_any.hpp"
#include "../includes.hpp"

using namespace std;

/*
支持多个类型的对象
*/
namespace BASIC_ANY
{
    struct A
    {
        int x{1};
        int y{5};
    };

    struct Base
    {
        virtual Base* copy() = 0;
    };
    
    template<typename T>
    struct Derived: public Base
    {
        T data_;
        
        Derived(const T& val):data_{val} {
        }

        virtual Base* copy() {
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
        basic_any(const T& data):ptr_{new Derived<T>(data)} {
            std::cout<<"construct function"<<" | ";
        }

        //复制构造函数
        basic_any(const basic_any& rhs)
        {   
            std::cout<<"copy construct function"<<" | ";

            if (ptr_) {
                delete ptr_;
                ptr_ = nullptr;
            }

            if (rhs.ptr_) {
                ptr_ = rhs.ptr_->copy();
            }
        }

        //赋值函数
        basic_any& operator= (const basic_any& rhs)
        {
            std::cout<<"copy function"<<" | ";

            if (ptr_) {
                delete ptr_;
                ptr_ = nullptr;
            } 

            if(rhs.ptr_)
            {
                ptr_ = rhs.ptr_->copy();
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
            if(ptr_) {
                return static_cast<Derived<T> *>(ptr_)->data_;
            } else {
                throw "ptr nullptr issue!";
            }
        }
    private:
        Base *ptr_{nullptr};
    };

    void test_run()
    {
        FUNCTION_START()
        try
        {
            //char
            basic_any char_val;
            char_val = 's';
            cout<<char_val.get<char>()<<"\n";

            //int/float
            basic_any x = 3;
            x = 3.141;
            cout<<x.get<double>()<<"\n";

            //struct
            A val;
            basic_any y = val;
            cout<<y.get<A>().x<<" | ";
            cout<<y.get<A>().y<<"\n";

            //string
            basic_any str_val = std::string{"hello"};
            cout<<str_val.get<string>()<<" | ";
            str_val = 16;
            cout<<str_val.get<int>()<<"\n";

            basic_any any_val;
            cout<<any_val.get<int>(); 
        }
        catch(const char* str)
        {
            std::cout << str << '\n';
        }
        
        FUNCTION_END()
    }
}
