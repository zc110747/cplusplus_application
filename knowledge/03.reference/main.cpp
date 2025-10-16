//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. 左值、右值、引用和右值引用、移动语义
//      2. 引用折叠、万能引用和完美转发
//      3. std::ref、std::cref和std::reference_wrapper
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

// 左值，右值
// 左值引用
// 纯右值和将亡值
namespace REFER_VALUE
{
    int get_value() {
        int a = 1;      //a是左值, 1是右值(纯右值)
        return a;
    }

    void test(void)
    {
        std::cout << "==================== VALUE ====================" << std::endl;

        // a是左值，0是右值(纯右值)
        int a = 0;         
        a = a+1;               
        std::cout << a << " | ";   //1

        // 左值引用
        int &a_lr = a;              
        a_lr = 2;
        std::cout << a_lr << " " << a << " | ";   //2, 2
        std::cout << "addr:" << (uint64_t)&a_lr << " " << (uint64_t)&a << " | ";  

        // 右值引用
        //int &&a_rr = a;           // 右值引用不能绑定左值
        int &&a_rr = std::move(a);  // 右值引用通过std::move绑定左值，std::move返回是将亡值
        a = 3;
        std::cout << a_rr << " " << a << " | ";  //3, 3
        std::cout << "addr:" << (uint64_t)&a_rr << " " << (uint64_t)&a << " | "; 

        // get_value返回值为右值，仅允许绑定const的左值引用
        const int& b = get_value();
        std::cout << b << " | ";
        const bool& bb = false;
        std::cout << std::boolalpha << bb << " | ";

        //右值引用绑定(将亡值)
        int &&b1 = get_value();
        b1 = a;
        std::cout << b1 << " | ";

        //右值引用绑定(纯右值)
        int &&b2 = 1;
        b2 = a;
        std::cout << b2 << std::endl;
    }
}

// 函数参数
// 左值引用，右值引用
namespace REFER_FUNCTION
{
    void lref_val(int& a, int& b)
    {
        a++;
        b++;
        std::cout << (uint64_t)&a << " " << (uint64_t)&b << " | ";
    }

    void rref_val(int&& a, int&& b)
    {
        a++;
        b++;
        //int &&c = a; //error, 函数形参都是左值 
        std::cout << (uint64_t)&a << " " << (uint64_t)&b << " | ";
    }

    void test(void)
    {
        std::cout << "==================== REFER_FUNCTION ====================" << std::endl;

        int v1{1}, v2{2};
        
        std::cout << v1 << " " << v2 << " | ";
        std::cout << (uint64_t)&v1 << " " << (uint64_t)&v2 << " | ";

        lref_val(v1, v2);
        std::cout << v1 << " " << v2 << " | ";

        rref_val(std::move(v1), std::move(v2));
        std::cout << v1 << " " << v2 << std::endl;
    }
}

// 移动语义
// std::move 强制转换成右值
// static_cast<T&&>()
// 移动构造函数用于内部资源的移动
namespace REFER_MOVE
{
    class T
    {
    public:
        /// @brief constructor
        T(int a) {
            *ptr = a;
            std::cout << "T struct" << " | ";
        }

        /// @brief constructor
        T() {
            *ptr = 1;
            std::cout << "T struct" << " | ";
        }

        /// @brief copy constructor
        T(const T &t) {
            // std::make_unique是c++14引入
            ptr.reset(new int(*t.ptr));
            std::cout << "T copy struct" << " | ";
        }

        /// @brief move constructor
        T(T &&t) noexcept
            : ptr{std::move(t.ptr)} { //移动构造函数
            std::cout << "T move struct" << " | ";
        }

        /// @brief operator= copy
        T& operator=(const T &t) { //拷贝赋值函数
            ptr.reset(new int(*t.ptr));
            std::cout << "T copy=" << " | ";
            return *this;
        }

        /// @brief operator= move
        T& operator=(T &&t) { //移动赋值函数
            ptr = std::move(t.ptr);
            std::cout << "T move=" << " | ";
            return *this;
        } 

        /// @brief operator= destruct
        ~T() {
            std::cout << "T distruct:" << " | ";
        }

        void show(void) {
            std::cout << "T show:" << *ptr << " | ";
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
            std::cout << "RefClass default" << std::endl;
        } 
        RefClass(const RefClass &ref) {
            std::cout << "RefClass copy" << std::endl;
        }
        RefClass(const RefClass &&ref) {
            std::cout << "RefClass mov" << std::endl;
        }
        ~RefClass() {
            std::cout << "RefClass distruct" << std::endl;
        }
    };

    RefClass get_result() {
        return RefClass();
    }

    void test(void)
    {
        std::cout << "==================== REFER_MOVE ====================" << std::endl;
        
        {
            T t = get_rvalue();
            
            static T t1;
            t = std::move(t1);
            t.show();
        }
        std::cout << "\n";

        {
            T&& t1 = get_value();
        }
        std::cout << "\n";

        {
            int a = 1;
            int&& b = static_cast<int &&>(a);
            std::cout << (uint64_t)&a << " " << (uint64_t)&b << " | ";
        }
        std::cout << "\n";

        {
            T t1;
            T&& t2 = std::move(t1);
        }
     
        {
            std::cout << "\nresult test!!!!!" << std::endl;
            const auto &r1 = get_result();
            const auto &&r2 = get_result();
        }

        std::cout << std::endl;
    }
}

// 引用折叠
namespace REFER_FOLD
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

    void test(void)
    {
        std::cout << "==================== REFER_FOLD ====================" << std::endl;

        using AL = int&;
        using AR = int&&;

        int a = 1;
        AL a01 = a;
        AL& a02 = a;
        AL&& a03 = a;
        AR a11 = std::move(a);
        AR& a12 = a;
        AR&& a13 = std::move(a);

        std::cout << ref_check<decltype(a)>::value << " | ";      //val
        std::cout << ref_check<decltype(a01)>::value << " | ";    //lval
        std::cout << ref_check<decltype(a02)>::value << " | ";    //lval
        std::cout << ref_check<decltype(a03)>::value << " | ";    //lval
        std::cout << ref_check<decltype(a11)>::value << " | ";    //rval
        std::cout << ref_check<decltype(a12)>::value << " | ";    //lval
        std::cout << ref_check<decltype(a13)>::value << " | ";    //rval

        std::cout << std::endl;
    }
}

//完美转发
namespace REFER_FORWARD
{
    class A{
    };
   
    void foo(A &t){
        std::cout << "T& type" << " | ";
    }
    void foo(A &&t){
        std::cout << "T&& type" << " | ";
    }

    A get_a(){
        return A();
    }

    template<class T>
    void isForwad(T &&t){
        foo(std::forward<T>(t));
    }

    void test(void)
    {
        std::cout << "==================== REFER_FORWARD ====================" << std::endl;
    
        A t1;
        foo(t1);                    // t1为左值，调用左值引用T&

        // 完美转发
        A& t_lef = t1;
        isForwad(t1);               // t1左值，推导类型A&, std::forward折叠为T&
        isForwad(t_lef);            // t_lef左值引用，推导类型A&, std::forward折叠为T&
        isForwad(std::move(t1));    // 推导类型为A, std::forward折叠为T&&，std::move转后为将亡值
        isForwad(get_a());          // 推导类型为A, std::forward折叠为T&&, get_a返回为纯右值
        
        std::cout << std::endl;
    }
}

// std::ref应用
// std::cref
// std::reference_wrapper
namespace REFER_CREF
{
    void func(int &a, int &b)
    {
        a += 1;
        b += 1;
    }

    void test(void)
    {
        std::cout << "==================== REFER_CREF ====================" << std::endl;

        int a{1}, b{2}, c{3};
        auto ref_a = std::ref(a);

        //ref变量
        ++ref_a;
        std::cout << a << " " << ref_a << " | ";
        ++a;
        std::cout << a << " " << ref_a.get() << " | ";

        //ref引用数组
        std::reference_wrapper<int> refs[] = {a, b, c};
        refs[1]++;
        refs[2] += 1;
        for(const auto& val:refs)
        {
            std::cout << val << " ";
        }
        std::cout << " | ";

        //const ref引用，可以查看改变，但不允许修改 
        auto ref_a1 = std::cref(a);
        a++;
        std::cout << a << " " << ref_a1.get() << " | ";

        {
            int a = 1, b = 1;
            int &r1 = a;
            int &r2 = b;

            //引用在bind时不生效，通过std::ref可以生效
            //通过std::ref引用可以被传递
            auto f = bind(func, r1, std::ref(r2));
            f();
            
            std::cout << a << " " << b << " | ";
        }

        std::cout << std::endl; 
    }
}

int main(void)
{
    REFER_VALUE::test();

    REFER_FUNCTION::test();

    REFER_MOVE::test();

    REFER_FOLD::test();

    REFER_FORWARD::test();

    REFER_CREF::test();
    return 0;
}