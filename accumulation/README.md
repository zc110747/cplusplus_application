# accumulation

保存在互联网/公众号上看到的学习文章对应的测试代码。凡是能确认来源的，都在文件内注明出处。

与 `knowledge/` 的区别：`knowledge/` 是按知识点重新组织的示例，这里是**按文章来源原样保留**的验证代码，
可能包含同一知识点的多种写法，用于对照。

## 文件索引

| 文件 | 内容 |
| --- | --- |
| `basic_any.cpp` / `includes/basic_any.hpp` | 手写 `any`：类型擦除、`copy()` 克隆、`dynamic_cast` 下行、`bad_any_cast` |
| `enable_if.cpp` / `includes/enable_if.hpp` | `std::enable_if` / `enable_if_t` 做 SFINAE 重载选择 |
| `enable_share.cpp` / `includes/enable_share.hpp` | `std::enable_shared_from_this` 的用法与前提（对象必须已由 `shared_ptr` 管理） |
| `lambda.cpp` / `includes/lambda.hpp` | lambda 捕获方式与泛型 lambda |
| `singleton.cpp` / `singleton.hpp` | 单例：局部静态变量（C++11 起线程安全）与 `std::call_once` 两种写法 |
| `main.cpp` | 依次调用上述各项 |

## 构建

```bash
cd accumulation
make          # 产物：../executables/ModernCpp
make ex       # 构建并直接运行
```

> 注意：`algorithm/` 与 `accumulation/` 的 `Makefile` 都输出 `../executables/ModernCpp`，
> 两者会互相覆盖，构建完一个再跑另一个（见 `Problem.md` 的 B-08）。

## 说明

- `basic_any` 的 `Derived<T>` 通过基类 `Base::copy()` 实现克隆，拷贝构造/拷贝赋值都是深拷贝；
  类型不匹配或对象为空时抛 `bad_any_cast`，不会落到未定义行为。
- 这一组代码用于验证「把标准库设施自己实现一遍会踩哪些坑」，正式项目请直接用 `std::any`。
