/******************************************************************
 * 现代C++语言指的是在C++11之后，增加的方便C++开发，以及增强功能的新特性，
 * 这些特性十分繁杂，因此这里也仅测试和展示其中的一部分。
 * 本篇参考书籍:
 * <现代C++语言核心特性解析>
 * 第01章 新基础类型
 * 第02章 内联和嵌套命名空间
 * 第03章 auto占位符
 * 第04章 decltype说明符
 * 第05章 函数返回类型后置
 * 第06章 右值引用
 * 第07章 lambda表达式
 * 第08章 非静态数据成员默认初始化
 * 第09章 列表初始化
 * 第10章 默认和删除函数
 * 第13章 继承构造函数
 * 第14章 强枚举类型
 * 第17章 基于范围的for循环
 * 第19章 static_assert声明
 * 第21章 noexecpt关键字
 * 第23章 指针字面量nullptr
 * 第24章 三向比较
 * 第26章 扩展的inline说明符
 * 第27章 常量表达式
 * 第28章 确定的表达式求值顺序
 * 第29章 字面量优化
 * 第30章 alignas和alignof
 * 第36章 typename优化
 * 第39章 用户自定义推导指引
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
#include "Chapter-26/inline.hpp"
#include "Chapter-27/constexpr.hpp"
#include "Chapter-28/eval_order.hpp"
#include "Chapter-29/literal.hpp"
#include "Chapter-30/align.hpp"
#include "Chapter-36/typename.hpp"
#include "Chapter-39/derivation.hpp"

constexpr int chapter = 39;

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
    else if (chapter == 26)
    {
        INLINE::test_study();
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
    else if (chapter == 39)
    {
        DERIVATION::test_study();
    }
    return 0;
}