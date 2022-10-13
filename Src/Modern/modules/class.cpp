#include <iostream>
#include <string>
#include <cassert>
#include <vector>

class UserBase
{
protected:
    int a{0};
    std::string str0;
    std::string str1;

public:

    UserBase(std::string &&a, std::string &&b):
    str0(std::move(a)), str1(std::move(b)){

    }

    virtual void printv(){
        std::cout<<"UserBaseV:"<<a<<" | ";
    }

    void print(){
        std::cout<<"UserBase:"<<a<<" | ";
    }

    void print_str(){
        std::cout<<str0<<":"<<str1<<" | ";
    }
};

class UserOne: public UserBase
{
public:
    //使用基类的构造函数
    using UserBase::UserBase;

    virtual void printv() final{
        std::cout<<"UserOneV:"<<":"<<a<<" | ";
    }

    void print(){
        std::cout<<"UserOne:"<<a<<" | ";
    }
};

class ClassWithInit
{
public:
    ClassWithInit(const std::initializer_list<int> &UserList){
        assert(UserList.size() == 3);
        auto list_iter = UserList.begin();
        a = *(list_iter++);
        b = *(list_iter++);
        c = *(list_iter++);
    }
    ~ClassWithInit(){
    }

    void print(){
        std::cout<<"a:"<<a<<",b:"<<b<<",c:"<<c<<" | ";
    }
private:
    int a;
    int b;
    int c;
};

union U
{
    //联合体内有非平凡对象，需要有构造/析构函数
    U(){};
    ~U(){};

    int x1;
    float x2;
    std::string x3;
    std::vector<int> x4;
};

int class_process(void)
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
    std::cout<<s<<" | ";

    ClassWithInit cwi = {1, 2, 3};
    cwi.print();

    ClassWithInit cwi1{4, 5, 6};
    cwi1.print();

    U u;
    new(&u.x3) std::string("hello world!");
    std::cout<<u.x3<<" | ";
    u.x3.~basic_string();

    new(&u.x4) std::vector<int>;
    u.x4.push_back(24);
    std::cout<<u.x4[0]<<" | ";
    u.x4.~vector();
    return 0;
}