//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. 类的封装
//      2. 类的继承
//      3. 类的多态
//      4. 类的功能扩展
//      5. 类的其它功能
//
// Author:
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <cstdio>
#include <stdexcept>
#include <vector>
#include <map>
#include <algorithm>

// 类的封装，继承，友元，权限控制
// 运算符重载
namespace PACKAGE
{
    double calc_circle_area(double r);
    class CircleWapper;

    /// 用于管理圆的类
    class Circle {
        /// 友元函数和友元类，用于访问类的私有成员和保护成员。
        friend double calc_circle_area(double r);
        friend class CircleWapper;

    private:
        /// @brief 圆的半径。
        double radius_{0.0};

    protected:
        /// @brief 设置圆的半径。
        /// @param r 圆的半径。
        void set_radius(double r) {
            radius_ = r;
        }

    public:
        /// @brief 显示构造函数，用于防止隐式转换
        explicit Circle(double r) : radius_(r) {}

        /// @brief 默认构造函数
        Circle() = default;

        /// @brief 拷贝构造函数，用于创建一个新对象，该对象是现有对象的副本。
        Circle(const Circle &circle) : radius_(circle.radius_) {
            std::cout << "circle copy structure" << std::endl;
        }

        /// @brief 移动构造函数，用于创建一个新对象，该对象是现有对象的副本。
        Circle(Circle &&circle) : radius_(circle.radius_) {
            std::cout << "circle move structure" << std::endl;
        }
        
        /// @brief 默认析构函数，用于释放对象的资源。
        ~Circle() = default;
        
        /// @brief 拷贝赋值函数，用于将一个对象的值赋给另一个对象。
        Circle& operator=(const Circle &circle) {
            std::cout << "拷贝赋值函数" << std::endl;
            return *this;
        }

        /// @brief 移动赋值函数，用于将一个对象的值赋给另一个对象。
        Circle& operator=(Circle &&circle) {
            std::cout << "移动赋值函数" << std::endl;
            return *this;
        }
        
        /// @brief 运算符重载 -，用于计算两个圆的面积差。
        int operator-(const Circle &circle) {
            return area() - circle.area();
        }

        /// @brief 计算圆的面积。
        double area() const {
            return 3.14 * radius_ * radius_;
        }

        /// @brief 获取圆的半径。
        double get_radius(void) const {
            return radius_;
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
        std::cout << "============== PACKAGE ==============" << std::endl;

        Circle circle(10);
        CircleWapper wapper(10);
        Circle circle_1(2);

        auto add = circle + circle_1;
        auto sub = circle - circle_1;

        std::cout << "circle area: " << circle.area() <<  std::endl;
        std::cout << "circle area: " << calc_circle_area(10) <<  std::endl;
        std::cout << "circle area: " << wapper.area() <<  std::endl;
        std::cout << "circle area: " << add <<  std::endl;
        std::cout << "circle area: " << sub <<  std::endl;
        
        auto circle_2 = circle;             // 拷贝构造函数
        auto circle_3 = std::move(circle);  // 移动构造函数
        std::cout << "circle area: " << circle_2.area() <<  std::endl;
        std::cout  << "circle_3 area: " << circle_3.area() <<  std::endl;
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
        B(int a, int b): A(a), val_b_(b) {} // 委托构造函数，可以在构造函数中调用其它构造函数(包含基类构造函数)
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
        std::cout << "============== EXTEND ==============" << std::endl;

        {
            B b(1, 2);
            b.set_a_val(10);
            b.set_b_val(20);
            std::cout << "area: " << b.get_area()<<std::endl;
        }

        {
            D d;
            d.set_a_val(10);
            d.set_b_val(20);
            std::cout << "area: " << d.get_value()<<std::endl;
        }
    }
}

//虚函数，纯虚函数，接口类
//动态绑定和虚函数表
namespace VIRTUAL
{
    //接口类
    class base {
    public:
        virtual void show() = 0;    //纯虚函数    
    };

    class A : public base { 
    public:
        void show() override {      //类的重写(overriding重写是指在派生类中重新定义基类的虚函数，以改变其行)
            std::cout << "A: " << val_<<std::endl;
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
    private:
        int val_{0};
    };

    class B final : public A {
    public:
        void show() {               //类的隐藏(overhiding隐藏是指在派生类中定义一个与基类同名的函数，以改变其行为)
            std::cout << "B: " << get_val() << std::endl;
        }
    };

    void test(void)
    {
        std::cout << "==============  VIRTUAL ===============" << std::endl;

        A a;
        B b;

        a.set_val(10);
        a.show();

        a.set_val();
        a.show();

        b.set_val(10);
        b.show();


        // 动态绑定
        {
            A *ptr;
        
            ptr = &a;
            ptr->show();    //调用A
            ptr = &b;
            ptr->show();    //调用B
        }
    }
}

// 类的参数初始化
namespace INITIALIZATION
{
    // 列表初始化
    class user_initialize
    {
    public:
        user_initialize() = default;
        user_initialize(std::initializer_list<int> list) {
            vec_ = std::vector(list);  
        };
        void show(void) {
            for(auto val:vec_) {
                std::cout<<val << " ";
            }
            std::cout<<std::endl;
        }

        inline static int val1_ = 1;     //静态数据成员(非const)的inline初始化
        const static  int const_val_ = 2; //静态数据const成员初始化

        int val_ = 0;                     //非静态数据成员的立即初始化(=, {})
    private:
        std::vector<int> vec_{0};        //非静态数据成员的默认初始化
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
            std::cout << "Singleton show" << std::endl;
        }
    };

    template<typename T>
    class InitObj{
    public:
        InitObj() = default;
        InitObj(std::initializer_list<T> l){
            for (auto obj: l)
            {
                data.push_back(std::move(obj));
            }
        }

        InitObj &operator [] (std::initializer_list<int> l){
            for (auto obj:l) {
                idx.push_back(obj);
            }
            return *this;
        }

        InitObj &operator = (int value) {
            if (idx.empty() != true) {
                for (auto len:idx){
                    data.resize(((unsigned int)len > data.size())? len:data.size());
                    data[len - 1] = value;
                }
                idx.clear();
            }
            return *this;
        }

        void show_val(void) {
            for_each(data.begin(), data.end(), [](T &t){
                    std::cout<<t << " ";
            });
            std::cout<<std::endl;
        }
    private:
        std::vector<int> idx;
        std::vector<T> data;
    };

    void test(void)
    {
        std::cout << "==============  INITIALIZATION ===============" << std::endl;

        std::vector <int> vec{1, 2, 3};
        for(auto val:vec) {
            std::cout<<val << " ";
        }
        std::cout << "\n";

        std::map<int, float> m = {{1, 0.1f}, {2, 2.0f}};
        for (const auto &pair : m) {
            std::cout << pair.first << ": " << pair.second << std::endl;
        }

        // 列表初始化
        user_initialize val{1, 2, 3};
        val.show();

        std::cout<<user_initialize::val1_ << std::endl;
        std::cout<<user_initialize::const_val_ << std::endl;
        
        InitObj<int> ibj2;
        ibj2[{1, 2, 5}] = 3;
        ibj2[{3, 4, 6, 7}] = 1;
        ibj2.show_val();

        // 单例模式
        Singleton::getInstance().show();

        //非静态数据成员的sizeof
        std::cout << "sizeof :" << sizeof(((user_initialize*)0)->val_) << std::endl;
        std::cout << "sizeof :" << sizeof(user_initialize::val_) << std::endl;
    }
}

namespace RAII
{
    class resource_ptr {
    private:
        int *ptr{nullptr};
    public:
        resource_ptr(int *p) : ptr(p) {
            std::cout << "constructor" << std::endl;
        }
        ~resource_ptr() { 
            if (ptr)    
                delete ptr;
            ptr = nullptr; 
            std::cout << "destructor" << std::endl;
        }

        int* get_ptr(void) {
            return ptr;
        }
    };

    void test(void)
    {
        std::cout << "==============  RAII ===============" << std::endl;

        {
            resource_ptr ptr(new int(10));
            std::cout<<*ptr.get_ptr() << std::endl;
        }
    }
}

using std::string;
namespace EXTRA
{
    //枚举类型的列表初始化
    enum byte : unsigned char {};

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

    //联合体T拥有一个非POD的成员s。
    //而string有非平凡的构造函数，因此T的构造函数被删除，其类型的变量t也就无法声明成功。
    //解决这个问题的办法是，由程序员自己为非受限联合体定义构造函数
    union T
    {
        int i;
        std::string s;
    public:
        T() { 
            new (&s) std::string("hello world!");
        }
        ~T() { 
            s.~string(); 
        }

        void show(void) {
            std::cout << s << std::endl;
        }
    };

    template<typename T = char>
    class info_object{
    public:
        friend T;
        info_object():name_(__func__){
            std::cout << "info_object Init No Parameter!" << std::endl;
        }
        ~info_object(){
            std::cout << "Dispose info_object" << std::endl;
        }
        virtual const char *show_parameter(void)
        {
            std::cout << "name: " << this->name_ << " ";
            std::cout << "version: " << this->version_ << " ";
            std::cout << "extra: " << this->extra_ << std::endl;
            return __func__;
        }

    protected:
        std::string name_;

    private:
        std::string version_{"0.0.1"};
        int extra_{1};
    };

    void test(void) 
    {
        std::cout << "==============  EXTRA ===============" << std::endl;

        byte b = byte{1};
        std::cout << "b: " << static_cast<int>(b) << std::endl;
        byte b1{255};
        std::cout << "b1: " << static_cast<int>(b1) << std::endl;

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

        T t;
        t.show();

        // 在C++里，允许在函数内部定义类，这种类被称作局部类
        class locality:public info_object<locality>{
        public:
            using info_object<locality>::info_object;
            locality() : info_object<locality>(){
                name_ = __func__;
            }
        };

        locality loc;
        loc.show_parameter();
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
