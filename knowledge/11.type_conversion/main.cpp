<<<<<<< HEAD
//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. const_cast
//      2. reinterpret_cast
//      3. static_cast
//      4. dynamic_cast
//      5. typeid和RTTI
//      6. explicit 显示标识符
//
// Author:
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>

namespace CONST_CAST
{
    //const_cast
    int test(void)
    {
        std::cout << "============= const_cast =============" << std::endl;

        const volatile int a = 1;           //加volatile可以保证a不被优化，这样再后续a的地方读值为2
        auto *pa = const_cast<int *>(&a);
        *pa = 2;
        std::cout << a << " " << *pa << " | ";      //修改const指针是ub特性，

        int i = 1;
        const int b = i;
        auto *pb = const_cast<int *>(&b);
        *pb = 2;
        std::cout << b << " " << *pb << " | ";

        auto& c = const_cast<int&>(b);
        c = 3;
        std::cout << b<< " " << c << std::endl;
        return 0;
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
           std::cout << "B print" << " | ";
        }
    };
    class C:public B{
    public:
        void print(){
           std::cout << "C print" << " | ";
        }
    };

    //reinterpret_cast
    //reinterpret_cast类似于c的强制转换，不建议使用
    int test(void)
    {
        std::cout << "============= reinterpret_cast =============" << std::endl;
        int a = 0x34353600;
        auto pb = &a;
        float *pf = reinterpret_cast<decltype(pf)>(pb);
        std::cout << *pf << " | ";

        A a1{1};
        B *b1 = reinterpret_cast<B *>(&a1); //可以转换，但不符合应用
        std::cout << a1.val << " | ";

        C c1;
        B& d1 = reinterpret_cast<B&>(c1);
        d1.print();                         
        C& c2 = reinterpret_cast<C&>(d1);
        c2.print();

        std::cout << std::endl;
        return 0;
    }
}

namespace STATIC_CAST
{
    class A
    {
    public:
        int a = 1;
        int get_a() {
            return a;
        }
    };

    class B{
    public:
        int b = 2;
        virtual int get_b() {
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

    int test(void)
    {
        std::cout << "============= static_cast =============" << std::endl;
        
        {
            A *pa = new A;
            B *pb = new B;
            C *pc = new C;

            //多重继承，基类转派生类
            //其中A布局正常，B的布局无法识别
            C *pc1 = static_cast<C *>(pa);
            std::cout << pc1->get_a()  << " | ";
            //cout<<pc1->get_b() << " | ";
           
           //其中a的值不正确，B的布局识别正常
            C *pc2 = static_cast<C *>(pb);
            //cout<<pc2->get_a() << " | ";  
            std::cout << pc2->get_b()  << " | ";

            //多重继承，派生类转基类, 工作正常
            B *pb1 = static_cast<B *>(pc);
            std::cout << pb1->get_b() << " | ";

            //多重继承，派生类转基类, 工作正常
            A *pa1 = static_cast<A *>(pc);
            std::cout << pa1->get_a() << std::endl;
        }

        {
            C c;
            //派生类引用转基类引用
            A& a = static_cast<A &>(c);
            std::cout << a.get_a() << " | ";

            B& b = static_cast<B &>(c);
            std::cout << b.get_b() << " | ";

            C&& d = static_cast<C &&>(c);
            std::cout << c.get_a()  << " | ";
            std::cout << c.get_b()  << std::endl;
        }

        //enum转int
        {
            int a = static_cast<int>(E1::first);
            std::cout << a << std::endl;
        }

        return 0;
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
            std::cout << k << " | ";
        }
    };

    int test(void)
    {
        std::cout << "============= dynamic_cast =============" << std::endl;
        {
            Base *pba = new Base;
            Base *pbb = new Derived;
            Derived *pbc;

            // 基类转派生类，不具有完整的派生类类型，返回nullptr
            pbc = dynamic_cast<Derived *>(pba);
            if(pbc == nullptr) {
                std::cout << "nullptr" << " | ";
            }

            // 基类转派生类，具有完整的派生类类型，返回实际类型
            pbc = dynamic_cast<Derived *>(pbb);
            if(pbc != nullptr) {
                std::cout << "not nullptr" << std::endl;
            }

            delete pba;
            delete pbb;
        }

        {
            // 派生类转基类，多继承
            BaseE *pE = dynamic_cast<BaseE *>(new Derived());
            Derived *pd = dynamic_cast<Derived *>(pE);
            Base *pb = dynamic_cast<Base *>(pd);
            std::cout << pE->j << " | ";
            pd->f();
            std::cout << pb->i << std::endl;

            delete pE;
        }

        {
            //引用的派生类和基类互转
            Derived d;
            Base& pb = dynamic_cast<Base&>(d);
            Derived& d1 = dynamic_cast<Derived&>(pb);
            std::cout << pb.i << " | ";
            std::cout << d1.i << std::endl;
        }
        return 0;
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

    int test(void)
    {
        std::cout << "============= typeid =============" << std::endl;

        A a, a1;
        std::cout << typeid(a).name() << " "<<typeid(a).hash_code() << " | ";
        std::cout << typeid(a1).name() << " "<<typeid(a1).hash_code() << " | ";
        if(typeid(a) == typeid(a1)) {
            std::cout << "type equal" << std::endl;
        }

        B *pb;
        std::cout << typeid(pb).name() << " "<<typeid(pb).hash_code() << std::endl;

        C *pc = new C;
        B *pb1 = static_cast<B *>(pc);
        std::cout << typeid(pb1).name() << " "<<typeid(pb1).hash_code() << " | ";
        std::cout << typeid(pc).name() << " "<<typeid(pc).hash_code() << std::endl;

        int l, *pl;

        pl = &l;
        std::cout << typeid(l).name() << " "<<typeid(l).hash_code() << " | ";
        std::cout << typeid(*pl).name() << " "<<typeid(*pl).hash_code() << " | ";
        std::cout << typeid(pl).name() << " "<<typeid(pl).hash_code() << std::endl;

        std::cout << std::boolalpha<<typeid(A).before(typeid(B)) << std::endl;
        return 0;
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
       std::cout << "A:"<<a._i << " "<<a._j << " | ";
    }

    void func_print(C c)
    {
       std::cout << "C:"<<c._i << " "<<c._j << " | ";
    }

    int test(void)
    {
        std::cout << "============= explicit =============" << std::endl;

        A a1(1, 2);
        func_print(a1);

        A a2(1);
        func_print(static_cast<C&>(a2));

        //无法进行隐式的类型转换
        //func_print({1}); //explict 
        func_print({1, 2});

        std::cout << std::endl;

        return 0;
    }
}

int main(void)
{
    CONST_CAST::test();

    REINTERPRET_CAST::test();

    STATIC_CAST::test();

    DYNAMIC_CAST::test();

    TYPEID::test();

    EXPLICIT::test();

    return 0;
=======
//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. const_cast
//      2. reinterpret_cast
//      3. static_cast
//      4. dynamic_cast
//      5. typeid和RTTI
//      6. explicit 显示标识符
//
// Author:
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>

namespace CONST_CAST
{
    //const_cast
    int test(void)
    {
        std::cout << "============= const_cast =============" << std::endl;

        const volatile int a = 1;           //加volatile可以保证a不被优化，这样再后续a的地方读值为2
        auto *pa = const_cast<int *>(&a);
        *pa = 2;
        std::cout << a << " " << *pa << " | ";      //修改const指针是ub特性，

        int i = 1;
        const int b = i;
        auto *pb = const_cast<int *>(&b);
        *pb = 2;
        std::cout << b << " " << *pb << " | ";

        auto& c = const_cast<int&>(b);
        c = 3;
        std::cout << b<< " " << c << std::endl;
        return 0;
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
           std::cout << "B print" << " | ";
        }
    };
    class C:public B{
    public:
        void print(){
           std::cout << "C print" << " | ";
        }
    };

    //reinterpret_cast
    //reinterpret_cast类似于c的强制转换，不建议使用
    int test(void)
    {
        std::cout << "============= reinterpret_cast =============" << std::endl;
        int a = 0x34353600;
        auto pb = &a;
        float *pf = reinterpret_cast<decltype(pf)>(pb);
        std::cout << *pf << " | ";

        A a1{1};
        B *b1 = reinterpret_cast<B *>(&a1); //可以转换，但不符合应用
        std::cout << a1.val << " | ";

        C c1;
        B& d1 = reinterpret_cast<B&>(c1);
        d1.print();                         
        C& c2 = reinterpret_cast<C&>(d1);
        c2.print();

        std::cout << std::endl;
        return 0;
    }
}

namespace STATIC_CAST
{
    class A
    {
    public:
        int a = 1;
        int get_a() {
            return a;
        }
    };

    class B{
    public:
        int b = 2;
        virtual int get_b() {
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

    int test(void)
    {
        std::cout << "============= static_cast =============" << std::endl;
        
        {
            A *pa = new A;
            B *pb = new B;
            C *pc = new C;

            //多重继承，基类转派生类
            //其中A布局正常，B的布局无法识别
            C *pc1 = static_cast<C *>(pa);
            std::cout << pc1->get_a()  << " | ";
            //cout<<pc1->get_b() << " | ";
           
           //其中a的值不正确，B的布局识别正常
            C *pc2 = static_cast<C *>(pb);
            //cout<<pc2->get_a() << " | ";  
            std::cout << pc2->get_b()  << " | ";

            //多重继承，派生类转基类, 工作正常
            B *pb1 = static_cast<B *>(pc);
            std::cout << pb1->get_b() << " | ";

            //多重继承，派生类转基类, 工作正常
            A *pa1 = static_cast<A *>(pc);
            std::cout << pa1->get_a() << std::endl;
        }

        {
            C c;
            //派生类引用转基类引用
            A& a = static_cast<A &>(c);
            std::cout << a.get_a() << " | ";

            B& b = static_cast<B &>(c);
            std::cout << b.get_b() << " | ";

            C&& d = static_cast<C &&>(c);
            std::cout << c.get_a()  << " | ";
            std::cout << c.get_b()  << std::endl;
        }

        //enum转int
        {
            int a = static_cast<int>(E1::first);
            std::cout << a << std::endl;
        }

        return 0;
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
            std::cout << k << " | ";
        }
    };

    int test(void)
    {
        std::cout << "============= dynamic_cast =============" << std::endl;
        {
            Base *pba = new Base;
            Base *pbb = new Derived;
            Derived *pbc;

            // 基类转派生类，不具有完整的派生类类型，返回nullptr
            pbc = dynamic_cast<Derived *>(pba);
            if(pbc == nullptr) {
                std::cout << "nullptr" << " | ";
            }

            // 基类转派生类，具有完整的派生类类型，返回实际类型
            pbc = dynamic_cast<Derived *>(pbb);
            if(pbc != nullptr) {
                std::cout << "not nullptr" << std::endl;
            }

            delete pba;
            delete pbb;
        }

        {
            // 派生类转基类，多继承
            BaseE *pE = dynamic_cast<BaseE *>(new Derived());
            Derived *pd = dynamic_cast<Derived *>(pE);
            Base *pb = dynamic_cast<Base *>(pd);
            std::cout << pE->j << " | ";
            pd->f();
            std::cout << pb->i << std::endl;

            delete pE;
        }

        {
            //引用的派生类和基类互转
            Derived d;
            Base& pb = dynamic_cast<Base&>(d);
            Derived& d1 = dynamic_cast<Derived&>(pb);
            std::cout << pb.i << " | ";
            std::cout << d1.i << std::endl;
        }
        return 0;
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

    int test(void)
    {
        std::cout << "============= typeid =============" << std::endl;

        A a, a1;
        std::cout << typeid(a).name() << " "<<typeid(a).hash_code() << " | ";
        std::cout << typeid(a1).name() << " "<<typeid(a1).hash_code() << " | ";
        if(typeid(a) == typeid(a1)) {
            std::cout << "type equal" << std::endl;
        }

        B *pb;
        std::cout << typeid(pb).name() << " "<<typeid(pb).hash_code() << std::endl;

        C *pc = new C;
        B *pb1 = static_cast<B *>(pc);
        std::cout << typeid(pb1).name() << " "<<typeid(pb1).hash_code() << " | ";
        std::cout << typeid(pc).name() << " "<<typeid(pc).hash_code() << std::endl;

        int l, *pl;

        pl = &l;
        std::cout << typeid(l).name() << " "<<typeid(l).hash_code() << " | ";
        std::cout << typeid(*pl).name() << " "<<typeid(*pl).hash_code() << " | ";
        std::cout << typeid(pl).name() << " "<<typeid(pl).hash_code() << std::endl;

        std::cout << std::boolalpha<<typeid(A).before(typeid(B)) << std::endl;
        return 0;
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
       std::cout << "A:"<<a._i << " "<<a._j << " | ";
    }

    void func_print(C c)
    {
       std::cout << "C:"<<c._i << " "<<c._j << " | ";
    }

    int test(void)
    {
        std::cout << "============= explicit =============" << std::endl;

        A a1(1, 2);
        func_print(a1);

        A a2(1);
        func_print(static_cast<C&>(a2));

        //无法进行隐式的类型转换
        //func_print({1}); //explict 
        func_print({1, 2});

        std::cout << std::endl;

        return 0;
    }
}

int main(void)
{
    CONST_CAST::test();

    REINTERPRET_CAST::test();

    STATIC_CAST::test();

    DYNAMIC_CAST::test();

    TYPEID::test();

    EXPLICIT::test();

    return 0;
>>>>>>> 171775b3443edbb693d4aa0be5b9466005143b33
}