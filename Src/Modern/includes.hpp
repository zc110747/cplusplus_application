//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      includes.hpp
//
//  Purpose:
//
// Author:
//     	@听心跳的声音
//
//  Assumptions:
//
//  Revision History:
//      4/19/2023   Create New Version
/////////////////////////////////////////////////////////////////////////////
_Pragma("once")

#include <iostream>
#include <string>
#include <vector>
#include <type_traits>

#define FUNCTION_START()  {std::cout<<__func__<<":\n";}{
#define FUNCTION_END()    }{std::cout<<"\n\n";  } 

//define compile support version
#define _CPP14  201402
#define _CPP17  201703
#define _CPP20  202002

#define MORE_THAN_CPP14   (__cplusplus>=_CPP14)
#define MORE_THAN_CPP17   (__cplusplus>=_CPP17)
#define MORE_THAN_CPP20   (__cplusplus>=_CPP20)

