//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      template_alias.cpp
//
//  Purpose:
//
//  Author:
//     	@听心跳的声音
//
//  Assumptions:
//
//  Revision History:
//      4/19/2023   Create New Version
/////////////////////////////////////////////////////////////////////////////
#include "template_alias.hpp"
#include "../includes.hpp"

using namespace std;

namespace TEMPLATE_ALIAS
{
    //22.2 别名模板
    template<class T>
    using int_map = std::map<int, T>;

    void test_study()
    {
        FUNCTION_START()

        //22.1 类型别名
        //using identifier = type-id
        cout<<"\nchapter22.1\n  ";
        using vector_iter = std::vector<int>::iterator;
        std::vector<int> vec1{1, 2, 3};
        for(vector_iter iter=vec1.begin(); iter != vec1.end(); iter++)
        {
            cout<<*iter<<" ";
        }
        cout<<" | ";

        //22.2 别名模板
        //template <template-parameter-list>
        //using identifier = type-id;
        cout<<"\nchapter22.2\n  ";
        int_map<std::string> int2string;
        int2string[1] = "5";    
        int2string[4] = "24";   
        for(const auto& val: int2string)
        {
            cout<<val.first<<" "<<val.second<<" | ";
        }
        FUNCTION_END()
    }
}
