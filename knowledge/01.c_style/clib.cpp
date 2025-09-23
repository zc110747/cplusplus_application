//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      clib.cpp
//
//  Purpose:
//     1. #include <cassert> 包含assert语句处理
//     2. #include <cctype> 包含字符类型判断的一些处理
//     3. #include <cfenv> 定义浮点处理模式
//     4. #include <cfloat> 定义浮点判断得范围
//     5. #include <cstdlib> 包含转换，随机数，内存申请，环境变量，运算方法
//     6. #include <cstring> 包含字符串处理的一些函数
//
// Author:
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include <cstdio>
#include <cctype>
#include <cerrno>
#include <cfenv>
#include <cfloat>
#include <cmath>
#include <cstdarg>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cstddef>
#include <utility>

#ifndef PI
#define PI 3.14
#endif

typedef enum
{
	LOG_TRACE = 0,
	LOG_DEBUG,
	LOG_INFO,
	LOG_WARN,
	LOG_ERROR,
	LOG_FATAL,
}LOG_LEVEL;
#define LOGGER_MAX_BUFFER_SIZE      128

static char LoggerMaxBuffer[LOGGER_MAX_BUFFER_SIZE];
int print_log(LOG_LEVEL level, unsigned int tick,  const char* fmt, ...)
{
    va_list valist;
    int len, bufferlen, outlen = 0;
    char *pbuf;

    bufferlen = LOGGER_MAX_BUFFER_SIZE - 1;
    pbuf = LoggerMaxBuffer;

    // 打印函数变量
    len = snprintf(pbuf, bufferlen, "time:%d, level:%d info:", tick, level);
    if (len <= 0 || len >= bufferlen) {
        return -1;
    }
    outlen += len;
    bufferlen -= len;
    pbuf = &pbuf[len];

    // 打印函数可变变量
    va_start(valist, fmt);
    len = vsnprintf(pbuf, bufferlen, fmt, valist);
    va_end(valist);
    if((len<=0) || (len>=bufferlen))
    {
        return -1;
    } 
    outlen += len;
    bufferlen -= len;
    pbuf = &pbuf[len];

    if (bufferlen < 3) {
        return -1;
    }

    pbuf[0] = '\r';
    pbuf[1] = '\n';
    pbuf[2] = '\0';
    outlen += 3;
    printf("%s", LoggerMaxBuffer);

    return 0;
}

bool areEqual(float a, float b) {
    return (a - b < FLT_EPSILON) && (b - a < FLT_EPSILON);
}

void c_standard_lib(void)
{
    int *ptr = (int *)malloc(sizeof(int));
 
    printf("============= %s ==============\n", __func__);

    // #include <cassert>
    assert(ptr != NULL);
    free(ptr);

    // #include <cctype>
    printf("isalnum:%x isalpha:%x \n", isalnum('a'), isalpha('a'));
    printf("isupper:%x islower:%x \n", isupper('a'), islower('a'));
    printf("tolower:%c toupper:%c \n", tolower('A'), toupper('a'));

    // #include <cerrno>
    printf("errno:%d \n", errno);

    // #include <cfenv>
    // #include <cmath>
    printf ("rounding using ");
    switch (fegetround()) {
        case FE_DOWNWARD: printf ("downward"); break;
        case FE_TONEAREST: printf ("to-nearest"); break;
        case FE_TOWARDZERO: printf ("toward-zero"); break;
        case FE_UPWARD: printf ("upward"); break;
        default: printf ("unknown");
    }
    printf (" rounding:\n");

    float x = 0.1f + 0.2f;
    float y = 0.3f;
    if (areEqual(x, y)) {
        printf("x和y相等\n");
    } else {
        printf("x和y不相等\n");
    }

    printf ("rint (2.3) = %.1f\n", rint(2.3));
    printf ("rint (3.8) = %.1f\n", rint(3.8));
    printf ("rint (-2.3) = %.1f\n", rint(-2.3));
    printf ("rint (-3.8) = %.1f\n", rint(-3.8));
    printf ("sin: %.1f, cos: %.1f\n", sin(PI), cos(PI));
    printf ("tan: %.1f, cot: %.1f\n", tan(PI), atan(1));

    // #include <cstdarg>
    print_log(LOG_INFO, 0, "Hello World!");

    // #include <cstdlib>
    printf("atof: %.2f, atoi: %d\n", atof("0.23"), atoi("24"));
    printf("strtof: %.2f, strtol: %ld\n", strtof("0.23", NULL), strtol("24", NULL, 10));

    srand(time(NULL));
    printf("rand: %d\n", rand()%10);
    
    printf("abs:%d, labs:%ld\n", abs(-1), labs(-4));

    // #include <cstring>
    char str1[20] = "hello";
    char str2[20] = "world";
    strcpy(str1, str2);
    if (strncmp(str1, str2, 5) == 0) {
        printf("equal str1 == str2\n");
    }
    printf("str1: %s, str2: %s\n", str1, str2);

    // #include <cstddef>
    std::byte b1{0x55};
    std::byte b2 = static_cast<std::byte>(10);

    auto b3 = b1 & b2;
    auto b4 = b1 | b2;
    auto b5 = b1 ^ b2;
    auto b6 = ~b1;
    auto b7 = b1 << 2;

    printf("b1: %d, b2: %d, b3: %d, b4: %d, b5: %d, b6: %d, b7: %d\n", 
        std::to_integer<int>(b1), std::to_integer<int>(b2), std::to_integer<int>(b3), 
        std::to_integer<int>(b4), std::to_integer<int>(b5), std::to_integer<int>(b6), 
        std::to_integer<int>(b7));
}