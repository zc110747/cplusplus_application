//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      reference.cpp
//
//  Purpose:
//
// Author:
//     	@听心跳的声音
//
//  Assumptions:
//
//  Revision History:
//      4/19/2023   Create New Version
/////////////////////////////////////////////////////////////////////////////
#include "reference.hpp"
#include "../includes.hpp"

using namespace std;

namespace REFERENCE
{
    //6.1 左值和右值
    void set_val(int val)
    {
        int *p = &val;  //函数的输入值，可以取值，所以为右值
        int x = val;
        cout<<(long long)p<<" | ";
    }

    //6.3 右值引用
    class X
    {
    public:
        X() {
            cout<<__func__<<" Struct"<<" | ";
        }
        X(const X&) {
            cout<<__func__<<" Copy Struct"<<" | ";
        }
        X& operator = (const X&){
            cout<<__func__<<" Struct"<<" | ";
            return *this;
        }
        ~X(){
            cout<<__func__<<" Destruct"<<" | ";
        }
        void show()
        {
            cout<<"show X"<<" | ";
        }
    };
    auto make_x = []()->X{ return X();};

    //6.4 右值的性能优化空间
    constexpr auto PoolSize = 4096;
    class MemoryPool
    {
    public:
        MemoryPool(): pool_(new char[PoolSize]){
            cout<<__func__<<" Struct"<<" | ";
        }
        ~MemoryPool(){
            if(pool_ != nullptr)
            {
                cout<<__func__<<" Destruct"<<" | ";
                delete pool_;
                pool_ = nullptr;
            }
        }
        MemoryPool(const MemoryPool& other): pool_(new char [PoolSize]){
            cout<<__func__<<" Copy Struct"<<" | ";
            memcpy(pool_, other.pool_, PoolSize);
        }
        MemoryPool(MemoryPool&& other) noexcept{
            cout<<__func__<<" Move Struct"<<" | ";
            pool_ = other.pool_;
            other.pool_ = nullptr;
        }
        MemoryPool& operator= (const MemoryPool& other){
            cout<<__func__<<" Operator"<<" | ";
            char *memPool = pool_;
            pool_ = new char[PoolSize];
            memcpy(pool_, other.pool_, PoolSize);
            if(memPool != nullptr)
                delete memPool;
            return *this;
        }
        MemoryPool& operator= (MemoryPool &&other) noexcept{
            cout<<__func__<<" Move Operator"<<" | ";
            if(pool_ != nullptr)
            {
                delete pool_;
            }
            pool_ = other.pool_;
            other.pool_ = nullptr;
            return *this;
        }
        bool is_pool_valid(){
            return pool_ != nullptr;
        }
    public:
        int a{0};

    private:
        char *pool_;
    };

    auto make_pool = []()->MemoryPool{
        return MemoryPool();
    };

    //6.6 值类别
    int get_x()
    {
        int x = 1;
        return x;
    }

    //6.8 万能引用和引用折叠
    template<typename T>
    void RunCode(T &m){cout<<"lvalue ref"<<" | "; }

    template<typename T>
    void RunCode(T &&m){cout<<"rvalue ref"<<" | ";}

    template<typename T>
    void RunCode(const T &m){cout<<"const lvalue ref"<<" | "; }

    template<typename T>
    void RunCode(const T &&m){cout<<"const rvalue ref"<<" | "; }

    //6.9 完美转发
    #if __MORE_THAN_CPP14__
    void perfect_forword(auto&& t){
        RunCode(std::forward<decltype(t)>(t));
    };
    #endif

    void test_run()
    {
        FUNCTION_START()

        //6.1 左值和右值
        //左值是指定特定内存地址具有名称的值或对象
        //右值是不指向稳定内存地址的匿名者
        //可通过地址符&来判断左值和右值
        cout<<"\nchapter6.1\n  ";
        int x_1 = 0;        //x_1是左值，0是右值
        int y_1 = 2;
        int z_1 = x_1+y_1;      //x+y的结果是右值
        auto *p_1 = &++x_1;      //左值，返回的是x_1
        x_1++;                  //右值，返回的是x_1的复制
        cout<<z_1<<" | "<<(long long)p_1<<" | "<<x_1<<" | ";
        set_val(6);

        //6.2 左值引用
        //非常量左值的引用必须是左值，常量左值则可以引用左值和右值.
        cout<<"\nchapter6.2\n  ";
        int x_2 = 1;
        int &ref_x_2 = x_2;
        const int &y_2 = 2;
        ref_x_2 = 5;
        cout<<x_2<<" "<<y_2;

        //6.3 右值引用
        //右值引用可以引用右值的方法，可以延长右值的生命周期
        {
            cout<<"\nchapter6.3\n  ";
            int&& x_3 = 1;
            int y_3 = 2;
            int z_3 = 4;
            int&& a_3 = y_3 + z_3;
            cout<<x_3<<" | "<<a_3<<" | ";
            X&& x_obj = make_x();
            x_obj.show();
        }

        //6.4 右值的性能优化空间
        {
            cout<<"\nchapter6.4\n  ";
            MemoryPool pool;                            //MemoryPool Struct
            MemoryPool pool_a = pool;                    //MemoryPool Copy Struct                     1-Obj
        }
        
        //6.5 移动语义
        {
            cout<<"\nchapter6.5\n  ";
            MemoryPool pool;                            //MemoryPool Struct
            pool = make_pool();                         //MemoryPool Struct, operator= Move Operator 
            MemoryPool pool_b(std::move(pool));         //MemoryPool Move Struct                     1-Obj
        }
        
        //6.6 值类别
        //左值(lvalue), 纯右值(rvalue), 将亡值(xvalue)
        //              expression(表达式)
        //    glvalue(泛左值)        rvalue(右值)
        //lvalue(左值)   xvalue(将亡值)     pvalue(纯右值)
        //将亡值
        //1.通过std::move或static_cast<T &&>转换的右值引用
        //2.将纯右值转换为临时对象(C++17后，C++17之前纯左值)
        {
            cout<<"\nchapter6.6\n  ";
            int a_6 = 0;
            int &&ref_a_6 = std::move(a_6); //std::move(a_6) 第一种将亡值
            ref_a_6 = 1;
            cout<<ref_a_6<<" | "<<a_6<<" | ";

            int &&ref_b_6 = get_x();        //get_x()返回的是临时对象，第二种将亡值
            cout<<ref_b_6<<" | ";

        }

        //6.7 将左值转换为右值
        cout<<"\nchapter6.7\n  ";
        int x_7 = 7;
        int &&y_7 = static_cast<int &&>(x_7);
        int &&z_7 = std::move(x_7);
        z_7 = 8;
        cout<<x_7<<" | "<<y_7<<" | "<<z_7<<" | ";

        //6.8 万能引用和引用折叠
        //类模板型  T实际类型   最终类型
        //T&        R           R&
        //T&        R&          R&
        //T&        R&&         R&
        //T&&       R           R&&
        //T&&       R&          R&
        //T&&       R&&         R&&       
        cout<<"\nchapter6.8\n  ";
        int x_8 = 1;
        int &y_8 = x_8;
        int &&z_8 = std::move(x_8);
        RunCode(x_8);
        RunCode(y_8);
        RunCode(z_8);
        RunCode(std::move(x_8));
        RunCode<int &>(x_8);
        cout<<"\n  ";

        //6.9 完美转发
        cout<<"\nchapter6.9\n  ";
        #if __MORE_THAN_CPP14__
        int x_9 = 1;
        int &y_9 = x_9;
        int &&z_9 = std::move(x_9);
        perfect_forword(x_9);
        perfect_forword(y_9);
        perfect_forword(z_9);
        perfect_forword(std::move(x_9));
        perfect_forword<int &>(x_8);
        #else
         __LOWRE_THAN_CPP14_RUN;
        #endif
        FUNCTION_END()
    }

}