/*
智能指针是C++中的一种特殊类型的指针，它提供了自动内存管理的功能。
智能指针通过在对象不再被使用时自动释放其所指向的内存，从而避免了内存泄漏的问题。
C++标准库提供了三种主要的智能指针：unique_ptr、shared_ptr和weak_ptr。

1. unique_ptr
unique_ptr是一种独占所有权的智能指针，它确保同一时间只有一个unique_ptr指向某个对象。
当unique_ptr被销毁时，它所指向的对象也会被自动删除。unique_ptr不支持复制操作，但可以通过std::move进行所有权的转移

2. shared_ptr
shared_ptr是一种共享所有权的智能指针，它允许多个shared_ptr指向同一个对象。
当最后一个指向对象的shared_ptr被销毁时，对象才会被自动删除。shared_ptr通过引用计数来管理对象的生命周期。

3. weak_ptr
weak_ptr是一种弱引用的智能指针，它指向由shared_ptr管理的对象，但不会增加对象的引用计数。
weak_ptr主要用于解决循环引用的问题，避免内存泄漏。
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
        cout<<spb1.use_count()<<" | ";

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

        FUNCTION_END()
    }
}

int main(void)
{
    SHARED_PTR::ptr_test();

    WEAK_PTR::ptr_test();

    UNIQUE_PTR::ptr_test();
    return 0;
}