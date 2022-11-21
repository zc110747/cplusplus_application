/**********************************************************
 * 说明: RAII_test.cpp
 * 参考: https://cplusplus.com/doc/tutorial/typecasting/
 * RAII: Run-time type information
 * implicit convert
 * Keyword explicit 用于禁止带参数的类进行隐式的类型转换
 * typeid - 获取类型信息type_info
 * 四种转换方式
 * const_cast
 * reinterpret_cast
 * static_cast
 * dynamic_cast
************************************************************/

#include <iostream>

using namespace std;

#define FUNCTION_START()  {cout<<__func__<<":\n  ";}{
#define FUNCTION_END()    }{cout<<"\n\n";  }   

namespace CONST_CAST
{
    //const_cast
    //const_cast用于设置和删除指针的const或volatile，可用于将const指针传递给非const实参函数
    //除非完全了解需求，不然不建议这么做
    void typecast_process(void)
    {
        FUNCTION_START()

        const volatile int a = 1;   //加volatile可以保证a不被优化，这样再后续a的地方读值为2
        auto *pa = const_cast<int *>(&a);
        *pa = 2;
        std::cout<<a<<" "<<*pa<<" | "; //修改const指针是ub特性，

        int i = 1;
        const int b = i;
        auto *pb = const_cast<int *>(&b);
        *pb = 2;
        std::cout<<b<<" "<<*pb<<" | ";

        auto& c = const_cast<int&>(b);
        c = 3;
        std::cout<<b<<" "<<c<<" | ";

        FUNCTION_END()
    }
}


namespace REINTERPRET_CAST
{
    class A{};
    class B{
    public:
        void print(){
            cout<<"B print"<<" | ";
        }
    };
    class C:public B{
    public:
        void print(){
            cout<<"C print"<<" | ";
        }
    };

    //reinterpret_cast
    //reinterpret_cast用于任意类型类的转换，包含非继承类
    //reinterpret_cast类似于c的强制转换，不建议使用
    void typecast_process(void)
    {
        FUNCTION_START()

        int a = 0x34353600;
        auto pb = &a;
        float *pf = reinterpret_cast<decltype(pf)>(pb);
        std::cout<<*pf<<" | ";

        A a1;
        B *b1 = reinterpret_cast<B *>(&a1); //可以转换，但不符合应用

        C c1;
        B& d1 = reinterpret_cast<B&>(c1);
        d1.print();                         
        C& c2 = reinterpret_cast<C&>(d1);
        c2.print();

        FUNCTION_END()
    }
}

namespace DYNAMIC_CAST
{
    /*
        dynamic_cast:动态转换用类的指针或者引用转换,只适用于多态类(包含virtual任意函数即可) 
        否则会报(source type is not polymorphic)错误
        1.支持派生类转基类，类似于隐式转换(upcast)
        2.支持基类转派生类，转换失败返回nullptr(downcast)
        dynamic_cast编译器需要支持RTTI(g++ -fno-rtti可关闭RTTI)
    */

    class Base
    {
    public:
        int i = 1;
        Base() {}
        virtual ~Base()= default;
    };

    class BaseE
    {    
    public:
        int j = 2;
        BaseE() {}
        virtual ~BaseE()= default;
    };

    class Derived: public Base, public BaseE
    {
    public:
        int k = 3;
        void f(){
            std::cout<<k<<" | ";
        }
    };

    void typecast_process(void)
    {
        FUNCTION_START()
        {
            Base *pba = new Base;
            Base *pbb = new Derived;
            Derived *pbc;

            //基类转派生类，不具有完整的派生类类型，返回nullptr
            pbc = dynamic_cast<Derived *>(pba);
            if(pbc == nullptr)
                cout<<"nullptr"<<" | ";

            //基类转派生类，具有完整的派生类类型，返回实际类型
            pbc = dynamic_cast<Derived *>(pbb);
            if(pbc != nullptr)
                cout<<"not nullptr"<<" | ";

            delete pba;
            delete pbb;
        }

        {
            //派生类转基类，多继承
            BaseE *pE = dynamic_cast<BaseE *>(new Derived());
            Derived *pd = dynamic_cast<Derived *>(pE);
            Base *pb = dynamic_cast<Base *>(pd);
            cout<<pE->j<<" | ";
            pd->f();
            cout<<pb->i<<" | ";

            delete pE;
        }

        {
            //引用的派生类和基类互转
            Derived d;
            Base& pb = dynamic_cast<Base&>(d);
            Derived& d1 = dynamic_cast<Derived&>(pb);
            cout<<pb.i<<" | ";
            cout<<d1.i<<" | ";
        }
        FUNCTION_END()
    }
}

namespace STATIC_CAST
{
    /*
        static_cast:静态转换用于类的指针或者引用的转换，需要为有继承关系的类
        仅再编译阶段检查，需要程序员保证转换的有效性，不过不会产生动态转换的开销
        基类转派生类需要
    */

    class A
    {
    public:
        int a = 1;
        int get_a(){
            return a;
        }
    };

    class B{
    public:
        int b = 2;
        virtual int get_b(){
            return b;
        }
    };

    class C:public A, public B
    {
    public:
        int c = 3;
        virtual int get_c(){
            return c;
        }
    };

    enum E1
    {
        first = 5,
    };

    void typecast_process(void)
    {
        FUNCTION_START()
        
        {
            A *pa = new A;
            B *pb = new B;
            C *pc = new C;

            //多重继承，基类转派生类
            //其中A布局正常，B的布局无法识别
            C *pc1 = static_cast<C *>(pa);
            cout<<pc1->get_a()<<" | ";
            //cout<<pc1->get_b()<<" | ";
           
           //其中a的值不正确，B的布局识别正常
            C *pc2 = static_cast<C *>(pb);
            cout<<pc2->get_a()<<" | ";  
            cout<<pc2->get_b()<<" | ";

            //多重继承，派生类转基类, 工作正常
            B *pb1 = static_cast<B *>(pc);
            cout<<pb1->get_b()<<" | ";

            A *pa1 = static_cast<A *>(pc);
            cout<<pa1->get_a()<<" | ";
        }

        {
            C c;
            //派生类引用转基类引用
            A& a = static_cast<A &>(c);
            cout<<a.get_a()<<" | ";

            B& b = static_cast<B &>(c);
            cout<<b.get_b()<<" | ";
        }

        //enum转int
        {
            int a = static_cast<int>(E1::first);
            cout<<a<<" | ";
        }
        FUNCTION_END()
    }
}

namespace TYPEID
{
    /*
    typeid 返回const的类型type_info
    type_info支持操作
    operator==, operator!=重载的等于和不等于运算符
    before 用于比较类型被标识的先后顺序，之前返回true，其它返回false
    name 返回标识类型的字符串序列
    hash_code 返回识别类型的hash值
    */
    class A{};
    class B{};
    class C:public A, public B 
    {};

    void typeid_process()
    {
        FUNCTION_START()

        A a, a1;
        cout<<typeid(a).name()<<" "<<typeid(a).hash_code()<<" | ";
        cout<<typeid(a1).name()<<" "<<typeid(a1).hash_code()<<" | ";
        if(typeid(a) == typeid(a1))
            cout<<"type equal"<<"\n  ";

        B *pb;
        cout<<typeid(pb).name()<<" "<<typeid(pb).hash_code()<<"\n  ";

        C *pc = new C;
        B *pb1 = static_cast<B *>(pc);
        cout<<typeid(pb1).name()<<" "<<typeid(pb1).hash_code()<<" | ";
        cout<<typeid(pc).name()<<" "<<typeid(pc).hash_code()<<"\n  ";

        int l, *pl;
        cout<<typeid(l).name()<<" "<<typeid(l).hash_code()<<" | ";
        cout<<typeid(*pl).name()<<" "<<typeid(*pl).hash_code()<<" | ";
        cout<<typeid(pl).name()<<" "<<typeid(pl).hash_code()<<"\n  ";

        cout<<std::boolalpha<<typeid(A).before(typeid(B))<<" | ";
        cout<<std::boolalpha<<typeid(int).before(typeid(int))<<" | ";
        FUNCTION_END()
    }
}

namespace EXPLICIT
{   
    /*
    explict 修饰类的构造函数(作用于单个参数的构造函数，如果后续参数有默认值，也可以防止隐式转换)
    */
    class A{
    public:
        explicit A(int i):_i(i), _j(0){
        }
        A(int i, int j):_i(i), _j(j){
        }
        int _i;
        int _j;
    };

    class C:public A{
    };

    void func_print(A a)
    {
        cout<<"A:"<<a._i<<" "<<a._j<<" | ";
    }

    void func_print(C c)
    {
        cout<<"C:"<<c._i<<" "<<c._j<<" | ";
    }

    void explicit_process(void)
    {
        FUNCTION_START()

        A a1(1, 2);
        func_print(a1);

        A a2(1);
        func_print(static_cast<C&>(a2));

        //无法进行隐式的类型转换
        //func_print({1}); //explict 
        func_print({1, 2});
        FUNCTION_END();
    }
}

int main(void)
{
    CONST_CAST::typecast_process();

    REINTERPRET_CAST::typecast_process();

    DYNAMIC_CAST::typecast_process();

    STATIC_CAST::typecast_process();

    TYPEID::typeid_process();

    EXPLICIT::explicit_process();
}