/******************************************************************
 * 第34章 基础特性的其它优化
********************************************************************/
#include <iostream>
#include <vector>
#include <algorithm>
#include <new>

using namespace std;

//显示自定义类型转换符
namespace EXPLICIT_TYPE
{
    template<class T>
    class Storage{
    public:
        Storage()=default;
        Storage(std::initializer_list<T> l): data_(l){
        }
        explicit operator bool() const {return !data_.empty();}
    private:
        vector<T> data_;
    };

    struct X{
        const int n;
    };
    union U{
        X x;
        float f;
    };

    #define FUNCTION_START()  {cout<<__func__<<":\n  ";}{
    #define FUNCTION_END()    }{cout<<"\n\n";  } 

    void process()
    {
        FUNCTION_START()

        Storage<float> s1{1., 2., 3.};
        Storage<int> s2{1, 2, 3};

        cout<<boolalpha;
        // cout<<(s1 == s2)<<" | ";
        // cout<<s1 + s2;
        cout<<static_cast<bool>(s1)<<" | ";
        cout<<static_cast<bool>(s2)<<" | ";

        //在某些期待上下文为bool类型的语境中，即使转换声明为显示，
        //也可以隐式执行布尔转换
        //if、while、for的控制表达式
        //内建逻辑运算符!、&&和||操作符
        //条件运算符的首个操作数?:
        //static_assert中的bool常量表达式
        //noexpect说明符中的表达式
        if(s1)
        {
            cout<<"s1 not empty!"<<" | ";
        }

        //如果新的对象在已被某个对象占用的内存上构建，那么原始对象
        //的指针、引用以及对象名会自动转向新的对象，除非对象是常量
        //类型或对象中有常量数据成员或者引用类型
        U u1 = {{1}};
        cout<<u1.x.n<<" | ";
        X *p = new(&u1.x) X{2};
        cout<<p->n<<" | ";
        cout<<u1.x.n<<" | ";

        //防止编译器追踪到数据的来源以阻止编译器对数据进行优化
        cout<<*std::launder(&u1.x.n)<<" | ";

        FUNCTION_END() 
    }
}

namespace RVO_PROCESS
{
    class X
    {
    public:
        X() {cout<<"x construct"<<" | ";}
        X(const X& x) {cout<<"x copy construct"<<" | ";}
        ~X() {cout<<"x destory"<<" | ";}
    };

    X make_x(){
        X x1;
        return x1;
    }

    //返回值
    void process()
    {
        FUNCTION_START()

        X x1 = make_x();

        FUNCTION_END() 
    }
}

namespace NEW_INFER
{
    void process()
    {
        FUNCTION_START()

        int x[]{1, 2, 3};
        char s[]{"hello world"};
        cout<<s<<" | ";
        int *px = new int[3]{1, 2, 3};

        //数组转换为已知范围的数组
        //指向数组的指针声明
        int arr[2] = {1, 2};
        int (&r)[2] = arr;
        cout<<r[0]<<" | ";
        r[1] = 4;
        cout<<arr[1]<<" | ";

        delete[] px;

        FUNCTION_END()         
    }
}

namespace CLASS_IMPROVE
{
    struct X{
        X() {}
        ~X() {
            std::cout<<"X destory"<<" | ";
        }
        
        void operator delete(void *ptr)
        {
            std::cout<<"call delete"<<" | ";
            ::operator delete(ptr);
        }
    };

    void process(void)
    {
        FUNCTION_START()

        X *px = new X;
        delete px;

        FUNCTION_END()  
    }
}

int other_improve_process(void)
{
    EXPLICIT_TYPE::process();
    
    RVO_PROCESS::process();

    NEW_INFER::process();

    CLASS_IMPROVE::process();
    return 0;
}