//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      choose_init.cpp
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
#include "choose_init.hpp"
#include "../includes.hpp"
#include <condition_variable>
#include <chrono>
using namespace std;

namespace CHOOSE_INIT
{
    void test_run()
    {
        FUNCTION_START()

        //18.1 支持初始化语句的if
        //if语句中支持初始化语句, 结构为if(init, condition)
        //if中的初始化变量贯穿整个生命周期，else if中初始化变量只存在于else if和后续的语句
        cout<<"\nchapter18.1\n  ";
        #if __MORE_THAN_CPP17__
        auto foo1 = [](int x)->bool
        {
            if(x>3) 
                return false;
            return true;
        };
        if(bool b = foo1(4); b){
            cout<<std::boolalpha<<b<<" 3"<<" | ";
        }
        else if(bool b1 = foo1(2); b1){
            cout<<std::boolalpha<<b<<" 2"<<" | ";
        }
        else
        {
            cout<<"null"<<" | ";
        }
        #else
        __LOWRE_THAN_CPP17_RUN;
        #endif

        //18.1 支持初始化语句的switch
        //switch语句中支持初始化语句, 结构为switch(init, condition)
        cout<<"\nchapter18.2\n  ";
        #if __MORE_THAN_CPP17__
        switch(int a=1; a)
        {
            case 1:
                cout<<1<<" | ";
                break;
            default:
                cout<<"default"<<" | ";
                break;
        }

        std::condition_variable cv;
        std::mutex cv_m;

        switch (std::unique_lock lk(cv_m); cv.wait_for(lk, 100ms))
        {
        case std::cv_status::timeout:
            cout<<"cv timeout"<<" | ";
            break;
        
        case std::cv_status::no_timeout:
            cout<<"cv no_timeout"<<" | ";
            break;
        }
        #else
        __LOWRE_THAN_CPP17_RUN;
        #endif 
        FUNCTION_END()
    }
}
