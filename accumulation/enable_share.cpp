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
//     	@公众号：<嵌入式技术总结>
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

/*
enable_shared_from_this是一个基类模板

1. 用于解决在类成员函数中获取类对象的shared_ptr的需求。它提供了一种机制，使类能够安全地从成员函数内部获得指向自身的shared_ptr。
2. 对于普通结构体/类指针，通过this可以获取对象本身，对于share_ptr声明的指针对象，this被share_ptr对象管理，相同的功能我们就需要获取share_ptr对象，shared_from_this()就是基于此设计的。
*/
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

    void test_run()
    {
        FUNCTION_START()

        share_point_ptr gp1 = make_shared<Point>();
        share_point_ptr gp2 = gp1->get_share();
        share_point_ptr gp3 = gp1->shared_from_this();

        cout<<gp1->a<<" | "<<gp2->a<<" | ";

        //返回引用计数，此时引用计数都为3
        cout<<gp1.use_count()<<" | ";
        cout<<gp2.use_count()<<" | ";

        //the object belongs to the same owner group as x, this function returns false
        if(!gp1.owner_before(gp2) && !gp2.owner_before(gp1))
            cout<<"gp1 and gp2 share ownership";

        FUNCTION_END()
    }
}
