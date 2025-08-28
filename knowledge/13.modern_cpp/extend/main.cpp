/*
扩展的C++语法糖

1. 带初始化的条件语句if, switch
在C++17中，if和switch语句支持在条件判断之前进行初始化。这是通过在if或switch关键字后面直接添加一个初始化语句来实现的

2. nullptr
在C++中，nullptr 是一个关键字，用于表示空指针。
它是在C++11标准中引入的，用于替代 NULL 宏，以提供更安全和类型安全的空指针表示。

3. 结构化绑定
在C++17中，引入了结构化绑定（Structured Binding）的概念。
它允许你将一个复杂的表达式分解成多个独立的变量，从而更方便地访问和操作表达式的各个部分。

4. 扩展的for表达式
在C++中，扩展的for表达式（也称为范围-based for循环）是一种简化遍历容器（如数组、向量、列表等）的语法
for (declaration : range_expression) {
    // 循环体
}
*/
#include <iostream>
#include <vector>
#include <algorithm>
#include <condition_variable>
#include <mutex>
#include <chrono>
#include <map>
#include <tuple>

using namespace std::chrono_literals;
std::condition_variable cv;
std::mutex cv_m;

std::tuple<int, double, std::string> get_data() {
    return std::make_tuple(42, 3.14, "Hello, Structured Binding!");
}

int main(int argc, char* argv[]) 
{
    std::vector<int> vecs = {1, 2, 3, 4, 5};

    // 带初始化的条件语句if, switch
    std::cout<<"\n===== 带初始化的条件语句if, switch ===== \n";
    if (auto it = std::find(vecs.begin(), vecs.end(), 3); it != vecs.end()) {
        std::cout << "Found 3 at position: " << std::distance(vecs.begin(), it) << std::endl;
    } else {
        std::cout << "3 not found in the vector." << std::endl;
    }

    switch(std::unique_lock<std::mutex> lk(cv_m); cv.wait_for(lk, 100ms)) {
        case std::cv_status::timeout:
            std::cout << "timeout" << std::endl;
            break;
        case std::cv_status::no_timeout:
            std::cout << "no timeout" << std::endl;
            break;
    }

    // nullptr
    std::cout<<"\n===== nullptr ===== \n";
    int* ptr = nullptr;
    if (ptr == nullptr) {
        std::cout << "ptr is null" << std::endl;
    }

    // 结构化绑定
    std::cout<<"\n===== 结构化绑定 ===== \n";
    auto [x, y] = std::make_pair(10, 20);
    std::cout << "x: " << x << ", y: " << y << std::endl;

    auto [num, pi, message] = get_data();
    std::cout << "num: " << num << ", pi: " << pi << ", message: " << message << std::endl;

    int arr[] = {1, 2, 3, 4, 5};
    auto [first, second, third, fourth, fifth] = arr;
    std::cout << "Array: " << first << ", " << second << ", " << third << ", " << fourth << ", " << fifth << std::endl;

  
    std::map<std::string, int> scores = {
        {"Alice", 90},
        {"Bob", 85},
        {"Charlie", 95}
    };

    // 扩展的for表达式
    for (const auto& [name, score] : scores) {
        std::cout << name << ": " << score << std::endl;
    }

    int numbers[] = {1, 2, 3, 4, 5};
    for (int number : numbers) {
        std::cout << number << " ";
    }
    std::cout << std::endl; 

    return 0;
}