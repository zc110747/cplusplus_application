//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//     1. 左值，右值和引用
//     2. 引用折叠
//     3. std::ref和std::cref
//
// Author:
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <stdint.h>
#include <functional>
#include <memory>

using namespace std;

/*
FUNCTION_START()
FUNCTION_END()
*/
#define FUNCTION_START()  {cout<<__func__<<":\n";}{
#define FUNCTION_END()    }{cout<<"\n\n";  }   

//左值，右值
//左值引用
//纯右值和将亡值
namespace VALUE
{
    int get_value() 
    {
        int a = 1;
        return a;
    }

    void reference_test(void)
    {
        cout<<"VALUE ";

        FUNCTION_START()

        //a是左值，0是右值(纯右值)
        int a = 0;         
        a = a+1;               
        cout<<a<<" | ";   //1

        //左值引用
        int &a_lr = a;              
        a_lr = 2;
        cout<<a_lr<<" "<<a<<" | ";   //2, 2
        cout<<"addr:"<<(uint64_t)&a_lr<<" "<<(uint64_t)&a<<" | ";  

        //右值引用
        //int &&a_rr = a;           //右值引用不能绑定左值
        int &&a_rr = std::move(a);  //右值引用通过std::move绑定左值，std::move返回是将亡值
        a = 3;
        cout<<a_rr<<" "<<a<<" | ";  //3, 3
        cout<<"addr:"<<(uint64_t)&a_rr<<" "<<(uint64_t)&a<<" | "; 

        //get_value返回值为右值，仅允许绑定const的左值引用
        const int& b = get_value();
        cout<<b<<" | ";
        const bool& bb = false;
        cout<<std::boolalpha<<bb<<" | ";

        //右值引用绑定(将亡值)
        int &&b1 = get_value();
        b1 = a;
        cout<<b1<<" | ";

        //右值引用绑定(纯右值)
        int &&b2 = 1;
        b2 = a;
        cout<<b2<<" | ";

        FUNCTION_END()
    }
}

//函数参数
//左值引用，右值引用
namespace FUNCTION
{
    void lref_val(int& a, int& b)
    {
        a++;
        b++;
        cout<<(uint64_t)&a<<" "<<(uint64_t)&b<<" | ";
    }

    void rref_val(int&& a, int&& b)
    {
        a++;
        b++;
        //int &&c = a; //error, 函数形参都是左值 
        cout<<(uint64_t)&a<<" "<<(uint64_t)&b<<" | ";
    }

    void reference_test(void)
    {
        cout<<"FUNCTION ";

        FUNCTION_START()

        int v1{1}, v2{2};
        
        cout<<v1<<" "<<v2<<" | ";
        cout<<(uint64_t)&v1<<" "<<(uint64_t)&v2<<" | ";

        lref_val(v1, v2);
        cout<<v1<<" "<<v2<<" | ";

        rref_val(std::move(v1), std::move(v2));
        cout<<v1<<" "<<v2<<" | ";

        FUNCTION_END()
    }
}

//移动语义
//std::move 强制转换成右值
//static_cast<T&&>()
//移动构造函数用于内部资源的移动
namespace MOVE
{
    class T
    {
    public:
        /// @brief constructor
        T(int a) {
            *ptr = a;
            cout<<"T struct"<<" | ";
        }

        /// @brief constructor
        T() {
            *ptr = 1;
            cout<<"T struct"<<" | ";
        }

        /// @brief copy constructor
        T(const T &t) {
            //std::make_unique是c++14引入
            ptr.reset(new int(*t.ptr));
            cout<<"T copy struct"<<" | ";
        }

        /// @brief move constructor
        T(T &&t) noexcept
            : ptr{std::move(t.ptr)} { //移动构造函数
            cout<<"T move struct"<<" | ";
        }

        /// @brief operator= copy
        T& operator=(const T &t) { //拷贝赋值函数
            ptr.reset(new int(*t.ptr));
            cout<<"T copy="<<" | ";
            return *this;
        }

        /// @brief operator= move
        T& operator=(T &&t) { //移动赋值函数
            ptr = std::move(t.ptr);
            cout<<"T move="<<" | ";
            return *this;
        } 

        /// @brief operator= destruct
        ~T() {
            cout<<"T distruct:"<<" | ";
        }

        void show(void) {
            cout<<"T show:"<<*ptr<<" | ";
        }
    private:
        std::unique_ptr<int> ptr{new int};
    };

    T &&get_rvalue(){
        static T t1;
        return std::move(t1);
    }

    T get_value(){
        T t;
        return t;
    }

    class RefClass 
    {
    public:
        RefClass() {
            std::cout<<"RefClass default"<<std::endl;
        } 
        RefClass(const RefClass &ref) {
            std::cout<<"RefClass copy"<<std::endl;
        }
        RefClass(const RefClass &&ref) {
            std::cout<<"RefClass mov"<<std::endl;
        }
        ~RefClass() {
            std::cout<<"RefClass distruct"<<std::endl;
        }
    };

    RefClass get_result() {
        return RefClass();
    }

    void reference_test(void)
    {
        cout<<"MOVE ";
        FUNCTION_START()

        {
            T t = get_rvalue();
            
            static T t1;
            t = std::move(t1);
            t.show();
        }
        cout<<"\n";

        {
            T&& t1 = get_value();
        }
        cout<<"\n";

        {
            int a = 1;
            int&& b = static_cast<int &&>(a);
            cout<<(uint64_t)&a<<" "<<(uint64_t)&b<<" | ";
        }
        cout<<"\n";

        {
            T t1;
            T&& t2 = std::move(t1);
        }
     
        {
            std::cout<<"\nresult test!!!!!"<<std::endl;
            const auto &r1 = get_result();
            const auto &&r2 = get_result();
        }

        FUNCTION_END()
    }
}

//引用折叠
namespace FOLD
{
    template<typename T>
    struct ref_check{
        constexpr static const char *value = "val";
    };
    
    template< typename T>
    struct ref_check<T&>{
        constexpr static const char *value = "lval";
    };

    template< typename T>
    struct ref_check<T&&>{
        constexpr static const char *value = "rval";
    };

    void reference_test(void)
    {
        cout<<"FOLD ";
        FUNCTION_START()

        using AL = int&;
        using AR = int&&;

        int a = 1;
        AL a01 = a;
        AL& a02 = a;
        AL&& a03 = a;
        AR a11 = std::move(a);
        AR& a12 = a;
        AR&& a13 = std::move(a);

        cout<<ref_check<decltype(a)>::value<<" | ";      //val
        cout<<ref_check<decltype(a01)>::value<<" | ";    //lval
        cout<<ref_check<decltype(a02)>::value<<" | ";    //lval
        cout<<ref_check<decltype(a03)>::value<<" | ";    //lval
        cout<<ref_check<decltype(a11)>::value<<" | ";    //rval
        cout<<ref_check<decltype(a12)>::value<<" | ";    //lval
        cout<<ref_check<decltype(a13)>::value<<" | ";    //rval
        FUNCTION_END()
    }
}

//完美转发
namespace FORWARD
{
    class A{
    };
   
    void foo(A &t){
        cout<<"T& type"<<" | ";
    }
    void foo(A &&t){
        cout<<"T&& type"<<" | ";
    }

    A get_a(){
        return A();
    }

    template<class T>
    void isForwad(T &&t){
        foo(std::forward<T>(t));
    }

    void reference_test(void)
    {
        cout<<"FORWARD ";
        FUNCTION_START()

        A t1;
        foo(t1);                    //t1为左值，调用左值引用T&

        //完美转发
        A& t_lef = t1;
        isForwad(t1);               //t1左值，推导类型A&, std::forward折叠为T&
        isForwad(t_lef);            //t_lef左值引用，推导类型A&, std::forward折叠为T&
        isForwad(std::move(t1));    //推导类型为A, std::forward折叠为T&&，std::move转后为将亡值
        isForwad(get_a());          //推导类型为A, std::forward折叠为T&&, get_a返回为纯右值
        
        FUNCTION_END()       
    }
}

//std::ref应用
namespace REF
{
    void func(int &a, int &b)
    {
        a += 1;
        b += 1;
    }

    void reference_test(void)
    {
        cout<<"REF ";
        FUNCTION_START()

        int a{1}, b{2}, c{3};
        auto ref_a = std::ref(a);

        //ref变量
        ++ref_a;
        cout<<a<<" "<<ref_a<<" | ";
        ++a;
        cout<<a<<" "<<ref_a.get()<<" | ";

        //ref引用数组
        std::reference_wrapper<int> refs[] = {a, b, c};
        refs[1]++;
        refs[2] += 1;
        for(const auto& val:refs)
        {
            cout<<val<<" ";
        }
        cout<<" | ";

        //const ref引用，可以查看改变，但不允许修改 
        auto ref_a1 = std::cref(a);
        a++;
        cout<<a<<" "<<ref_a1.get()<<" | ";

        {
            int a = 1, b = 1;
            int &r1 = a;
            int &r2 = b;

            //引用在bind时不生效，通过std::ref可以生效
            //通过std::ref引用可以被传递
            auto f = bind(func, r1, std::ref(r2));
            f();
            
            cout<<a<<" "<<b<<" | ";
        }

        FUNCTION_END()  
    }
}

int main(void)
{
    VALUE::reference_test();

    FUNCTION::reference_test();

    MOVE::reference_test();

    FOLD::reference_test();

    FORWARD::reference_test();

    REF::reference_test();
    return 0;
}