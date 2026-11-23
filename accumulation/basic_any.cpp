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

#include <exception>

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

    //类型不匹配或对象为空时抛出的异常
    struct bad_any_cast: public std::exception
    {
        const char* what() const noexcept override {
            return "bad_any_cast: object empty or type mismatch";
        }
    };

    struct Base
    {
        virtual Base* copy() const = 0;

        //多态基类必须有虚析构, 否则 delete Base* 不会调用派生类析构函数
        virtual ~Base() = default;
    };
    
    template<typename T>
    struct Derived: public Base
    {
        T data_;
        
        explicit Derived(const T& val):data_{val} {
        }

        Base* copy() const override {
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
        basic_any(const basic_any& rhs):ptr_{rhs.ptr_ ? rhs.ptr_->copy() : nullptr}
        {   
            std::cout<<"copy construct function"<<" | ";
        }

        //赋值函数: 先复制后释放, 自赋值时 rhs 与 this 相同也不会访问已释放内存
        basic_any& operator= (const basic_any& rhs)
        {
            std::cout<<"copy function"<<" | ";

            if (this == &rhs) {
                return *this;
            }

            Base *new_ptr = rhs.ptr_ ? rhs.ptr_->copy() : nullptr;
            delete ptr_;
            ptr_ = new_ptr;
            return *this;
        }

        virtual ~basic_any()
        {
            delete ptr_;
            ptr_ = nullptr;
        }

        template<typename T>
        T &get()
        {
            //dynamic_cast 校验真实类型, 类型不符时抛异常而不是未定义行为
            Derived<T> *p = dynamic_cast<Derived<T> *>(ptr_);
            if (p == nullptr) {
                throw bad_any_cast{};
            }
            return p->data_;
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

            //自赋值: 赋值运算符内部先复制后释放, 不会把自身数据删掉
            basic_any self_val = 5;
            self_val = self_val;
            cout<<self_val.get<int>()<<"\n";

            basic_any any_val;
            cout<<any_val.get<int>(); 
        }
        catch(const std::exception& e)
        {
            std::cout << e.what() << '\n';
        }
        
        FUNCTION_END()
    }
}
