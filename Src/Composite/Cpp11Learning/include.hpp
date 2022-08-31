//#pragma once
_Pragma("once")

#include <iostream>
#include <stdio.h>

int basic_test(void) noexcept;

#define LOG(...) { \
    fprintf(stderr, "%s: Line %d:", __FILE__, __LINE__); \
    fprintf(stderr, __VA_ARGS__); \
    fprintf(stderr, "\n");  \
}
