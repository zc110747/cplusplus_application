<<<<<<< HEAD
﻿//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. shared_ptr智能指针
//      2. weak_ptr智能指针
//      3. unique_ptr智能指针
//
// Author:
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <algorithm>
#include <memory>

using namespace std;
#define FUNCTION_START()  {cout<<__func__<<":\n  ";}{
#define FUNCTION_END()    }{cout<<"\n\n";  }   

namespace SHARED_PTR
{
    class A
    {
    public:
        ~A(){
            cout<<"destructed"<<" | ";
        }
        void print(){
            cout<<"A"<<" | ";
        }
    };

    class B: public enable_shared_from_this<B>{
    };

    void ptr_test(void)
    {
        FUNCTION_START()

        std::cout<<"shared_ptr test"<<endl;
        shared_ptr<A> sp1(new A);
        auto sp2(sp1);
        decltype(sp1) sp3;
        sp3 = sp1;

        cout<<sp1.use_count()<<" | ";
        cout<<"sp1 destory"<<" | ";
        sp1.reset();
        cout<<"sp2 destory"<<" | ";
        sp2.reset();
        cout<<"sp3 destory"<<" | ";
        sp3.reset();    //only sp3 destory, destructed the A
        cout<<"\n  ";

        shared_ptr<int> spint(new int(10));
        auto spint1 = spint;
        *spint = 10;
        cout<<*spint.get()<<" | ";
        *spint1 = 5;
        cout<<*spint.get()<<" | ";

        //用于构造share_ptr的函数
        auto spa = make_shared<A>();
        spa->print();
        spa.reset();

        //通过enable_shared_from_this获取类的本身的共享指针
        auto spb = make_shared<B>();
        std::shared_ptr<B> spb1 = spb->shared_from_this();
        auto spb2 = spb1;
        cout<<"share count:"<<spb1.use_count()<<" | ";

        auto spb3 = std::move(spb1);
        cout<<"share count:"<<spb1.use_count()<<" | ";
        cout<<"share count:"<<spb3.use_count()<<" | ";

        FUNCTION_END() 
    }
}

namespace WEAK_PTR
{
    class A
    {
    public:
        ~A(){
            cout<<"destructed"<<" | ";
        }
    };

    void ptr_test(void)
    {
        FUNCTION_START()
        
        shared_ptr<A> sp(new A());
        weak_ptr<A> wp(sp);

        //weak_ptr的引用不影响对shared_ptr的引用计数
        cout<<wp.use_count()<<" | ";
        auto sp1 = sp;
        cout<<wp.use_count()<<" | ";
        sp1.reset();
        cout<<wp.use_count()<<" | ";
        
        //lock需要在对象释放前获取
        auto sp2 = wp.lock();
        sp.reset();
        cout<<wp.use_count()<<" | ";
        sp2.reset();
        cout<<wp.use_count()<<" | ";

        FUNCTION_END()
    }
}

namespace UNIQUE_PTR
{
    class A
    {
    public:
        ~A(){
            cout<<"destructed"<<"\n  ";
        }
    };

    void ptr_test(void)
    {
        FUNCTION_START()
    
        unique_ptr<A> upa{new A};
        auto upa_1 = std::move(upa); //upa: nullpr upa_1:A*
        if(upa == nullptr)
            cout<<"empty"<<" | ";
        upa_1.reset();
                                         //upi   upi_1   p
        unique_ptr<int> upi(new int(10));//(10)  null    null
        auto upi_1 = std::move(upi);     //null  10      null
        auto p = upi_1.get();            //null  10      10
        *p = 20;                         //null  20      20
        p = nullptr;                     //null  20      null

        //release将指针管理返还
        auto q = upi_1.release();
        cout<<*q<<" | ";
        if(upi_1 == nullptr)
            cout<<"empty"<<" | ";
        delete q;
        q = nullptr;

        // std::make_unique申请
        auto ptr = std::make_unique<int>(10);
        auto ptr_1 = ptr.get();
        *ptr_1 = 5;
        cout << *ptr << endl;

        auto ptr_arr = std::make_unique<int[]>(5);
        ptr_arr[0] = 1;
        cout << ptr_arr[0] <<endl;

        FUNCTION_END()
    }
}

int main(void)
{
    SHARED_PTR::ptr_test();

    WEAK_PTR::ptr_test();

    UNIQUE_PTR::ptr_test();
    return 0;
=======
﻿/*

*/
#include <iostream>
#include <algorithm>
#include <memory>

using namespace std;
#define FUNCTION_START()  {cout<<__func__<<":\n  ";}{
#define FUNCTION_END()    }{cout<<"\n\n";  }   

namespace SHARED_PTR
{
    class A
    {
    public:
        ~A(){
            cout<<"destructed"<<" | ";
        }
        void print(){
            cout<<"A"<<" | ";
        }
    };

    class B: public enable_shared_from_this<B>{
    };

    void ptr_test(void)
    {
        FUNCTION_START()

        std::cout<<"shared_ptr test"<<endl;
        shared_ptr<A> sp1(new A);
        auto sp2(sp1);
        decltype(sp1) sp3;
        sp3 = sp1;

        cout<<sp1.use_count()<<" | ";
        cout<<"sp1 destory"<<" | ";
        sp1.reset();
        cout<<"sp2 destory"<<" | ";
        sp2.reset();
        cout<<"sp3 destory"<<" | ";
        sp3.reset();    //only sp3 destory, destructed the A
        cout<<"\n  ";

        shared_ptr<int> spint(new int(10));
        auto spint1 = spint;
        *spint = 10;
        cout<<*spint.get()<<" | ";
        *spint1 = 5;
        cout<<*spint.get()<<" | ";

        //用于构造share_ptr的函数
        auto spa = make_shared<A>();
        spa->print();
        spa.reset();

        //通过enable_shared_from_this获取类的本身的共享指针
        auto spb = make_shared<B>();
        std::shared_ptr<B> spb1 = spb->shared_from_this();
        auto spb2 = spb1;
        cout<<"share count:"<<spb1.use_count()<<" | ";

        auto spb3 = std::move(spb1);
        cout<<"share count:"<<spb1.use_count()<<" | ";
        cout<<"share count:"<<spb3.use_count()<<" | ";

        FUNCTION_END() 
    }
}

namespace WEAK_PTR
{
    class A
    {
    public:
        ~A(){
            cout<<"destructed"<<" | ";
        }
    };

    void ptr_test(void)
    {
        FUNCTION_START()
        
        shared_ptr<A> sp(new A());
        weak_ptr<A> wp(sp);

        //weak_ptr的引用不影响对shared_ptr的引用计数
        cout<<wp.use_count()<<" | ";
        auto sp1 = sp;
        cout<<wp.use_count()<<" | ";
        sp1.reset();
        cout<<wp.use_count()<<" | ";
        
        //lock需要在对象释放前获取
        auto sp2 = wp.lock();
        sp.reset();
        cout<<wp.use_count()<<" | ";
        sp2.reset();
        cout<<wp.use_count()<<" | ";

        FUNCTION_END()
    }
}

namespace UNIQUE_PTR
{
    class A
    {
    public:
        ~A(){
            cout<<"destructed"<<"\n  ";
        }
    };

    void ptr_test(void)
    {
        FUNCTION_START()
    
        unique_ptr<A> upa{new A};
        auto upa_1 = std::move(upa); //upa: nullpr upa_1:A*
        if(upa == nullptr)
            cout<<"empty"<<" | ";
        upa_1.reset();
                                         //upi   upi_1   p
        unique_ptr<int> upi(new int(10));//(10)  null    null
        auto upi_1 = std::move(upi);     //null  10      null
        auto p = upi_1.get();            //null  10      10
        *p = 20;                         //null  20      20
        p = nullptr;                     //null  20      null

        //release将指针管理返还
        auto q = upi_1.release();
        cout<<*q<<" | ";
        if(upi_1 == nullptr)
            cout<<"empty"<<" | ";
        delete q;
        q = nullptr;

        // std::make_unique申请
        auto ptr = std::make_unique<int>(10);
        auto ptr_1 = ptr.get();
        *ptr_1 = 5;
        cout << *ptr << endl;

        auto ptr_arr = std::make_unique<int[]>(5);
        ptr_arr[0] = 1;
        cout << ptr_arr[0] <<endl;

        FUNCTION_END()
    }
}

int main(void)
{
    SHARED_PTR::ptr_test();

    WEAK_PTR::ptr_test();

    UNIQUE_PTR::ptr_test();
    return 0;
>>>>>>> 62558f2a9fe27a1dd64315cc3ed8ad9fadd71fcb
}