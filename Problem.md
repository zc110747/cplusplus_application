# Problem.md —— 项目问题清单

> 本文正文只**记录问题**，条目描述保持取证时的原始状态（基于 commit `7574c3b`），
> 修复进展统一记录在下方「修复状态」表中。所有条目均基于**实际编译 / 实际运行**取证，
> 未验证的推测会被显式标注「未验证」。
> 正文保留取证时的原始描述（"未改动 README / 源码"指取证阶段而言），实际修复已全部落地。

- 取证时间：2026-11-23
- 编译器：`g++ 15.2.0`（MSYS2 mingw64）、`GNU Make 4.2.1`、`CMake 4.2.1`
- 方法：在仓库**副本**中执行 `make`（不污染工作区）、`-Wall -Wextra` 全量语法检查、
  对可疑代码单独编译并在独立进程中运行以观察行为
- 收尾回归（2026-11-23）：副本内 `make` 全量 97/97 目标通过、**0 warning**（`-Wall -Wextra` 已进 `build.mk`）；
  关键验证程序（`t_any` / `t_list` / `t_bmp` / `t_bmp_edge` / `t_coro_lifetime` / `t_resptr`）复跑全部通过

## 修复状态

| 条目 | 状态 | 验证证据 |
| --- | --- | --- |
| B-01 ~ B-05 | ✅ 已修复 | 副本内全量构建 97/97 个 Makefile 目标通过 |
| B-06 `rm -f $(objects)` | ✅ 已修复 | 连续两次 `make` 不再重新编译（仍会重复链接，原因见 B-10） |
| M-01 析构条件写反 | ✅ 已修复 | 1000 次构造/析构：`new=1000 delete=1000 leaked=0` |
| M-02 自赋值 | ✅ 已修复 | `b = b` 后取回原值 `7`，不再 `terminate` |
| M-03 `Base` 缺虚析构 | ✅ 已修复 | `-Wdelete-non-virtual-dtor` 无告警 |
| M-04 `static_cast` 下行转换 | ✅ 已修复 | 类型不符 / 空对象均抛 `bad_any_cast` |
| M-05 单链表 | ✅ 已修复 | `delete_elem(3/100/-1)` 返回 ERROR 且无崩溃；析构 `new=5 delete=5`；深拷贝互不影响；左值实参不再被移动 |
| M-06 双链表 | ✅ 已修复 | 删除唯一节点后反向遍历为空（`tail` 不再悬垂）；删空后正反向均为空；析构无泄漏 |
| M-08 BMP 位深 < 8 | ✅ 已修复 | 5 个自带样例（1/4/8/24/32bpp）逐像素与独立解析器完全一致 |
| M-09 BMP 其它缺陷 | ✅ 已修复 | 24bpp 宽 3 行对齐、负高度 top-down、截断文件、非 bmp 文件、空对象存盘不破坏文件：5 项边界用例通过 |
| M-07 `resource_ptr` 可拷贝 | ✅ 已修复 | `static_assert` 校验拷贝构造/拷贝赋值已删除、移动可用且 `noexcept`；移动后源指针为 `0`；`new=2 delete=2 leaked=0` |
| M-10 `operator new/delete` 不成对 | ✅ 已修复 | 实跑 `07.std/new`：`safety memory` 8 → 32 → 40 → **0**（`delete` / `delete[]` / `nothrow delete` 三条路径全部归还）；5 线程 × 200 轮 × 2 次分配后仍为 **0** |
| T-01 ~ T-03 定时器 | ✅ 已修复 | 改为持有 `std::thread` 并 `join`；`stop()` 用 `exchange`、`run()` 用谓词 `wait_for`；实跑循环数稳定在 5 次后正常退出（不再刷屏） |
| T-04 / T-05 协程库生命周期 | ✅ 已修复 | `TimerHandle`/`Event`/`Channel` 持有 `weak_ptr` 存活令牌；旧头文件同测试 `exit=139`（SIGSEGV），新头文件输出 `cancel after scheduler death: 1 (no crash)` |
| T-06 / T-07 thread_pool | ✅ 已修复 | `init()` 幂等（`workers_` 非空即返回）；`std::invoke_result_t` 替换 `std::result_of`；实跑 10 个任务全部完成 |
| T-08 multi_queue | ✅ 已修复 | `sprintf` → `snprintf` 并校验长度与缓冲区边界；实跑输出 `high thread N` / `low thread N` 无越界（该示例本身是 Ctrl+C 结束的常驻观察程序） |
| T-09 `thread/main.cpp` detach | ✅ 已修复 | `detach()` → `join()`，不再依赖进程存活；实跑正常退出 |
| T-10 CRC 表无同步 | ✅ 已修复 | 惰性建表改用 `std::call_once` + `std::once_flag`；实跑 `calc_crc8: 0x6D` 与 CRC-8/MAXIM 标准值一致 |
| L-01 ~ L-04 logger | ✅ 已修复 | 模板定义移入头文件并新增 `logger_user.cpp` 第二个 TU 验证（`log from another translation unit: 42`）；宏改 `do{...}while(0)` 无分号；`LOG_MODULE_ON` 默认开；去掉头文件里的 `using namespace` |
| L-05 / L-06 `Event` / `event_bus` | ✅ 已修复 | `Event` 补充线程契约注释；`Callback` 改 `std::function<void(const Event&, const void*)>`；实跑输出 `duplicate subscribe accepted: 0`（重复注册被拒） |
| L-07 `safety_cpp` | ✅ 已修复 | `~A(){}` → `virtual ~A() = default;`；`static_assert` 文案与条件一致；`MyColor MyColor;` 改名消除类型/变量同名 |
| L-08 `calc_md5` 返回类型 | ✅ 已修复 | 改为 `unsigned int` 并逐级检查返回；实跑 `calc md5-32: 486eb652…`、`calc md5-16: 74adb864…` 与标准一致 |
| U-01 保留标识符 | ✅ 已修复 | 保护宏统一为 `*_HPP_`（如 `THREAD_POOL_HPP_`、`MATH_TOOLS_HPP_`） |
| U-02 `_Pragma("once")` | ✅ 已修复 | 51 处统一为 `#pragma once` |
| U-03 `09.layout` memcpy 示范 | ✅ 已修复 | 对含析构函数的类型不再 `memcpy`，改赋值并断言 `is_trivially_copyable_v == false`；另加 `PlainPoint` 演示合法 `memcpy` |
| U-04 `calc_crc32` 类型标注 | ✅ 已修复 | 中间变量 `(uint16_t)` → `(uint32_t)`，`printf` 用 `0x%08X` |
| U-05 `exec` GCC 扩展 | ✅ 已修复 | 语句表达式改即时 lambda；`popen` 演示改本地 `echo`，不再依赖外网；`main()` 去参数 |
| U-06 `unistd.h` 可移植性 | ✅ 已修复 | 删除 24 处未使用的 `#include <unistd.h>` |
| D-01 ~ D-04 README / plans | ✅ 已修复 | 12 处事实错误全部订正（`unordered_set/multiset` 描述、`random_shuffle`→`shuffle`、`std::scope_lock`→`std::scoped_lock`、`std::disable_if` 删除、编译器版本、`excepted`/`SFINEA`/目录名） |
| D-05 `typeindex` 示例缺失 | ✅ 已修复 | `11.type_conversion/main.cpp` 新增 `TYPE_INDEX` 段（`std::type_index` + `unordered_map` + `hash_code`），实跑有输出 |
| D-06 `15.tools` 无 README | ✅ 已修复 | 新增 `knowledge/15.tools/README.md`（11 个子模块索引 + 构建方式 + 已知限制），根 README 已加链接 |
| D-07 注释与实现不符 | ✅ 已修复 | 文件名/选项/语义注释逐条对齐实现 |
| S-01 297 条告警 / 未开 `-Wall -Wextra` | ✅ 已修复 | **297 → 0**：`build/build.mk` 启用 `-Wall -Wextra`，`-lpthread -lm` 从 `CFLAGS` 拆到 `LDLIBS`；全量构建 97/97 且 **0 warning** |
| S-02 属性示例无效 | ✅ 已修复 | `is_constant_evaluated` 由 `if constexpr` 改普通 `if`（否则恒为 true）；`[[noreturn]]` 函数改为真的不返回；`X<=Y<=Z` 加括号说明；逗号下标等示教点改为 `#pragma GCC diagnostic` 就近关闭 |
| S-03 `tags` 生成物入库 | ✅ 已修复 | `git rm --cached learning_notes/modern_book/tags`；`.gitignore` 新增 `tags` 规则（`git check-ignore` 已验证） |
| S-04 `.gitignore` 失效规则 | ✅ 已修复 | 删除不存在的 `asio_book/asio/*` |
| S-06 索引完整性 | ✅ 已修复 | 新增 `algorithm/README.md`、`accumulation/README.md`（3 行 → 完整索引）、`learning_notes/README.md`（42 章对照表）；根 `README.md` 增加「其它目录」一节 |
| B-11 `build/` 索引大小写 | ✅ 已修复 | `git mv Build→build` 纯大小写重命名，前后 blob 哈希同为 `f5d089f`；索引现有路径 `build/build.mk` 与 97 个 Makefile 的 `include ../build/build.mk` 一致 |
| B-10 / S-05 产物名 `target` vs `target.exe` | ⬜ 待处理 | 需同步修改 97 个 `Makefile`（加 `$(EXEEXT)`），本轮未做；只影响增量构建效率，不影响正确性 |

## 摘要

| 类别 | 条目数 | 严重度 | 说明 |
| --- | --- | --- | --- |
| 构建失败 | 4 / 97 个 Makefile 目标（B-01/02/04/05），另加 1 条 CMake 路径不可用（B-03） | P0 | 2 个是源码/工具链真实缺陷，2 个是构建脚本本身写错 |
| 构建脚本质量 | 4 条（B-06~B-09） | P1/P2 | 增量构建失效、`build.mk` 选项混乱、产物同名冲突、残留目录 |
| 崩溃 / 段错误 | 3 处（已实测 SIGSEGV / `std::terminate`） | P0 | 链表越界删除 2 处、basic_any 自赋值 1 处 |
| 确定性资源泄漏 | 1 处（实测 1000/1000 未释放） | P0 | `basic_any` 析构条件写反 |
| 悬垂对象 / 生命周期 | 5 条（T-01~T-05） | P1 | `detach()` + `this`、`TimerHandle` 与 Scheduler 生命周期等 |
| 逻辑 / 接口缺陷 | 8 条（L-01~L-08） | P1 | 模板声明与定义分离、宏语法、BMP 位深处理等 |
| 未定义行为 / 可移植性 | 6 条（U-01~U-06） | P2 | 保留标识符、GCC 扩展、`malloc` 与 `new` 混用 |
| 文档与代码不一致 | 7 条（D-01~D-07，含 12 处具体错误） | P2 | 描述互换、API 名错误、✅ 标记无对应代码 |
| 规范与可维护性 | 6 条（S-01~S-06） | P3 | 297 条告警、重复文件、孤岛代码、生成物入库 |

---

## 1. 构建与工具链问题

### B-01 `knowledge/01.base_style` 编译失败：`__int32_t` 非标准类型（P0）

- 位置：`knowledge/01.base_style/basic.cpp:92`
- 代码：`__int32_t a8 = 1;`
- 现象：`make` 直接失败，整个 01 模块无法产出 `target`
- 证据（实际执行）：

```
$ make -C knowledge/01.base_style
basic.cpp:92:9: error: '__int32_t' was not declared in this scope; did you mean 'uint32_t'?
make: *** [../../build/build.mk:21: basic.o] Error 1
```

- 说明：`__int32_t` 是 MSVC 的内部类型名，标准库与 libstdc++ 都不提供。该文件注释声称用于演示"类型推导"，
  与平台相关的类型名混入可移植示例，且 Makefile 的 `objects` 列表确实包含 `basic.o`，属于**主干构建路径**。

### B-02 `learning_notes/modern_book/Makefile` 引用路径错误，make 无法启动（P0）

- 位置：`learning_notes/modern_book/Makefile:68`
- 代码：`include ../build/build.mk`（该目录下没有 `build/`，`build.mk` 在仓库根）
- 证据：

```
$ make -C learning_notes/modern_book
Makefile:68: ../build/build.mk: No such file or directory
make: *** No rule to make target '../build/build.mk'.
```

- 说明：同为二级目录的 `accumulation/`、`algorithm/` 用 `../build/build.mk` 是对的；
  `modern_book` 深了一层，应为 `../../build/build.mk`。**该模块的 Makefile 构建 100% 不可用。**

### B-03 同一目录的 CMake 构建同样不可用（P0）

- 位置：`learning_notes/modern_book/CMakeLists.txt`
- 三个独立问题：
  1. `:9 set(CMAKE_SYSTEM_NAME Linux)` —— 在 Windows 主机上强制声明交叉编译目标系统；
  2. `:12 set(CMAKE_CXX_STANDARD 17)` —— 源码含 `concepts` / 协程（`Chapter-33/coroutines.cpp`），C++17 编不过；
  3. `:20 include_directories(*)` —— 字面量 `*`，无意义的搜索路径。
- 证据（在仓库副本中实跑）：

```
$ cmake -S . -B build -G "Unix Makefiles"   # 配置阶段通过
$ cmake --build build
.../include/c++/15.2.0/coroutine:43:3: error: #error "the <coroutine> header requires -fcoroutines"
make.exe[2]: *** [CMakeFiles/target.dir/Chapter-33/coroutines.cpp.obj] Error 1
```

- 结论：`modern_book` 的 **Makefile 与 CMake 两条构建路径都不可用**，只有存在旧二进制时才"看起来能用"。

### B-04 `12.thread_coroutine/execution` 链接失败：`-ltbb` 未提供（P0）

- 位置：`knowledge/12.thread_coroutine/execution/Makefile`（`STD_VERSION = -std=c++23 -ltbb`）
- 证据：

```
ld.exe: cannot find -ltbb: No such file or directory
make: *** [../../../build/build.mk:24: target] Error 1
```

- 附带缺陷：把链接库 `-ltbb` 塞进了 `STD_VERSION` 变量，导致编译阶段也会带上该链接选项
  （`g++ -std=c++23 -ltbb -lpthread -lm -fconcepts -c main.cpp`），语义混乱。

### B-05 `knowledge/06.stl/mdspan` 无法构建：`<mdspan>` 在本工具链不存在（P0）

- 位置：`knowledge/06.stl/mdspan/main.cpp:18`（`#include <mdspan>`）
- 证据：

```
$ ls $(g++ -print-file-name=include)/../ | grep mdspan     # 无输出
$ make -C knowledge/06.stl/mdspan
main.cpp:18:10: fatal error: mdspan: No such file or directory
```

- 说明：libstdc++ 15.2 已提供 `expected / flat_map / generator / print / spanstream / stacktrace`，
  **唯独缺少 `mdspan`**（实测目录比对）。而 `knowledge/plans.md` 把 mdspan 标记为 ✅ 已完成——
  即"计划表声称完成，实际不可编译"。该目录也因此没有产物（其他目录都有）。

### B-06 构建系统每次都全量重编译，增量构建完全失效（P1）

- 位置：`build/build.mk:23-25`（链接规则里 `rm -f $(objects)`，见第 25 行）
- 证据（同一目录连续两次 `make`，源码未改动）：

```
$ make && make
g++ ... -c main.cpp -o main.o        # 第一次
g++ -o target main.o ...
rm -f main.o
g++ ... -c main.cpp -o main.o        # 第二次，仍然重新编译
g++ -o target main.o ...
rm -f main.o
```

- 影响：97 个目标目录、152 个 `.cpp`，任何一次全量构建都要重编全部翻译单元；同时使 `make` 失去了依赖检查的意义。

### B-07 `build/build.mk` 的通用缺陷（P1）

| 位置 | 问题 |
| --- | --- |
| `build.mk:14` | `CFLAGS += ... -lpthread -lm ...`：链接库混进编译选项，`-c` 阶段无意义 |
| `build.mk:14` | `-fconcepts` 是 Concepts **TS**（C++17）开关，C++20/23 下无需且易误导 |
| `build.mk:14` | 全局没有 `-Wall -Wextra`，导致 297 条告警长期无人处理（见 S-01） |
| `build.mk:11` | `STD_VERSION ?= -std=c++20` 作为默认值，但 README 与多个模块实际使用 C++23 特性；30+ 个 Makefile 未显式指定标准，属"默认值陷阱" |
| `build.mk:26` | `mkdir $(executables_path)` 未加 `-p`，且路径未加引号 |
| `build.mk:30-35` | `clean` 只删 `.o`，不删产物；`tags: ctags -R *` 会在当前目录生成 `tags`（正是 `learning_notes/modern_book/tags` 入库的来源） |

### B-08 三个工程输出同名产物，且输出目录不一致（P1）

- `accumulation/Makefile:24-25`、`algorithm/Makefile`：`executables = ModernCpp`、`executables_path = ../executables`
  → 两个工程写**同一个文件** `executables/ModernCpp`，后构建者覆盖前者。
- `learning_notes/modern_book/Makefile:62-63`：同样 `ModernCpp`，但 `../executables` 解析为
  `learning_notes/executables/`（与前者不是同一目录，也不在 README 描述的任何位置）。
- `knowledge/09.layout`、`10.smart_pointer`、`11.type_conversion` 输出到仓库根 `executables/`
  （`studylayout` / `studysmartpoint` / `studytypecast`），与前两者风格又不一致。

### B-09 残留目录与孤岛文件（P2）

| 位置 | 问题 |
| --- | --- |
| `knowledge/14.cpp_new_feature/span/` | 目录里只有一个构建产物 `target`，**没有 `main.cpp`、没有 `Makefile`**（真正的 spanstream 示例在 `06.stl/span/`） |
| `knowledge/06.stl/mdspan/` | 有 `Makefile` 但永远无法生成产物（B-05） |
| `learning_notes/modern_book/basic/other_improve.cpp` | 定义了 `other_improve_process()`，但 Makefile 的 `objects` 未列出、`main.cpp` 也未调用 → 从未被编译的死代码 |
| `algorithm/template.txt` 与 `learning_notes/modern_book/template.txt` | 两份内容完全相同（md5 `e75569af...`）的重复文件 |
| `.vscode/settings.json`（99 行）与 `cplusplus.code-workspace`（72 行） | 两份重复维护的 `files.associations` 清单，内容高度重叠 |

### B-10 Windows 下产物名 `target` 与 `target.exe` 不一致，导致每次 make 都重复链接（P2）

- 位置：全部 `executables = target` 的模块 + `build/build.mk:23-29`
- 现象：`make` 每次都重新执行链接命令，`make --debug=b` 输出 `File 'target' does not exist.`，
  而磁盘上实际是 `target.exe`（`ls -la target*` 可见）
- 证据：

```
$ make --debug=b 2>&1 | grep target
 File 'all' does not exist.
   File 'target' does not exist.
  Must remake target 'target'.
g++ -o target main.o ...
```

- 原因：mingw64 的 `g++ -o target` 会自动补 `.exe` 后缀（实测 `g++ -o mtest m.cpp` 产物为 `mtest.exe`），
  而 mingw 版 `make` 按字面量 `target` 判断是否存在 → 永远判定"不存在"。
- 附注：这也修正了 S-05 的前提——"没有任何 Makefile 会产出 `target.exe`"只在 MSYS 版 g++ 下成立；
  用 mingw64 g++ 构建时，`target.exe` 正是当前构建规则的真实产物。
- 影响：不影响正确性，只影响增量构建效率；彻底修法是让 `executables` 带上 `$(EXEEXT)`（或统一 `target.exe`），
  需同步修改 97 个 Makefile，本轮未做。

### B-11 `build/` 在 git 索引里是大写 `Build`，大小写敏感文件系统上全量构建失效（P1，已修）

- 位置：git 索引 `Build/build.mk`；磁盘目录名与全部 Makefile 都写作 `build/build.mk`
- 现象：`git ls-files` 列出的是 `Build/build.mk`，而 97 个 Makefile 一律 `include ../build/build.mk`
  （深层模块为 `../../build/build.mk`、`../../../build/build.mk`）。
  Windows 文件系统大小写不敏感，本机完全无感；在 Linux/macOS 上 clone/checkout 会生成 `Build/` 目录，
  97 个目标的 `include` 全部失败，等于整个仓库构建不了。
- 证据：

```
$ git ls-files | grep -i build.mk
Build/build.mk
$ ls -d [Bb]uild*
build
```

- 修复：纯大小写重命名，内容不变——`git mv Build _btmpbuild && git mv _btmpbuild build`；
  重命名前后 blob 哈希同为 `f5d089f`，证明只改了大小写、未动内容。

---

## 2. 内存与资源管理缺陷

### M-01 `basic_any` 析构条件写反：确定性内存泄漏（P0，已实测）

- 位置：`accumulation/basic_any.cpp:97-104`
- 代码：

```cpp
virtual ~basic_any() {
    if(!ptr_) {          // 条件取反：只在 ptr_ 为空时才 delete
        delete ptr_;
        ptr_ = nullptr;
    }
}
```

- 证据（独立进程，重载全局 `new/delete` 计数，循环构造 1000 个对象）：

```
constructed 1000 objects: new=1000 delete=0 leaked=1000
```

- 影响：`ptr_` 非空时永不释放 → **每构造一次泄漏一次**；`delete nullptr` 本身是空操作，所以"能删"的分支永远是空转。

### M-02 `basic_any` 自赋值导致对象被销毁（P0，已实测）

- 位置：`accumulation/basic_any.cpp:81-95`（`operator=` 无自赋值检查）
- 代码路径：`b = b` → 先 `delete ptr_` → 再 `rhs.ptr_->copy()`，而 `rhs` 就是 `*this`，此时 `ptr_` 已释放。
- 证据：

```
--- 2) 自赋值 ---
construct function | copy function | terminate called after throwing an instance of 'char const*'
exit=3
```

- 说明：`get<int>()` 在之后抛出 `"ptr nullptr issue!"`（该异常是 `const char*`，未被捕获 → `std::terminate`），
  证明自赋值后对象内部指针已失效。

### M-03 `basic_any` 的 `Base` 缺少虚析构函数（P0）

- 位置：`accumulation/basic_any.cpp:35-38`（`struct Base { virtual Base* copy() = 0; };`）
- 调用点：`:71`、`:86`、`:101` 全部是 `delete ptr_;`（`ptr_` 类型为 `Base*`）
- 证据：`-Wdelete-non-virtual-dtor` 报 3 条告警：

```
warning: deleting object of abstract class type 'BASIC_ANY::Base' which has non-virtual destructor
         will cause undefined behavior [-Wdelete-non-virtual-dtor]
```

- 影响：即使 M-01 修好，`delete Base*` 仍不会调用 `Derived<T>` 的析构（T 含堆资源时二次泄漏）。

### M-04 `basic_any::get<T>()` 用 `static_cast` 做下行转换（P1）

- 位置：`accumulation/basic_any.cpp:106-114`
- 问题：存入 `int` 却 `get<double>()` 时，`static_cast<Derived<double>*>` 对实际是 `Derived<int>` 的对象是 UB，
  不会抛异常。注解却声称 `any` 的类型安全语义。

### M-05 单链表无析构、可浅拷贝、越界删除即崩溃（P0，已实测）

- 位置：`algorithm/004.singlelist.hpp`

| 行 | 问题 |
| --- | --- |
| `:41-137` | 类中只有 `new Node<T>`（`:52/:56/:92`），**没有析构函数**，也没有 `delete` 全链表 → 每个 `SingleList` 实例泄漏全部节点 |
| `:44-45` | `name_(__func__)`：把 `static const char[]` 赋给 `char*`，丢掉 const |
| （隐式） | 未删除拷贝构造/赋值 → 浅拷贝 `head` 指针，两个对象共享同一链；`operator=` 还会泄漏自己原有的链 |
| `:75-77` | `insert_elem(int, T& elem)` 内部 `std::move(elem)` → **对调用者的左值做移动**，实参被掏空 |
| `:126-128` | `index == size` 时 `p` 走到尾节点，`nod = p->next` 为 `nullptr`，随后 `p->next = nod->next` **空指针解引用** |

- 证据（3 元素链表，`delete_elem(3)`）：

```
$ ./t_singlelist.exe
before: 1 2 3
Segmentation fault   (exit=139)
```

### M-06 双链表同样的问题，外加 `tail` 悬垂（P0，已实测）

- 位置：`algorithm/005.doublelist.hpp`
- 与 M-05 同构：无析构（`:41-166`）、浅拷贝、`insert_elem(T&)` 移动左值（`:88-90`）、
  `delete_elem(size)` 空指针解引用（`:151-152`，实测 `exit=139`）。
- 额外：`:143-149` 删除唯一节点时 `head = nullptr`，但 **`tail` 未置空** → `tail` 指向已 `delete` 的节点，
  之后任何 `show_reseverd()`（`:78-86`）都是 use-after-free。示例代码 `:195-196` 恰好紧接着又插入了节点，掩盖了该缺陷。

### M-07 `resource_ptr` 缺少拷贝/移动控制（P1）

- 位置：`knowledge/04.class/main.cpp:402-424`（`namespace RAII` 在 `:400`）
- 问题：RAII 语义下持有裸 `int*` 并 `delete`，但拷贝构造/赋值未删除或实现 → 一次拷贝即双重释放；
  也没有移动语义。作为"RAII 示例"实际演示了反面教材。

### M-08 BMP 解码：位深 < 8 时缓冲区尺寸算错，仓库自带样例即可复现（P0，已实测）

- 位置：`knowledge/15.tools/image/image.hpp:107-115`
- 代码：`bytes_per_pixel_ = color_depth_ / 8; if (bytes_per_pixel_ == 0) bytes_per_pixel_ = 1;`
- 证据（用仓库自带的 `bmp_8.bmp`、`bmp_16.bmp` 实跑）：

```
bmp_8.bmp:   color_depth=1  bytes_per_pixel=1  image_data_size_=384000   实际像素数据 = 48000   （8 倍越界读）
bmp_16.bmp:  color_depth=4  bytes_per_pixel=1  image_data_size_=384000   实际像素数据 = 192000  （2 倍越界读）
bmp_256.bmp: color_depth=8  bytes_per_pixel=1  image_data_size_=384000   实际 = 384000         （正确）
```

- 影响：1bpp/4bpp 图解码后 `image_data_` 中绝大多数是未初始化的垃圾；`file.read` 只读到 EOF 就返回，
  返回值未检查，因此错误被完全吞掉。

### M-09 BMP 解码其它缺陷（P1）

| 位置 | 问题 |
| --- | --- |
| `image.hpp:227` | 行字节按 `width * bpp` 计算，**未按 BMP 规范做 4 字节行对齐**（`(w*bpp+3) & ~3`）。仓库样例宽度恰好都是 4 的倍数（800、232），缺陷被掩盖；任意 24bpp×宽 3 的图会整体错位 |
| `image.hpp:58-59` | `bwidth/bheight` 声明为 `uint32_t`，但 BMP 高度允许为负（top-down 位图）→ 负值变巨大无符号数，随后的 `make_unique` 直接抛 `bad_alloc` |
| `image.hpp:92,101,115` | 三处 `file.read` 都不检查 `gcount()`，文件截断时静默读垃圾 |
| `image.hpp:104-111` | 未校验 `bfType == 0x4d42`，任意文件都能"解码成功" |
| `image.hpp:269-284` | `width_/height_/bytes_per_pixel_/color_depth_/compress_/image_data_size_` **均未初始化**；解码前调用 `show_info()` / `save_to_bmp()` 读的是栈垃圾 |
| `image.hpp:137-147` | `save_to_bmp` 先 `trunc` 打开文件，再检查 `image_data_` 是否为空 → 失败路径也会破坏已有同名文件 |
| `image.hpp:75` | `bmp_decoder(const bmp_decoder&) = default;` 实际因 `unique_ptr` 成员而被隐式删除，写成 `= default` 具有误导性 |
| `image.hpp:98-102` | 第二次解码到同一对象时若新文件无调色板，`pattle_len_`/`palette_data_` 保留旧值 → 保存时写出上一张图的调色板 |

### M-10 `07.std/new` 的 `MEM_ALLOC` 分配/释放家族不完整（P1）

- 位置：`knowledge/07.std/new/main.cpp:114-206`
- 问题：用 `malloc` 实现 `operator new`/`new[]`，却**只提供 sized 版本**的
  `operator delete(void*, size_t)` 与 `operator delete[](void*, size_t)`（`:190`、`:199`）：
  - 缺少 `operator delete(void*)` / `operator delete[](void*)`；
  - 缺少 `operator delete(void*, const std::nothrow_t&)`（`new(std::nothrow)` 失败/构造抛异常时的配对版本）；
  - 结果为 `malloc` 与全局 `operator delete` 混用，目前**只因为 libstdc++ 默认 `operator delete` 恰好调用 `free`** 才没出事，
    属"依赖实现巧合"的写法。
- 另：`:208 get_size()` 返回 `int`，而 `mem_size` 是 `size_t`（窄化）；`mem_size -= size`（`:186`）在
  `new[]` 场景下元素尺寸与整块尺寸语义混淆。

---

## 3. 并发与对象生命周期

### T-01 定时器类 `detach()` 后仍访问 `this`（P1）

- 位置：`knowledge/15.tools/timer/main.cpp:34`、`:141`（`std::thread(bind(&X::run, this)).detach();`）
- 问题：`~TIME_MANAGE_CV()`（`:16-18`）只置标志并 `notify_all`，**不 join、无法 join**（线程已 detach）；
  析构返回后线程仍在 `run()` 里访问 `millisecond_ / timeout_handler_ / mutex_ / cond_` → use-after-free。
  `TIME_MANAGE_POSIX::~TIME_MANAGE_POSIX()`（`:117-122`）更严重：在同样没有同步的情况下
  `pthread_cond_destroy / pthread_mutex_destroy`。
- 附带：`start()`（`:128-142`）每次调用都重复 `pthread_mutex_init / pthread_cond_init`（不 destroy 旧的）→ 资源泄漏；
  若对象从未 `start()` 就析构，被 destroy 的是**未初始化的 pthread 对象**（UB）。

### T-02 `TIME_MANAGE_POSIX` 会把停止请求清掉，导致停不下来（P1）

- 位置：`knowledge/15.tools/timer/main.cpp:186`（`run()` 开头 `stop_loop_ = false;`）
- 问题：`stop()`（`:146-155`）先置 `stop_loop_ = true` 再广播；若这时线程尚未进入 `run()`，随后 `run()` 把标志清零
  → 停止请求丢失，循环永远等下去（lost wakeup）。而 `stop()` 本身还有 `if (stop_loop_) return;` 的提前返回。

### T-03 `TIME_MANAGE_CV` 的 `stop()` / `start()` 存在窗口竞争（P2）

- 位置：`timer/main.cpp:39-46`（`stop`）、`:72-85`（`run`）、`:55-66`（`test`）
- 问题：`test()` 里 `stop()` 之后 1ms 内就再次 `start()`，此时旧线程可能还停在 `task()` 回调里；
  `start()` 直接改 `timeout_handler_ / millisecond_ / is_running_`，两个线程并发读写同一组非原子成员。
- 另：`run()` 用 `wait_for` 但只在 `cv_status::timeout` 时执行回调，其它返回值直接 `break` 退出循环（伪唤醒即退出）；
  `test()` 是 `while(1)` 且无退出条件，测试函数永不返回。

### T-04 `TimerHandle` / `Event` / `Channel` 与 `Scheduler` 的析构顺序未受约束（P1）

- 位置：`coroutine_lib/include/cppcoroutine/timer.hpp:89-97`、`event.hpp:113-115`、`channel.hpp:86`
- 问题：三者的析构/`cancel()` 都会通过保存的 `Scheduler*`（`control_->scheduler` / `scheduler_`）回调进 Scheduler。
  若 `Scheduler` 先析构（`scheduler.hpp:637-660` 只清理自己的容器，不通知外部对象），随后 `TimerHandle` 析构
  → `cancel_callback_timer()` → `impl_->notify_external()`，访问已销毁的 `Impl` → use-after-free。
  头文件里没有任何关于"必须先销毁这些对象"的说明或断言。

### T-05 `Scheduler::cancel()` 跨线程时返回值与文档不符（P2）

- 位置：`coroutine_lib/include/cppcoroutine/scheduler.hpp:164-169`（文档）对比 `:929-940`（实现）
- 文档：`Returns true if the request was accepted (the task existed and was not already terminal).`
- 实现：非 loop 线程时只是把 lambda 入队，然后**无条件 `return true`**（任务 id 非法、任务已完成也返回 true）。
  即跨线程调用时该返回值不携带任何信息。

### T-06 `thread_pool` 可重复 `init()`，线程数翻倍（P2）

- 位置：`knowledge/15.tools/thread_pool/thread_pool.hpp:58-83`
- 问题：`init()` 只做 `workers_.emplace_back(...)`，没有"已初始化"保护；重复调用会得到 2 倍线程数，
  且 `thread_count_` 覆盖为第二次的值（与实际线程数不符）。析构时 `stop_` 置位、`notify_all` 后 join 所有线程（这部分是对的）。

### T-07 `thread_pool::enqueue` 使用已从 C++20 移除的 `std::result_of`（P2，可移植性）

- 位置：`thread_pool.hpp:92-93`（`std::result_of<F(Args...)>::type`）
- 事实核对：`std::result_of` 在 C++17 弃用、**C++20 从标准中移除**；
  本机 libstdc++ 仍保留为兼容扩展，因此 `-std=c++20` 下实测**可以编译通过**（已单独实例化验证），
  但在 libc++ / MSVC 严格模式或未来的 libstdc++ 中会失败。应使用 `std::invoke_result_t`。

### T-08 `multi_queue` 的格式化字符串与实参类型不匹配（P1）

- 位置：`knowledge/15.tools/multi_queue/main.cpp:35`、`:49`
- 代码：`size_t index` + `sprintf(buffer, "high thread %ld", index)`
- 问题：Windows/LLP64 下 `long` 为 32 位而 `size_t` 为 64 位 → 变参类型不匹配（未定义行为），
  且目标缓冲区 `char buffer[24]` 与"字符串长度 + 1"之间没有边界检查，`memcpy(..., size+1)` 的目标字段只有 32 字节。
- 另：`thread_queue.hpp:52` 写 `int ret = false;`（bool 常量赋给 int，函数返回类型是 bool），
  且 `receive()` 在没有 `send()` 唤醒机制的情况下无法中止。

### T-09 `thread/main.cpp` detach 后立刻依赖进程存活（P2）

- 位置：`knowledge/12.thread_coroutine/thread/main.cpp:87-88`
- 问题：`t2.detach()` 后仅 `sleep 100ms` 就继续，`main` 返回时该线程可能仍在写 `std::cout`；
  同时前面用 `std::hex` 输出 `native_handle` / `thread::id`（`:76`、`:41`、`:48`）会让后续输出保持十六进制状态。

### T-10 `math_tools` 的惰性 CRC 表无同步（P2）

- 位置：`knowledge/15.tools/math_tools/math_tools.hpp:155-167`
- 问题：`crc8_search_table()` 用 `is_table_generated_` 做懒初始化，类本身通过 `get_instance()` 暴露为单例；
  多线程首次并发调用会同时写 `crc8_table_[256]`（数据竞争），且 `generate_crc8_table()` 可被外部以任意多项式调用，
  会把已生成标记为"有效"的表整体替换掉。

---

## 4. 逻辑与接口缺陷

### L-01 `logger` 的模板成员：声明在头文件、定义在 `main.cpp`（P1）

- 位置：声明 `knowledge/15.tools/logger/logger.hpp:65-66`；定义 `knowledge/15.tools/logger/main.cpp:27-51`
- 问题：`logger_message(LOGGER_LEVEL, const char*, Args...)` 是模板，定义只存在于 `main.cpp` 这一个翻译单元。
  任何其它 `.cpp` 使用 `LOG_DEBUG / LOG_INFO / LOG_WARN / LOG_ERROR / LOG_FATAL` 都会得到
  undefined reference（模板不会被隐式实例化导出）。
  只有 `LOG_MESSAGE` 因为命中了头文件里内联定义的 `logger_message(const std::string&, ...)` 重载才可用。

### L-02 `logger` 宏以 `while(0);` 结尾，破坏 `if/else`（P1）

- 位置：`logger.hpp:98-118`
- 问题：宏体是 `do { ... } while(0);`（分号写在宏里），展开后多出一个空语句：

```cpp
if (cond) LOG_INFO("x"); else LOG_INFO("y");   // 展开为 if(cond) do{...}while(0);; else ...  → 编译错误
```

- 反证：`logger/main.cpp:101` 的 `LOG_FATAL("This is a fatal message")` **故意漏写分号仍然编译通过**，
  正说明宏把分号吞进了自己体内——把缺陷当成了便利。

### L-03 `logger` 在宏未定义时静默关闭全部日志（P2）

- 位置：`logger.hpp:97`（`#if LOG_MODULE_ON == 1`）
- 问题：`LOG_MODULE_ON` 没有任何默认定义，包含该头文件却忘记 `#define` 时所有 `LOG_*` 展开为空，
  编译无警告（除非开 `-Wundef`）、运行无输出，故障极难定位。同一文件的 `#else` 分支（`:120-125`）也是如此。

### L-04 `logger.hpp` 把 `using namespace LOGGER;` 写在头文件里（P2）

- 位置：`logger.hpp:95`
- 影响：任何包含该头的翻译单元都被迫引入 `LOGGER` 命名空间，与头文件应有的隔离性冲突。

### L-05 `Event` 的写入接口没有任何线程安全约定（P2）

- 位置：`coroutine_lib/include/cppcoroutine/event.hpp:143-181`
- 问题：`notify_one / notify_all / reset` 直接读写 `signaled_` 与 `waiters_`，无锁；
  而 `scheduler.hpp:6-12` 只声明了 `post/request_stop/cancel/TimerHandle::cancel` 是线程安全的。
  `Event` 自身的头注释没有说明"只能由 loop 线程调用"，这与常见 Event 的用法直觉（从任意线程 notify）冲突。

### L-06 `05.template/event_bus.cpp` 的接口约束（P2）

- 位置：`knowledge/05.template/event_bus.cpp:108`（`using Callback = void (*)(const Event&, void*);`）、`:389-395`
- 问题：
  1. 回调只能是裸函数指针，无法订阅捕获 lambda / 成员函数，与"事件总线"的常规预期不符（需 `void*` 手工传上下文）；
  2. 示例用 `const_cast<char*>("Main")` 把字符串字面量转成 `char*` 再当 `void*` 用，应直接 `const char*` + 回调内转换；
  3. `SubscriberList::add` 不去重，同一回调可被重复注册并重复触发。

### L-07 `safety_cpp` 的两个示例问题（P2）

- 位置：`knowledge/15.tools/safety_cpp/main.cpp`
- `:22` `~A(){}` 与 `:24` `virtual void printf() = 0;` —— 多态基类**非虚析构**；`A*` 删除派生对象是 UB。
- `:86-92` `MyColor MyColor;` —— 局部变量名与类型名同名，`MyColor = MyColor::Red;` 依赖名字查找的侥幸通过，可读性极差。
- `:55` `static_assert(H <= 10, "H Must be less than 10")` —— 判据与文案矛盾（10 是允许的）。

### L-08 `math_tools::calc_md5` 返回类型错误（P2）

- 位置：`math_tools.hpp:171-193`
- 问题：返回类型 `uint16_t`，实际返回 `unsigned int md_len`（MD5 固定 16）→ 窄化；调用方无法从返回值获得语义。
  同时未检查 `EVP_MD_CTX_new()` 是否为空、`EVP_DigestInit_ex/Final_ex` 的返回值。

---

## 5. 未定义行为与可移植性

### U-01 头文件保护宏使用保留标识符（P2）

- 位置：`logger/logger.hpp:24-25`（`__LOGGER_HPP__`）、`thread_pool/thread_pool.hpp:16-17`（`__THREAD_POOL_HPP__`）、
  `math_tools/math_tools.hpp:16-17`（`_MATH_TOOLS_HPP_`）
- 问题：含双下划线或"下划线 + 大写"的标识符在全局作用域由实现保留（C++ [lex.name]/3），用户代码不得占用。

### U-02 `_Pragma("once")` 与 `#pragma once` 混用（P3）

- 位置：`multi_queue/{event,semaphore,thread_queue}.hpp` 用 `_Pragma("once")`（3 处）；
  `image/image.hpp:19`、`ipml/models.hpp:2` 用 `#pragma once`（2 处）
- 问题：两种写法并存；`_Pragma("once")` 在 MSVC 下不受支持（`#pragma once` 才通用）。

### U-03 `09.layout` 对"标准布局"做了错误示范：memcpy 非平凡可复制类型（P1）

- 位置：`knowledge/09.layout/main.cpp:176`（`MyClass3 obj4(0);` 在 `:175`）
- 代码：`MyClass3 obj4(0); memcpy(&obj4, &obj3, sizeof(MyClass3));`
- 证据：

```
warning: 'void* memcpy(void*, const void*, size_t)' writing to an object of
         non-trivially copyable type 'class STANDARD_LAYOUT_TYPE::MyClass3' [-Wclass-memaccess]
```

- 问题：`is_standard_layout_v<MyClass3>` 为 true 并**不**代表可以用 `memcpy` 拷贝（那是 `is_trivially_copyable` 的事）。
  该示例把两个概念混为一谈，正是最容易被读者学错的点。

### U-04 `math_tools::calc_crc32` 的类型标注错误（P2）

- 位置：`math_tools.hpp:120`：`crc ^= (uint16_t)*ptr++ << 24;`
- 事实核对：`(uint16_t)` 不对位宽做任何扩展，表达式经整型提升后是 `int` 左移 24 位。
  - 在 **C++17 及更早**（README 声称支持 g++ 9.5）这是有符号左移溢出 → **UB**；
  - 在 **C++20 起**左移被定义为模运算，实测结果正确（本机无法链接 UBSan，故不做运行时断言）。
- 结论：类型标注应为 `uint32_t`；当前写法在旧标准下是 UB，在新标准下是"靠语言规则兜住"。

### U-05 `exec` 使用 GCC 语句表达式，且示例依赖外网与硬编码占位密钥（P2）

- 位置：`knowledge/15.tools/exec/main.cpp:44-47`（`int a = ({ int b = 3; func_add(b, 2); });`）、`:25`
- 问题：
  1. `({ ... })` 是 GCC 扩展，MSVC/clang-cl 不支持（项目多数示例同时给出 msvc 命令行，此处不可移植）；
  2. `command` 是硬编码的 `curl 'https://devapi.qweather.com/...key=***'` —— 依赖外网、依赖 curl、密钥为占位符，
     "单文件可在线运行"的说法（README:11）对该示例不成立。

### U-06 `unistd.h` / `popen` / `pthread_*` 让多个"可在线运行"的示例不可移植（P2）

- 位置：`15.tools/timer/main.cpp:8`、`15.tools/exec/main.cpp:20`、`15.tools/logger/logger.hpp:27`、
  `12.thread_coroutine/atomic/main.cpp:22` 等
- 问题：`godbolt.org`（README 推荐环境）与 MSVC 都没有 `unistd.h`；`timer` 直接调用 `pthread_*`
  却未 `#include <pthread.h>`（靠 libstdc++ 传递包含），在其它标准库下不可编译。

---

## 6. 文档与代码不一致

### D-01 `README.md` 中 `unordered_multiset` / `unordered_set` 的描述互换（P2）

- 位置：`README.md:196-197`

```
- unordered_multiset：无序集合，元素唯一。      ← 错：multiset 允许重复
- unordered_set：无序集合，允许重复元素。        ← 错：set 唯一
```

### D-02 `README.md` 中的 API 名 / 事实错误（P2）

| 位置 | 写法 | 问题 |
| --- | --- | --- |
| `README.md:262` | `std::enable_if 和 std::disable_if` | **标准库没有 `std::disable_if`** |
| `README.md:362` | `std::scope_lock` | 正确名是 `std::scoped_lock` |
| `README.md:210` | 排序算法列表含 `random_shuffle`、`reserve` | `random_shuffle` 已于 C++17 从标准移除；`reserve` 是容器成员函数，不是算法 |
| `README.md:235` | charconv "用于在不同类型的字符之间进行转换" | 实际是"数值 ↔ 字符串"转换 |
| `README.md:89` | stdfloat "浮点数类型的限制和范围的常量" | `<stdfloat>` 提供定宽浮点类型（`std::float32_t` 等），limits 是 `<limits>` 的职责 |
| `README.md:464-466` | `-fconcepts` 标为"开启 C++20 概念支持"、`-fcoroutines` 标为"开启 C++20 协程支持" | 二者是 C++17 时代的 TS 开关；`-std=c++20` 下无需，且 `-fconcepts` 与标准 `concepts` 不是一回事 |
| `README.md:391` | `decltype(auto)` "保留 cv 限定符(const、volatile)" | 保留的是引用性与值类别，措辞不准确 |

### D-03 `README.md:11` 的编译器版本声明与代码要求矛盾（P2）

- 原文："测试使用版本是 `g++9.5.0`、同时使用 `g++15.2.0` 验证，早期版本可能编译报错"
- 反证：`14.cpp_new_feature/{print,stacktrace,deducing_this}/Makefile` 使用 `-std=c++26`，
  `flat_map/generator/mdspan/spanstream` 需要 C++23，`06.stl/README.md:1729` 仍推荐已被移除的 `std::random_shuffle`。
  g++ 9.5 无法编译这些内容，"单文件可直接在线运行"（同段）对 `popen/pthread/openssl` 示例也不成立。

### D-04 `knowledge/plans.md` 的标注与拼写问题（P2）

| 位置 | 问题 |
| --- | --- |
| `plans.md:51` | `typeindex` 标 ✅ / `11.type_conversion`，但该模块 `main.cpp` **完全没有** `type_index` 代码（全仓 grep 仅命中 README 与 plans） |
| `plans.md:89` | `excepted` → 应为 `expected` |
| `plans.md:140` | `SFINEA` → 应为 `SFINAE` |
| `plans.md:177` | 所属模块写 `4.class`（其余均为 `04.class` 形式） |
| `plans.md` 整体 | `mdspan` 标 ✅ 但本工具链无法编译（B-05）；`scoped_allocator` 标在 `07.std`，实际目录名为 `scope_allocator` |
| `README.md` C++26 列表 | 缺 `inplace_vector`（`plans.md` 中有），两处清单不同步 |

### D-05 `knowledge/11.type_conversion/README.md:95-120` 展示的 `typeindex` 示例在代码中不存在（P2）

- 文档给出了 `#include <typeindex>` 的完整示例与成员表，但模块 `main.cpp` 里没有对应实现 →
  读者按文档去找例程会落空（与 D-04 的 ✅ 标记叠加）。

### D-06 `knowledge/15.tools/` 没有 README（P2）

- 15 个知识模块中只有 `15.tools/` 缺少 `README.md`（其余 14 个都有，`06.stl/README.md` 达 1841 行、`07.std` 达 2807 行）。
  根 `README.md:443-455` 对该模块只列了 9 个工具名，`coroutine_lib`（15 个文件、约 3600 行，本仓库最复杂的代码）
  完全没有出现在任何索引里。

### D-07 注释与实现不符的零散点（P3）

| 位置 | 注释 | 实际 |
| --- | --- | --- |
| `learning_notes/modern_book/main.cpp:5-6` | "gcc `-std=c++11`"、"msvc `/std:c++20`" | Makefile 默认 `-std=c++20`，CMake 用 C++17，三处互不相同 |
| `math_tools.hpp:169` | `/// @brief md5_calc` | 函数名是 `calc_md5` |
| `multi_queue/thread_queue.hpp:77` | "semaphore used to protect the queue" | 保护队列的是 `mutex_`，semaphore 只做计数通知 |
| `04.singlelist.hpp:6-7` | `Name: signallist.cpp` | 文件名是 `004.singlelist.hpp` |
| `image.hpp:170-171` | 注释称 8 位图需要调色板 | 分支条件用的是 `bytes_per_pixel_ == 1`，把 1bpp/4bpp 也一并算进去了（正是 M-08 的成因） |

---

## 7. 规范与可维护性

### S-01 全量告警 297 条，构建未启用 `-Wall -Wextra`（P2）

以 `-std=c++23 -Wall -Wextra` 扫描 152 个 `.cpp`，分布如下：

| 告警 | 条数 | 说明 |
| --- | --- | --- |
| `-Wunused-parameter` | 189 | `main(int argc, char* argv[])` 用法统一忽略参数 |
| `-Wunused-variable` | 35 | 演示代码里"设了不用"的变量 |
| `-Wdeprecated-literal-operator` | 20 | 自定义字面量写成 `operator"" _my`（引号后有空格），C++23 起弃用 |
| `-Wunused-but-set-variable` | 14 | |
| `-Wsign-compare` | 14 | `int` 与 `size_t`/`uint32_t` 比较（如 `image.hpp:230` 的 `for(int i...; i < height_;`） |
| `-Wdeprecated-declarations` | 11 | 使用已弃用 API |
| `-Wattributes` | 4 | 见 S-02 |
| `-Wparentheses` | 3 | 如 `atomic/main.cpp:98` 的 `while (it = list_head)` |
| `-Wmissing-field-initializers` | 2 | |
| `-Wclass-memaccess` | 1 | 见 U-03 |
| 其余各 1 条 | 4 | `unused-result` / `unused-local-typedefs` / `tautological-compare` / `enum-compare` / `comma-subscript` |

### S-02 属性写错导致示例没有演示它声称的特性（P2）

| 位置 | 现象 |
| --- | --- |
| `learning_notes/modern_book/Chapter-31/attribute.cpp:102` | `warning: 'may_unused' attribute directive ignored` —— `[[may_unused]]` 拼错（应为 `maybe_unused`），整段示例无效 |
| `knowledge/13.modern_cpp/attribute/main.cpp:36`、`Chapter-31:61` | `'carries_dependency' attribute ignored` |
| `knowledge/13.modern_cpp/attribute/main.cpp:67` | `'likely' attribute ignored`（`[[likely]]` 放在了不生效的位置） |
| `learning_notes/modern_book/Chapter-27/constexpr.cpp:183` | `std::is_constant_evaluated` 在 `if constexpr` 中恒为 true —— 用法错误，起不到区分编译期/运行期的作用 |
| `learning_notes/modern_book/Chapter-42/template_optimize.cpp:135-136` | `X <= Y <= Z` 连写，与数学含义不符（`-Wparentheses`） |
| `learning_notes/modern_book/Chapter-34/other_optimize.cpp:223` | 数组下标里的顶层逗号表达式在 C++23 语义变化（`-Wcomma-subscript`） |

### S-03 `learning_notes/modern_book/tags` 是生成物但已入库（P2）

- 71 KB 的 ctags 索引被 git 跟踪（`git ls-files` 可查），而 `.gitignore` 没有 `tags` 规则，
  `build/build.mk:36-37` 的 `tags:` 目标又会重新生成它 → 每次 `make tags` 都会产生无意义 diff。

### S-04 `.gitignore` 存在失效规则（P3）

- `.gitignore:2` `asio_book/asio/*` —— 仓库中不存在 `asio_book` 目录。
- 关于构建产物：`target` / `*.exe` / `executables/*` 规则**是生效的**（`git check-ignore` 已验证），
  但源码树里仍残留 94 个被忽略的旧产物（如 `target`、`target.exe` 同名并存于
  `14.cpp_new_feature/{concepts,coroutines,print,ranges}`、`07.std/source_location`、`12.thread_coroutine/future`），
  导致"源码目录看起来有可执行文件、实际与当前代码不同步"。

### S-05 产物命名不统一：`target` / `target.exe` / 两者并存（P3）

- 事实（用 `ls` 精确匹配统计，避免 MSYS2 把 `target` 自动解析为 `target.exe` 造成误判）：
  - 66 个目录只有 `target`；
  - 11 个目录只有 `target.exe`；
  - 7 个目录两者并存：`05.template`、`07.std/source_location`、`12.thread_coroutine/future`、
    `14.cpp_new_feature/{concepts,coroutines,print,ranges}`。
- 关键点：**没有任何 Makefile 会产出 `target.exe`**（全部写 `executables = target`），
  所以那 11 个目录里的 `.exe` 既不来自当前构建规则，也无法通过 `make` 复现——
  源码目录里"看起来有二进制"，实际与当前代码并不同步。

### S-06 `README.md` 与子 README 的索引完整性（P3）

- 根 `README.md` 每个小节都给出"详细文档"链接，唯独 `### tools`（`:443`）没有链接——对应目录也没有 README（D-06）。
- `algorithm/`、`accumulation/`（仅 3 行）、`learning_notes/` 均无说明文件，但都在根 README 之外独立存在。

---

## 8. 附录：取证命令

```bash
# 1) 全量构建（在仓库副本中执行，不污染工作区）
tar -C . --exclude=.git --exclude=target --exclude='*.exe' -cf - . | tar -C /tmp/copy -xf -
for d in $(find /tmp/copy -name Makefile -printf '%h\n' | sort -u); do (cd "$d" && make -k); done
# 结果：97 个目标，93 成功 / 4 失败（01.base_style、06.stl/mdspan、12.execution、modern_book）

# 2) 告警统计
for f in $(find . -name '*.cpp' -not -path './.git/*'); do
  g++ -std=c++23 -Wall -Wextra -fsyntax-only "$f" 2>&1 | grep -oE '\[-W[a-z-]+\]'
done | sort | uniq -c | sort -rn

# 3) basic_any 泄漏与自赋值（重载全局 new/delete 计数）
g++ -std=c++20 -Iaccumulation -Iaccumulation/includes t_basic_any.cpp -o t && ./t
#   -> new=1000 delete=0 leaked=1000 ；自赋值后 terminate（exit=3）

# 4) 链表越界删除
g++ -std=c++20 -Ialgorithm t_singlelist.cpp -o t && ./t     # -> Segmentation fault (139)
g++ -std=c++20 -Ialgorithm t_dl.cpp        -o t && ./t     # -> Segmentation fault (139)

# 5) BMP 解码（使用仓库自带样例）
g++ -std=c++20 t_bmp.cpp -o t && ./t bmp/bmp_8.bmp
#   -> color_depth=1, bytes_per_pixel=1, image_data_size_=384000（真实 48000）

# 6) modern_book 的 CMake 路径
cmake -S learning_notes/modern_book -B /tmp/mb -G "Unix Makefiles" && cmake --build /tmp/mb
#   -> coroutine:43: error: the <coroutine> header requires -fcoroutines

# 7) 构建产物忽略状态
git check-ignore -v knowledge/06.stl/array/target knowledge/07.std/charconv/target.exe
```

---

## 9. 说明

- 本清单**不包含修复建议的实现**，也未改动仓库中的任何源文件。
- 标注"已实测"的条目均可由附录命令复现；标注"未验证"的条目为纯静态推导。
- `libstdc++ 版本导致的环境差异`（如 `<mdspan>` 缺失、`-ltbb` 未安装）与"代码自身缺陷"已分别归入 B 类与其它类别，请勿混为一谈。
