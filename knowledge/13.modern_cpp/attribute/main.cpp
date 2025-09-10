/*
1. attribute属性说明符
[[noreturn]]: 表示函数不会返回，通常用于标记函数不会正常返回
[[carries_dependency]]: 用于在多线程编程中优化内存访问顺序
[[fallthrough]]: 表示在switch语句中，当一个case语句执行完后，会继续执行下一个case语句，而不是跳出switch语句
[[nodiscard]]: 表示函数的返回值可能会被忽略
[[maybe_unused]]: 表示变量可能会被忽略
[[likely]]: 表示某个条件很可能为真
[[unlikely]]: 表示某个条件很可能为假
[[no_unique_address]]: 表示变量的地址可能会被重复使用
*/
#include <iostream>
#include <atomic>
#include <memory>
#include <cstdlib>

// [[noreturn]] 属性表示函数不会返回
[[noreturn]] void terminate_program() {
    std::cout << "程序即将终止" << std::endl;
    std::exit(1);
}

// [[carries_dependency]] 属性用于在多线程编程中优化内存访问顺序
void process_data(std::atomic<int>& data, [[carries_dependency]] int dependency) {
    // 假设这里有一些依赖于dependency的操作
    data.store(dependency, std::memory_order_release);
}

// [[fallthrough]] 属性表示在switch语句中，当一个case语句执行完后，会继续执行下一个case语句
void handle_event(int event) {
    switch (event) {
        case 1:
            std::cout << "处理事件1" << std::endl;
            [[fallthrough]];
        case 2:
            std::cout << "处理事件2" << std::endl;
            break;
        default:
            std::cout << "未知事件" << std::endl;
            break;
    }
}

// [[nodiscard]] 属性表示函数的返回值可能会被忽略
[[nodiscard]] int get_result() {
    return 42;
}

// [[maybe_unused]] 属性表示变量可能会被忽略
void some_function([[maybe_unused]] int value) {
    // 假设这里有一些操作，但value可能不会被使用
}

// [[likely]] 和 [[unlikely]] 属性表示某个条件很可能为真或为假
void process_condition([[likely]] bool condition) {
    if (condition) {
        std::cout << "条件很可能为真" << std::endl;
    } else {
        std::cout << "条件很可能为假" << std::endl;
    }
}

// [[no_unique_address]] 属性表示变量的地址可能会被重复使用
struct Empty {};

struct Data {
    int value;
    [[no_unique_address]] Empty empty;
};

int main() {
    std::atomic<int> data(0);
    int dependency = 42;

    process_data(data, dependency);

    handle_event(1);

    int result = get_result();
    if (result != 0) {
        std::cout << "获取到结果: " << result << std::endl;
    }

    some_function(10);

    process_condition(true);

    Data d;
    std::cout << "Data结构体大小: " << sizeof(d) << std::endl;
    
    terminate_program();

    return 0;
}