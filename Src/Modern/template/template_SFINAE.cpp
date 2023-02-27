/******************************************************************
 * 第四十章 SFINAE
 * 在模板重载时，当模板形参替换为指定的实参或由函数实参推导形参
 * 的过程中出现失败，则放弃这个重载而不抛出异常。
 * 标准规定，在直接上下文中使用模板实参替换形参后，类型或者表达式不符合语法，
 * 则替换失败；
 * 而替换后在非直接上下文中产生的副作用导致的错误被当作编译错误。
********************************************************************/
#include <iostream>
#include "template_SFINAE.hpp"

using namespace std;

#define FUNCTION_START()  {cout<<__func__<<":\n  ";}{
#define FUNCTION_END()    }{cout<<"\n\n";  }  

namespace SFINAE_1
{
    template<int i> 
    struct X {};

    char foo(int);
    char foo(float);
    template<class T> 
    X<sizeof(foo((T)0))> f(T)
    {
        return X<sizeof(foo((T)0))>();
    }

    void test(void)
    {
        FUNCTION_START()

        auto val = f(1);
        cout<<typeid(f(1)).name()<<" | ";

        FUNCTION_END()
    }
}

namespace SFINAE_2
{
    int foo(...){
        return 1;
    }

    template<class T>
    T foo(T &t)
    {
        T tt(t);
        return tt;
    }
    
    class bar
    {
    public:
        bar() {};
        bar(bar &)  {};
        bar(bar &&) {};
    };

    void test(void)
    {
        FUNCTION_START()

        double X = 7.1;
        
        //X可以推导为T foo(T &t)
        auto val = foo(X);  
        cout<<typeid(val).name()<<" | ";
        auto vol = foo(5);
        cout<<typeid(vol).name()<<" | ";

        bar b;
        foo(b);
        
        FUNCTION_END()
    }
}

namespace SFINAE_3
{
    struct X{
       X operator+(const X x){
            return x;
       }
    };
    
    struct Y{
        Y(X){}
    };

    template<class T>
    auto foo(T t1, T t2)->decltype(t1 + t2)
    {
        cout<<"foo decltype"<<" | ";
        return t1+t2;
    }

    X foo(Y, Y){
        cout<<"foo"<<" | ";
        return X();
    }

    void test(void)
    {
        FUNCTION_START()

        X x1, x2;
        foo(x1, x2);
        foo(1, 2);

        Y y1(x1), y2(x2);
        foo(y1, y2);
        
        FUNCTION_END()    
    }
}
void template_sfinae(void)
{
    FUNCTION_START()

    SFINAE_1::test();

    SFINAE_2::test();

    SFINAE_3::test();

    FUNCTION_END()
}