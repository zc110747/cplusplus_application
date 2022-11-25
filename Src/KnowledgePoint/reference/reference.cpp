/************************************************************************************
 * reference
 * 参考:https://en.cppreference.com/w/cpp/language/value_category
 * 左值(lvalue):允许取地址，有名字的是左值(大部分情况)
 * 右值：分为将亡值(expiring rvalue)和纯右值(pure rvalue)
 *  纯右值:临时变量，不和对象关联的字面量，值返回的函数返回值，lambda表达式等
 *  将亡值:std::move返回值，T&&右值引用的函数返回值
 * 左值引用:指向左值的引用
 * 右值引用:对右值进行引用
 * 引用折叠
 *   左值引用类型与所有类型组合都是左值引用类型
 *   其它情况下和右值引用组合是右值引用类型
 * 完美转发
 *   通过std::forward传递正确的类型到指定函数
***********************************************************************************/
#include <iostream>
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
    int get_value(){
        int a = 1;
        return a;
    }

    void refer_test(void)
    {
        FUNCTION_START()

         //a是左值，0是右值
        int a = 0;         
        a = a+1;                    //a+1是纯右值pure rlave
        cout<<a<<" | ";

        int &a_lr = a;              //a_r为绑定在左值的引用 左值引用
        a_lr = 2;
        cout<<a_lr<<" "<<a<<" | ";
        cout<<(uint64_t)&a_lr<<" "<<(uint64_t)&a<<" | "; 

        //int &&a_rr = a;           //右值引用不能绑定左值
        int &&a_rr = std::move(a);  //右值引用通过std::move绑定左值，std::move返回是将亡值
        a = 3;
        cout<<a_rr<<" "<<a<<" | ";
        cout<<(uint64_t)&a_rr<<" "<<(uint64_t)&a<<" | "; 

        //get_value返回为右值
        //仅允许const的左值引用绑定
        const int& b = get_value();
        cout<<b<<" | ";
        const bool& bb = false;
        cout<<std::boolalpha<<bb<<" | ";

        //get_value返回为
        int &&b1 = get_value();
        b1 = a;
        cout<<b1<<" | ";

        //右值引用绑定变量，引用变量有地址
        int &&c = 1;
        cout<<(uint64_t)&c<<" "<<c<<" | "; 

        FUNCTION_END()
    }
}

//函数参数
//左值引用，右值引用
namespace FUNCATION
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

    void refer_test(void)
    {
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
        T t1;
        return std::move(t1);
    }

    T get_value(){
        T t;
        return t;
    }

    void refer_test(void)
    {
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
            cout<<a<<" "<<b<<" | ";
        }
        cout<<"\n";

        {
            T t1;
            T t2 = std::move(t1);
        }
        FUNCTION_END()
    }
}

/*
引用折叠规则
TR的类型        声明v的类型       实际v的类型
T&              TR               A&
T&              TR&              A&
T&              TR&&             A&
T&&             TR               A&&
T&&             TR&              A&
T&&             TR&&             A&&
*/
namespace FOLD
{
    template<typename T>
    struct RefCheck{
        constexpr static const char *value = "val";
    };
    
    template< typename T>
    struct RefCheck<T&>{
        constexpr static const char *value = "lval";
    };

    template< typename T>
    struct RefCheck<T&&>{
        constexpr static const char *value = "rval";
    };

    void refer_test(void)
    {
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

        cout<<RefCheck<decltype(a01)>::value<<" | ";
        cout<<RefCheck<decltype(a02)>::value<<" | ";
        cout<<RefCheck<decltype(a03)>::value<<" | ";
        cout<<RefCheck<decltype(a11)>::value<<" | ";
        cout<<RefCheck<decltype(a12)>::value<<" | ";
        cout<<RefCheck<decltype(a13)>::value<<" | ";
        FUNCTION_END()
    }
}

namespace FORWARD
{
    class A{
    };
    
    void foo(A &t){
        cout<<"T left ref"<<" | ";
    }
    void foo(A &&t){
        cout<<"T right ref"<<" | ";
    }

    A get_a(){
        return A();
    }

    template<class T>
    void isForwad(T &&t){
        foo(std::forward<T>(t));
    }

    void refer_test(void)
    {
        FUNCTION_START()

        A t1;
        foo(t1);

        //完美转发
        A& t_lef = t1;
        isForwad(t1);               //t1左值，推导类型A&, std::forward折叠为&
        isForwad(t_lef);            //t_lef左值引用，推导类型A&, std::forward折叠为&
        isForwad(std::move(t1));    //推导类型为A, std::forward折叠为&&，std::move转后为将亡值
        isForwad(get_a());          //推导类型为A, std::forward折叠为&&, get_a返回为纯右值
        
        FUNCTION_END()       
    }
}

namespace REF
{
    void refer_test(void)
    {
        FUNCTION_START()

        int a{1}, b{2}, c{3};
        auto ref_a = std::ref(a);

        ++ref_a;
        cout<<a<<" "<<ref_a<<" | ";
        ++a;
        cout<<a<<" "<<ref_a.get()<<" | ";

        //引用数组
        std::reference_wrapper<int> refs[] = {a, b, c};
        refs[1]++;
        refs[2]+=1;
        for(const auto& val:refs)
        {
            cout<<val<<" ";
        }
        cout<<" | ";

        //const ref引用，可以查看改变，但不允许修改 
        auto ref_a1 = std::cref(a);
        a++;
        cout<<a<<" "<<ref_a1.get()<<" | ";

        FUNCTION_END()  
    }
}

int main(void)
{
    VALUE::refer_test();

    FUNCATION::refer_test();

    MOVE::refer_test();

    FOLD::refer_test();

    FORWARD::refer_test();

    REF::refer_test();
    return 0;
}