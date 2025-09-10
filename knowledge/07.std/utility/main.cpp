/*

*/

#include <iostream>
#include <utility>
#include <tuple>
#include <string>

int add(int a, int b) {
    return a + b;
}

auto multiply = [](int a, int b) {
    return a * b;
};

int main() 
{
    auto args = std::make_tuple(3, 4);

    int sum = std::apply(add, args);
    std::cout << "Sum: " << sum << std::endl;

    int product = std::apply(multiply, args);
    std::cout << "Product: " << product << std::endl;

    auto tuple_args = std::forward_as_tuple("hello", 24, 'a');
    std::apply([](auto&&... args) {
        ((std::cout << args << " "), ...);
        std::cout << std::endl; 
    }, tuple_args);

    auto func = [](std::string s, int i, char c) {
        std::cout << "String: " << s;
        std::cout << " | Integer: " << i;
        std::cout << " | Character: " << c << std::endl;
    };
    std::apply(func, tuple_args);

    return 0;
}