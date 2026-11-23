# 设置 CMake 最低版本要求
cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(target)

# 指定 C++ 标准(源码含 concepts / 协程, 需要 C++20 以上)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 添加可执行文件
## 添加头文件搜索路径(工程根目录)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

## 添加子文件
file(GLOB_RECURSE SRC_FILES
            */*.cpp
            main.cpp
)
# 过滤掉 build 目录下的文件
list(FILTER SRC_FILES EXCLUDE REGEX ".*/build/.*\\.cpp$")

add_executable(target ${SRC_FILES})

# 链接 pthread 和 math 库到目标可执行文件
target_link_libraries(target PRIVATE pthread m)

# 添加宏定义
target_compile_definitions(target PRIVATE DEBUG_MODE)

# 添加编译选项
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Os")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os")