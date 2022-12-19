/******************************************************************
 * 现代C++语言指的是在C++11之后，增加的方便C++开发，以及增强功能的新特性，
 * 这些特性十分繁杂，因此这里也仅测试和展示其中的一部分。
 * 本篇参考书籍:
 * <现代C++语言核心特性解析>
 * 第一章 新基础类型
 *          basic/basic.cpp
 * 
 * 第二章 内联和嵌套命名空间
 *          namespace/namespace.cpp
 * 
 * 第三章 auto占位符
 *          type/type.cpp
 * 
 * 第四章 decltype说明符
 *          type/type.cpp
 * 
 * 第五章 函数返回类型后置 
 *          type/type.cpp
 * 
 * 第六章 右值引用
 *          reference/reference.cpp
 * 
 * 第七章 lambda表达式 
 *          lambda/lambda.cpp
 * 
 * 第八章 非静态成员默认初始化
 *          basic/class.cpp
 * 
 * 第九章 列表初始化
 *          basic/class.cpp
 * 
 * 第十章 默认和删除函数 
 *          basic/class.cpp
 * 
 * 第十一章 非受限联合类型
 *          basic/class.cpp
 * 
 * 第十二章 委托构造函数
 *          basic/class.cpp
 * 
 * 第十三章 继承构造函数
 *          basic/class.cpp
 * 
 * 第十四章 强枚举类型
 *          type/type.cpp
 * 
 * 第十五章 扩展的聚合类型 
 *          basic/class.cpp
 * 
 * 第十六章 override和final说明符 
 *          basic/class.cpp
 * 
 * 第十七章 基于范围的for循环
 *          template/template.cpp
 * 
 * 第十八章 支持初始化语句的if和switch
 *           basic/basic.cpp
 * 
 * 第十九章 静态断言(static_assert)
 *           basic/assert.cpp
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
 * 
 * 第二十七章 常量表达式
 *           constexpr/constexpr.cpp
 * 
 * 第二十八章 确定的表达式求值顺序 
 *           basic/basic.cpp
 * 
 * 第二十九章 字面量优化 
 *           basic/basic.cpp
 * 
 * 第三十章   alignas和alignof 
 *           basic/align.cpp
 * 
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
 * 第四十章   @SFINAE
 * 第四十一章 @概念和约束
 * 第四十二章 @模板特性的其它优化  
********************************************************************/
#include "basic/basic.hpp"
#include "basic/class.hpp"
#include "template/template.hpp"
#include "reference/reference.hpp"
#include "type/type.hpp"
#include "type/noexcept_process.hpp"
#include "lambda/lambda.hpp"
#include "constexpr/constexpr.hpp"
#include "namespace/namespace.hpp"
#include "struct_bind/bind_process.hpp"
#include "thread/thread_process.hpp"
#include "thread/semaphore.hpp"
#include "basic/align.hpp"
#include "attribute/attribute.hpp"
#include "template/temp_arg.hpp"
#include "template/temp_improve.hpp"
#include "template/temp_deduction.hpp"

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
    //noexcept_process();

    /********************************
     * 类的处理
    *********************************/
    //class_process();

    /********************************
     * 模板
    *********************************/
    //template_process();
    //temp_arg_process();
    //template_improve();
    //temp_deduction();

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

    /********************************
     * tuple
    *********************************/
    //struct_bind_process();

    /********************************
     * thread
    *********************************/
    //thread_process();
    //semaphore_process();

    /********************************
     * align
    *********************************/
    align_process();

    /********************************
     * attribute
    *********************************/
    //attribute_process();
    
    return 0;
}