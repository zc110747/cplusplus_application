# learning_notes

按书籍逐章整理的笔记与配套代码，与 `knowledge/`（按知识点重组）互为补充：
这里保持**书的章节顺序**，便于原书对照阅读。

## modern_book

《现代 C++ 语言核心特性解析》的逐章实践，共 42 章，每章一对
`Chapter-NN/xxx.cpp` + `xxx.hpp`（少数章节只有 `.cpp`），由 `main.cpp` 依次调度。

| 章 | 主题 | 章 | 主题 |
| --- | --- | --- | --- |
| 01 | 基础类型 | 22 | 模板别名 |
| 02 | 命名空间 | 23 | `nullptr` |
| 03 | `auto` | 24 | 三向比较 `<=>` |
| 04 | `decltype` | 25 | `thread_local` |
| 05 | 后置返回类型 | 26 | `inline` 变量/函数 |
| 06 | 引用与移动语义 | 27 | `constexpr` / `consteval` |
| 07 | lambda 表达式 | 28 | 求值顺序 |
| 08 | 初始化 | 29 | 字面量与自定义字面量 |
| 09 | `initializer_list` / 指定初始化 | 30 | 对齐 |
| 10 | 构造与委托构造 | 31 | 属性说明符 |
| 11 | 联合体 | 32 | 预处理 |
| 12 | 委托构造函数 | 33 | 协程 |
| 13 | 继承与继承构造函数 | 34 | 其它优化（destroying delete、逗号下标等） |
| 14 | 强枚举 | 35 | 变量模板与形参包展开 |
| 15 | 聚合类型 | 36 | `typename` |
| 16 | `override` / `final` 与重写陷阱 | 37 | 模板改进 |
| 17 | 范围 for | 38 | 模板推导 |
| 18 | 初始化方式的选择 | 39 | 派生与继承 |
| 19 | `static_assert` | 40 | SFINAE |
| 20 | 结构化绑定 | 41 | concepts |
| 21 | `noexcept` | 42 | 模板优化（外部模板、`>>` 解析） |

### 构建

```bash
# 方式一：Make（与 knowledge/ 一致，产物 learning_notes/executables/ModernCpp）
cd learning_notes/modern_book
make

# 方式二：CMake（产物在本目录下，标准固定为 C++23）
cmake -S . -B build -G Ninja
cmake --build build
```

> 两种方式的产物路径不同：`Makefile` 输出到 `../executables/ModernCpp`，
> `CMakeLists.txt` 输出到本目录（见 `Problem.md` 的 B-08）。

### 说明

- 章节代码用 `includes.hpp` 里的版本宏（`__MORE_THAN_CPP20__` 等）区分不同标准下的分支，
  低版本编译器走 `__LOWRE_THAN_CPP20_RUN` 占位。
- 少数章节**故意演示错误/不推荐的写法**（如 `Chapter-16` 的错误重写、`Chapter-34` 的逗号下标、
  `Chapter-30` 已弃用的 `aligned_storage_t`），这些位置用
  `#pragma GCC diagnostic push/ignored/pop` 就近关闭告警，不要改成全局静音。
- `tags` 是 `make tags` 生成的 ctags 索引，**不入库**（已在 `.gitignore` 中）。
