/******************************************************************
 * 现代C++语言指的是在C++11之后，增加的方便C++开发，以及增强功能的新特性，
 * 这些特性十分繁杂，因此这里也仅测试和展示其中的一部分。
 * 本篇参考书籍:
 * <现代C++语言核心特性解析>
 * 
 * 第十一章 非受限联合类型
 *          basic/class.cpp
 * 
 * 第十二章 委托构造函数
 *          basic/class.cpp
 * 
 * 第十五章 扩展的聚合类型 
 *          basic/class.cpp
 * 
 * 第十六章 override和final说明符 
 *          basic/class.cpp
 * 
 * 第十八章 支持初始化语句的if和switch
 *           basic/basic.cpp
 * 
 * 第二十章 结构化绑定
 *          struct_bind/bind_process.cpp
 * 
 * 第二十一章 noexcept关键字
 *          type/noexcept_process.cpp
 * 
 * 第二十二章 类型别名和别名模板 
 *          template/template.cpp
 * 
 * 第二十三章 指针字面量nullptr -- finish(modules/basic)
 *          basic/basic.cpp
 * 
 * 第二十四章 @三向比较(C++20)
 * 第二十五章 线程局部存储 
 *          thread/thread_process.cpp
 * 
 * 第二十六章 扩展的inline说明符
 *           basic/class.cpp

 * 第三十一章 属性说明符和标准属性 
 *           attribute/attribute.cpp
 * 
 * 第三十二章 新增预处理器和宏
 *           basic/basic.cpp 
 * 
 * 第三十三章 @协程
 * 第三十四章 @基础特性的其它优化
 * 
 * 第三十五章 可变参数模板
 *           template/temp_arg
 * 
 * 第三十六章 typename优化
 *           template/temp_arg
 * 
 * 第三十七章 模板参数优化    
 *           template/temp_improve
 * 
 * 第三十八章 类模板的模板实参推导
 *           template/temp_deduction
 * 
 * 第三十九章 用户自定义推导指引
 *           template/temp_deduction
 * 
 * 第四十章   SFINAE
 *           template/template_SFINAE
 * 
 * 第四十一章 @概念和约束
 * 第四十二章 @模板特性的其它优化  
********************************************************************/
#include "Chapter-01/base_type.hpp"
#include "Chapter-02/namespace.hpp"
#include "Chapter-03/auto.hpp"
#include "Chapter-04/decltype.hpp"
#include "Chapter-05/postf_return.hpp"
#include "Chapter-06/reference.hpp"
#include "Chapter-07/lambda.hpp"
#include "Chapter-08/initialization.hpp"
#include "Chapter-09/initializer_list.hpp"
#include "Chapter-10/construct.hpp"
#include "Chapter-13/inherit.hpp"
#include "Chapter-14/strong_enum.hpp"
#include "Chapter-17/for_loop.hpp"
#include "Chapter-19/static_assert.hpp"
#include "Chapter-21/noexcept.hpp"
#include "Chapter-23/nullptr.hpp"
#include "Chapter-24/spaceship.hpp"
#include "Chapter-27/constexpr.hpp"
#include "Chapter-28/eval_order.hpp"
#include "Chapter-29/literal.hpp"
#include "Chapter-30/align.hpp"
#include "Chapter-36/typename.hpp"

constexpr int chapter = 24;

int main(int argc, char *argv[])
{
    if(chapter == 1)
    {
        BASE_TYPE::test_study();
    }
    else if (chapter == 2)
    {
        NAMESPACE::test_study();
    }
    else if (chapter == 3)
    {
        AUTO::test_study();
    }
    else if (chapter == 4)
    {
        DECLTYPE::test_study();
    }
    else if (chapter == 5)
    {
        POSTF_RETURN::test_study(); 
    }
    else if (chapter == 6)
    {
        REFERENCE::test_study();
    }
    else if (chapter == 7)
    {
        LAMBDA::test_study();
    }
    else if (chapter == 8)
    {
        INITIALIZATION::test_study();
    }
    else if (chapter == 9)
    {
        INITIALIZE_LIST::test_study();
    }
    else if (chapter == 10)
    {
        CONSTRUCT::test_study();
    }
    else if (chapter == 13)
    {
        INHERIT::test_study();
    }
    else if (chapter == 14)
    {
        STRONG_ENUM::test_study();
    }
    else if (chapter == 17)
    {
        FOR_LOOP::test_study();
    }
    else if (chapter == 19)
    {
        STATIC_ASSERT::test_study();
    }
    else if (chapter == 21)
    {
        NOEXCEPT::test_study();
    }
    else if (chapter == 23)
    {
        NULLPTR::test_study();
    }
    else if (chapter == 24)
    {
        SPACESHIP::test_study();
    }  
    else if (chapter == 27)
    {
        CONSTEXPR::test_study();
    }
    else if (chapter == 28)
    {
        EVAL_ORDER::test_study();
    }
    else if (chapter == 29)
    {
        LITERAL::test_study();
    }
    else if (chapter == 30)
    {
        ALIGN::test_study();
    }
    else if (chapter == 36)
    {
        TYPENAME::test_study();
    }

    return 0;
}