/*
lambda
在C++中，lambda表达式是一种匿名函数，它允许你在需要函数对象的地方定义一个临时的函数。
lambda表达式通常用于简化代码，特别是在需要传递函数作为参数的情况下。
[capture list](parameters) -> return_type {
// 函数体
} 
capture list: 捕获列表，用于捕获外部变量
[] 无捕获的lambda表达式
[=] 值捕获，lambda表达式通过值捕获所有外部变量
[&] 通过引用捕获所有外部变量的lambda表达式
[var] 只捕获变量var
[&var] 只捕获变量var的引用
[this] 捕获当前类中的this指针，按引用捕获
[*this] 捕获当前类中的this指针，按值捕获，值捕获默认定义为const(C++17)
[=, &var] 捕获所有外部变量的lambda表达式
[&, var] 捕获所有外部变量的lambda表达式，但是var是通过值捕获的

函数对象
函数对象是一个类，该类重载了函数调用运算符()。通过重载()运算符，我们可以像调用函数一样调用对象。

std::function
std::function 是一个功能强大的可调用对象包装器，可以包装任何类型的可调用对象（函数、函数对象、lambda、bind 表达式等）。

std::bind
std::bind 是 C++ 标准库中的一个函数模板，它位于 <functional> 头文件中。其主要作用是将一个可调用对象（如函数、函数对象、成员函数等）与一组参数绑定，生成一个新的可调用对象。这个新的可调用对象可以在后续的代码中被调用，而不需要再次提供已经绑定的参数。

泛型lambda表达式
泛型lambda表达式是C++14引入的特性，它允许在lambda表达式中使用auto占位符来表示参数类型，使得lambda可以接受不同类型的参数，实现类似函数模板的泛型功能。

lambda 捕获初始化器
Lambda 捕获初始化器是 C++14 引入的特性，它允许你在 lambda 捕获列表中对捕获的变量进行自定义初始化，还能创建新的局部变量供 lambda 函数体使用，即使这些变量在 lambda 外部并不存在
*/
#include <vector>
#include <algorithm>
#include <iostream>
#include <functional>

using std::cout;
using std::endl;

class LambdaProcess
{
private:
    mutable int x{0};
    mutable int y{0};

public:
    void test(int a, int b){
        auto v1 = []{cout<<"no value"<<endl;};
        auto v2 = [=, this]{cout<<x+y+a+b<<endl;};
        auto v3 = [&]{cout<<x+y+a+b<<endl;};
        auto v4 = [this]{cout<<x<<" "<<y<<endl;};
        auto v5 = [this, a, b]{x=5; y=3; cout<<x+y+a+b<<endl;};
        auto v6 = [*this] { x=10; y=20; cout<<x<<" "<<y<<endl;};

        v1();
        v2();
        v3();
        v4();

        v5();
        std::cout<<"x: "<<x<<" y: "<<y<<std::endl;

        v6();
        std::cout<<"x: "<<x<<" y: "<<y<<std::endl;
    }
};

void callback_func(int a, std::function<void(int a)> callback) 
{
    if (callback) {
        callback(a);
    }
}

void lambda_test(void)
{
    std::vector<int> vec{1, 2, 3, 4};
    
    // 约束泛型lambda
    auto print2 = []<typename T>(std::vector<T>& vec) {
        std::cout<<"约束泛型lambda: ";
        for (auto x : vec) {
            std::cout<<x<<" ";
        }
        std::cout<<endl;
    };
    print2(vec);
  
    // 完美转发
    auto lambdafunc = [&]<typename T>(T&& number) {
        print2(std::forward<T>(number));
    };
    lambdafunc(vec);

    // 泛型lambda
    auto func = [](auto a, auto b) {
        std::cout<<"泛型lambda: ";
        return a+b;
    };
    std::cout<<func(2, 4)<<std::endl;

    //lambda 捕获初始化器
    auto func2 = [moveVec = std::move(vec)](){
        std::cout<<"lambda捕获初始化器: ";
        for (auto x : moveVec) {
            std::cout<<x<<" ";
        }
        std::cout<<endl;
    };
    func2();
}

// 仿函数，函数对象
class func_obj
{
public:
    func_obj(int x = 0, int y = 0):m_x(x), m_y(y){}
    void operator()(int a)
    {
        cout<<m_x<<" "<<m_y<<" "<<a<<endl;
    }
private:
    int m_x;
    int m_y;
};

class Hello {
public:
    void hello(const std::string& value) {
        std::cout << "Hello, " << value << "!" << std::endl;
    }
};

std::function<int(int, int)> add = [](int a, int b) {
    return a + b;
};

int main(int argc, char* argv[])
{
    std::vector<int> vInt{1, 2, 3, 5, 7};
    std::for_each(vInt.begin(), vInt.end(), [](auto &ref){
        cout<<ref<<" ";
    });
    cout<<endl;

    //lambda &reference value from
    auto func = [&vInt](int size) -> int{
        auto min = size>vInt.size()?vInt.size():size;
        return min;
    };
    cout<<func(3)<<endl;

    //groovy, copy value
    auto num = [vInt](int size){
        //vInt[0] = 3;
        auto min = size>vInt.size()?vInt.size():size;
        return min;
    }(6);
    cout<<num<<endl;

    //value change by lambda(copy)
    auto f1 = [=](){
        //vInt[0] = 3;
        cout<<vInt[0]<<endl;
    };
    f1();
    cout<<vInt[0]<<endl;

    auto  f2 = [&vInt](){
        vInt[0] = 5;
        cout<<vInt[0]<<endl;
    };
    f2();
    cout<<vInt[0]<<endl;

    LambdaProcess LamProcess;
    LamProcess.test(1, 2);

    lambda_test();

    // 定义支持lambda函数
    callback_func(5, [](int a) {
        cout<<"callback func:"<<a<<endl;
    });

    // 定义函数对象
    std::cout<<"\n=======function object=======\n";
    func_obj obj(1, 2);
    obj(3);

    // std::function
    std::cout<<"\n=======std::function=======\n";
    const auto& func1 = add;
    std::cout<<func1(1, 2)<<std::endl;
    
    // 使用std::bind绑定成员函数
    Hello hello_;
    std::function<void(const std::string&)> func2 = std::bind(&Hello::hello, hello_, std::placeholders::_1);
    func2("world");
    
    // 使用std::bind绑定成员参数
    auto addTwo = std::bind(add, 2, std::placeholders::_1);

    int result = addTwo(3);
    std::cout << "Result: " << result << std::endl;
    return 0;
} 
