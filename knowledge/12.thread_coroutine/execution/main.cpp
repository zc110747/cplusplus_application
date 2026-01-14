//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      
//
// Author:
//      @公众号 <嵌入式技术总结>
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <algorithm>
#include <random>
#include <vector>
#include <ranges>
#include <chrono>
#include <execution>

void measure(auto policy, std::vector<int> v)
{
    const auto start = std::chrono::steady_clock::now();

    std::sort(policy, v.begin(), v.end());

    const auto finish = std::chrono::steady_clock::now();

    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(finish - start)
              << '\n';
};

void measure(std::vector<int> v)
{
    const auto start = std::chrono::steady_clock::now();

    std::sort(v.begin(), v.end());

    const auto finish = std::chrono::steady_clock::now();

    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(finish - start)
              << '\n';    
}

int main(int argc, const char* argv[])
{
    std::vector<int> vec(1'000'000);
    std::mt19937 gen(std::random_device{}());
    
    std::ranges::generate(vec, [&gen]() { return gen() % 1000; });
    
    std::cout << " default: ";
    measure(vec);
    std::cout << " seq: ";
    measure(std::execution::seq, vec);
    std::cout << " par: ";
    measure(std::execution::par, vec);
    std::cout << " par_unseq: ";
    measure(std::execution::par_unseq, vec);
    std::cout << " unseq: ";
    measure(std::execution::unseq, vec);

    return 0;
}