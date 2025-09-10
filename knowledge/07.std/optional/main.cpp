/*

*/
#include <iostream>
#include <optional>

std::optional<int> find_value(int value)
{
    if (value == 46) {
        return value;
    } else {
        return std::nullopt;
    }
}

int main(int argc, char *argv[]) 
{
    std::optional<int> opt1;
    std::optional<int> opt2 = 42;

    // has_value
    if (opt1.has_value()) {
        std::cout << "opt1 has value: " << opt1.value() << std::endl;
    } else {
        std::cout << "opt1 has no value" << std::endl;
    }

    //value
    if (opt2.has_value()) {
        std::cout << "opt2 has value: " << opt2.value() << std::endl;
    } else {
        std::cout << "opt2 has no value" << std::endl;
    }

    //vlaue_or
    std::cout << "opt1:" << opt1.value_or(0) << std::endl;
    std::cout << "opt2:" << opt2.value_or(0) << std::endl;

    opt1.swap(opt2);
    std::cout << "opt1:" <<  opt1.value_or(0) << std::endl;

    //reset
    opt2.reset();

    //emplace
    opt2.emplace(51);
    std::cout << "opt2:" << opt2.value_or(0) << std::endl;

    return 0;
}