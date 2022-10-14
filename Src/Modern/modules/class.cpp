/******************************************************************
 * 1.类的封装，继承，派生
 * 2.非静态成员的默认初始化，位域默认初始化(C++20)
 * 3.列表初始化, initialize_list,
 * 4.非受限的联合体
 * 5.委托构造函数
 * 6.继承构造函数
********************************************************************/
#include <iostream>
#include <string>
#include <cassert>
#include <vector>
#include <list>
#include <deque>
#include <algorithm>

using namespace std;

static void template_class(void);

class UserBase
{
protected:
    int a{0};
    std::string str0;
    std::string str1;

public:
    UserBase() = default;
    UserBase(std::string &&a, std::string &&b):str0(std::move(a)), str1(std::move(b)){
    }

    virtual void printv(){
        cout<<"UserBaseV:"<<a<<" | ";
    }

    void print(){
        cout<<"UserBase:"<<a<<" | ";
    }

    void print_str(){
        cout<<str0<<":"<<str1<<" | ";
    }
};

class UserOne: public UserBase
{
public:
    //继承构造函数
    using UserBase::UserBase;

    virtual void printv() final{
        cout<<"UserOneV:"<<":"<<a<<" | ";
    }

    void print(){
        cout<<"UserOne:"<<a<<" | ";
    }
};

int class_process(void)
{
    {
        UserBase Base0("s0", "s1");
        UserOne User0("u0", "u1");

        UserBase *p = &Base0;
        p->print();     //UserBase:0
        p->printv();    //UserBase:0
        p->print_str(); //s0:s1 基类

        p = &User0;
        p->print();     //UserBase:0, 非虚函数, 访问基类
        p->printv();    //UserOne::0
        p->print_str(); //u0:u1 基类,访问派生类的变量

        auto&& str = "hello world!d";
        std::string s(str);
        cout<<s<<" | ";
    }

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

    //位域的默认初始化
    {
        struct S{
            int y:8 = 11;
            int z:4 = 7;
        };
        S s1;
        cout<<"S:"<<s1.y<<" "<<s1.z<<" | ";
    }

    //
    {
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
    }

    //委托构造函数, 继承构造函数
    {
        class X
        {
        public:
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
            using X::X;
        };
        Xe xe1, xe2{1}, xe3{1.0};
        xe1.print();
        xe2.print();
        xe3.print();
    }

    {
        template_class();     
    }

    return 0;
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
    template<class T> X(T first, T last) : l_(first, last) {}
    std::list<int> l_;
};

static void template_class(void)
{
    X x1{vector<int>{1, 2, 3}}; 
    x1.print();
    
    X x2(deque<int>{3, 4, 5}); 
    x2.print();
}