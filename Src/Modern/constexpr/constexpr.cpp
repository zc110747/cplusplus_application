/******************************************************************
 * constexpr
 * 常量表达式要求在编译期必须有准确得值
 * if constexpr (表达式) {
 *    //...
 * }
 * C++20 引入consteval和constinit
********************************************************************/
#include "constexpr.hpp"
#include <iostream>
#include <type_traits>

using namespace std;

#define FUNCTION_START()  {cout<<__func__<<":\n  ";}{
#define FUNCTION_END()    }{cout<<"\n\n";  }   

constexpr int x_size = 16;

constexpr int const_size()
{
    return x_size;
}

constexpr uint32_t factorial(uint32_t a)
{
    return a > 0?factorial(a-1)*a:1;
}

constexpr double sum(double x)
{
    return x>0?(sum(x-1)+x):0;
}
static void constexpr_temp_process(void);

int constexpr_process(void)
{
    FUNCTION_START()

    int arr[x_size] = {1, 2, 3};
    cout<<"data:"<<sizeof(arr)/sizeof(int)<<" | ";

    //constexpr函数
    auto a = const_size();
    cout<<"function:"<<a<<" | ";

    //constexpr lambda函数
    constexpr auto x = []()constexpr->auto{
        return const_size();
    }();
    int arr_x[x];
    cout<<"lambda:"<<sizeof(arr_x)/sizeof(int)<<" | ";

    constexpr auto val = factorial(4);
    cout<<"function:"<<val<<" | ";

    int arr_f[factorial(2)];
    cout<<"function:"<<sizeof(arr_f)/sizeof(int)<<" | ";

    //枚举使用constexpr
    {
        typedef enum{
            f0 = factorial(1),
            f1 = factorial(2),
            f2 = factorial(3),
            f3 = factorial(4),
        }const_enum;
        cout<<const_enum::f0<<" "<<const_enum::f1<<" ";
        cout<<const_enum::f2<<" "<<const_enum::f3<<" | ";
    }

    //constepr浮点
    {
        constexpr auto x = sum(5.1);
        cout<<"float:"<<x<<" | ";
    }

    //constexpr structure
    {
        struct X{
            int x1;
        };
        constexpr X x = {2};
        int arr[x.x1];
        cout<<"struct:"<<sizeof(arr)/sizeof(int)<<" | ";
    }

    //constexpr class
    {
        //析构函数是平凡的
        //1.自定义类型中不能有用户自定义的析构函数
        //2.析构函数不能是虚函数
        //3.基类和成员的析构函数必须是平凡的
        class X{
            public:
                constexpr X():x1(5){}
                constexpr X(int i): x1(i){}
                constexpr int get() const {
                    return x1;
                }
                constexpr void set(int x){
                    x1 = x;
                }
            private:
                int x1;
        };
        constexpr X x{3};
        int arr[x.get()];
        cout<<"class:"<<sizeof(arr)/sizeof(int)<<" | ";
    }
    
    constexpr_temp_process();

    FUNCTION_END()
    return 0;
}

template<class T> bool is_same_value(T a, T b){
    return a == b;
}

template<> bool is_same_value<double>(double a, double b){
    if(abs(a - b) < 0.0001){
        return true;
    }else{
        return false;
    }
}

template<class T> bool is_same_value_ext(T a, T b){
    if constexpr (std::is_same<T, double>::value) {
        if(std::abs(a - b) < 0.0001){
            return true;
        }else{
            return false;
        }
    }
    else { 
        return a == b;
    }
}

template<class T> auto any2i(T t)
{
    if constexpr (std::is_same<T, string>::value) {
        if constexpr (T::npos == -1){
            return aoti(t.cstr());
        }
    } else {
        return t;
    }
}

static void constexpr_temp_process(void)
{
    cout<<std::boolalpha<<is_same_value(0.1, 0.100001)<<" | ";
    cout<<std::boolalpha<<is_same_value_ext(0.1, 0.100001)<<" | ";
    cout<<std::boolalpha<<is_same_value_ext(0.1, 0.2)<<" | ";
    
    cout<<any2i("6")<<" | ";
    cout<<any2i(6)<<" | ";
}