/*
引用

左值是指那些可以出现在赋值操作符左边的表达式，即可以被取地址的表达式。左值通常有一个持久的内存地址，
并且在表达式结束后仍然存在

右值是指那些只能出现在赋值操作符右边的表达式，即不能被取地址的表达式。右值通常是临时的，表达式结束后就不再存在。
分为将亡值(expiring rvalue)和纯右值(pure rvalue)
- 纯右值: 临时变量，不和对象关联的字面量，值返回的函数返回值，lambda表达式等
- 将亡值: std::move返回值，T&&右值引用的函数返回值

左值引用是对左值的引用，它允许你通过引用修改原始左值。
格式：类型& 引用名 = 左值表达式;

右值引用是对右值的引用，它允许你延长右值的生命周期，并在某些情况下实现移动语义。
格式：类型&& 引用名 = 右值表达式;

移动语义（Move Semantics）是C++11引入的一个重要特性，它允许对象的资源（如内存、文件句柄等）在不进行深拷贝的情况下
从一个对象转移到另一个对象，从而提高程序的性能和效率。
移动语义通过引入右值引用（Rvalue Reference）和移动构造函数（Move Constructor）以及
移动赋值运算符（Move Assignment Operator）来实现。

引用折叠是指在模板推导或类型别名中，当引用类型与其他类型组合时，编译器会根据一定的规则进行类型折叠。
规则：
1. 左值引用类型与所有类型组合都是左值引用类型
2. 右值引用类型与非左值引用组合则是右值引用类型
类模板型       T实际类型        最终类型
T&             R               R&
T&             R&              R&
T&             R&&             R&
T&&            R               R&&
T&&            R&              R&
T&&            R&&             R&&

万能引用（Universal Reference）是C++11中引入的一个概念，它是一种特殊的引用类型，可以绑定到左值或右值。
万能引用通常用于模板函数中，以实现完美转发（Perfect Forwarding）

完美转发是指在模板函数中，通过 std::forward 函数将参数以原始的左值或右值类型传递给另一个函数，从而保留参数的原始属性。
语法: std::forward<T>(t)

std::ref 和 std::cref 是 C++ 标准库中的函数模板，用于创建 std::reference_wrapper 对象，
这些对象可以像引用一样使用，但可以被复制和存储，主要用于和std::bind配合使用
std::ref 函数接受一个左值引用参数，并返回一个 std::reference_wrapper 对象，
该对象可以被复制和存储，并且可以像引用一样使用。
std::cref 函数接受一个常量左值引用参数，并返回一个 std::reference_wrapper 对象，
该对象可以被复制和存储，并且可以像常量引用一样使用。
*/
#include <iostream>
#include <stdint.h>
#include <functional>

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
        T(){
            cout<<"T struct"<<" | ";
        }
        T(T &&t){ //移动构造函数
            cout<<"T move struct"<<" | ";
        }
        ~T(){
            cout<<"T distruct"<<" | ";
        }
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
            std::cout<<"default"<<std::endl;
        } 
        RefClass(const RefClass &ref) {
            std::cout<<"copy"<<std::endl;
        }
        RefClass(const RefClass &&ref) {
            std::cout<<"mov"<<std::endl;
        } 
    };

    RefClass getResult() {
        return RefClass();
    }

    void reference_test(void)
    {
        cout<<"MOVE ";
        FUNCTION_START()

        {
            T t = get_rvalue();
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
            const auto &r1 = getResult();
            const auto &&r2 = getResult();
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