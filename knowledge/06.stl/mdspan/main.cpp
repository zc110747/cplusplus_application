//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. std::mdspan声明
//
// Author:
//      @公众号：<嵌入式技术总结>
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <version>

#if __cpp_lib_mdspan >= 202207L
#include <mdspan>
#endif

namespace MDSPAN
{
    int test(void)
    {
// check https://gcc.gnu.org/onlinedocs/libstdc++/manual/status.html#status.iso.2023 for more information
#if __cpp_lib_mdspan >= 202207L
        std::cout <<"============================ SPAN ============================" << std::endl;
        int arr[] = {1, 2, 3, 4};
        std::mdspan<int, std::extents<int, 4>> mdspan(arr);
        std::cout << "mdspan[0]: " << mdspan[0] << std::endl;
        std::cout << "mdspan[1]: " << mdspan[1] << std::endl;
        std::cout << "mdspan[2]: " << mdspan[2] << std::endl;
        std::cout << "mdspan[3]: " << mdspan[3] << std::endl; 
#else
        std::cout << "mdspan is not available" << std::endl;
#endif
        return 0;
    } 
}

int main(int argc, char* argv[])
{
    MDSPAN::test();

    return 0;
}
