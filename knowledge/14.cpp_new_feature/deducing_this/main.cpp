//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//
// Author:
//      @公众号：<嵌入式技术总结>
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <print>

struct T
{
    int arr[10];

    decltype(auto) operator[](this auto&& self, std::size_t idx) { return self.arr[idx]; }
};

template<class Derived>
class Base {
public:
    void interface(this Derived& self) {
        self.impl();
    }
};

class Device : public Base<Device> {
public:
    void impl() {
        std::println("Device::impl");
    }
};

int main()
{
    auto fact = [](this auto self, int n) {
        if (n <= 1) return 1;
        return n * self(n - 1);
    };
    std::println("{}", fact(5)); // output 120

    T t = {1, 2, 3, 4, 5};
    std::println("{}", t[0]); // output 42

    Device dev;

    dev.interface(); // output "Device::impl"
    return 0;
}
