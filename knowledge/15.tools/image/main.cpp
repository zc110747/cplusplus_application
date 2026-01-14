//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      图像处理程序
//
//  Author:
//     	@公众号 <嵌入式技术总结>
//
//  Assumptions:
//
//  Revision History:
//      4/19/2023   Create New Version
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "image.hpp"

using namespace std;

#define BMP_IMAGE "bmp/bmp_24.bmp"

int main(int argc, char *argv[])
{
    bmp_img::bmp_decoder decoder;
    
    if(!decoder.decode_bmp(BMP_IMAGE))
    {
        cout << "decode bmp failed" << endl;
        return -1; 
    } 
    decoder.show_info();
    decoder.save_to_bmp("test.bmp");

    return 1;
}