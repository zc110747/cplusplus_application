/******************************************************************
 * 现代C++语言指的是在C++11之后，增加的方便C++开发，以及增强功能的新特性，
 * 这些特性十分繁杂，因此这里也仅测试和展示其中的一部分。
 * 编译选项
 * gcc      -std=c++11 -lpthread -fconcepts, 可以是C++14,C++17,C++2a
 * msvc     /std:c++20  可以是C++17, C++14, C++11
 * clang    -std=c++11
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
 * 第11章 非受限联合类型
 * 第12章 委托构造函数
 * 第13章 继承构造函数
 * 第14章 强枚举类型
 * 第15章 扩展的聚合类型
 * 第16章 override和final运算符
 * 第17章 基于范围的for循环
 * 第18章 支持初始化语句的if和switch
 * 第19章 static_assert声明
 * 第20章 结构化绑定
 * 第21章 noexecpt关键字
 * 第22章 类型别名和别名模板
 * 第23章 指针字面量nullptr
 * 第24章 三向比较
 * 第25章 线程局部存储
 * 第26章 扩展的inline说明符
 * 第27章 常量表达式
 * 第28章 确定的表达式求值顺序
 * 第29章 字面量优化
 * 第30章 alignas和alignof
 * 第31章 属性说明符和标准属性
 * 第32章 新增预处理器和宏
 * 第33章 协程
 * 第34章 基础特性的其它优化
 * 第35章 可变参数模板
 * 第36章 typename优化
 * 第37章 模板参数优化
 * 第38章 类模板的模板实参推导
 * 第39章 用户自定义推导指引
 * 第40章 SFINAE
 * 第41章 概念和约束(C++20)
 * 第42章 模板特性的其它优化
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
#include "Chapter-11/union.hpp"
#include "Chapter-12/delegation.hpp"
#include "Chapter-13/inherit.hpp"
#include "Chapter-14/strong_enum.hpp"
#include "Chapter-15/aggregate.hpp"
#include "Chapter-16/override.hpp"
#include "Chapter-17/for_loop.hpp"
#include "Chapter-18/choose_init.hpp"
#include "Chapter-19/static_assert.hpp"
#include "Chapter-20/struct_bind.hpp"
#include "Chapter-21/noexcept.hpp"
#include "Chapter-22/template_alias.hpp"
#include "Chapter-23/nullptr.hpp"
#include "Chapter-24/spaceship.hpp"
#include "Chapter-25/thread_local.hpp"
#include "Chapter-26/inline.hpp"
#include "Chapter-27/constexpr.hpp"
#include "Chapter-28/eval_order.hpp"
#include "Chapter-29/literal.hpp"
#include "Chapter-30/align.hpp"
#include "Chapter-31/attribute.hpp"
#include "Chapter-32/preprocess.hpp"
#include "Chapter-33/coroutines.hpp"
#include "Chapter-34/other_optimize.hpp"
#include "Chapter-35/template_var.hpp"
#include "Chapter-36/typename.hpp"
#include "Chapter-37/template_improve.hpp"
#include "Chapter-38/template_derive.hpp"
#include "Chapter-39/derivation.hpp"
#include "Chapter-40/sfinae.hpp"
#include "Chapter-41/concept.hpp"
#include "Chapter-42/template_optimize.hpp"
#include <iostream>

#ifndef __CHAPTER 
#define __CHAPTER          1
#endif

typedef void (*pfunc)(void);

pfunc function_list[] = {
    BASE_TYPE::test_study,          NAMESPACE::test_study,        AUTO::test_study,             DECLTYPE::test_study,
    POSTF_RETURN::test_study,       REFERENCE::test_study,        LAMBDA::test_study,           INITIALIZATION::test_study,
    INITIALIZE_LIST::test_study,    CONSTRUCT::test_study,        UNION::test_study,            DELEGATION::test_study,
    INHERIT::test_study,            STRONG_ENUM::test_study,      AGGREGATE::test_study,        OVERRIDE::test_study,
    FOR_LOOP::test_study,           CHOOSE_INIT::test_study,      STATIC_ASSERT::test_study,    STRUCT_BIND::test_study,
    NOEXCEPT::test_study,           TEMPLATE_ALIAS::test_study,   NULLPTR::test_study,          SPACESHIP::test_study,
    THREAD_LOCAL::test_study,       INLINE::test_study,           CONSTEXPR::test_study,        EVAL_ORDER::test_study,
    LITERAL::test_study,            ALIGN::test_study,            ATTRIBUTE::test_study,        PREPROCESS::test_study,
    COROUTINES::test_study,         OTHER_OPTIMIZE::test_study,   TEMPLATE_VAR::test_study,     TYPENAME::test_study,
    TEMPLATE_IMPROVE::test_study,   TEMPLATE_DERIVE::test_study,  DERIVATION::test_study,       SFINAE::test_study,
    CONCEPT::test_study,            TEMPLATE_OPTIMIZE::test_study
};

int main(int argc, char *argv[])
{
    std::cout<<"cpp version:"<<__cplusplus<<std::endl;

    //must max than 1(include)
    if(__CHAPTER >= 1)
        function_list[__CHAPTER-1]();

    return 0;
}