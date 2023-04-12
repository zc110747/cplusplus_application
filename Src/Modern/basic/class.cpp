/******************************************************************
 * 第八章 非静态成员默认初始化
 * 第九章 列表初始化
 * 第十章 默认和删除函数
 * 第十一章 非受限联合类型
 * 第十二章 委托构造函数 
 * 第十三章 继承构造函数
 * 第十五章 扩展的聚合类型
 * 第十六章 override和final说明符 
 * ----------------------------------------------------------------
 * 1.类的封装，继承，派生
 * 2.默认和删除函数
 * 3.非受限的联合体
 * 4.委托构造函数
 * 5.继承构造函数
 * 6.非静态成员的默认初始化，位域默认初始化(C++20)
 * 7.列表初始化, initializer_list,
 * 8.default - delete
 * 9.C++类的类型: 聚合, POD, TrivialType, TrivialCopyable, 
 *               Standard-layout Type, 标准布局兼容
 * 10.重写，重载和隐藏
 * 11.扩展的inline说明符
********************************************************************/
#include <iostream>
#include <string>
#include <cassert>
#include <vector>
#include <list>
#include <deque>
#include <algorithm>

using namespace std;

#define FUNCTION_START()  {cout<<__func__<<":\n  ";}{
#define FUNCTION_END()    }{cout<<"\n\n";  } 

namespace BASIC_CLASS
{
    class A
    {
    public:
        void print(void)
        {
            cout<<"A"<<" | ";
        }
        //构造函数自动生成
        //析构函数自动生成
    };

    class B
    {
    public:
        void print(void){
            cout<<"A"<<" | ";
        }
    public:
        B() = default;
        B(const char *ptr){}             //声明任何构造函数，都会阻碍默认构造函数的创建
        B(const B &type) = delete;       //禁止调用复制构造函数
        void *operator new(std::size_t) = delete;
    };

    void process(void)
    {
        FUNCTION_START()

        A a;                    //默认构造函数
        A a1 = a;               //复制构造函数  
        A a2(a);                //复制赋值运算符函数
        A &&a3(std::move(a));   //移动构造函数  
        A &&a4 = std::move(a);  //移动赋值运算符函数
        a4.print();
        cout<<boolalpha<<is_trivially_copyable_v<A><<" | ";

        B b{"class defined"};
        B b1;   //如果没有default,则报错 
        //B b2 = b;
        //B *pb = new B;
        B&& b3 = std::move(b);
        b3.print();
        cout<<"trivial:"<<boolalpha<<is_trivial_v<B><<" | ";
        cout<<"standard_layout_v"<<is_standard_layout_v<B><<" | ";

        FUNCTION_END()
    }
}

namespace NOLIMIT_UNION
{
    //联合体扩展支持除引用外的所有对象
    union U
    {
        //联合体内有非平凡对象，需要有构造/析构函数
        U(){};
        ~U(){};

        int x1;
        float x2;
        std::string x3;             //带有类的即为非平凡对象
        std::vector<int> x4;
        inline static int x5 = 1;
    };

    void process(void)
    {
        FUNCTION_START()

        U u;
        new(&u.x3) std::string("hello world!");
        cout<<u.x3<<" | ";
        u.x3.~basic_string();

        new(&u.x4) std::vector<int>;
        u.x4.push_back(24);
        cout<<u.x4[0]<<" | ";
        u.x4.~vector();

        cout<<U::x5<<" | ";
        FUNCTION_END()
    }
}

namespace OVER_CLASS
{
    class A_Base
    {
    public:
        virtual void f() {
            cout<<"A_Base::f"<<" | ";
        }
        void f(int a){  
            cout<<"A_Base::f(int)"<<" | ";
        }
        void f1(){
            cout<<"A_Base::f1"<<" | ";
        }

        virtual void f2() final {
            cout<<"A_Base::f2"<<" | ";
        }
    };

    class A_Derive final:public A_Base
    {
    public:
        //using A_Base::f; //通过using，可以将所有相关函数引入派生类
        void f() override {
            cout<<"A_Derive::f"<<" | ";
        }
        void f1(){
            cout<<"A_Derive::f1"<<" | ";
        }
    };

    //error, 声明为final的类不允许被继承
    // class A1_Derive:A_Derive{
    // };
    
    class C_Base
    {
    public:
        void show() {
            a = 1;
            cout<<"show C base:"<<a<<" | ";
        }
    protected:
        int a{0};
    };

    class C_Extern: public C_Base
    {
    public:
        void show() {
            a = 5;
            C_Base::show();
            cout<<"show C Extend:"<<a<<" | ";
        }  
    };

    void process(void)
    {
        FUNCTION_START()

        // 覆盖(override), 派生类覆盖了基类的虚函数(具有相同的函数签名和类型)
        // 重载(overload), 一个类有两个或者两个以上函数，函数名相同，签名不同
        // 隐藏(overwrite), 派生类出现基类的同名函数，如果签名不同，则基类函数无论是否为虚函数，都将隐藏。
        //                 如果签名相同，则需要确定是否为虚函数，是虚函数则重写，不是则为隐藏
        //                 (隐藏后，将无法调用基类的函数，如果需要使用，需要通过using引入派生类)
        A_Derive a1;
        a1.f();         //A_Derive::f,基类带virtual, 发生overwrite, 访问派生类
        //a1.f(1);      //error, 派生类出现基类同名函数，发生隐藏, 只能调用派生类函数
        a1.f1();        //A_Derive::f1,
        
        A_Base *a_ptr = static_cast<A_Base *>(&a1);
        a_ptr->f();     //A_Derive::f，函数名和签名相同，但是基类带virtual, overwrite, 会优先访问派生类
        a_ptr->f(2);    //A_Base::f(int), 直接调用基类的函数
        a_ptr->f1();    //A_Base::f1, 不在虚表中，调用基类的函数
        a_ptr->f2();    //A_Base::f2
        cout<<"\n  ";

        //override
        //声明在派生类中，用于表示函数需要覆盖基类的相同函数签名和类型的函数-基类需要时虚函数

        //final
        //1.配合虚函数使用，表示此函数不能够被其派生类override
        //2.配合类使用，表示类不能被继承

        C_Extern c;
        c.show();
        FUNCTION_END()
    }
}

namespace CONSTRUCT_CLASS
{
    //委托构造函数
    //类的某个类型的构造函数可以委托同类型的另一个构造函数进行初始化
    //1.每个构造函数既可以委托其它构造函数，也可以被委托
    //2.不要进行循环委托
    //3.如果函数时委托构造函数，初始化列表就不能进行数据成员或者基类初始化
    //4.执行顺序:初始化列表->委托构造函数本体，递归执行
    //5.代理构造函数执行完成后，如果触发异常，也会调用析构函数
    class X
    {
    public:
        //委托构造函数
        X():X(0, 0.){
            cout<<"X()"<<" | ";
        }
        X(double b1):X(0, b1){
            cout<<"X(double)"<<" | ";
        }
        X(int a1, double b1){
            a=a1; 
            b=b1; 
            cout<<"X(int, double)"<<" | ";
        }
        X(const X& other):X(other.a, other.b){
            cout<<"X(X&)"<<" | ";
        }
        ~X(){}
        void print()
        {
            cout<<a<<" "<<b<<" | ";
        }
    private:
        int a{5};
        double b{1.5};    
    };

    //继承构造函数
    //1.派生类是隐式继承基类构造函数，只有程序使用这些构造函数，编译器才会为派生类生成构造函数
    //2.派生类声明签名相同的函数会禁止继承相应的构造函数
    //3.派生类继承多个签名相同的构造函数会导致编译失败
    class Xe:public X{
    public:
        //继承构造函数
        using X::X;
    };

    class X_T
    {
    public:
        X_T(std::vector<int>&& v):X_T(v.begin(), v.end()) {}
        X_T(std::deque<int>&& v):X_T(v.begin(), v.end()) {}
        ~X_T(){}

        void print(){
            std::for_each(l_.begin(), l_.end(), [](const int& val){
                cout<<val<<" ";
            });
            cout<<" | ";
        }
    private:
        template<typename T> X_T(T first, T last) : l_(first, last) {}
        std::list<int> l_;
    };

    void process()
    {
        FUNCTION_START()

        X x1, x2{1}, x3{x1};
        x1.print();
        x2.print();
        x3.print();
        cout<<"\n  ";

        Xe xe1, xe2{1}, xe3{xe1};
        xe1.print();
        xe2.print();
        xe3.print();
        cout<<"\n  ";
        
        X_T xt1{vector<int>{1, 2, 3}}; 
        xt1.print();

        X_T xt2(deque<int>{3, 4, 5}); 
        xt2.print();
        FUNCTION_END()
    }
}

namespace INIT_CLASS
{
    //位域的默认初始化
    struct S{
        int y:8 = 11;
        int z:4 = 7;
    };

    //非静态成员的默认初始化
    //列表初始化
    class A
    {
    public:
        A(const std::initializer_list<int> &UserList)
        {
            assert(UserList.size() == 3);
            auto list_iter = UserList.begin();
            a = *(list_iter++);
            b = *(list_iter++);
            c = *(list_iter++);
            cout<<"A(initializer_list)"<<" | ";
        }
        A(int c1)
        {
            c = c1;
            cout<<"A(int)"<<" | ";
        }
        A(int a1, int b1, int c1){
            a = a1;
            b = b1;
            c = c1;
            cout<<"A(int, int, int)"<<" | ";
        }
        ~A(){
        }

        void print(){
            cout<<"a:"<<a<<",b:"<<b<<",c:"<<c<<" | ";
        }
    private:
        //非静态成员的默认初始化
        int a = 0;
        int b = 2;
        int c = 4;
    };

    void process(void)
    {
        FUNCTION_START()

        S s1;
        cout<<"S:"<<s1.y<<" "<<s1.z<<" | ";

        //列表初始化优先使用initializer_list为参数构造函数
        A cwi = {1, 2, 3}; 
        cwi.print();

        //列表初始化优先使用initializer_list为参数构造函数
        A cwi1{4, 5, 6};
        cwi1.print();

        A cwi2(3);
        cwi2.print();

        //普通初始化优先使用构造函数
        A cwi3(4, 5, 6);
        cwi3.print();

        FUNCTION_END()
    }
}

namespace AGGREGATE_CLSS
{
    //聚合类型
    //1.没有用户提供的构造函数
    //2.没有private和protected的非静态成员
    //3.没有虚函数
    //4.必须是公开的基类，不能有private或protected的基类
    //5.必须是非虚继承
    class Count{
    public:
        int count;
    };

    class MyStringWithCount:public string, public Count{
    public:
        int index = 0;
    };

    std::ostream& operator<<(std::ostream &os, MyStringWithCount &s){
        os<<s.index<<", "<<s.count<<", "<<s.c_str();
        return os;
    }

    class A
    {  
        public:
        void nothing()
        {
        }
    };

    class B
    {        //用户提供构造函数
        B(){}
    };

    class C
    {        //私有静态成员
        private:
        int a;
    };

    class D
    {       //虚函数
        public:
        virtual void virtdo()
        {
        }
    };
    class E
    {
        public:
        E() = default;
    };
    
    class A1: public virtual A
    {   //虚拟继承
    };  

    class A2: private A
    {         //私有继承
    };

    class A3: public A1, public virtual A
    { 
    };

    void process(void)
    {
        FUNCTION_START()

        A3 a;
        a.nothing();

        cout<<boolalpha<<is_aggregate_v<A><<", "<<is_aggregate_v<A1><<" | ";
        cout<<boolalpha<<is_aggregate_v<A2><<", "<<is_aggregate_v<A3><<" | ";
        cout<<boolalpha<<is_aggregate_v<B><<" | ";
        cout<<boolalpha<<is_aggregate_v<C><<" | ";
        cout<<boolalpha<<is_aggregate_v<D><<" | ";
        cout<<boolalpha<<is_aggregate_v<E><<" | ";

        //聚合类型支持列表初始化(按照声明顺序)
        MyStringWithCount val{"hello", 5, 3};
        cout<<val<<" | ";
    
        FUNCTION_END()
    }
}

int class_process(void)
{
    BASIC_CLASS::process();

    NOLIMIT_UNION::process();

    CONSTRUCT_CLASS::process();

    INIT_CLASS::process();

    AGGREGATE_CLSS::process();

    OVER_CLASS::process();
    cout<<__func__<<endl;
    return 0;
}
