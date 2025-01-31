/*
在C++中，类是一种用户定义的数据类型，它封装了数据和操作这些数据的函数。类是面向对象编程的基本构建块，
它允许你将数据和操作这些数据的函数组合在一起，形成一个逻辑单元。

1. 封装, 访问控制(private, protected, public)
封装是将数据（属性）和操作数据的方法（函数）捆绑在一起，形成一个类。通过封装，类的内部实现细节对外部是隐藏的，
外部只能通过类提供的公共接口来访问和操作数据。
private: 类的私有成员只能在类的内部以及友元的类或者函数访问
protected: 除private对象外，还允许派生的类访问
public: 类的公共成员可以在任何地方访问。

构造函数
- 默认构造函数(default)，默认构造函数是一个特殊的构造函数，它在创建类的对象时被调用，并且不需要任何参数。
- 拷贝构造函数(copy)，拷贝构造函数用于创建一个新对象，该对象是现有对象的副本。
- 移动构造函数(move)，移动构造函数是C++11引入的一个特性，它允许对象的资源（如内存、文件句柄等）从一个对象转移到另一个对象，而不是进行深拷。
- 删除构造函数(delete)，它使用delete关键字来显式地禁止编译器生成默认的构造函数
- 显示构造函数(explicit)，它使用explicit关键字来显式地声明，以防止隐式转换
- 委托构造函数，它允许一个构造函数调用另一个构造函数来初始化对象
- 继承构造函数，它允许派生类直接继承基类的构造函数，而不需要在派生类中显式地定义这些构造函数
析构函数(默认析构函数，删除析构函数，显示析构函数)

友元函数和友元类(friend)
友元函数是一个普通函数，但它可以访问类的所有成员。友元函数在类的定义中声明，但它不是类的成员函数。
友元类是一个类，它的所有成员函数都可以访问另一个类的私有成员和保护成员。友元类在另一个类的定义中声明。

运算符重载(operator+,-,*,/,+=)
算符重载允许你为自定义类型定义特定的运算符行为。这使得你可以像使用内置类型一样使用自定义类型的对象。

2. 继承，权限控制(private, protected, public)
继承是指一个类（称为派生类或子类）从另一个类（称为基类或父类）继承属性和行为。通过继承，派生类可以重用基类的代码，
并且可以在其基础上添加新的功能或修改原有功能。
类成员/派生方式     public          protected         private
基类public      派生类public      派生类protected   派生类private
基类protected   派生类protected   派生类protected   派生类private
基类private     派生类不可见       派生类不可见      派生类不可见

多重继承和虚继承
多重继承是指一个类可以从多个基类继承属性和方法。在C++中，一个派生类可以继承多个基类，从而获得这些基类的所有成员
虚继承是C++中的一种特殊继承方式，用于解决多重继承中的菱形继承问题
A -> B -> D
A -> C -> D
此时D类中会有两个A的成员，此时就会有二义性，被称为菱形继承，需要使用虚继承来解决

const和multable
const在类变量上，表示只读
const声明在函数中
- 在函数参数上，表示参数在函数内部不允许修改
- 在函数返回值前方，表示返回值不能够修改。
- 在函数后方，表示函数体内this指针不允许修改(函数内部不能够修改变量)，如果希望修改某参数，在参数前添加mutable
mutable关键字是一个属性修饰符，它允许类的某个数据成员在常量对象（const对象）中被修改

3. 虚函数，纯虚函数和接口类
虚函数是在基类中使用关键字virtual声明的函数，它允许在派生类中对该函数进行重写。
纯虚函数是在基类中使用关键字virtual和=0声明的函数，它没有具体的实现，要求派生类必须实现该函数。
接口类是一个只包含纯虚函数的类，它定义了一组操作，而不提供具体的实现。

overwrite关键字
overwrite是一个标识符，它用于指示某个函数或变量是从基类继承而来的，并且在派生类中被重新定义。

final关键字
final是一个关键字，用于指定某个类、虚函数或虚函数的重载不能被继承或重写。
- 当final用于类时，表示该类不能被继承
- 当final用于虚函数时，表示该虚函数不能在派生类中被重写

类的重写，重载，隐藏
重写(overwrite)是指在派生类中重新定义基类的虚函数，以改变其行为。
重载(overload)是指在同一个作用域内定义多个同名函数，它们的参数列表不同。
隐藏(overhide)是指在派生类中定义一个与基类同名的函数，以改变其行为。

4. 列表初始化
列表初始化是一种在C++中初始化对象的方法，它允许你使用花括号{}来初始化对象的成员。
列表初始化的语法如下：
ClassName object = {value1, value2, ...};

非静态数据成员的默认初始化(=, {})
静态数据成员(非const)的inline初始化

单例模式
单例模式是一种设计模式，它确保一个类只有一个实例，并提供一个全局访问点来访问该实例。

5. RAII(Resource Acquisition is initialization)
RAII（Resource Acquisition Is Initialization）是一种C++编程技术，用于管理资源的生命周期。
RAII的核心思想是将资源的获取和释放与对象的生命周期绑定在一起，确保资源在对象创建时被获取，在对象销毁时被释放。
RAII的重要应用是智能指针和std::lock_guard，会在后面说明

6. 强枚举类型(enum class)
强枚举类型（enum class）是C++11引入的一种新的枚举类型，它提供了更强的类型安全性和更好的封装性
*/
#include <iostream>
#include <cstdio>
#include <stdexcept>
#include <vector>

using namespace std;

#define FUNCTION_START()  {cout<<__func__<<":\n";}{
#define FUNCTION_END()    }{cout<<"\n\n";  }   

// 类的封装，继承，友元，权限控制
// 运算符重载
namespace PACKAGE
{
double calc_circle_area(double r);
class CircleWapper;

class Circle {
    friend double calc_circle_area(double r);
    friend class CircleWapper;
private:
    double radius;

protected:
    void set_radius(double r) {
        radius = r;
    }

public:
    explicit Circle(double r) : radius(r) {}            //显示构造函数
    Circle() = default;                                 // 默认构造函数
    Circle(const Circle &circle) : radius(circle.radius) { // 拷贝构造函数
        cout<<"拷贝构造函数"<<endl;
    } 
    Circle(Circle &&circle) : radius(circle.radius) {    // 移动构造函数
        cout<<"移动构造函数"<<endl;
    }     
    ~Circle() = default;                                // 默认析构函数
    
    int operator-(const Circle &circle) {
        return area() - circle.area();
    }
    Circle operator=(const Circle &circle) {
        cout<<"拷贝赋值函数"<<endl;
        return *this;
    }

    double area() const {
        return 3.14 * radius * radius;
    }

    double get_radius(void) const {
        return radius;
    }
};

class CircleWapper {
private:
    Circle circle_;
public:
    CircleWapper(double r) : circle_(r) {}

    double area() const {
        return circle_.area();
    }
};

// 运算符重载 +
int operator+(const Circle &a, const Circle &b) {
    return a.area() + b.area();
}

double calc_circle_area(double r) {
    Circle circle;
    circle.set_radius(r);
    return circle.area();
}

void test(void) 
{
    Circle circle(10);
    CircleWapper wapper(10);
    Circle circle_1(2);

    std::cout<<"=== PACKAGE ===\n";

    auto add = circle + circle_1;
    auto sub = circle - circle_1;

    cout << "circle area: " << circle.area() << endl;
    cout << "circle area: " << calc_circle_area(10) << endl;
    cout << "circle area: " << wapper.area() << endl;
    cout << "circle area: " << add << endl;
    cout << "circle area: " << sub << endl;
    
    auto circle_2 = circle;             // 拷贝构造函数
    auto circle_3 = std::move(circle);  // 移动构造函数
    cout << "circle area: " << circle_2.area() << endl;
    cout <<"circle_3 area: " << circle_3.area() << endl;
}
}

// 类的继承，继承构造函数，委托构造函数
// 多重继承
namespace EXTEND
{
class A {
private:
    int val_;
protected:
    void set_value(int a) {
        val_ = a;
    }
public:
    A() = default;
    A(int val) : val_(val) {}
    int get_value(void) const {
        return val_;
    }
};

class B : virtual public A {
private:
   mutable int val_b_;                  //声明为mutable，可以在const函数内修改
public:
    using A::A;                         // 继承构造函数
    B(int a, int b): A(a), val_b_(b) {} // 委托构造函数
    void set_a_val(int a) {
        set_value(a);
    }
    void set_b_val(int b) const {
        val_b_ = b;
    }
    int get_area(void) const {  //指针const，内部不能修改数据
        return get_value() * val_b_;
    }
};

class C : virtual public A {
public:
    const int c = 1;                    //只读变量
};

//多重菱形继承，需要使用虚继承
class D : public B, public C { 
public:
    int d;
};

void test(void)
{
    std::cout<<"=== EXTEND ===\n";

    {
        B b(1, 2);
        b.set_a_val(10);
        b.set_b_val(20);
        std::cout<<"area: "<<b.get_area()<<std::endl;
    }

    {
        D d;
        d.set_a_val(10);
        d.set_b_val(20);
        std::cout<<"area: "<<d.get_value()<<std::endl;
    }
}
}

//虚函数，纯虚函数，接口类
namespace VIRTUAL 
{
//接口类
class base {
public:
    virtual void show() = 0;    //纯虚函数    
};

class A : public base { 
private:
    int val_{0};
public:
    void show() override {      //类的重写(overriding重写是指在派生类中重新定义基类的虚函数，以改变其行)
        std::cout<<"A: "<<val_<<std::endl;
    }

    void set_val(int a) {
        val_ = a;
    }

    void set_val() {             //类的重载(overloading重载是指在同一个作用域内定义多个同名函数，它们的参数列表不同)
        val_ = 0;
    }

    virtual int get_val(void) const final{ //final标识符，用于类成员
        return val_;
    }
};

class B final : public A {
public:
    void show() {               //类的隐藏(overhiding隐藏是指在派生类中定义一个与基类同名的函数，以改变其行为)
        cout<<"B: "<<get_val()<<endl;
    }
};

void test(void)
{
    std::cout<<"=== VIRTUAL ===\n";

    A a;
    B b;

    a.set_val(10);
    a.show();

    a.set_val();
    a.show();

    b.set_val(10);
    b.show();
}
}

namespace INITIALIZATION
{
class user_initialize
{
public:
    user_initialize() = default;
    user_initialize(std::initializer_list<int> list) {
        vec_ = std::vector(list);  
    };
    void show(void) {
        for(auto val:vec_) {
            std::cout<<val<<" ";
        }
        std::cout<<std::endl;
    }

    inline static int val1_ = 1;     //静态数据成员(非const)的inline初始化
    static const int const_val_ = 2; //静态数据const成员初始化
private:
    std::vector<int> vec_{0};       //非静态数据成员的默认初始化
};

class Singleton {
public:
    //获取单例实例的静态成员函数
    static Singleton& getInstance() {
        static Singleton Instance;
        return Instance;
    }
    Singleton() = default;

    //禁止拷贝构造和赋值操作
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    void show() {
        cout<<"Singleton show"<<endl;
    }
};

void test(void)
{
    std::cout<<"=== INITIALIZATION ===\n";

    std::vector <int> vec{1, 2, 3};
    for(auto val:vec) {
        std::cout<<val<<" ";
    }
    cout<<"\n";

    user_initialize val{1, 2, 3};
    val.show();

    cout<<user_initialize::val1_<<endl;
    cout<<user_initialize::const_val_<<endl;
    
    Singleton::getInstance().show();
}
}

namespace RAII
{
class resource_ptr {
private:
    int *ptr{nullptr};
public:
    resource_ptr(int *p) : ptr(p) {
        cout<<"constructor"<<endl;
    }
    ~resource_ptr() { 
        if (ptr)    
            delete ptr;
        ptr = nullptr; 
        cout<<"destructor"<<endl;
    }

    int* get_ptr(void) {
        return ptr;
    }
};

void test(void)
{
    std::cout<<"=== RAII ===\n";
    {
        resource_ptr ptr(new int(10));
        cout<<*ptr.get_ptr()<<endl;
    }
}
}

namespace EXTRA
{
//强枚举类型
enum class Color {
    Red = 0,
    Green,
    Blue
};

//联合体
union ColorUnion {
    int colorCode;
    Color color;
};

void test(void) 
{
    Color c = Color::Red;
    // int x = c; // 错误：不能隐式转换为整数类型
    // if (c == 0) // 错误：不能与整数进行比较
    if (c == Color::Red) {
        std::cout << "The color is red." << std::endl;
    }

    ColorUnion cu;
    cu.colorCode = 1;
    if(cu.color == Color::Green) {
        std::cout << "The color is green." << std::endl;
    }
}
}

int main(int argc, char *argv[])
{
    PACKAGE::test();

    EXTEND::test();

    VIRTUAL::test();

    INITIALIZATION::test();

    RAII::test();

    EXTRA::test();
    return 0;
}
