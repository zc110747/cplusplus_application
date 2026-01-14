# reference

- [值和引用](#value_reference)
- [移动语义](#move_semantics)
- [引用折叠、万能引用](#reference_fold)
- [完美转发](#perfect_forwarding)
- [ref和cref应用](#ref_cref)
- [返回主页](../../README.md)

## value_reference

对于C++来说，值分为左值和右值。

- 左值是指那些可以出现在赋值操作符左边的表达式，即可以被取地址的表达式。左值通常有一个持久的内存地址，并且在表达式结束后仍然存在。
- 右值是指那些只能出现在赋值操作符右边的表达式，即不能被取地址的表达式。右值通常是临时的，表达式结束后就不再存在。

右值分为将亡值(expiring rvalue)和纯右值(pure rvalue)。

- 纯右值(pure rvalue): 临时变量（1+2、a+b等），不和对象关联的字面量（2、'a'等），非引用返回的函数返回的临时变量值，lambda表达式等。
- 将亡值(expiring rvalue): std::move返回值、T&&右值引用的函数返回值、或者访问临时对象的参数。

左值引用是对左值的引用，它允许你通过引用修改原始左值。另外对于常量左值引用，也可以绑定到纯右值，这在很多情况下降低了函数调用的次数，是之前比较重要的优化方法。

```cpp
// 格式：类型& 引用名 = 左值表达式;
int x = 10;
int &ref = x;
```

右值引用是对右值的引用，它允许你延长右值的生命周期，并在某些情况下实现移动语义。

```cpp  
// 格式：类型&& 引用名 = 右值表达式;
int x = 10;
int &&rref = std::move(x);
```

关于左值引用、常量左值引用、右值引用和常量右值引用能够引用的类型具体定义如下。

| 可以引用的类型 | 非常量左值 | 常量左值 | 非常量右值 | 常量右值 | 
| --- | --- | --- | --- | --- |
| 非常量左值引用 | √ | × | × | × |
| 常量左值引用 | √ | √ | √ | √ |
| 非常量右值引用 | × | × | √ | × |
| 常量右值引用 | × | × | √ | √ |

关于左值、右值、将亡值、纯右值的具体示例如下所示。

```cpp
#include <iostream>
#include <functional>

int get_value() {
    return 10;
}

int main(int argc, char *argv[]) 
{
    int x = 10;                         // x是左值, 可以取地址
    const int x1 = 10;                  // x1是左值
    int &lref1 = x;                      // 左值引用

    // 纯右值
    int &&rref1 = 10;                   // 纯右值
    int &&rref2 = get_value();          // 纯右值(值返回的函数返回值)
    int y = 10;
    int &&rref3 = x + y;                // 纯右值(右值引用赋值)
    std::function<int()>&& f = []() -> int { return 10; };  // lambda表达式，纯右值

    // 将亡值
    int &&rref4 = std::move(x);         // 将亡值(std::move)

    std::cout << "x: " << x << std::endl;
    std::cout << "lref1: " << lref1 << std::endl;
    std::cout << "rref1: " << rref1 << std::endl;
    std::cout << "rref2: " << rref2 << std::endl;
    std::cout << "rref3: " << rref3 << std::endl;
    std::cout << "rref4: " << rref4 << std::endl;
    std::cout << "f(): " << f() << std::endl;

    const int &lref2 = f();             // 常量左值引用绑定到纯右值
    const int &lref3 = 5;               // 常量左值引用绑定到纯右值
    const int &lref4 = std::move(x);    // 常量左值引用绑定到将亡值
    const int &lref5 = x;               // 常量左值引用绑定到左值
    std::cout << "lref2: " << lref2 << std::endl;
    std::cout << "lref3: " << lref3 << std::endl;
    std::cout << "lref4: " << lref4 << std::endl;

    return 0;
}
```

## move_semantics

std::move时C++11引入的一个函数，它将左值转换为右值引用，从而触发移动语义。std::move返回一个右值引用，其功能等同于如下实现。

```cpp
// std::move等同于static_cast<T&&>(x)
int x = 10;
int &&rref = static_cast<int&&>(x);
```

移动语义（Move Semantics）是C++11引入的一个重要特性，它允许对象的资源（如内存、文件句柄等）在不进行深拷贝的情况下从一个对象转移到另一个对象，从而提高程序的性能和效率。

移动语义通过如下方式处理。

- 右值引用（Rvalue Reference）
- 移动构造函数（Move Constructor）
- 移动赋值运算符（Move Assignment Operator）

具体示例如下所示。

```cpp
include <iostream>

class demo
{
public:
    /// constructor
    demo(): ptr_(new int(0)) {
        std::cout << "constructor" << std::endl;
    }

    /// copy constructor
    demo(const demo &other): ptr_(new int(*other.ptr_)) {
        std::cout << "copy constructor" << std::endl;
    }

    /// move constructor
    demo(demo &&other): ptr_(other.ptr_) {
        other.ptr_ = nullptr;
        std::cout << "move constructor" << std::endl;
    }
    
    /// move assignment operator
    /// 移动构造函数和赋值构造函数的返回值需要是引用类型，否则会造成复制
    demo& operator=(demo &&other) {
        ptr_ = other.ptr_;
        other.ptr_ = nullptr;
        std::cout << "move assignment operator" << std::endl;
        return *this;
    }

    ~demo() {
        if (ptr_ != nullptr) {
            delete ptr_;
            ptr_ = nullptr;
        } else {
            std::cout << "invalid ptr" << std::endl;
        }
        std::cout << "destructor" << std::endl;
    }

    void show(void) {
        std::cout << "show:" << *ptr_ << std::endl;
    }

public:
    int *ptr_{nullptr};
};


int func() {
    return 1;
}

int main(int argc, char *argv[]) 
{
    int x = 10;             //x是左值, 10是右值
    int &ref = x;           //左值引用
    int &&rref = 10;        //右值引用
    int &&rref2 = func();   //右值引用
    int &&rref3 = std::move(x); //右值引用，左值绑定到右值需要用到std::move

    std::cout << "x: " << x << std::endl;       //10
    ref = 5;
    std::cout << "x: " << x << std::endl;       //5
    rref3 = 8;
    std::cout << "x: " << x << std::endl;       //8
    std::cout << "rref2: " << rref2 << std::endl; //1

    std::cout << "================== class move shows ===============" << std::endl;
    std::cout << "test 1:" << std::endl;
    {
        demo d1;
        demo&& d2 = std::move(d1);
        d2.show();
    }

    std::cout << "\ntest 2:" << std::endl;
    {
        demo d1;
        demo d3 = std::move(d1);    // 移动构造函数，d1将内存资源转移到d3
        d3.show();
    }

    std::cout << "\ntest 3:" << std::endl;
    {
        demo d1, d4;
        d4 = std::move(d1);
        d4.show();
    }

    return 0;
}
```

## reference_fold

引用折叠是指在模板推导或类型别名中，当引用类型与其他类型组合时，编译器会根据一定的规则进行类型折叠。

引用折叠的规则如下：

1. 左值引用类型与所有类型组合都是左值引用类型。
2. 右值引用类型与非左值引用组合则是右值引用类型。

| 类模板型 | 实际类型 | 组合类型 |
| --- | --- | --- |
| T& | R | R& |
| T& | R& | R& |
| T& | R&& | R& |
| T&& | R | R&& |
| T&& | R& | R& |
| T&& | R&& | R&& |

引用折叠配合auto关键字，就可以实现万能引用。这是C++11中引入的一个概念；它是一种特殊的引用类型，右值引用只能绑定到右值，而万能引用可以绑定到左值或右值。

具体示例如下所示。

```cpp
#include <iostream>
#include <type_traits>

template <typename T>
void print(T&& param)
{
    std::cout << std::is_lvalue_reference<T>::value << " | ";
    std::cout << std::is_rvalue_reference<T>::value << std::endl;
}

int main(int argc, char const *argv[])
{
    int a = 10;

    // 右值引用只能绑定到右值，绑定左值需要使用std::move
    int &&ref1 = std::move(a); 

    // 此时auto识别为int &
    auto &&ref2 = a;

    // 此时auto识别为int &&
    auto &&ref3 = std::move(a);

    std::cout << std::boolalpha;
    std::cout << "a lref: " << std::is_lvalue_reference<decltype(a)>::value << " ";
    std::cout << "a rref: " << std::is_rvalue_reference<decltype(a)>::value << std::endl;
    std::cout << "ref1 lref: " << std::is_lvalue_reference<decltype(ref1)>::value << " ";
    std::cout << "ref1 rref: " << std::is_rvalue_reference<decltype(ref1)>::value << std::endl;
    std::cout << "ref2 lref: " << std::is_lvalue_reference<decltype(ref2)>::value << " ";
    std::cout << "ref2 rref: " << std::is_rvalue_reference<decltype(ref2)>::value << std::endl;
    std::cout << "ref3 lref: " << std::is_lvalue_reference<decltype(ref3)>::value << " ";
    std::cout << "ref3 rref: " << std::is_rvalue_reference<decltype(ref3)>::value << std::endl;

    print(a);             // int& 类型
    print(std::move(a));  // int类型
    
    return 0;
}
```

## perfect_forwarding

完美转发是指在模板函数中，通过std::forward<T>(t)函数将参数以原始的左值或右值类型传递给另一个函数，从而保留参数的原始属性。另外只有在模板类型推导的上下文中，T&&才是万能引用；若T是明确指定的类型，T&&就只是普通的右值引用。

```cpp
// 完美转发，t可以保证使用原始的属性，不会被修改
// typenaeme T &&则是万能引用
template <typename T>
void perfect_forward(T&& t) {
    func(std::forward<T>(t));
}
```

具体实例如下所示。

```cpp
#include <iostream>

void run_code(int &&a) {
    std::cout << "int &&" << std::endl;
}

void run_code(int &a) {
    std::cout << "int &" << std::endl;
}

void run_code(const int &&a) {
    std::cout << "const int &&" << std::endl;
}

void run_code(const int &a) {
    std::cout << "const int &" << std::endl;
}

template <typename T>
void perfect_forward(T &&t) {
    run_code(std::forward<T>(t));
}

int main(int argc, char const *argv[])
{
    int a = 1;
    int b = 2;
    const int c = 3;
    const int d = 4;

    perfect_forward(a);             // int& 类型
    perfect_forward(std::move(a));  // int&& 类型
    perfect_forward(c);             // const int& 类型
    perfect_forward(std::move(d));  // const int&& 类型

    return 0;
}
```

## ref_cref

std::ref和std::cref是C++标准库中的函数模板，用于创建std::reference_wrapper对象；这些对象可以像引用一样使用，但可以被复制和存储。

1. std::ref函数接受一个左值引用参数，并返回一个std::reference_wrapper对象，该对象可以被复制和存储，并且可以像引用一样使用。
2. std::cref函数接受一个常量左值引用参数，并返回一个 std::reference_wrapper 对象，该对象可以被复制和存储，并且可以像常量引用一样使用。
3. std::reference_wrapper对象是一个类模板，用于封装一个对象引用，并允许对象被复制和存储。

std::ref和std::cref的主要用途是与std::bind函数一起使用，以实现函数对象的参数绑定。

```cpp
#include <functional>
#include <iostream>

void func(int &a, int &b)
{
    a += 1;
    b += 1;
}

int main(int argc, char *argv[]) 
{
    // 通过std::ref创建引用包装器
    int x = 10;
    std::reference_wrapper<int> ref = std::ref(x);
    ref.get() = 20; // 修改x的值
    std::cout << x << std::endl; // 输出 20

    // 通过std::cref创建常量引用包装器
    const int y = 30;
    std::reference_wrapper<const int> cref = std::cref(y);
    std::cout << cref.get() << std::endl; // 输出 y 的值
    
    // 在std::bind中使用引用包装器
    int a = 1;
    int b = 2;
    std::function<void()> f = std::bind(func, std::ref(a), std::ref(b));
    f();
    std::cout << a << " " << b << std::endl; // 输出 2 3

    // 不使用引用包装器，直接传递参数，此时并没有改变a和b的值
    // func带引用，希望外部改变a和b的值，直接使用bind默认不改变，需要使用std::ref或std::cref
    a = 1;
    b = 2;
    std::function<void()> f1 = std::bind(func, a, b);
    f1();
    std::cout << a << " " << b << std::endl; // 输出 1 2

    return 0;
}
```
