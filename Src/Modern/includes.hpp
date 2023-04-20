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
#include <cstring>

#include <vector>
#include <tuple>
#include <type_traits>

#define FUNCTION_START()  {std::cout<<__func__<<":\n";}{
#define FUNCTION_END()    }{std::cout<<"\n\n";  } 

//define compile support version
#define _CPP14  201402
#define _CPP17  201703
#define _CPP20  202002

#define __MORE_THAN_CPP14__   (__cplusplus>=_CPP14)
#define __LOWRE_THAN_CPP14_RUN {std::cout<<"This function need supoort by cpp-14, use new g++ or --std=c++14";}
#define __MORE_THAN_CPP17__   (__cplusplus>=_CPP17)
#define __LOWRE_THAN_CPP17_RUN {std::cout<<"This function need supoort by cpp-17, use new g++ or --std=c++17";}
#define __MORE_THAN_CPP20__   (__cplusplus>=_CPP20)
#define __LOWRE_THAN_CPP20_RUN {std::cout<<"This function need supoort by cpp-20, use new g++ or --std=c++2a";}
