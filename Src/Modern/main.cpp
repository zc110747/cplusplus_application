/******************************************************************
 * 现代C++语言指的是在C++11之后，增加的方便C++开发，以及增强功能的新特性，
 * 这些特性十分繁杂，因此这里也仅测试和展示其中的一部分。
 * 本篇参考书籍:
 * <现代C++语言核心特性解析>
 * 第一章 新基础类型 -- finish
 * 第二章 内联和嵌套命名空间 -- finish(namespace)
 * 第三章 auto占位符 -- finish(type)
 * 第四章 decltype说明符 -- finish(type)
 * 第五章 函数返回类型后置 -- finish(type)
 * 第六章 右值引用 -- finish(reference)
 * 第七章 lambda表达式 -- finish(lambda)
 * 第八章 非静态成员默认初始化 -- finish(class)
 * 第九章 列表初始化 -- finish(class)
 * 第十章 默认和删除函数 -- finish(class)
 * 第十一章 非受限联合类型 -- finish(class)
 * 第十二章 委托构造函数 -- finish(class)
 * 第十三章 继承构造函数 -- finish(class)
 * 第十四章 强枚举类型 -- finish(type)
 * 第十五章 扩展的聚合类型
 * 第十六章 override和final说明符
 * 第十七章 基于范围的for循环 -- finish(template)
 * 第十八章 支持初始化语句的if和switch -- finish(basic)
 * 第十九章 静态断言(static_assert) -- finish(constexpr)
 * 第二十章 结构化绑定
 * 第二十一章 noexcept
 * 第二十二章 类型别名和别名模板
 * 第二十三章 指针字面量nullptr -- finish(basic)
 * 第二十四章 三向比较(C++20)
 * 第二十五章 线程局部存储
 * 第二十六章 扩展的inline说明符
 * 第二十七章 常量表达式 -- finish(constexpr)
 * 第二十八章 确定的表达式求值顺序
 * 第二十九章 字面量优化
 * 第三十章   alignas和alignof
 * 第三十一章 属性说明符和标准属性
 * 第三十二章 新增预处理器和宏
 * 第三十三章 协程
 * 第三十四章 基础特性的其它优化
 * 第三十五章 可变参数模板
 * 第三十六章 typename优化
 * 第三十七章 模板参数优化
********************************************************************/
#include "modules/basic.hpp"
#include "modules/class.hpp"
#include "template/template.hpp"
#include "reference/reference.hpp"
#include "type/type.hpp"
#include "lambda/lambda.hpp"
#include "constexpr/constexpr.hpp"
#include "namespace/namespace.hpp"

int main(int argc, char *argv[])
{
    /********************************
     * 基础
    *********************************/
    //basic_process();

    /********************************
     * type
    *********************************/
    //type_process();

    /********************************
     * 类的处理
    *********************************/
    class_process();

    /********************************
     * 模板
    *********************************/
    //template_process();

    /********************************
     * 引用
    *********************************/
    //reference_process();

    /********************************
     * lambda标识符
    *********************************/
    //lambda_process();

    /********************************
     * constexpr
    *********************************/
    //constexpr_process();

    /********************************
     * namespace
    *********************************/
    //namespace_process();

    return 0;
}