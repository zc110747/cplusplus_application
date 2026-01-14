# class

在C++中，类是一种用户定义的数据类型，它封装了数据和操作这些数据的函数。类是面向对象编程的基本构建块，它允许你将数据和操作这些数据的函数组合在一起，形成一个逻辑单元。

- [类的封装特性](#packaged)
  - [访问控制权限](#access_control)
  - [构造函数](#constructor)
     - [委托构造函数](#delegate_constructor)
     - [继承构造函数](#inherit_constructor)
  - [析构函数](#destructor)
  - [this指针](#this)
  - [friend友元函数和友元类](#friend)
  - [运算符重载](#operator_overload)
- [类的继承](#extend)
  - [继承访问控制](#inherit_access_control)
  - [多重继承](#multiple_inherit)
  - [虚继承](#virtual_inherit)
- [类的多态](#polymorphism)
  - [多态类型](#polymorphism_type)
  - [虚函数](#virtual_function)
  - [override关键字](#override)
  - [final关键字](#final)
  - [重写、重载和隐藏](#overload_hide)
- [类功能扩展](#feature_extend)
  - [const和mutable关键字](#const_mutable)
  - [列表初始化initialize_list](#initialize_list)
  - [非静态成员的初始化](#no_static_member_init)
  - [静态成员const和inline初始化](#static_member_init)
  - [单例模式](#singleton)
  - [类成员的sizeof](#sizeof_member)
  - [RAII资源管理](#raii)
  - [枚举类型的列表初始化和强枚举类型](#enum_extend)
  - [非受限联合体](#no_limit_union)
  - [函数体内类实现](#class_in_func) 
- [返回主页](../../README.md)

## packaged

封装是将数据（属性）和操作数据的方法（函数）捆绑在一起，形成一个类。通过封装，类的内部实现细节对外部是隐藏的，外部只能通过类提供的公共接口来访问和操作数据。

### access_control

对于封装来说，最重要的就是不同权限配合的访问控制权限，包含private、protected和public三种类型，具体如下所示。

- private: 类的私有成员只能在类的内部以及友元的类或者函数访问(friend)。
- protected: 除类内部、友元类或者友元函数外，还允许派生的类访问，这里派生类指的是直接或间接继承该类的类。
- public: 类的公共成员可以被任意对象、函数访问。

具体示例如下所示。

```cpp
// demo class
class demo {
public:
    /// constructor
    demo() {};

    /// destructor
    ~demo() {};

protected:
    /// protected member
    int protected_var_;

private:
    /// private member
    int private_var_;
};

int main(int argc, char *argv[]) {
    demo demo_obj;
    return 0;
}
```

### constructor

构造函数是一种特殊的成员函数，它在创建类的对象时被调用，用于初始化对象的成员变量。构造函数的名称与类的名称相同，并且没有返回类型。构造函数可以有参数，也可以没有参数。

- 默认构造函数（default），默认构造函数是一个特殊的构造函数，它在创建类的对象时被调用，并且不需要任何参数。如果没有显式定义构造函数，编译器会自动生成一个默认构造函数。也可以使用"constructor() = default"显示的告知编译器生成默认构造函数。其格式如下所示。

```cpp
/// default constructor
demo() = default;
```

- 拷贝构造函数（copy）和拷贝赋值运算符，拷贝构造函数用于创建一个新对象，该对象是现有对象的副本。具体格式如下所示。

```cpp
// copy constructor
demo(const demo& other) {
    // body
}

// copy assignment operator
demo& operator=(const demo& other) {
    // body
    return *this;
}
```

- 移动构造函数（move）和移动赋值运算符，移动构造函数是C++11引入的一个特性，它允许对象的资源（如内存、文件句柄等）从一个对象转移到另一个对象，而不是进行深拷。具体格式如下所示。

```cpp
// move constructor
demo(demo&& other) {
    // body
}

// move assignment operator
demo& operator=(demo&& other) {
    // body
    return *this;
}
```

- 删除构造函数（delete），使用delete关键字来显式地禁止编译器生成默认的构造函数。

```cpp
// 禁止拷贝构造函数和拷贝赋值运算符
demo(const demo&) = delete;
demo& operator=(const demo&) = delete;
```

- 显示构造函数（explicit），使用explicit关键字来显式地声明，以防止隐式转换。

```cpp
// 显示构造函数
explicit demo(int a) {}
```

- 委托构造函数（delegate），它允许一个构造函数调用另一个构造函数来初始化对象。

```cpp
// 委托构造函数，允许一个构造函数调用另一个构造函数来初始化对象
demo(int a, int b) {}
demo(int a) : demo(a, 0) {}
```
- 继承构造函数（inherit），它允许派生类直接继承基类的构造函数，而不需要在派生类中显式地定义这些构造函数。

```cpp
// 允许派生类直接继承基类的构造函数
using demo::demo;
```

具体示例如下所示。

```cpp
#include <iostream>

// demo class
class demo {
public:
    /// constructor
    demo() {};

    /// explicit constructor
    explicit demo(int var): private_var_(var) {};

    /// delegate constructor, other member should in function
    /// error C3511: a call to a delegating constructor shall be the only member-initializer
    demo(int var1, int var2): demo(var1){
        protected_var_ = var2;
    };

    /// copy constructor
    demo(const demo& other) {
        protected_var_ = other.protected_var_;
        private_var_ = other.private_var_;
        std::cout << "demo copy constructor"<< std::endl;
    };

    /// move constructor
    demo(demo&& other) {
        protected_var_ = other.protected_var_;
        private_var_ = other.private_var_;
        std::cout << "demo move constructor"<< std::endl;    
    };

    /// copy assignment operator
    demo& operator=(const demo& other) {
        protected_var_ = other.protected_var_;
        private_var_ = other.private_var_;
        return *this;
    };

    /// move assignment operator
    demo& operator=(demo&& other) {
        protected_var_ = other.protected_var_;
        private_var_ = other.private_var_;
        return *this;
    };

    /// destructor
    ~demo() {};

protected:
    /// protected member
    int protected_var_;

private:
    /// private member
    int private_var_;
};

class demo2: public demo {
public:
    /// inherit constructor
    using demo::demo;
};

class demo3: public demo {
public:
    /// default constructor
    demo3() = default;

    /// constructor，不使用using，需要在函数体中初始化基类
    demo3(int var): demo(var) {
    }    
};

class demo4 {
public:
    /// delete constructor
    demo4() = delete;

    /// delete copy constructor
    demo4(const demo4& other) = delete;

    /// delete move constructor
    demo4(demo4&& other) = delete;

    /// delete copy assignment operator
    demo4& operator=(const demo4& other) = delete;

    /// delete move assignment operator
    demo4& operator=(demo4&& other) = delete;

    /// explicit constructor
    explicit demo4(int var1) {
    }
};

int main(int argc, char *argv[]) 
{
    demo d1_1, d1_2(1), d1_3(1, 2);
    demo2 d2_1, d2_2(1), d2_3(1, 2);
    demo3 d3_1, d3_2(1);
    demo4 d4_2(1);

    return 0;
}
```

#### delegate_constructor

委托构造函数是一种特殊的构造函数，它允许一个构造函数调用另一个构造函数来初始化对象。委托构造函数的名称与类的名称相同，并且在参数列表中使用冒号（:）来调用另一个构造函数。

委托构造函数的语法如下所示。

```cpp
class demo {
public:
    demo(int x): x_(x) {}
    demo(int x, int y): demo(x){
        y_ = y;
    }

private:
    int x_;
    int y_;
};
```

对于委托构造函数，需要注意以下特性。

- 每个构造函数都可以委托另一个构造函数为代理，但是不能形成循环调用。
- 委托构造函数必须在构造函数的初始化列表中调用，不能在函数体中调用，在初始化列表中不能对其它数据成员和基类进行初始化。
- 委托构造函数的执行顺序是先执行代理构造函数的初始化列表、函数的主体，最后执行委托构造函数的主体。
- 委托构造函数执行完成后，委托构造函数主体抛出了异常，则自动调用该类型的析构函数。

可使用模板构造函数来实现通用的构造函数，这样在调用委托构造函数时可以简化操作。

具体示例如下所示。

```cpp
#include <iostream>
#include <vector>
#include <deque>
#include <list>

class demo {
public:
    /// constructor
    demo() : x_(0), y_(0) {};

    demo(int var1, double var2): x_(var1), y_(var2) {
        std::cout << "demo(int var1, double var2)" << std::endl;
    };

    demo(int va1): demo(va1, 0.0) {
        std::cout << "demo(int va1)" << std::endl;
    };

    demo(double val): demo(0, val) {
        std::cout << "demo(double val)" << std::endl;
    };

    void print(void) {
        std::cout << "x_: " << x_ << ", y_: " << y_ << std::endl;
    }
private:
    int x_;
    double y_;
};

class demo1
{
private:
    std::list<int> l_;

public:
    template<class T> demo1(T first, T last): l_(first, last) {}
    
    demo1(std::vector<int> vec): demo1(vec.begin(), vec.end()) {};
    
    demo1(std::deque<int> deq): demo1(deq.begin(), deq.end()) {};


    void print(void) {
        for (auto it = l_.begin(); it != l_.end(); it++) {
            std::cout << *it << " ";
        }
        std::cout<<std::endl;
    }
};

int main(int argc, const char *argv[])
{
    demo d1;
    demo d2(1);
    demo d3(1.0);
    demo d4(1, 2.0);

    d1.print();
    d2.print();
    d3.print();
    d4.print();

    demo1 d5(std::vector<int>({1, 2, 3}));
    demo1 d6(std::deque<int>({4, 5, 6}));

    d5.print();
    d6.print();
    return 0;
}
```

#### inherit_constructor

继承构造函数是一种特殊的构造函数，它允许派生类的构造函数调用基类的构造函数来初始化基类的成员。

对于继承构造函数，其格式如下所示。

```cpp
class Base {
public:
    Base() {}
};

/// inherit constructor
class Derived: public Base {
public:
    using Base::Base;

    /// derived call base constructor
    Derived(): Base() {}
};
```

对于继承构造函数，包含以下特性。

- 派生类是隐式继承了构造函数，只有程序中使用了构造函数才会为派生类生成，派生类不会继承基类的默认构造函数和复制构造函数。
- 继承构造函数不会影响派生类默认构造函数的隐式声明，也就是说对于继承基类构造函数的派生类，编译器依然会为其自动生成默认构造函数的代码。
- 在派生类中声明签名相同的构造函数，在使用继承构造函数时，会屏蔽相应的构造函数。
- 派生类继承多个签名相同的构造函数会导致编译失败，一般多重继承时有可能触发。
- 继承构造函数的基类构造函数不能为私有，否则无法编译通过。

具体示例如下所示。

```cpp
#include <iostream>

class demo
{
public:
    /// constructor
    demo() : x_(0), y_(0) {};

    demo(int var1, double var2): x_(var1), y_(var2) {
        std::cout << "demo(int var1, double var2)" << std::endl;
    };

    demo(int va1): demo(va1, 0.0) {
        std::cout << "demo(int va1)" << std::endl;
    };

    demo(double val): demo(0, val) {
        std::cout << "demo(double val)" << std::endl;
    };

protected:
    /// protected member
    int x_;

    double y_;
};

class demo1: public demo {
public:
    using demo::demo;

    demo1(int var1, double var2, float var3): demo(var1, var2) {
        z_ = var3;
    }

    void print(void) 
    {
        std::cout << x_ <<" " << y_ << " " << z_ << std::endl;
    }

private:
    float z_{0};
};

int main(int argc, const char *argv[])
{
    demo1 d1;
    demo1 d2(1);
    demo1 d3(1.2);
    demo1 d4(1, 2.1);
    demo1 d5(1, 2.1, 3.5);

    d1.print();
    d2.print();
    d3.print();
    d4.print();
    d5.print();
    return 0;
}
```

### destructor

析构函数是一种特殊的成员函数，它在销毁类的对象时被调用，用于释放对象的资源。析构函数的名称与类的名称相同，前面加上波浪号（~），并且没有参数和返回类型。

- 默认析构函数(default)，默认析构函数是一个特殊的析构函数，它在销毁类的对象时被调用，并且不需要任何参数。类中如果没有定义析构函数，编译器会自动生成默认析构函数。也可以使用"~destructor() = default;"来显式的生成默认的析构函数。

```cpp
// 默认析构函数(default)，使用default关键字来显式地指定编译器生成默认的析构函数。
~demo() = default;
```

- 删除析构函数(delete)，使用delete关键字来显式地禁止编译器生成默认的析构函数(只能被new申请，不能直接创建）。

```cpp
// 删除析构函数(delete)，使用delete关键字来显式地禁止编译器生成默认的析构函数(只能被new申请，不能直接创建）。
// 析构函数删除后，也不能通过delete来删除对象，可以用于单例模式限制删除对象，在程序结束后由系统回收。
~demo() = delete;
```

- 虚析构函数(virtual destructor), 虚析构函数是一种特殊的析构函数，它在销毁类的对象时被调用，用于释放对象的资源。虚析构函数使用virtual关键字修饰,用于保证派生类继承时，调用析构函数时可以正确地析构派生类的对象。如果不确定类是否会被其它类继承，一般都定义为虚析构函数。

```cpp
// 虚析构函数
virtual ~demo() {
}
```

具体示例如下所示。

```cpp
#include <iostream>

// demo class
class demo {
public:
    /// default constructor
    demo() = default;
    
    /// default destructor
    ~demo() = default;
};

class demo2 {
public:
    /// delete destructor
    ~demo2() = delete;
};

class demo_base {
public:
    /// virtual destructor
    virtual ~demo_base() {
        std::cout << "demo_base destructor" << std::endl;
    }
};

class demo_derived final : public demo_base {
public:
    /// default destructor
    ~demo_derived() {
        std::cout << "demo_derived destructor" << std::endl;
    }
};

int main(int argc, char *argv[]) 
{
    demo d1;

    // demo2 d2;        // use of deleted function 'demo2::~demo2()'
    demo2* pd2 = new demo2();

    demo_derived *pd = new demo_derived();
    demo_base *pb = pd;
    delete pb;           // 删除pb能够正确的调用demo_derived、demo_base的析构函数
    return 0;
}
```

关于删除构造函数和删除析构函数，其实现基础来自于C++的显示删除语法。在C++中，支持通过函数后面添加"= delete”来显式地禁止生成某类函数，可以用于普通函数，不过并没有具体意义，主要用于类中，可以指定限制类不支持某些操作，例如拷贝构造函数、移动构造函数、拷贝赋值运算符、移动赋值运算符、new运算符等。

具体示例如下所示。

```cpp
#include <iostream>

class demo1 {
public:
    /// delete new, 不支持通过new申请
    void *operator new(size_t size) = delete;

    demo1() = default;

    // 不支持拷贝构造函数
    demo1(const demo1& other) = delete;

    void test() {
        std::cout << "test" << std::endl;
    }
};

int main(int argc, char const *argv[])
{
    demo1 d1;

    d1.test();

    // demo1 *pd2 = new demo1();      // use of deleted function 'static void* demo1::operator new(size_t)'
    
    // demo1 d3(d1);               // use of deleted function 'demo1::demo1(const demo1&)'
    return 0;
}
```

### this

this指针是一个指向当前对象的指针，它在类的**非静态成员函数**中使用。this指针可以用于访问当前对象的成员变量和成员函数。如果了解过Python中的对象，就可以知道在对象函数接口中声明的self，指向当前对象；this指针和self功能很相似，是指向当前对象的指针，用于在成员函数内部访问对象的其它成员。

对于this具有如下特性：

- this指针是一个指针，指向当前对象的地址。
- this指针只能在非静态成员函数内部使用。静态成员函数不与对象绑定，因此内部没有this指针。
- 对于特定的对象，其地址固定的，因此this也是常量指针，不能够修改。

this的主要用途如下所示。

- 在成员函数中区分出类变量(支持this访问或直接访问)和输入变量。
- 通过*this返回，可以实现链式调用。
- 获取对象本身的指针。

具体示例如下所示。

```cpp
#include <iostream>

class demo
{
public:
    demo(int x): x_(x) {};

    // 获取x_的值
    int get_x(void) const {
        return this->x_;
    }

    // 设置x_的值
    void set_x(int x) {
        this->x_ = x;
    }

    // 链式调用，返回当前对象的引用
    demo& add(int x) {
        this->x_ += x;
        return *this;
    }

    // 获取当前对象本身指针
    demo *get_self() {
        return this;
    }

    void test() {
        std::cout << "test" << std::endl;
    }

private:
    int x_;
};

int main(int argc, char *argv[]) 
{
    demo d1(1);
    std::cout << d1.get_x() << std::endl;
    d1.set_x(2);
    std::cout << d1.get_x() << std::endl;

    // 链式调用
    d1.add(1).add(2).add(3);
    std::cout << d1.get_x() << std::endl;

    // 获取自身的指针
    d1.get_self()->test();

    // 此时传递到test中的this为nullptr，不过内部未使用，工作正常
    static_cast<demo*>(nullptr)->test();
    return 0;
}
```

### friend

友元是一种特殊的访问权限，它允许一个类或函数访问另一个类的私有成员和保护成员。友元可以是类、类的静态函数或者普通函数等。

- 友元函数是一个普通函数，但它可以访问类的所有成员。友元函数在类的定义中声明，但它不是类的成员函数。
- 友元类是一个类，它的所有成员函数都可以访问另一个类的私有成员和保护成员。友元类在类的定义中声明指定，被指定的类具有此类的全部访问权限。

友元一定程度上破坏了类的封装性，它允许类的外部函数访问类的私有成员和保护成员。不过在特定的场景下，如测试框架、调试打印等场景，友元函数可以提供方便的访问方式，简化了操作，C++种友元不是洪水猛兽，而是一种可选的工具；在理解自己需求时妥善使用，才是最好的选择。

对于C++11来说，friend声明一个类是另一个类的友元时，可以不在使用class关键字，同样能够使用typedef或者using定义的类别名。基于此改进，可以为类模板声明友元，格式如下所示。

```cpp
// 模板声明友元
// 如果T类型是基础类型如int、float则被忽略
// T类型是类时则能够访问demo的所有变量。
template <class T>
class demo {
public:
    friend T;
};
```

关于友元的具体示例如下所示。

```cpp
#include <iostream>

class friend_class;

//typedef friend_class FC1;
using FC1 = friend_class;

template<typename T = int>
class demo {
public:
    /// constructor
    demo() = default;

    /// constructor
    demo(int a) : var_(a) {}
    
    /// friend function
    friend void friend_func(demo<>& obj);

    /// friend class
    friend FC1;

    // friend class
    friend T;
    
    /// member function
    friend demo operator+(demo& obj1, demo& obj2)
    {
        return demo(obj1.var_ + obj2.var_);
    }
private:
    int var_{-1};
};

class friend_class {
public:
    void func(demo<>& obj) {
        std::cout << obj.var_ << std::endl;
    }
};

class friend_test_class {
public:
    void func(demo<friend_test_class>& obj) {
        std::cout << obj.var_ << std::endl;
    }
};

void friend_func(demo<>& obj) {
    std::cout << obj.var_ << std::endl;
}

int main(int argc, char* argv[])
{
    // 测试友元函数
    demo<> obj;
    friend_func(obj);

    friend_class fc;
    fc.func(obj);

    auto&& obj1 = obj + obj;
    friend_func(obj1);

    // 测试友元类模板
    demo<friend_test_class> obj2;
    friend_test_class test;
    test.func(obj2);

    return 0;
}
```

### operator_overload

算符重载允许你为自定义类型定义特定的运算符行为。这使得你可以像使用内置类型一样使用自定义类型的对象。

常见能够重载的运算符有：

1. 算术运算符（+、-、*、/、%）
2. 赋值运算符（=、+=、-=、*=、/=、%=）
3. 比较运算符（==、!=、<、>、<=、>=）
4. 逻辑运算符（&&、||、!）
5. 自增自减运算符（++、--）
6. 位运算符（&、|、^、~、<<、>>）
7. 下标运算符（[]）
8. 函数调用运算符（()）
9. 输入输出运算符（<<、>>）

对于运算符重载，C++提供了以下几种方式：

```cpp
// 类成员函数重载
demo operator+(const demo& other) const {
    return demo(this->var_ + other.var_);
}

// 友元函数重载
friend demo operator+(const demo&obj1, const demo&obj2) {
    return demo(obj1.var_ + obj2.var_);
}
```

具体示例如下所示。

```cpp
/// demo class
class demo {
public:
    /// friend function
    friend int operator-(const demo& obj1, const demo& obj2);

    /// default constructor
    demo() = default;
    
    /// default destructor
    ~demo() = default;

    /// operator+
    int operator+(const demo& other) {
        return this->var_ + other.var_;
    }

private:
    int var_{-1};
};

int operator-(const demo& obj1, const demo& obj2) {
    return obj1.var_ + obj2.var_;
};

int main(int argc, char* argv[]) 
{
    demo obj1;
    demo obj2;
    auto result1 = obj1 + obj2;
    auto result2 = obj1 - obj2;

    return 0;
}
```

## inheritance

继承是指一个类（称为派生类或子类）从另一个类（称为基类或父类）继承属性和行为。通过继承，派生类可以重用基类的代码，并且可以在其基础上添加新的功能或修改原有功能。

### inherit_access_control

对于继承来说，也支持不同权限的继承方式，包含private、protected和public三种，具体如下所示。

| 类成员/派生方式 | public | protected | private |
| --- | --- | --- | --- |
| 基类public成员 | public | protected | private |
| 基类protected成员 | protected | protected | private |
| 基类private成员 | 不可见 | 不可见 | 不可见 |

```cpp
#include <iostream>

class base
{
public:
    int a{0};

protected:
    int b{0};

private:
    int c{0};
};

class derived : public base {
    void print() {
        std::cout << a << std::endl;     // public
        std::cout << b << std::endl;     // protected
        // std::cout << c << std::endl;  // private, 不可见
    }
};

class derived1 : protected base {
public:
    void print() {
        std::cout << a << std::endl;     // protected
        std::cout << b << std::endl;     // protected
        // std::cout << c << std::endl;  // private, 不可见
    }
};

class derived2 : private base {
public:
    void print() {
        std::cout << a << std::endl;     // private
        std::cout << b << std::endl;     // private
        // std::cout << c << std::endl;  // private, 不可见
    }
};

int main(int argc, char const *argv[])
{
    derived d;
    std::cout << d.a << std::endl;

    derived1 d1;
    // std::cout << d1.a << std::endl;  // protected, 不可见
    d1.print();

    derived2 d2;
    //std::cout << d2.a << std::endl; // private, 不可见
    d2.print();
}
```

### multiple_inherit

多重继承是指一个类可以从多个基类继承属性和方法。在C++中，一个派生类可以继承多个基类，从而获得这些基类的所有成员。

```cpp
#include <iostream>

/// a class
class a {
public:
    int a{0};
};

/// b class
class b {
public:
    int b{0};
};

/// c class
class c : public a, public b {};

int main(int argc, char* argv[])
{
    c c_obj;

    c_obj.a = 1;
    c_obj.b = 2;
    std::cout << c_obj.a << " " << c_obj.b << std::endl;
    return 0;
}
```

### virtual_inherit

虚继承是C++中的一种特殊继承方式，用于解决多重继承中的菱形继承问题。

菱形继承的场景如下，B、C继承于D，B、C都继承于A；此时D类中会有两个A的成员，就会有二义性，被称为菱形继承。使用虚继承可以解决这个问题，虚继承会将A类的成员共享给B和C类，从而解决二义性问题。

```cpp
#include <iostream>

/// a class
class a {
public:
    int a{0};
};

/// b class
class b : virtual public a {};

/// c class
class c : virtual public a {};

/// d class
class d : public b, public c {};

int main(int argc, char* argv[])
{
    d d_obj;
    d_obj.a = 1;
    std::cout << d_obj.a << std::endl;
    return 0;
}
```

## polymorphism

多态是指同一个接口可以被不同的对象实现，从而实现不同的行为。在C++中，多态主要通过虚函数和继承来实现, 核心思想是：同一个接口，不同的实现。

### polymorphism_type

按照动态类型的区别，主要分为静态多态和动态多态。

- 静态多态（编译时多态）是在编译阶段就确定要调用的函数，主要通过函数重载和运算符重载来实现。
- 动态多态（运行时多态）是在运行时根据对象的实际类型来决定调用哪个函数，主要通过虚函数和继承来实现。

具体示例如下所示。

```cpp
#include <iostream>

/// demo class
class demo {
public:
    /// default constructor
    demo() = default;

    /// default destructor
    virtual ~demo() = default;

    /// virtual function
    virtual void func() {
        std::cout << "demo func" << std::endl;
    }

    /// 函数重载
    void func(int a) {
        std::cout << "demo func " << a << std::endl;
    }
};

/// demo2 class
class demo2 : public demo {
public:
    /// default constructor
    demo2() = default;

    /// default destructor
    ~demo2() = default;

    /// virtual function
    void func() override {
        std::cout << "demo2 func" << std::endl;
    }
};

class demo3 : public demo {
public:
    /// default constructor
    demo3() = default;

    /// default destructor
    ~demo3() = default;

    /// virtual function
    void func() override {
        std::cout << "demo3 func" << std::endl;
    }
};

int main(int argc, char* argv[])
{
    demo* p_demo2 = new demo2();
    demo* p_demo3 = new demo3();

    p_demo2->func();
    p_demo2->func(2);
    p_demo3->func();

    demo2 *p_demo2_1 = dynamic_cast<demo2*>(p_demo2);
    demo3 *p_demo3_1 = dynamic_cast<demo3*>(p_demo3);

    p_demo2_1->func();
    p_demo3_1->func();

    delete p_demo2;
    delete p_demo3;
    return 0;
}
```

### virtual_function

虚函数是C++中实现多态的一种机制，它允许在基类中定义一个函数，并且在派生类中重写该函数。当通过基类指针或引用调用该函数时，实际调用的是派生类中重写的函数，而不是基类中的函数。

1. 虚函数必须在基类中声明为virtual，并且在派生类中重写时也必须使用virtual关键字。
2. 虚函数可以在基类中定义，也可以在派生类中定义。
3. 虚函数可以被继承，并且可以在派生类中重写。
4. 虚函数可以被动态绑定，即通过基类指针或引用调用虚函数时，实际调用的是派生类中重写的函数。

纯虚函数是一种特殊的虚函数，它没有函数体，并且在基类中必须被重写。纯虚函数的声明方式是在函数声明的末尾加上=0；具体格式如下所示。

```cpp
// 纯虚函数
virtual void func() = 0;
```

接口类是一种特殊的类，它只包含纯虚函数，并且没有数据成员。接口类的目的是为了定义一组操作，而不提供具体的实现。接口类不能被实例化，但可以被继承用于限定派生类需要支持的操作。

```cpp
// 接口类
class virtual_demo {
public:
    /// 纯虚函数
    virtual void func1() = 0;
};
```

虚函数表是一个存储类成员函数指针的数组，每个拥有虚函数的对象都会有一个指向该类虚函数表的指针（通常被叫做虚表指针，vptr）。虚函数表在编译时生成，在运行时对象会生成虚函数指针(vptr)指向虚函数表。

具体示例如下所示。

```cpp
#include <iostream>

/// 接口类
class virtual_demo {
public:
    /// 纯虚函数
    virtual void func1() = 0;
};

/// 派生类
class demo : public virtual_demo {
public:
    void func1() override { 
        std::cout << "func1" << std::endl;
    }
};

using FuncPtr = void(*)();

void printVTable(demo* obj) {
    
    // 获取对象的虚表指针
    void*** vptr = reinterpret_cast<void***>(obj);
    
    // 获取虚函数表
    void** vtable = *vptr;

    // 调用虚函数表中的函数
    std::cout << "Calling functions from vtable:" << std::endl;
    FuncPtr func1 = reinterpret_cast<FuncPtr>(vtable[0]);
    func1();
}

int main(int argc, char *argv[]) 
{
    demo d1;
    d1.func1();

    printVTable(&d1);
    return 0;
}
```

虚析构函数是虚函数的重要应用场景，是实现多态安全析构的关键机制。当基类指针指向派生类对象时，只有通过虚析构函数才能确保完整释放派生类资源。任何可能被继承的类从设计上都应该被声明为虚析构，这样可以避免派生类继承时资源泄漏。

具体示例如下所示。

```cpp
#include <iostream>

/// Base class
class Base {
public:
    virtual ~Base() { 
        std::cout << "Base destructor" << std::endl;
    }
};

class Derived : public Base {
private:
    int *ptr;

public:
    Derived() : ptr(new int(10)) {}

    ~Derived() {
        delete ptr; 
        std::cout << "Derived destructor" << std::endl;
    }
};

int main(int argc, char *argv[]) 
{
    Base* p_base = new Derived();
    delete p_base;  // 调用派生类析构函数，在调用基类的析构函数，确保完整释放资源
    return 0;
}
```

### override

override是一个标识符，它用于指示某个函数是从基类继承而来的，并且在派生类中被重写基类中的相应虚函数。

- override函数需要声明在派生类中。
- override函数需要重写基类中的虚函数(基类中标明为虚函数，否则无法重写)。

通过override标识，可以确保派生类种必然重写了基类中的虚函数；如果不存在，则编译会报错。通过编译器检查，可以降低拼写错误导致未正确重写的风险。

具体示例如下所示。

```cpp
#include <iostream>

/// virtual_demo
class virtual_demo {
public:
    virtual void func1() {
        std::cout << "virtual_demo" << std::endl;
    }
};

/// demo
class demo: public virtual_demo {
public:
    void func1() override { // 必须重写基类中的虚函数
        std::cout << "demo" << std::endl;
    }
};

int main(int argc, char *argv[]) 
{
    demo d1;
    d1.func1();

    return 0;
}
```

### final

final是一个关键字，用于指定类或者虚函数不能被继承或重写。

- 当final用于类时，表示该类不能被继承。
- 当final用于函数时，表示该虚函数不能在派生类中被重写。

final关键字可以保证类或类中的虚函数不会被重写，可以降低被错误运用的风险。

具体示例如下所示。

```cpp
#include <iostream>

class demo final {
public:
    void func1() {
        std::cout << "demo" << std::endl;
    }
};

// 编译报错，demo类不能被继承
// class demo_derived: public demo {
// };

/// virtual_demo
class virtual_demo {
public:
    virtual void func1() final {
        std::cout << "virtual_demo" << std::endl;
    }
};

class demo_derived: public virtual_demo {
// public:
//     void func1() override { // 编译报错，不能重写final函数
//         std::cout << "demo_derived" << std::endl;
//     }
};

int main(int argc, char *argv[]) 
{
    demo d1;
    d1.func1();

    virtual_demo v1;
    v1.func1();

    return 0;
}
```

### overload_hide

对于C++中，对于类和函数，包含重要的特性：包含重写、重载和隐藏三种语法。

- 重写(overrider)是指在派生类中重新定义基类的虚函数，这样调用派生类时重写基类的函数，这里的覆盖需要满足有相同的函数名、参数列表和返回类型。
- 重载(overload)是指在同一个作用域内定义多个同名函数，它们的函数名相同，但是参数列表不同，例如类有多个不同参数的构造函数。
- 隐藏(overhide)是指在派生类中定义一个与基类同名不带virtual的函数，会隐藏基类中所有相同名称的函数，不能够直接调用。参数不同的同名函数也会隐藏，如果仍然使用此函数，则需要使用using关键字引入派生类。

其中重载是基于C++的函数调用性质决定的，编译时函数会根据函数名、参数列表和返回值组合成标识符，这样不同参数类型的函数就直接被识别成不同的函数；因此函数重载不仅支持在类中，也支持在全局作用域中。重写是基于虚函数的性质，只有在基类中声明为虚函数的函数才可以在派生类中被重写。如果不存在virtual，则派生类会隐藏基类中所有同名称的函数(参数不同的也会隐藏)，如果派生类仍然希望访问基类的同名函数，则使用using关键字引入。

具体示例如下所示。

```cpp
#include <iostream>

/// class base
class base {
public:
    void func0() {
        std::cout << "base func0" << std::endl;
    }

    // 函数重载(overload)
    void func0(int a) {
        std::cout << "base func0 with value" << std::endl;
    }

    virtual void func1(int i) {
        std::cout << "base func1 val" << std::endl;
    } 

    virtual void func1() {
        std::cout << "base func1" << std::endl;
    }

    void func2(int i) {
        std::cout << "base func2 val" << std::endl;
    }

    void func2() {
        std::cout << "base func2" << std::endl;
    }
};

/// class b
class derived : public base {
public:
    /// 重写(override)
    void func1() override {
        std::cout << "derived func1" << std::endl;
    }

    /// 隐藏(overhide)
    void func2() {
        std::cout << "derived func2" << std::endl;
    }
};

class derived1 : public base {
public:
    // func2会隐藏基类中所有func2函数，希望调用基类的func2函数，需要使用using关键字引入
    using base::func2; 

    void func2() {
        std::cout << "derived func2" << std::endl;
    }
};

int main(int argc, char *argv[])
{
    base b1;
    derived d1;
    derived1 d2;

    b1.func0();
    b1.func0(1);
    
    d1.func1();
    d1.func2();
    // d1.func2(1);       // func2(1)也被隐藏
    d1.base::func2();     // 调用基类被隐藏的func2

    d2.func2();
    d2.func2(1);
    return 0;
}
```

## feature_extend

### const_mutable

对于const关键字，可以使用在类变量、类函数参数、类函数返回值和类函数后方。

- const在类变量中，表示变量只读。
- const声明在函数中时，根据位置不同如下所示。
  - 在函数参数上，表示参数在函数内部不允许修改。
  - 在函数返回值前时，表示返回值不能够修改。
  - 在函数后方，表示函数体内this指针不允许修改(函数内部不能够修改变量)；当然如果希望修改，可以在类变量声明前添加mutable关键字。

mutable关键字是一个属性修饰符，它允许类的某个数据成员在常量对象（const对象）中被修改。

```cpp
#include <iostream>

class demo {
public:
    /// default constructor
    demo() = default;

    /// default destructor
    ~demo() = default;

    /// const function
    /// 函数体内this指针不允许修改(函数内部不能够修改变量)；当然如果希望修改，可以在类变量声明前添加mutable关键字。
    int func(void) const {
        var_ = 1;
        
        /// var2_ = 1; // error C3491: 'var2_': cannot assign to a variable that is const
        return var_;
    }

    const int func1(const int val) {
        return val;
    }
private:
    /// var_
    mutable int var_{-1};   // 声明为mutable, 表示在const函数中可以修改

    const int var1_{1};   // 声明为const, 表示在const函数中不能修改

    /// var2_
    int var2_ = 1;
};

int main(int argc, char* argv[])
{
    demo obj;
    
    std::cout << "var_: " << obj.func() << std::endl;
    std::cout << "var1_: " << obj.func1(1) << std::endl;
    return 0;
}
```

### initialize_list

列表初始化是一种在C++中初始化对象的方法，它允许你使用花括号{}来初始化对象的成员。使用std::initializer_list作为参数的构造函数，可以扩展自定义类型支持列表初始化。

std::initializer_list参考网址: <https://en.cppreference.com/w/cpp/header/initializer_list.html>

std::initializer_list支持的方法如下所示。

| 方法 | 描述 |
| --- | --- |
| size_t size() const | 返回初始化列表中的元素数量 |
| const T* begin() const | 返回指向初始化列表第一个元素的指针 |
| const T* end() const | 返回指向初始化列表最后一个元素之后位置的指针 |

关于C++支持列表初始化的对象如下所示。

- 对于C++的基础类型，如int、double、float等，可以直接使用花括号{}进行初始化。

```cpp
// 基础类型
int var = {1};
int var1{1};

int arr[] = {1, 2, 3};
int arr1[]{1, 2, 3};
```

- 对于C++标准模板库和标准库中的对象，如vector、map等，已经支持了列表初始化。

```cpp
// std::vector 初始化
std::vector<int> vec = {1, 2, 3};
std::vector<int> vec1{1, 2, 3};

// std::map 初始化
std::map<int, std::string> map = {{1, "one"}, {2, "two"}};
std::map<int, std::string> map1{{1, "one"}, {2, "two"}};
```

- 对于自定义的class类型，可以引入std::initializer_list作为参数的构造函数，支持列表初始化。

```cpp
// 自定义class支持列表初始化
demo(std::initializer_list<int> list) {
    for (auto &val : list) {
        vec_.push_back(val);
    }
};
```

- 对于同时支持列表初始化和构造函数的类初始化，使用{}优先调用std::initializer_list作为参数的构造函数。

具体示例如下所示。

```cpp
#include <initializer_list>
#include <iostream>
#include <vector>
#include <string>

/// vec_str class
class vec_str {
public:
    vec_str() = default;

    vec_str(std::initializer_list<std::string> list) {
        vec_.assign(list.begin(), list.end());
    }

    void print() {
        for (auto &str : vec_) {
            std::cout << str << " ";
        }
        std::cout << std::endl;
    }

private:
    std::vector<std::string> vec_;
};

/// demo class
class demo {
public:
    /// default constructor
    demo() = default;

    /// default destructor
    ~demo() = default; 

    /// constructor
    demo(int var1, int var2) : var1_(var1), var2_(var2) {}

    /// initializer list constructor
    demo(std::initializer_list<int> list) {
        var1_ = list.begin()[0];
        var2_ = list.begin()[1];   
    };

    void print(void) {
        std::cout << var1_ << " " << var2_ << std::endl;
    }
private:
    /// 非静态成员的立即初始化
    int var1_ = 1;
    
    int var2_{2};
};

class demo2 {
public:
    demo2() = default;

    demo2(int var1, int var2) {
        auto val = var1 + var2;
        vec_.push_back(val);
    }

    void print(void) {
        for (auto &val : vec_) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

protected:
    std::vector<int> vec_;
};

class demo3: public demo2 {
public:
    using demo2::demo2;
    demo3() = default;
    demo3(std::initializer_list<int> list) {
        vec_.assign(list.begin(), list.end());
    }
};

class demo4 {
public:
    demo4 &operator [](std::initializer_list<int> l) {
        for (const auto& i: l) {
            vec_index_.push_back(i);
        }
        return *this;
    }

    demo4 &operator = (int val) {
        if (!vec_index_.empty()) {
            for (const auto& i: vec_index_) {
                if (i > vec_value_.size()) {
                    vec_value_.resize(i);
                }
                vec_value_[i-1] = val;
            }
            vec_index_.clear();
        }
        return *this;
    }

    void print(void) {
        for (const auto& val : vec_value_) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
private:
    std::vector<int> vec_index_;
    std::vector<int> vec_value_;
};

int main(int argc, char* argv[])
{
    // 基础类型
    int var = {1};
    int var1{1};

    int arr[] = {1, 2, 3};
    int arr1[]{1, 2, 3};

    demo obj = {1, 2};
    obj.print(); 
    
    vec_str vec_obj = {"hello", "world"};
    vec_obj.print();

    // 不支持列表初始化，调用构造函数
    demo2 obj2 = {1, 2};
    obj2.print();       // 3

    // 支持列表初始化，调用std::initializer_list作为参数的构造函数
    demo3 obj3 = {1, 2};
    obj3.print();       // 1 2

    // 基础类型支持列表初始化
    int *i1 = new int{1};
    double *d1 = new double{1.0f};
    char *c1 = new char{'a'};

    std::cout << *i1 << " " << *d1 << " " << *c1 << std::endl;
    delete i1;
    delete d1;
    delete c1;

    demo4 obj4;
    obj4[{1, 3}] = 2;
    obj4[{2, 4, 5}] = 1;
    obj4.print();

    return 0;
}
```

### no_static_member_init

类的非静态成员的立即初始化是指在类的定义中为非静态成员变量提供初始值，其具有如下特性。

- 类的非静态成员可以在类的定义中使用`等号=`或`花括号{}`来进行初始化。
- 对于位域，也支持通过`等号=`或`花括号{}`进行初始化。
- 对于联合体，也支持类的非静态成员立即初始化。
- 类对象的初始化顺序为"立即初始化 => 成员初始化列表 => 构造函数调用", 结果以最后一步的值为准。

具体示例如下所示。

```cpp
#include <iostream>

/// demo class
class demo {
public:
    /// constructor
    demo(): var1_(2) {
        var1_ = 3;
    }

    /// default destructor
    ~demo() = default; 

    /// 
    void show(void) {
        std::cout << var1_ << std::endl;
    }

private:
    /// 非静态成员的立即初始化
    int var1_ = 1;

    int var2_{2};
};

int a = 1;
struct bitfield {
    int x:8 = 1;
    int y:8 {2};
    int z: (true?8 : a) = 24;
};

union demo_union {
    int var1_ = 0x1234;

    struct {
        int x:8;
        int y:8;
    }bits;
};

int main(int argc, char* argv[])
{
    demo obj;

    /// 打印结果
    obj.show();

    bitfield field;
    std::cout << field.x << std::endl;
    std::cout << field.y << std::endl;
    std::cout << field.z << std::endl;

    demo_union u1;
    std::cout << std::hex;
    std::cout << "0x" << u1.bits.x << std::endl;
    std::cout << "0x" << u1.bits.y << std::endl;

    return 0;
}
```

### static_member_init

类的静态数据成员支持const和inline初始化(C++17)。

- 类的静态数据成员如果是const类型，可以直接立即初始化。

```cpp
// 类的静态数据成员如果是const类型，可以直接立即初始化。
class demo {
    const static int val_{0};
};
```

- 类的静态数据成员如果是非const类型，可以使用inline进行立即初始化(C++17支持)。

```cpp
// 类的静态数据成员如果是非const类型，可以使用inline进行立即初始化。
class demo {
    inline static int val_{0};
};
```


具体示例代码如下所示。

```cpp
/// demo class
class demo {
public:
    /// default constructor
    demo() = default;
    /// default destructor
    ~demo() = default;

private:
    /// const静态成员的立即初始化
    const static int var1_ = 1;

    /// 静态成员的立即初始化
    inline static int var2_{2};
};

int main(int argc, char* argv[])
{
    demo obj;
    return 0;
}
```

### singleton

单例模式是一种设计模式，它确保一个类只有一个实例，并提供一个全局访问点来访问该实例。在C++11后，单例模式可以使用内联静态成员变量来实现，可以保证只有一个实例被创建。

```cpp
#include <iostream>

/// 单例模式
class Singleton {
public:
    /// 获取单例实例
    static Singleton *getInstance() {
        static Singleton instance;
        return &instance;
    };

    void show(void) {
        std::cout << "Singleton" << std::endl;
    }
};

class Singleton2 {
public:
    /// 获取单例实例
    static Singleton2* getInstance() {
        if (pinstance_ == nullptr) {
            pinstance_ = new Singleton2();
        }
        return pinstance_;
    }

    void show(void) {
        std::cout << "Singleton2" << std::endl;
    }
private:
    inline static Singleton2* pinstance_{nullptr};
};

int main(int argc, char* argv[])
{
    Singleton::getInstance()->show();
    Singleton2::getInstance()->show();
    return 0;
}
```

### sizeof_member

对于类的静态成员变量，既可以使用sizeof(对象.成员名)来获取成员变量的长度，也可以使用sizeof(类名::成员名)来获取成员变量的长度。而非静态成员，在C++11之前只能使用sizeof(对象.成员名)来获取成员变量的长度，C++11之后也可以使用sizeof(类名::成员名)来获取成员变量的长度。

```cpp
#include <iostream>

class demo {
public:
    /// default constructor
    demo() = default;
    /// default destructor
    ~demo() = default;  
        
    int var1_;

    static int var2_;
};

int main(int argc, char* argv[])    
{
    demo d1;

    // 非静态成员变量的sizeof
    std::cout << "nullptr sizeof(var1_) = " << sizeof(static_cast<demo *>(nullptr)->var1_) << std::endl;    // 4
    std::cout << "d1.var1_ sizeof = " << sizeof(d1.var1_) << std::endl;                                     // 4
    std::cout << "demo::var1_ sizeof = " << sizeof(demo::var1_) << std::endl;                               // C++11后支持

    // 静态成员变量的sizeof
    std::cout << "nullptr sizeof(var2_) = " << sizeof(static_cast<demo *>(nullptr)->var2_) << std::endl;     // 4
    std::cout << "d1.var2_ sizeof = " << sizeof(d1.var2_) << std::endl;                                      // 4
    std::cout << "demo::var2_ sizeof = " << sizeof(demo::var2_) << std::endl;                                // 4
    return 0;
}
```

### raii

RAII全称是Resource Acquisition Is Initialization，是一种C++编程技术，用于管理资源的生命周期。RAII的核心思想是将资源的获取和释放与对象的生命周期绑定在一起，确保资源在对象创建时被获取，在对象销毁时被释放。其利用的是C++的构造函数和析构函数的机制，在对象创建时获取资源，在对象销毁时释放资源。
RAII的重要应用是智能指针和std::lock_guard，会在相应章节进行说明。

具体示例如下所示。

```cpp
#include <iostream>

/// ResourceHolder class
template<typename T>
class ResourceHolder {
public:
    /// constructor
    ResourceHolder(T* resource) : resource_(resource) {}
    
    /// destructor
    ~ResourceHolder() {
        if (resource_ != nullptr) {
            std::cout << "delete resource_" << std::endl;
            delete resource_; 
        }
    }
private:
    T* resource_{nullptr};
};

int main(int argc, char* argv[]) 
{
    {
        ResourceHolder<int> holder(new int(10));
    }
    
    return 0;   
}
```

## enum_extend

对于C++中，枚举的类型、枚举的成员遍历都是全局可见的，这样如果不同的枚举类型定义了相同的枚举标识符，那么就会产生冲突。另外，枚举的成员也是全局可见，因此匿名枚举也可以被支持。

关于枚举类型的应用，具体示例如下所示。

```cpp
#include <iostream>

/// 枚举类型
enum Color {
    RED,
    GREEN,
    BLUE
};

// 匿名枚举类型
enum { Male, Female };

int main(int argc, const char* argv[]) 
{
    Color c1 = RED;
    Color c2 = {GREEN};
    Color c3 = {BLUE};

    std::cout << (c1 == RED) << std::endl;
    std::cout << (c2 == GREEN) << std::endl;
    std::cout << (c3 == BLUE) << std::endl;

    auto c4 = Male;
    auto c5 = Female;
    if (c4 == Male) {
        std::cout << "Male" << std::endl;
    }

    if (c5 == Female) {
        std::cout << "Female" << std::endl;
    }
    return 0;
}
```

枚举类型在处理时，可以被隐式转换为整型，缺乏类型检查，这会导致一些潜在的错误；另外内部变量也会暴露到全局作用域，这会导致命名冲突。

### enum_class

强枚举类型（enum class）是C++11引入的一种新的枚举类型，它提供了更强的类型安全性和更好的封装性。

强枚举类型具有以下特征：

- 枚举标识符属于强枚举类型的作用域，允许不同强枚举类型的枚举标识符具有相同的名称，因此匿名强枚举类型可以被声明，但无法被赋值。
- 强枚举类型不会隐式转换为整型，需要使用作用域运算符来访问枚举标识符。
- 可以指定强枚举类型的底层类型，默认是int类型；对于C++11，普通枚举类型也支持指定底层类型，不过没有默认指定类型。
- C++17开始，指定了类型的枚举和强枚举类型，可以支持使用整型进行直接列表初始化。

另外使用using关键字可以打开强枚举类型，简化操作。

具体示例如下所示。

```cpp
#include <iostream>

/// 强枚举类型
enum class Color {
    RED,
    GREEN,
    BLUE
};

enum class Color2 {
    RED = 1,
    GREEN = 4,
    BLUE = 9
};

enum class Color3: unsigned int {
    RED = 1,
    GREEN = 2,
    BLUE = 12
};

enum Color4: unsigned int {
    RED = 1,
    GREEN = 2,
    BLUE = 12
};

int main(int argc, const char* argv[]) 
{
    Color c1 = Color::RED;
    Color c2 = Color::GREEN;
    Color c3 = Color::BLUE;

    Color2 c4 = Color2::RED;
    Color2 c5 = Color2::GREEN;
    Color2 c6 = Color2::BLUE;

    std::cout << std::boolalpha;
    std::cout << (c1 < c2) << std::endl;
    std::cout << (c4 != c5) << std::endl;

    Color3 c7 = Color3::RED;

    if (c7 == Color3::RED) {
        std::cout << "c7 is RED" << std::endl;
    } else if (c7 == Color3::GREEN) {
        std::cout << "c7 is GREEN" << std::endl;
    } else if (c7 == Color3::BLUE) {
        std::cout << "c7 is BLUE" << std::endl;
    } else {
        std::cout << "c7 is unknown" << std::endl;
    }

    // 非强枚举类型可以隐式转换为整型
    Color4 c8 = Color4::RED;
    if (c8 == 1) {
        std::cout << "c8 is RED" << std::endl;
    } else if (c8 == 2) {
        std::cout << "c8 is GREEN" << std::endl;
    } else if (c8 == 12) {
        std::cout << "c8 is BLUE" << std::endl;
    } else {
        std::cout << "c8 is unknown" << std::endl;
    }

    // 指定底层类型的枚举、强枚举类型支持使用底层类型进行直接列表初始化(C++17)
    Color2 c9{Color2::RED};
    Color2 c10{1};  // 底层类型为int，1可以直接转换为Color2::RED
    std::cout << (c9 == c10) << std::endl; // true

    // 使用using enum可以直接访问枚举标识符，无需使用作用域运算符
    using enum Color2;
    Color2 c11{GREEN};
    std::cout << (c11 == Color2::GREEN) << std::endl; // true
}
```

### no_limit_union

联合体中的成员存在非平凡类型，如std::vector、std::string等，默认构造和析构函数将被隐式删除；此时需要指定联合体的构造函数和析构函数，并在其中处理非平凡类型，从而任何非引用类型都可以成为联合体的数据成员，这样的联合体即所谓的非受限联合体。

对于非受限联合体，具有以下特点。

- 突破传统联合体只能是POD类型的限制。
- 手动内存管理，需要显示调用非平凡类型的构造函数和析构函数。
- 默认函数删除，编译器会自动删除默认构造、析构函数，需要手动实现。

具体示例如下所示。

```cpp
#include <string>
#include <vector>
#include <iostream>

using namespace std;

/// 非受限联合体
union demo
{
    int i;
    std::string s;

    demo() { new (&s) std::string("hello world!");}
    ~demo() { s.~string();}

    void show(void) {
        cout << s << endl;
    }
};

union demo2 {
    int x;
    float y;
    std::string str;    
    
    demo2() {}
    
    demo2(const std::string& s) {
        new(&str) std::string(s);
    }

    ~demo2() {
        str.~basic_string();
    }
};

int main(int argc, char* argv[])
{
    demo d;
    
    d.show();

    std::cout << d.i << std::endl;

    demo2 d2("hello world!");
    std::cout << d2.str << std::endl;
    return 0;
}
```

### class_in_func

在C++里，允许在函数内部定义类，这种类被称作局部类。局部类定义在函数作用域内，其作用域也局限于该函数，外部无法直接访问。

```cpp
#include <iostream>

int main(int argc, char* argv[])
{   
    // 函数局部类
    class demo
    {
    public:
        demo() {
            std::cout << "demo constructor" << std::endl;
        }
        ~demo() {
            std::cout << "demo destructor" << std::endl;
        }
    };

    demo d;

    return 0;
}
```

### class_default_operator

类的默认比较函数，默认情况下，类的比较函数是按成员变量的顺序进行比较的；具体示例如下所示。

```cpp
#include <iostream>

class demo
{
public:
    int i;
    
    friend bool operator==(demo, demo) = default;
    friend bool operator!=(demo, demo) = default;
};

int main(int argc, char* argv[])
{
    demo d1{1};
    demo d2{2};

    std::cout << std::boolalpha;
    std::cout << (d1 == d2) << std::endl; // false
    std::cout << (d1 != d2) << std::endl; // true
    return 0;
}
```
