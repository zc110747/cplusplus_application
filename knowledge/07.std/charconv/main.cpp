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
#include <charconv>
#include <iostream>
#include <system_error>
#include <string_view>

int main()
{
    char buffer[64];

    // Test 1: Integer to characters
    int value = 12345;

    auto result = std::to_chars(
        buffer,
        buffer + sizeof(buffer),
        value
    );

    if (result.ec == std::errc{})
    {
        std::cout << "[INFO] Integer to characters: "
                  << std::string_view(
                         buffer,
                         result.ptr - buffer)
                  << '\n';
    }
    else
    {
        std::cout << "[ERROR] Integer conversion failed.\n";
    }

    // Test 2: Characters to integer
    std::string_view text = "-6789";
    int parsedValue = 0;

    auto parseResult = std::from_chars(
        text.data(),
        text.data() + text.size(),
        parsedValue
    );

    if (parseResult.ec == std::errc{} &&
        parseResult.ptr == text.data() + text.size())
    {
        std::cout << "[INFO] Parsed integer: "
                  << parsedValue << '\n';
    }
    else
    {
        std::cout << "[ERROR] Integer parsing failed.\n";
    }

    // Test 3: Convert an integer to hexadecimal
    int hexValue = 255;

    auto hexResult = std::to_chars(
        buffer,
        buffer + sizeof(buffer),
        hexValue,
        16
    );

    if (hexResult.ec == std::errc{})
    {
        std::cout << "[INFO] Hexadecimal: "
                  << std::string_view(
                         buffer,
                         hexResult.ptr - buffer)
                  << '\n';
    }

    // Test 4: Parse an integer from a substring
    std::string_view input = "123abc";
    int number = 0;

    auto partialResult = std::from_chars(
        input.data(),
        input.data() + input.size(),
        number
    );

    if (partialResult.ec == std::errc{})
    {
        std::cout << "[INFO] Parsed prefix: "
                  << number << '\n';

        std::cout << "[INFO] Remaining characters: "
                  << std::string_view(
                         partialResult.ptr,
                         input.data() + input.size()
                             - partialResult.ptr)
                  << '\n';
    }

    return 0;
}