//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. generator
//
// Author:
//      @公众号：<嵌入式技术总结>
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <generator>
#include <vector>
#include <numeric>
#include <cstdint>
#include <iostream>
#include <span>

struct Frame {
    std::vector<std::uint8_t> payload;
};

std::generator<Frame> parse_frames(
    std::span<const std::uint8_t> bytes)
{
    std::size_t pos = 0;

    while (pos < bytes.size()) {
        // 寻找帧头
        if (bytes[pos] != 0xAA) {
            ++pos;
            continue;
        }

        // 至少需要帧头和长度
        if (bytes.size() - pos < 2) {
            co_return;
        }

        const std::size_t length = bytes[pos + 1];

        // 数据不完整：等待后续数据的真实协议实现
        // 应保留剩余字节，待下次接收后继续解析
        if (bytes.size() - pos - 2 < length) {
            co_return;
        }

        Frame frame;
        const auto first = bytes.begin() + pos + 2;
        const auto last = first + length;

        frame.payload.assign(first, last);

        co_yield std::move(frame);

        pos += 2 + length;
    }
}

// 第 1 阶段：生成或读取原始 ADC 数据
std::generator<int> raw_samples()
{
    for (int value : {100, 120, 4095, 130, 140, 150}) {
        co_yield value;
    }
}

// 第 2 阶段：过滤超出合理范围的数据
std::generator<int> valid_samples(
    std::generator<int> source)
{
    for (int value : source) {
        if (value >= 0 && value <= 2000) {
            co_yield value;
        }
    }
}

// 第 3 阶段：将 ADC 值转换为电压
std::generator<double> to_voltage(
    std::generator<int> source)
{
    constexpr double vref = 3.3;
    constexpr double max_adc = 4095.0;

    for (int value : source) {
        co_yield value * vref / max_adc;
    }
}

int main()
{
    auto raw = raw_samples();
    auto valid = valid_samples(std::move(raw));
    auto voltage = to_voltage(std::move(valid));

    double sum = 0.0;
    int count = 0;

    for (double value : voltage) {
        std::cout << "Voltage: " << value << " V\n";
        sum += value;
        ++count;
    }

    if (count != 0) {
        std::cout << "Average: "
                  << sum / count << " V\n";
    }

     const std::vector<std::uint8_t> buffer{
        0xAA, 0x03, 0x10, 0x20, 0x30,   // 解析: 0x10, 0x20, 0x30
        0xAA, 0x02, 0x40, 0x50,         // 解析: 0x40, 0x50
        0xAB, 0x01,                     // 解析: 错误帧
        0xAA, 0x01, 0x60, 0xAB          // 解析: 0x60
    };
    for (const auto& frame : parse_frames(buffer)) {
        std::cout << "Frame: ";

        for (auto byte : frame.payload) {
            std::cout <<"0x" << std::hex
                      << static_cast<int>(byte) << ' ';
        }

        std::cout << '\n';
    }
}

