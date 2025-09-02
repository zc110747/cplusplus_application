//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      strong_enum.cpp
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
#include "strong_enum.hpp"
#include "../includes.hpp"

using namespace std;

namespace STRONG_ENUM
{
    //14.1 枚举类型的弊端
    enum School{
        principal,
        teacher,
        student
    };
    enum Company{
        chairman,
        manage,
        employee,
    };
    enum E{
        e1 = 1,
        e2 = 2,
        e3 = 0xfffffff0
    };

    template<int a, int b>
    struct add{
        enum {
            result = a+b
        };
    };

    //14.2 使用强枚举类型
    enum class HighSchool : unsigned int
    {
        student = 1,
        teacher,
        principal,
    };

    enum class University: unsigned int
    {
        student = 1,
        teacher,
        principal,
    };

    enum class E2:unsigned int{
        e2_1 = 1,
        e2_2,
        e2_3 = 0xfffffff0
    };
    
    //14.3 列表初始化有底层类型枚举对象
    enum class Color{
        Red,
        Green,
        Blue
    };
    enum Color2:int{
        Red,
        Green,
        Blue
    };
    
    template<typename T>
    auto print_enum(T const &value)->typename std::underlying_type<T>::type{
        return static_cast<typename std::underlying_type<T>::type>(value);
    }

    //14.4 使用using打开强枚举类型
    #if __MORE_THAN_CPP20__
    const char* get_color(Color c)
    {
        using enum Color;
        switch(c)
        {
            case Red:
                return "Red";
            case Green:
                return "Green";
            case Blue:
                return "Blue";
            default:
                return "none";
        }
    }

    const char* get_color1(Color c)
    {
        switch(c)
        {
            using Color::Red;
            case Red:
                return "Red";
            case Color::Green:
                return "Green";
            case Color::Blue:
                return "Blue";
            default:
                return "none";
        }
    }
    #endif

    void test_run()
    {
        FUNCTION_START()

        //14.1 枚举类型的弊端
        //不同的枚举类型在比较时会隐式转换为整型
        cout<<"\nchapter14.1\n  ";
        School x1 = student;
        Company y1 = manage;
        auto b1 = (x1 == y1);  
        cout<<b1<<" | ";
        auto c1 = e1<e3; //不同平台到enum存储不一致，输出可能不一致
        cout<<c1<<" | ";
        cout<<add<3, 5>::result<<" | ";

        //14.2 使用强枚举类型
        //通过在类型种添加class，使用强枚举类型
        //1.枚举标识符属于强枚举类型的作用域
        //2.枚举标识符不会隐式转换为整型
        //3.能指定强枚举类型的底层类型，默认为int类型
        //4.枚举类型也可以指定底层类型
        cout<<"\nchapter14.2\n  ";
        HighSchool x_2 = HighSchool::student;
        University y_2 = University::student;
        bool b_2 = x_2 < HighSchool::principal;
        cout<<b_2<<" | ";
        E2 z_2 = E2::e2_1;
        bool c_2 = E2::e2_1 < E2::e2_2;
        cout<<c_2<<" | ";

        //14.3 列表初始化有底层类型枚举对象
        //对于指定了底层类型的枚举对象(强枚举类型默认使用int)，可以使用列表初始化
        //强枚举类型的列表初始化不支持缩窄转换
        //Color c{7.5}; //编译不通过
        //强枚举类型不会和整型隐式相互转换
        //Color c = 5; 或 int a = c; //编译不通过
        //通过std::underlying_type<T>::type获得列表初始化的底层类型
        cout<<"\nchapter14.3\n  ";
        #if __MORE_THAN_CPP17__
            Color c_3{1};
            Color2 c2_3{2};
            cout<<print_enum(c_3)<<" | "<<print_enum(c2_3)<<" | ";
        #else
            __LOWRE_THAN_CPP17_RUN;
        #endif 

        //19.4 使用using打开强枚举类型
        cout<<"\nchapter14.4\n  ";
        #if __MORE_THAN_CPP20__
        auto ptr = get_color(Color::Red);
        cout<<ptr<<" | ";

        ptr = get_color1(Color::Red);
        cout<<ptr<<" | ";
        #else
        __LOWRE_THAN_CPP20_RUN;
        #endif

        FUNCTION_END()
    }
}

