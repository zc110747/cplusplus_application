/*
C++提供了几种类型转换操作符，用于在不同类型之间进行转换。

1. RTTI(Run-time type information) - 运行时类型信息
RTTI（Run-Time Type Identification）是C++中的一种机制，用于在运行时获取对象的类型信息。
RTTI主要通过以下两个操作符来实现(typeid和dynamic_cast)。

2. typeid
返回一个std::type_info对象，该对象包含了对象的类型信息。typeid可以用于获取对象的类型名称、比较两个对象的类型是否相同等

3. 隐式转换和显式标识符(explicit)
隐式转换是指在不需要显式类型转换操作符的情况下，编译器自动将一种类型的值转换为另一种类型的值
显式标识符（explicit）是一个关键字，用于修饰类的构造函数。当构造函数被声明为显式时，编译器将禁止隐式转换，即禁止使用单个参数的构造函数进行隐式类型转换

4. dynamic_cast
用于在具有继承关系的类指针或引用之间进行安全的向下转型。
如果转型失败，dynamic_cast会返回nullptr（对于指针）或抛出std::bad_cast异常（对于引用）

5. static_cast
类型转换操作符，用于在编译时进行类型转换。主要转换情况：
- 基本数据类型之间的转换：例如，将 int 转换为 double，或将 float 转换为 int。
- 类层次结构中的转换：
  - 向上转型：将派生类指针或引用转换为基类指针或引用。这是安全的，因为派生类对象包含了基类的所有成员。
  - 向下转型：将基类指针或引用转换为派生类指针或引用。这是不安全的，因为基类对象可能不包含派生类的所有成员。在这种情况下，static_cast 不会进行运行时检查，因此可能会导致未定义行为。
- 将枚举类型转换为整数类型：例如，将枚举值转换为对应的整数值。
- 将指针或引用转换为void类型：这在需要传递通用指针或引用时很有用。
- 将void类型转换为指针或引用：这在需要从通用指针或引用中恢复具体类型时很有用。

6. const_cast
类型转换操作符，用于在编译时去除指针或引用的const或volatile属性。主要转换情况：
- 将const指针或引用转换为非const指针或引用：这在需要将const指针传递给非const实参函数时非常有用。
- 将volatile指针或引用转换为非volatile指针或引用：这在需要将volatile指针传递给非volatile实参函数时非常有用。

7. reinterpret_cast
类型转换操作符，用于在编译时进行任意类型的转换。主要转换情况：
- 将指针或引用转换为不同类型的指针或引用：这在需要将一种类型的指针或引用转换为另一种类型的指针或引用时非常有用。
- 将整数类型转换为指针类型：这在需要将整数类型的值解释为指针时非常有用。
- 将指针类型转换为整数类型：这在需要将指针的值解释为整数时非常有用。
*/
#include <iostream>

using namespace std;

#define FUNCTION_START()  {cout<<__func__<<":\n  ";}{
#define FUNCTION_END()    }{cout<<"\n\n";  }   

namespace CONST_CAST
{
    //const_cast
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
    class A{
    public:
        int val;
    };
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
    //reinterpret_cast类似于c的强制转换，不建议使用
   void typecast_process(void)
    {
        FUNCTION_START()

        int a = 0x34353600;
        auto pb = &a;
        float *pf = reinterpret_cast<decltype(pf)>(pb);
        std::cout<<*pf<<" | ";

        A a1{1};
        B *b1 = reinterpret_cast<B *>(&a1); //可以转换，但不符合应用
        std::cout<<a1.val<<" | ";

        C c1;
        B& d1 = reinterpret_cast<B&>(c1);
        d1.print();                         
        C& c2 = reinterpret_cast<C&>(d1);
        c2.print();

        FUNCTION_END()
    }
}

namespace STATIC_CAST
{
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
            //cout<<pc2->get_a()<<" | ";  
            cout<<pc2->get_b()<<" | ";

            //多重继承，派生类转基类, 工作正常
            B *pb1 = static_cast<B *>(pc);
            cout<<pb1->get_b()<<" | ";

            //多重继承，派生类转基类, 工作正常
            A *pa1 = static_cast<A *>(pc);
            cout<<pa1->get_a()<<"\n  ";
        }

        {
            C c;
            //派生类引用转基类引用
            A& a = static_cast<A &>(c);
            cout<<a.get_a()<<" | ";

            B& b = static_cast<B &>(c);
            cout<<b.get_b()<<" | ";

            C&& d = static_cast<C &&>(c);
            cout<<c.get_a()<<" | ";
            cout<<c.get_b()<<"\n  ";
        }

        //enum转int
        {
            int a = static_cast<int>(E1::first);
            cout<<a<<" | ";
        }
        FUNCTION_END()
    }
}

namespace DYNAMIC_CAST
{
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
                cout<<"not nullptr"<<"\n  ";

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
            cout<<pb->i<<"\n  ";

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

        pl = &l;
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

    STATIC_CAST::typecast_process();

    DYNAMIC_CAST::typecast_process();

    TYPEID::typeid_process();

    EXPLICIT::explicit_process();

    return 0;
}