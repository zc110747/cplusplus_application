#include <iostream>
#include <string>

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
        std::cout<<"UserBaseV:"<<a<<std::endl;
    }

    void print(){
        std::cout<<"UserBase:"<<a<<std::endl;
    }

    void print_str(){
        std::cout<<str0<<":"<<str1<<std::endl;
    }
};

class UserOne: public UserBase
{
public:
    //使用基类的构造函数
    using UserBase::UserBase;

    virtual void printv() final{
        std::cout<<"UserOneV:"<<":"<<a<<std::endl;
    }

    void print(){
        std::cout<<"UserOne:"<<a<<std::endl;
    }
};

int class_study(void)
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
    std::cout<<s<<std::endl;

    return 0;
}