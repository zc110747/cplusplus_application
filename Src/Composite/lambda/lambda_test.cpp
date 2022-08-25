/************************************************************************************
 * lambda
 * https://www.cplusplus.com/reference/string/string/
 * Strings are objects that represent sequences of characters
 * 1.Need compile with command -std=c++11
 * 2.Generate excutable file studylambda
 * 
 * 
***********************************************************************************/
#include <vector>
#include <algorithm>
#include <iostream>

using std::cout;
using std::endl;

/*
[]  no capture variable.
[&] capture all extern variable, as reference.
[=] capture all extern variable, as copy, not allow change.
[=, &var] capture all extern variable as copy, exinclue var as reference.
[bar] only capture variable.
[this] capture the point in class.
*/
class LambdaProcess
{
private:
    int x{0};
    int y{0};
public:
    void test(int a, int b){
        auto v1 = []{cout<<"no value"<<endl;};
        auto v2 = [=]{cout<<x+y+a+b<<endl;};
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
} 
