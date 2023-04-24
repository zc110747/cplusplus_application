//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      enable_share.cpp
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
#include "enable_share.hpp"
#include "../includes.hpp"
#include <memory>

using namespace std;

namespace ENABLE_SHARE
{
    struct Point;
    using share_point_ptr = shared_ptr<Point>;
    struct Point : std::enable_shared_from_this<Point>
    {
        share_point_ptr get_share(){
            return shared_from_this();
        }
        int a{10};
    };

    void test_study()
    {
        FUNCTION_START()

        //
        cout<<"\nchapter1.1\n  ";
        share_point_ptr gp1 = make_shared<Point>();
        share_point_ptr gp2 = gp1->get_share();
        cout<<gp1->a<<" | "<<gp2->a<<" | ";
        
        FUNCTION_END()
    }
}
