//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      signal_leton.cpp
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
#include "signal_leton.hpp"
#include "../includes.hpp"

using namespace std;

namespace SIGNAL_LETON
{
    class signal_leton
    {
    public:
        static signal_leton* get_instance()
        {
            if(instance_ptr_ == nullptr)
            {
                cout<<"create signal_leton"<<" | ";
                instance_ptr_ = new signal_leton();
            }
            return instance_ptr_;
        }

        void show()
        {
            cout<<"show signal_leton"<<" | ";
        }

        ~signal_leton(){
            cout<<"release"<<" | ";
        }

        void release()
        {
            if(instance_ptr_ != nullptr)
            {
                delete instance_ptr_;
                instance_ptr_ = nullptr;
            }
        }
    private:
        inline static signal_leton *instance_ptr_{nullptr};
    };

    void test_study()
    {
        FUNCTION_START()

        signal_leton::get_instance()->show();
        signal_leton::get_instance()->show();
        signal_leton::get_instance()->release();
        signal_leton::get_instance()->show();
        signal_leton::get_instance()->release();
        FUNCTION_END()
    }
}
