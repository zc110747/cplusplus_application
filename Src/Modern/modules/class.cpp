/******************************************************************
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
static void template_class(void);

#define FUNCTION_START()  {cout<<__func__<<":\n  ";}{
#define FUNCTION_END()    }{cout<<"\n\n";  } 

int class_process(void)
{
    basic_class();

    noLimit_union();

    struct_class();

    init_class();

    template_class();     

    return 0;
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
    }

    {
        class A
        {
        public:
            void print(void){
                cout<<"A"<<" | ";
            }
        public:
            A()=default;
            A(const char *ptr){} //声明任何构造函数，都会阻碍默认构造函数的创建
            A(const A &type) = delete;       //禁止调用复制构造函数
        };

        A a{"class defined"};
        A a1;   //如果没有default,则报错 
        //A a2 = a1;
        A&& a2 = std::move(a1);
        a.print();
        a1.print();
        a2.print();
        cout<<"trivial:"<<boolalpha<<is_trivial_v<A><<" | ";
        cout<<"pod"<<is_pod_v<A><<" | ";
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
    FUNCTION_START()

    X x1{vector<int>{1, 2, 3}}; 
    x1.print();
    
    X x2(deque<int>{3, 4, 5}); 
    x2.print();

    FUNCTION_END()
}

