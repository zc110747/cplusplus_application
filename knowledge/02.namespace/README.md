# namespace

- [命名空间应用](#use_namespace)
- [嵌套命名空间](#nested_namespace)
- [内联命名空间](#inline_namespace)
- [命名空间别名](#namespace_alias)
- [返回主页](../../README.md)

## use_namespace

命名空间（namespace）是C++中用于解决命名冲突的一种机制；它允许你将全局作用域划分为不同的子作用域，每个子作用域都有自己的名字；这样就可以在不同的命名空间中定义相同名字的变量、函数或类，而不会产生冲突。

命名空间的定义格式如下：

```cpp
namespace <namespace_name> {
    // 声明或定义变量、函数、类等
}
```

命名空间的访问方式为：

```cpp
namespace_name::variable_name;
namespace_name::function_name();
namespace_name::ClassName object_name;
```

使用using关键字可以控制命名空间的访问权限，有两种形式：

1. using namespace namespace_name;：导入整个命名空间，使得该命名空间中的所有成员都可以直接使用，无需加上命名空间前缀。
2. using namespace_name::member_name;：只导入命名空间中的特定成员，该成员可以直接使用，无需加上命名空间前缀。

## nested_namespace

嵌套命名空间是指在一个命名空间内部再定义一个命名空间，访问嵌套命名空间中的成员时，需要使用多个::来指定完整的路径。其格式如下所示。

```cpp
namespace <namespace_name> {
    namespace <nested_namespace_name> {
        // 声明或定义变量、函数、类等
    } 
}

// 访问嵌套命名空间中的成员时，需要使用多个::来指定完整的路径：
<namespace_name>::<nested_namespace_name>::variable_name;
```

具体示例如下。

```cpp
#include <iostream>
namespace outer_namespace {
    namespace inner_namespace {
        int x = 10;
    }
}

int main(int argc, char *argv[]) 
{
    std::cout << outer_namespace::inner_namespace::x << std::endl;
    return 0;
}
```

## inline_namespace

内联命名空间是指在C++17及以上版本中，可以使用inline关键字来定义一个内联命名空间；访问内联命名空间中的成员时，可以使用嵌套的方式，也可以直接使用外层命名空间的名字。其格式如下所示。

```cpp
namespace <namespace_name> {
    inline namespace <nested_namespace_name> {
        // 声明或定义变量、函数、类等
    } 
}

// 访问内联命名空间中的成员时，需要使用多个::来指定完整的路径：
<namespace_name>::<nested_namespace_name>::variable_name;
<namespace_name>::variable_name;
```

详细示例如下。

```cpp
#include <iostream>

namespace outer_namespace {
    inline namespace inner_namespace {
        int x = 10;
    }
}

int main(int argc, char *argv[]) 
{    
    std::cout << outer_namespace::x << std::endl;
    
    outer_namespace::x = 20;
    std::cout << outer_namespace::x << std::endl;

    outer_namespace::inner_namespace::x = 30;

    std::cout << outer_namespace::inner_namespace::x << std::endl;
    return 0;
}
```

嵌套命名空间和内联命名空间可以组合使用（需要C++20支持），格式为：

```c
namespace outer_namespace::inline inner_namespace {
    // 声明或定义变量、函数、类等
}
```

## namespace_alias

命名空间别名是指给一个命名空间起一个别名，格式为：

```cpp
// 命名空间设置别名
namespace <alias_name> = <namespace_name>;
```

使用命名空间别名可以简化代码，减少重复输入，提高代码可读性。

```cpp
#include <iostream>

namespace ns = std;

int main(int argc, char *argv[]) 
{
    ns::cout << "Hello, world!" << ns::endl;
    return 0;
}
```
