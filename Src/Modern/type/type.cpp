/******************************************************************
 * 1.auto占位符
 * 2.decltype说明符
 * 3.函数返回类型后置 
 * 4.强枚举类型
********************************************************************/
#include "type.hpp"
#include <iostream>

using namespace std;

#define FUNCTION_START()  {cout<<__func__<<":\n  ";}{
#define FUNCTION_END()    }{cout<<"\n\n";  }  

static void auto_process(void);
static void decltype_process(void);

//强枚举类型
enum class HighSchool : unsigned int
{
    student = 1,
    teacher,
    principal,
};

enum class University: unsigned int
{
    student = 1,
    teacher,
    principal,
};

int type_process(void)
{
    auto_process();

    decltype_process();
    return 0;
}

//函数返回类型支持推导
auto func(int a, int b){
    return a+b;
}

template<auto N>
void f()
{
    cout<<N<<" | ";
}

//函数返回类型后置
template<typename T>
auto PrintEnum(T const &value)->typename std::underlying_type<T>::type{
    return static_cast<typename std::underlying_type<T>::type>(value);
}

/*********************************************************************************
1.auto推导规则
    (1)如果auto声明的变量是按值初始化，忽略CV限定符(const/volatile),
    但auto本身支持CV限定符, 引用初始化则保留
    (2)如果声明变量初始化时，目标对象是引用，引用属性被忽略
    (3)使用auto和万能引用声明变量时，对于左值会将auto推导为引用类型
    (4)使用auto声明变量，如果目标是数组或者函数，auto推导为对应的指针类型
    (5)直接使用列表初始化，必须为单元素，用等号加列表初始化，支持多个元素，
    但类型必须相同std::initializer_list<T>
**********************************************************************************/
static void auto_process(void)
{
    FUNCTION_START()

    //值初始化，忽略CV限定符
    {
        const int cx = 5;
        auto cy = cx;           //"int"
        volatile int vx = 5;
        auto vy = vx;           //"int"
        const auto ccy = cx;    //"int", with const
        auto &rcy = cx;         //"const int"  
        auto *pcy = &cx;        //"const int"     
    }

    //目标是引用，引用被忽略
    {
        int x = 5;
        int &rx = x;
        auto rrx = rx;         //int
    }

    //万能引用和引用折叠
    {
        int x = 5;
        auto &&rx = 5;          //"int"
        auto &&ry = x;          //"int &"
    }

    //数组，函数对应的指针类型，lambda直接为函数类型
    {
        int arr[5];

        auto pa = arr;          //"int *"
        auto pf = func;         //int (*pf)(int, int)
        auto pl = [](int a, int b){
            return a+b;
        };                      //lambda
    }

    {
        auto arr = {1, 2};      //std::initialize_list<int> 
        auto val1{3};
    }

    {
        class Base
        {
        public:
            virtual void f(){
                cout<<"Base::f()"<<" | ";
            }
        };

        class Derived: public Base
        {
        public:
            virtual void f(){
                cout<<"Base::Derived()"<<" | ";
            }
        };

        Base *d = new Derived();
        auto b = *d;
        b.f();
        cout<<typeid(b).name()<<" | ";

        auto &c = *d;
        c.f();
        cout<<typeid(c).name()<<" | ";

        delete d;
        d = nullptr;
    }

    //lambda泛型
    {
        auto f = [](auto a, auto b){
            return a*b;
        };
        cout<<f(1, 3.5)<<" | ";
    }

    f<3>();
    f<'c'>();

    {
        auto x = HighSchool::teacher;
        auto y = University::principal;
        auto z{1};
        //auto b = (x == y);
        cout<<"x="<<PrintEnum(x)<<",y="<<PrintEnum(y)<<" | ";

        auto &a = x;
        auto &&s = HighSchool::teacher;
        auto PrintEnumPerfect = [](auto &&x)->auto{
            return static_cast<typename std::underlying_type_t<typename std::remove_reference_t<decltype(x)>>>(std::forward<decltype(x)>(x));
        };
        cout<<PrintEnumPerfect(x)<<" | ";
        cout<<PrintEnumPerfect(a)<<" | ";
        cout<<PrintEnumPerfect(s)<<" | ";
    }
    FUNCTION_END() 
}

/***********************************************************************************
decltype推导规则(decltype(e))
    (1) 如果e是一个未加括号的标识符表达式（结构化绑定除外）或者未加括号的类成员访问，
        则decltype(e)推断出的类型是e的类型T
    (2）如果e是一个函数调用或者仿函数调用，那么decltype(e)推断出的类型是其返回值的类型
    (3）如果e是一个类型为T的左值，则decltype(e)是T&
    (4) 如果e是一个类型为T的将亡值，则decltype(e)是T&&
    (5) 除去以上情况，则decltype(e)是T 
**********************************************************************************/
struct S1
{
    int x;
    decltype(x) y;
    decltype(x+y) z;
};

template<class T1, class T2>
auto sum(T1 a1, T2 a2)->decltype(a1+a2)
{
    return (a1+a2);
}

template<class T>
auto return_ref(T& t)->decltype(t)
{
    return t;
}

const int &&foo();
static void decltype_process(void)
{
    FUNCTION_START()

    int x1 = 1;
    decltype(x1) x2 = 0;                //获取对象类型
    cout<<typeid(x2).name()<<" | ";     

    decltype(x1+x2) x3;                 //获取表达式类型
    cout<<typeid(x3).name()<<" | "; 

    S1 s;
    cout<<typeid(s).name()<<" | ";      

    auto f = [x1](decltype(x1) a, decltype(x1+x2) b){
        return x1+a+b;
    };
    cout<<f(1, 2)<<" | ";

    cout<<sum(1, 2.3)<<" | ";

    int ra = 0;
    cout<<std::boolalpha<<std::is_reference_v<decltype(return_ref(ra))><<" | ";

    decltype(foo()) rra = 5;               //函数的返回类型
    cout<<typeid(rra).name()<<" | ";   

    decltype((ra)) r1a = x1;                //左值,带括号，int& 
    cout<<typeid(r1a).name()<<" | ";   

    const int i = 1;
    decltype(i) v = 2;
    cout<<typeid(v).name()<<" | ";         

    FUNCTION_END() 
}