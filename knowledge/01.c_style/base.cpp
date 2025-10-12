//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      base.cpp
//
//  Purpose:
//     1. 基础数据类型
//     2. 数组和指针
//     3. 宏定义和扩展
//
// Author:
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <cstdio>

#define START_STR   "hello"
#define END_NUM     64

// #：将参数转换为字符串
#define TO_STRING(x) #x
#define CONCAT_STR(x, y) START_STR "_" TO_STRING(y)

// ##: 连接符，将两个参数连接起来
#define TEST_STRING "hello world"
#define CONCAT_STR2(x, y) x##y

void basic_type_process(void)
{
    printf("============= %s ==============\n", __func__);

    // 基础数据类型
    // 1. 基础数据类型
    {
        char a = '0';
        short b = 1;
        int c = 1;
        long d = 1;
        float e = 1.0;
        double f = 1.0;
        char *p = &a;
        int *q = &c;

        printf("a = %c, b = %hd, c = %d, d = %ld, e = %f, f = %lf\n", a, b, c, d, e, f);
        printf("p = %p, q = %p\n", p, q);
        printf("sizeof(a) = %ld, sizeof(b) = %ld, sizeof(c) = %ld, sizeof(d) = %ld, sizeof(e) = %ld, sizeof(f) = %ld\n", 
            sizeof(a), sizeof(b), sizeof(c), sizeof(d), sizeof(e), sizeof(f));
    }

   // 2.数组, 指针
    {
        int a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        int b[2][5] = {
            {1, 2, 3, 4, 5},
            {6, 7, 8, 9, 10}
        };
        int i, j;

        printf("sizeof(a)=%ld\n", sizeof(a));

        for(i = 0; i < 10; i++) {
            printf("a[%d] = %d, ", i, a[i]); 
        }
        printf("\n");

        for(i = 0; i < 2; i++) {
            for(j = 0; j < 5; j++) {
                printf("b[%d][%d] = %d, ", i, j, b[i][j]);
            }
            printf("\n");
        }

        int *pa = a;
        int(*pb)[5] = b;

        for(i = 0; i < 10; i++) {
            printf("a[%d] = %d, ", i, pa[i]); 
        }
        printf("\n");

        for(i = 0; i < 2; i++) {
            for(j = 0; j < 5; j++) {
                printf("b[%d][%d] = %d, ", i, j, pb[i][j]);
            }
            printf("\n");
        }
    }

    // 3.循环
    {
        int sum = 0;
        int i;
        
        for (i = 0; i < 10; i++) {
            sum += i;
        }
        printf("sum = %d\n", sum);

        i = 0;
        sum = 0;
        while(i < 10) {
            sum += i;
            i++;
        }
        printf("sum = %d\n", sum);

        i = 0;
        sum = 0;
        do {
            sum += i;
            i++;
        } while (i < 10);
        printf("sum = %d\n", sum);
    }

    //预定义宏
    {
        printf("__STDC__ = %d\n", __STDC__);
        printf("__DATE__ = %s\n", __DATE__);
        printf("__TIME__ = %s\n", __TIME__);
        printf("__FILE__ = %s\n", __FILE__);

        printf("%s\n", CONCAT_STR(START_STR, END_NUM));
        printf("%s\n", CONCAT_STR2(TEST_, STRING));
    }
}