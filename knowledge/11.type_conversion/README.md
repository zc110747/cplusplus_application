# type_conversion

C++提供了几种类型转换操作符，用于在不同类型之间进行转换。

- [RTTI](#rtti)
- [隐式类型转换](#type_conversion)
- [显示转换标识符](#explicit)
- [dynamic_cast](#dynamic_cast)
- [static_cast](#static_cast)
- [const_cast](#const_cast)
- [reinterpret_cast](#reinterpret_cast)

## rtti

RTTI（Run-Time Type Identification）是C++中的一种机制，用于在运行时获取对象的类型信息。RTTI主要通过typeid操作符和dynamic_cast操作符来实现。这里

### typeid

typeid是一个操作符，位于`<typeinfo>`头文件中，用于获取对象的类型信息。它返回一个std::type_info对象，该对象包含了对象的类型信息。typeid可以用于获取对象的类型名称、比较两个对象的类型是否相同等。其中std::type_info的格式如下所示。

typeinfo参考网址：<https://en.cppreference.com/w/cpp/header/typeinfo>

其格式如下所示。

```cpp
namespace std {
  class type_info
  {
  public:
    virtual ~type_info();
    constexpr bool operator==(const type_info& rhs) const noexcept;
    bool before(const type_info& rhs) const noexcept;     // 比较类型的顺序
    size_t hash_code() const noexcept;                    // 获取对象的哈希值
    const char* name() const noexcept;                   // 获取对象的类型名称
 
    type_info(const type_info&) = delete;
    type_info& operator=(const type_info&) = delete;
  };
}
```

结合上述描述，typeid操作符具有以下特征。

- typeid的返回值是一个左值，且其生命周期一直被扩展到程序生命周期结束。
- std::type_info不支持复制构造函数，若想保存std::type_info，只能获取其引用或者指针。
- typeid的返回值总是忽略类型的 cv 限定符，即typeid(const T)==typeid(T)。

具体示例如下所示。

```cpp
#include <iostream>
#include <typeinfo>

class Base {
public:
    virtual ~Base() {}
};

class Derived : public Base {};

int main(int argc, char *argv[])
{
    Base* b = new Derived;

    // 使用typeid获取类型信息
    std::cout << "Type: " << typeid(*b).name() << std::endl;

    // 比较类型
    if (typeid(*b) == typeid(Derived)) {
        std::cout << "b is a Derived object" << std::endl;
    }

    // 忽略类型的 cv 限定符
    if (typeid(const Derived) == typeid(Derived)) {
        std::cout << "const Derived is the same as Derived" << std::endl;
    }

    // 使用引用或者指针存储type_info对象
    auto &t1 = typeid(Derived);
    std::cout << "Type: " << t1.name() << std::endl;
    auto t3 = &typeid(Derived);
    std::cout << "Type: " << t3->name() << std::endl;

    // 使用dynamic_cast进行向下转型
    Derived* d = dynamic_cast<Derived*>(b);
    if (d) {
        std::cout << "b is a Derived object" << std::endl;
    }

    delete b;
    return 0;
}
```

### typeindex

typeindex是一个类，位于`<typeindex>`头文件中，用于获取对象的类型索引。typeindex对象包含对象的类型索引，该索引可以通过hash_code()方法获取。typeindex对象可以通过std::type_index(typeid(对象))操作符获取。

typeindex参考网址：<https://en.cppreference.com/w/cpp/header/typeindex>

typeindex支持的方法如下。

| 方法 | 描述 |
| --- | --- |
| opeator= | 赋值函数 |
| operator== | 比较两个typeindex对象是否相等 |
| operator< | 比较两个typeindex对象是否小于 |
| operator> | 比较两个typeindex对象是否大于 |
| operator<= | 比较两个typeindex对象是否小于等于 |
| operator>= | 比较两个typeindex对象是否大于等于 |
| hash_code() | 获取对象的哈希值 |
| name() | 获取对象的类型名称 |

具体示例如下所示。

```cpp
#include <iostream>
#include <memory>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <map>
 
struct A
{
    virtual ~A() {}
};
 
struct B : A {};
struct C : A {};

std::map<std::type_index, std::string> type_names = {
    {std::type_index(typeid(int)), "int"},
    {std::type_index(typeid(double)), "double"},
    {std::type_index(typeid(A)), "A"},
    {std::type_index(typeid(B)), "B"},
    {std::type_index(typeid(C)), "C"}
};

int main(int argc, char *argv[])
{
    int i;
    double d;
    A a;
    std::unique_ptr<A> b(new B);
    std::unique_ptr<A> c(new C);
 
    std::cout << "i is " << type_names[std::type_index(typeid(i))] << '\n';
    std::cout << "d is " << type_names[std::type_index(typeid(d))] << '\n';
    std::cout << "a is " << type_names[std::type_index(typeid(a))] << '\n';
    std::cout << "*b is " << type_names[std::type_index(typeid(*b))] << '\n';
    std::cout << "*c is " << type_names[std::type_index(typeid(*c))] << '\n';

    std::cout << std::type_index(typeid(a)).name() << '\n';
    std::cout << std::type_index(typeid(a)).hash_code() << '\n';
}
```

## type_conversion

类型转换是C++中比较复杂，其有一套隐藏的转换机制，被称为隐式转换。隐式转换是指在不需要显式类型转换操作符的情况下，编译器自动将一种类型的值转换为另一种类型的值。常见的隐式转换如下所示。

- 基本算术类型的转
  - 整型提升（如 char 或 short 提升为 int）。
  - 浮点数到整数的转换（例如，double 到 int）。
  - 整数到更大范围整数的转换（例如，int 到 long）
- 派生类到基类的转换：在多态和继承中，派生类对象可以被当作基类对象使用。
- 标准转换
  - 将bool转换为int（true 转换为 1，false 转换为 0）。
  - 将void*转换为其他指针类型（需要谨慎使用）。

具体示例如下所示。

```cpp
#include <iostream>

class Base {
public:
    virtual ~Base() {}

    virtual void func() {
        std::cout << "Base::func()" << std::endl;
    }
};

class Derived : public Base {
public:
    void func() override {
        std::cout << "Derived::func()" << std::endl;
    }
};

int main(int argc, char *argv[])
{
    int a = 10;
    char c1 = 'a';

    double b = a; // 隐式转换，int 转换为 double
    std::cout << b << std::endl;
    
    a = c1;
    std::cout << a << std::endl;

    long int li = a;
    std::cout << li << std::endl;

    bool b1 = li;
    std::cout << b1 << std::endl;

    Base* base = new Derived;
    base->func();
    return 0;
}
```

## explicit

对于类来说，支持通过隐式类型转换的“自动性”让程序员免于层层构造类型，不过这也会隐藏一些错误。再类的构造函数中，使用explicit关键字可以防止隐式类型转换。其格式如下所示。

```cpp
// explicit 关键字用于修饰构造函数，防止隐式类型转换
class ClassName {
    explicit ClassName(int val) {}
};

ClassName n1(1);
// 带explicit关键字的构造函数不能进行隐式类型转换
//className n1 = 1;
```

具体示例如下所示。

```cpp
#include <iostream>

struct A {
public:
    A(int a) : a_(a) {}
    int a_;
};

struct B {
public:
    explicit B(int a) : b_(a) {}
    int b_;
};

void displayA(const A& val) {
    std::cout << val.a_ << std::endl;
}

void displayB(const B& val) {
    std::cout << val.b_ << std::endl;
}

int main(int argc, char *argv[])
{
    A a0(1);
    A a1 = 1;   //隐式类型转换，1转换为A

    B b0(2);
    //B b1 = 2; // 编译错误，explicit 修饰的构造函数不能进行隐式转换

    displayA(a0);
    displayA(2);

    displayB(b0);
    //displayB(2); //编译错误，explicit 修饰的构造函数不能进行隐式转换
    return 0;
}
```

## dynamic_cast

dynamic_cast用于在具有继承关系的类指针或引用之间进行安全的向下转型。如果转型失败，dynamic_cast会返回nullptr（对于指针）或抛出std::bad_cast异常（对于引用）。

注意：**dynamic_cast只能用于具有继承关系的类指针或引用之间的转换，且在运行时进行转换，因此有一定的性能开销。**

```cpp
#include <iostream>

class Base {
public:
    virtual ~Base() {}

    virtual void func() = 0;
};

class Derived : public Base {
public:
    void func() {
        std::cout << "Derived::func()" << std::endl;
    }
};

int main(int argc, char *argv[])
{
    Base* base = new Derived;

    base->func();

    Derived* derived = dynamic_cast<Derived*>(base);
    if (derived) {
        derived->func();
    }

    return 0;
}
```

## static_cast

static_cast是类型转换操作符，用于在编译时进行类型转换。

主要转换情况如下所示。

1. 基本数据类型之间的转换：例如，将 int 转换为 double，或将 float 转换为 int。
2. 类层次结构中的转换：
  - 向上转型：将派生类指针或引用转换为基类指针或引用。这是安全的，因为派生类对象包含了基类的所有成员。
  - 向下转型：将基类指针或引用转换为派生类指针或引用。这是不安全的，因为基类对象可能不包含派生类的所有成员。在这种情况下，static_cast 不会进行运行时检查，因此可能会导致未定义行为。
3. 将枚举类型转换为整数类型：例如，将枚举值转换为对应的整数值。
4. 将指针或引用转换为void类型：这在需要传递通用指针或引用时很有用。
5. 将void类型转换为指针或引用：这在需要从通用指针或引用中恢复具体类型时很有用。

可以看到static_cast类似隐式转换的功能，但是static_cast更安全，因为它在编译时进行类型检查，而隐式转换并不进行检查，在运行时出错将很难分析发现。

```cpp
#include <iostream>

class Base {
public:
    virtual ~Base() {}

    virtual void func() = 0;
};

class Derived : public Base {
public:
    void func() {
        std::cout << "Derived::func()" << std::endl;
    }
};

int main(int argc, char *argv[])
{
    int i = 10;
    double d = static_cast<double>(i); // 将int转换为double
    std::cout << d << std::endl;

    Derived der;
    Base* base = static_cast<Base*>(&der); // 向上转型(安全)
    base->func();

    int* p = new int(10);
    void* vp = static_cast<void*>(p); // 转换为void*
    int* p2 = static_cast<int*>(vp); // 转换回来
    std::cout << *p2 << std::endl;

    enum Color { RED, GREEN, BLUE };
    i = static_cast<int>(RED); // 枚举转整型
    Color c = static_cast<Color>(1); // 整型转枚举
    std::cout << i << std::endl;
    std::cout << c << std::endl;
    std::cout << static_cast<int>(c) << std::endl;

    return 0;
}
```

## const_cast

类型转换操作符，用于在编译时去除指针或引用的const或volatile属性。主要转换情况：

1. 将const指针或引用转换为非const指针或引用：这在需要将const指针传递给非const实参函数时非常有用。
2. 将volatile指针或引用转换为非volatile指针或引用：这在需要将volatile指针传递给非volatile实参函数时非常有用。

```cpp
#include <iostream>

class MyClass {
public:
    void nonConstFunc() { 
        std::cout << "nonConstFunc" << std::endl;
    }
};

int main(int argc, char *argv[])
{
    const int i = 10;
    int* p = const_cast<int*>(&i); // 去除const属性
    std::cout << *p << std::endl;

    volatile int a = 1;
    int& b = const_cast<int&>(a);
    std::cout << b << std::endl;

    const MyClass obj;
    // obj.nonConstFunc(); // 错误：const对象不能调用非const成员函数
    const_cast<MyClass&>(obj).nonConstFunc(); // 合法但需谨慎
    
    return 0;
}
```

## reinterpret_cast

类型转换操作符，用于在编译时进行任意类型的转换。

1. 将指针或引用转换为不同类型的指针或引用：这在需要将一种类型的指针或引用转换为另一种类型的指针或引用时非常有用。
2. 将整数类型转换为指针类型：这在需要将整数类型的值解释为指针时非常有用。
3. 将指针类型转换为整数类型：这在需要将指针的值解释为整数时非常有用。

```cpp
#include <iostream>
#include <cstdint>

int main(int argc, char *argv[]) 
{
    // 1. 指针类型转换
    int num = 0x12345678;
    char* pChar = reinterpret_cast<char*>(&num);
    std::cout << "Bytes: ";
    for(size_t i = 0; i < sizeof(num); ++i) {
        std::cout << std::hex << (int)pChar[i] << " ";
    }
    std::cout << std::endl;

    // 2. 整数与指针互转
    uintptr_t ptrValue = reinterpret_cast<uintptr_t>(&num);
    int* pNum = reinterpret_cast<int*>(ptrValue);
    std::cout << "Num value: " << *pNum << std::endl;

    // 3. 不相关类指针转换
    struct A { int x; };
    struct B { int y; };
    
    A a{10};
    B* pB = reinterpret_cast<B*>(&a);
    std::cout << "B.y = " << pB->y << std::endl;

    // 4. 函数指针转换
    using FuncPtr = void(*)();
    FuncPtr func = reinterpret_cast<FuncPtr>(&main);
    
    // 5. 与C风格转换比较
    void* pVoid = &num;
    int* pInt = reinterpret_cast<int*>(pVoid); // 等同于 (int*)pVoid
    
    return 0;
}
```
