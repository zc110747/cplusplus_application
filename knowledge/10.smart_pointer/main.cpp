//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. std::unique_ptr说明
//      2. std::shared_ptr说明
//      3. std::weak_ptr说明
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

namespace UNIQUE_PTR
{
    class A
    {
    public:
        ~A(){
            std::cout << "destructed" << "\n  ";
        }
    };

    void test(void)
    {
        std::cout << "===================== UNIQUE_PTR =====================" << std::endl;

        std::unique_ptr<A> uptr_0{new A};
        auto uptr_1 = std::move(uptr_0); //uptr_0: nullpr uptr_1:A*
        if(uptr_0 == nullptr) {
            std::cout << "empty" << " | ";
        }
        uptr_1.reset();
                                            //upi   uptr_2   p
        std::unique_ptr<int> upi(new int(10));//(10)  null    null
        auto uptr_2 = std::move(upi);       //null  10      null
        auto p = uptr_2.get();              //null  10      10
        *p = 20;                            //null  20      20
        p = nullptr;                        //null  20      null

        //release将指针管理返还
        auto q = uptr_2.release();
        std::cout << *q  << " | ";
        if(uptr_2 == nullptr) {
            std::cout << "empty" << " | ";
        }
        delete q;
        q = nullptr;

        // std::make_unique申请
        auto ptr = std::make_unique<int>(10);
        auto ptr_1 = ptr.get();
        *ptr_1 = 5;
        std::cout << *ptr << std::endl;

        auto ptr_arr = std::make_unique<int[]>(5);
        ptr_arr[0] = 1;
        std::cout << ptr_arr[0] << std::endl;
    }
}

namespace SHARED_PTR
{
    class A
    {
    public:
        ~A(){
            std::cout << "destructed" << " | ";
        }
        void print(){
            std::cout << "A" << " | ";
        }
    };

    // 继承声明为public，可以在外部使用this指针
    class B : public std::enable_shared_from_this<B> {
    };

    void test(void)
    {
        std::cout << "===================== SHARED_PTR =====================" << std::endl;

        std::shared_ptr<A> sptr_0(new A);
        auto sptr_1(sptr_0);
        decltype(sptr_0) sptr_2;
        sptr_2 = sptr_0;

        std::cout << "share count:" << sptr_0.use_count() << " | ";
        std::cout << "sptr_0 destory" << " | ";
        sptr_0.reset();
        std::cout << "sptr_1 destory" << " | ";
        sptr_1.reset();
        std::cout << "sptr_2 destory" << " | ";
        sptr_2.reset();    //only sptr_2 destory, destructed the A
        std::cout << "\n  ";
        std::cout << sptr_0.use_count() << " | ";

        std::shared_ptr<int> sptr_3(new int(10));
        auto sptr_4 = sptr_3;
        *sptr_3 = 10;
        std::cout << *sptr_3.get() << " | ";
        *sptr_4 = 5;
        std::cout << *sptr_3.get() << " | ";

        //用于构造share_ptr的函数
        auto sptr_5 = std::make_shared<A>();
        sptr_5->print();
        sptr_5.reset();

        //通过enable_shared_from_this获取类的本身的共享指针
        auto sptr_6 = std::make_shared<B>();
        std::shared_ptr<B> sptr_7 = sptr_6->shared_from_this();
        auto sptr_8 = sptr_7;
        std::cout << "share count:"<<sptr_7.use_count() << " | ";

        auto sptr_9 = std::move(sptr_7);
        std::cout << "share count:"<<sptr_7.use_count() << " | ";
        std::cout << "share count:"<<sptr_9.use_count() << std::endl;
    }
}

namespace WEAK_PTR
{
    class A
    {
    public:
        ~A(){
            std::cout << "destructed" << " | ";
        }
    };

    void test(void)
    {
        std::cout << "===================== WEAK_PTR =====================" << std::endl;

        std::shared_ptr<A> sp(new A());
        std::weak_ptr<A> wp(sp);

        // std::weak_ptr的引用不影响对shared_ptr的引用计数
        std::cout << wp.use_count() << " | ";
        auto sptr_0 = sp;
        std::cout << wp.use_count() << " | ";
        sptr_0.reset();
        std::cout << wp.use_count() << " | ";
        
        //lock需要在对象释放前获取
        auto sptr_1 = wp.lock();
        sp.reset();
        std::cout << wp.use_count() << " | ";
        sptr_1.reset();
        std::cout << wp.use_count() << std::endl;
    }
}

int main(void)
{
    UNIQUE_PTR::test();

    SHARED_PTR::test();

    WEAK_PTR::test();

    return 0;
}