/*
lambda
在C++中，lambda表达式是一种匿名函数，它允许你在需要函数对象的地方定义一个临时的函数。
lambda表达式通常用于简化代码，特别是在需要传递函数作为参数的情况下。
[capture list](parameters) -> return_type {
// 函数体
}
 
capture list: 捕获列表，用于捕获外部变量
[] 无捕获的lambda表达式.
[=] 值捕获，lambda表达式通过值捕获所有外部变量.
[&] 通过引用捕获所有外部变量的lambda表达式.
[var] 只捕获变量var.
[&var] 只捕获变量var的引用.
[this] 捕获当前类中的this指针.
[=, &var] 捕获所有外部变量的lambda表达式.
[&, var] 捕获所有外部变量的lambda表达式，但是var是通过值捕获的.
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
    int x{0};
    int y{0};
public:
    void test(int a, int b){
        auto v1 = []{cout<<"no value"<<endl;};
        auto v2 = [=, this]{cout<<x+y+a+b<<endl;};
        auto v3 = [&]{cout<<x+y+a+b<<endl;};
        auto v4 = [this]{cout<<x<<" "<<y<<endl;};
        auto v5 = [this, a, b]{cout<<x+y+a+b<<endl;};

        v1();
        v2();
        v3();
        v4();
        v5();
    }
};

void lambda_test(void)
{
    //泛型lambda
    std::cout<<"=========================\n\n";
    auto func = [](auto a, auto b) {
        return a+b;
    };
    std::cout<<func(2, 4)<<std::endl;

    //类型限定的lambda
    auto print2 = []<typename T>(std::vector<T>& vec) {
        for (auto x : vec) {
            std::cout<<x<<" ";
        }
        std::cout<<endl;
    };
    std::vector<int> vec{1, 2, 3, 4};
    print2(vec);

    //完美转发
    auto lambdafunc = [&]<typename T>(T&& number) {
        print2(std::forward<T>(number));
    };
    lambdafunc(vec);
}

void callback_func(int a, std::function<void(int a)> callback) 
{
    if (callback) {
        callback(a);
    }
}

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

    return 0;
} 
