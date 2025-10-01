# cpluspuls 2020~2023新特性

关于g++的新特性，使用gcc 15.2版本进行测试。

## concepts

concepts和reguires是C++20引入的重要特性，用于描述模板类型是否符合某些条件的特性。其中concepts是一种用于描述类型要求的语法，而requires则是用于在模板定义中指定这些要求的关键字。

## barrier

std::barrier是C++23引入的标准库类，位于<barrier>头文件，用于在多线程环境中同步多个线程的执行。

## format

std::format是C++20 引入的标准库函数，位于<format>头文件，用于实现类型安全且灵活的字符串格式化，替代传统的printf系列函数和std::stringstream。
