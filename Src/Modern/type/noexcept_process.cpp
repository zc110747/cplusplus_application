/******************************************************************
 * 1.noexcept
********************************************************************/
#include "noexcept_process.hpp"
#include <iostream>

#define FUNCTION_START()  {cout<<__func__<<":\n  ";}{
#define FUNCTION_END()    }{cout<<"\n\n";  }  

using namespace std;

//noexcept 函数不抛出异常
int safe_func() noexcept
{
    return 1;
}

template<class T>
class X
{
private:
    T x;
};

//
auto return_func() noexcept(true)
{
    X<int> *ptr = nullptr;
    try
    {
        ptr = new X<int>;
    }
    catch(const std::exception& e)
    {
       //throw(e.what()); //增加noexcept的函数，如果有throw, 将会terminate应用, 而不是抛出异常
    }
    return ptr;
}

template<class T>
T func_ret(T &&t) noexcept(is_fundamental_v<T>)
{
    return t;
}

template<class T>
T func_ret1(T &&t) noexcept(noexcept(T(t)))
{
    return t;
}

int noexcept_process(void)
{
    FUNCTION_START()

    cout<<boolalpha<<noexcept(safe_func())<<" | ";
    cout<<boolalpha<<noexcept(return_func())<<" | ";
    cout<<boolalpha<<noexcept(func_ret(1))<<" | ";
    cout<<boolalpha<<noexcept(func_ret(X<int>()))<<" | "; 
    cout<<boolalpha<<noexcept(func_ret1(1))<<" | "; 
    
    FUNCTION_END() 

    return 0;
}