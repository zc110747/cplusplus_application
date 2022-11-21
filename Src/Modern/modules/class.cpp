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
 * 7.列表初始化, initialize_list,
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

static void basic_class(void);
static void init_class(void);
static void struct_class(void);
static void noLimit_union(void);
static void aggregate_class(void);
static void template_class(void);
static void over_class(void);

#define FUNCTION_START()  {cout<<__func__<<":\n  ";}{
#define FUNCTION_END()    }{cout<<"\n\n";  } 

int class_process(void)
{
    basic_class();

    noLimit_union();

    struct_class();

    init_class();

    aggregate_class();

    template_class();     

    over_class();
    return 0;
}

/*
重写(overwrite), 派生类覆盖了基类的虚函数(具有相同的函数签名和类型)
重载(overload), 一个类有两个或者两个以上函数，函数名相同，签名不同
隐藏(override), 派生类出现基类的同名函数，如果签名不同，则基类函数无论是否为虚函数，都将隐藏。
                如果签名相同，则需要确定是否为虚函数，是虚函数则重写，不是则为隐藏
                (隐藏后，将无法调用基类的函数，如果需要使用，需要通过using引入派生类)
*/
static void over_class(void)
{
    FUNCTION_START()

    //重写，重载，隐藏
    {
        class A{
        public:
            virtual void f(){
                cout<<"A::f()"<<" | ";
            }
            void f(int a){  //overload 重载
                cout<<"A::f(int)"<<" | ";
            }
            void f1(){
                cout<<"A::f(1)"<<" | ";
            }
        };
        class B:public A{
        public:
            //using A::f; //通过using，可以将所有相关函数引入派生类
            void f(){
                cout<<"B::f()"<<" | ";
            }
        };

        B b;
        b.f();
        //b.f(1);  //编译不通过，派生类具有相同函数名，被隐藏
        b.f1();
        A *ptr = static_cast<A *>(&b);
        ptr->f();  //B::f()，函数名和签名相同，重写
    }

    //override
    {
        class A1{
        public:
            virtual void f(){
                cout<<"A1::f()"<<" | ";
            }
        };
        class B1:public A1{
        public:
            virtual void f() override{
                cout<<"B1::f()"<<" | ";
            }
        };

        B1 b;
        b.f();
    }

    //final
    {
        class A2{
        public:
            virtual void f() final{
                cout<<"A2::f()"<<" | ";
            }
        };
        class B2:public A2{
            // void f(){  //final 不允许override
            // }
        };

        // B2 b;
        // b.f();
        // B2 b;
        // A2 *ptr = static_cast<A2 *>(&b);
        // ptr->f();
    }
    FUNCTION_END()
}


static void basic_class(void)
{
    FUNCTION_START()

    {
        class A
        {
        public:
            void print(void){
                cout<<"A"<<" | ";
            }
            //构造函数自动生成
            //析构函数自动生成
        };
        A a;                    //默认构造函数
        A a1 = a;               //复制构造函数  
        A a2(a);                //复制赋值运算符函数
        A &&a4(std::move(a));   //移动构造函数  
        A &&a3 = std::move(a);  //移动赋值运算符函数
        a.print();
        a1.print();
        a2.print();
        a3.print();
        cout<<boolalpha<<is_trivially_copyable_v<A><<" | ";
        cout<<boolalpha<<noexcept(basic_class())<<" | ";
    }

    //delete
    //default
    {
        class A
        {
        public:
            void print(void){
                cout<<"A"<<" | ";
            }
        public:
            A() = default;
            A(const char *ptr){} //声明任何构造函数，都会阻碍默认构造函数的创建
            A(const A &type) = delete;       //禁止调用复制构造函数
            void *operator new(std::size_t) = delete;
        };

        A a{"class defined"};
        A a1;   //如果没有default,则报错 
        //A a2 = a1;
        //A *pa = new A;
        A&& a2 = std::move(a1);
        a.print();
        a1.print();
        a2.print();
        cout<<"trivial:"<<boolalpha<<is_trivial_v<A><<" | ";
        cout<<"standard_layout_v"<<is_standard_layout_v<A><<" | ";
    }
    FUNCTION_END()
}

//非受限的联合体
static void noLimit_union(void)
{
    FUNCTION_START()

    union U
    {
        //联合体内有非平凡对象，需要有构造/析构函数
        U(){};
        ~U(){};

        int x1;
        float x2;
        std::string x3;             //带有类的即为非平凡对象
        std::vector<int> x4;
    };

    U u;
    new(&u.x3) std::string("hello world!");
    cout<<u.x3<<" | ";
    u.x3.~basic_string();

    new(&u.x4) std::vector<int>;
    u.x4.push_back(24);
    cout<<u.x4[0]<<" | ";
    u.x4.~vector();

    FUNCTION_END()
}

//委托构造函数
//继承构造函数
static void struct_class(void)
{
    FUNCTION_START()

    {
        class X
        {
        public:
            //委托构造函数
            X():X(0, 0.){}
            X(double b1):X(0, b1){}
            X(int a1):X(a1, 0.){}
            X(int a1, double b1){a=a1;b=b1;}
            ~X(){}
            void print(){
                cout<<"a="<<a<<",b="<<b<<" | ";
            }
        private:
            int a{5};
            double b{1.5};    
        };

        X x1, x2{1}, x3{1.0};
        x1.print();
        x2.print();
        x3.print();

        //继承构造函数
        class Xe:public X{
        public:
            //继承构造函数
            using X::X;
        };
        Xe xe1, xe2{1}, xe3{1.0};
        xe1.print();
        xe2.print();
        xe3.print();
    }

    FUNCTION_END()
}

//非静态成员的默认初始化
//位域的默认初始化
//列表初始化
static void init_class(void)
{
    FUNCTION_START()

    //位域的默认初始化
    {
        struct S{
            int y:8 = 11;
            int z:4 = 7;
        };
        S s1;
        cout<<"S:"<<s1.y<<" "<<s1.z<<" | ";
    }

    //列表初始化
    //initialize_list, 非静态成员的默认初始化
    {
        class ClassWithInit
        {
        public:
            ClassWithInit(const std::initializer_list<int> &UserList){
                assert(UserList.size() == 3);
                cout<<"ClassWithInit with initializerlist"<<" | ";
                auto list_iter = UserList.begin();
                a = *(list_iter++);
                b = *(list_iter++);
                c = *(list_iter++);
            }
            ClassWithInit(int c1){
                c = c1;
            }
            ClassWithInit(int a1, int b1, int c1){
                a = a1;
                b = b1;
                c = c1;
            }
            ~ClassWithInit(){
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
    
        ClassWithInit cwi = {1, 2, 3}; //优先以具有initializer_list为参数构造函数
        cwi.print();

        ClassWithInit cwi1{4, 5, 6};
        cwi1.print();

        ClassWithInit cwi2(3);
        cwi2.print();

        ClassWithInit cwi3(4, 5, 6);
        cwi3.print();
    }

    FUNCTION_END()
}

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

static void aggregate_class(void)
{   
    FUNCTION_START()

    {
        class A{  
        public:
            void nothing(){
            }
        };
        class B{        //用户提供构造函数
            B(){}
        };
        class C{        //私有静态成员
        private:
            int a;
        };
        class D{       //虚函数
        public:
            virtual void virtdo(){
            }
        };
        class E{
        public:
            E() = default;
        };
        class A1:public virtual A{  //虚拟继承
        };  
        class A2:private A{         //私有继承
        };
        class A3:public A1, public virtual A{ 
        };

        A3 a;
        a.nothing();

        cout<<boolalpha<<is_aggregate_v<A><<", "<<is_aggregate_v<A1><<" | ";
        cout<<boolalpha<<is_aggregate_v<A2><<", "<<is_aggregate_v<A3><<" | ";
        cout<<boolalpha<<is_aggregate_v<B><<" | ";
        cout<<boolalpha<<is_aggregate_v<C><<" | ";
        cout<<boolalpha<<is_aggregate_v<D><<" | ";
        cout<<boolalpha<<is_aggregate_v<E><<" | ";
    }

    {
        //聚合类型支持列表初始化(按照声明顺序)
        MyStringWithCount val{"hello", 5, 3};
        cout<<val<<" | ";
    }
    FUNCTION_END()
}

class X
{
public:
    X(std::vector<int>&& v):X(v.begin(), v.end()) {}
    X(std::deque<int>&& v):X(v.begin(), v.end()) {}
    ~X(){}

    void print(){
        std::for_each(l_.begin(), l_.end(), [](const int& val){
            cout<<val<<" ";
        });
        cout<<" | ";
    }
private:
    template<typename T> X(T first, T last) : l_(first, last) {}
    std::list<int> l_;
};

class A1
{
public:
    //static int b{1};
    static const int a{1};                  //常量静态成员可以在类内声明
    static inline std::string str{"hello"}; //添加inline的静态成员可以在类内声明
};

static void template_class(void)
{
    FUNCTION_START()

    X x1{vector<int>{1, 2, 3}}; 
    x1.print();
    
    X x2(deque<int>{3, 4, 5}); 
    x2.print();

    std::cout<<A1::a<<" | ";
    std::cout<<A1::str<<" | ";
    FUNCTION_END()
}
