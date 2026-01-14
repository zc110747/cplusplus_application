#ifndef TEST_HPP
#define TEST_HPP

#include <iostream>

template <typename T>
void extern_func(T t)
{
    std::cout << "func(T t): " << typeid(t).name() << std::endl;
}

#endif