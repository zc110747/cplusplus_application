# 15.tools —— 工具模块

本目录收录 C++ 应用中常用的工具类/组件示例，每个子目录是一个**独立可构建**的小工程（自带 `Makefile`），
根目录 `README.md` 的 `### tools` 小节是本页的入口。

## 1. 模块索引

| 目录 | 内容 | 入口 | 依赖 |
| --- | --- | --- | --- |
| `coroutine_lib` | 头文件形式的协程运行时：Scheduler 事件循环、Task、TimerHandle、Event、Channel | `main.cpp` | C++20（无第三方） |
| `exec` | 通过 `popen` 读取子进程输出 | `main.cpp` | POSIX（或 MinGW/MSYS2） |
| `image` | BMP 解码/存盘（1/4/8/24/32bpp，调色板、行对齐、top-down） | `main.cpp` | 无 |
| `ipml` | 模型加载的接口/实现分离示例（`models.hpp` + `models.cpp`） | `main.cpp` | 无 |
| `logger` | 基于 fmt 的日志封装：级别、互斥锁、格式化 | `main.cpp` | `fmt`（随目录提供，header-only） |
| `math_tools` | CRC8/16/32（直接法 + 查表法）、MD5、AES-256-CBC | `main.cpp` | OpenSSL |
| `multi_queue` | 基于信号量的双优先级线程队列 | `main.cpp` | 无（持续运行，Ctrl+C 退出） |
| `safety_cpp` | `static_cast` / `enum class` / `static_assert` / `[[nodiscard]]` 等安全写法 | `main.cpp` | 无 |
| `thread_pool` | 线程池：`init` / `enqueue`（返回 `std::future`） | `main.cpp` | 无 |
| `timer` | 定时器：`condition_variable` 版与 POSIX `pthread_cond_t` 版 | `main.cpp` | POSIX 线程库 |
| `void_t` | `std::void_t` 做成员检测（SFINAE） | `main.cpp` | 无 |

## 2. 构建

```bash
cd knowledge/15.tools/<模块>
make            # 编译并输出 target
make ex         # 编译后直接运行
make clean      # 清理 .o
```

所有模块共用仓库根的 `build/build.mk`，标准版本由各模块的 `STD_VERSION` 指定（默认 `-std=c++20`）。

## 3. 重点说明

### coroutine_lib

- **线程契约**：所有协程执行、状态迁移、协程帧销毁都发生在调用 `run()` 的线程（loop 线程）上。
  只有 `post()` / `request_stop()` / `cancel()` / `TimerHandle::cancel()` 是线程安全的。
- **对象生命周期**：`Event` / `Channel` / `TimerHandle` 持有一份 Scheduler 存活标志的弱引用，
  因此可以在 Scheduler 销毁之后再析构——析构时检测到 Scheduler 已死，只做本地清理，不再回调。
  协程本身仍必须在 Scheduler 销毁前结束。
- `cancel()` 的返回值语义：在 loop 线程调用时表示"任务存在且未终止"；跨线程调用只表示"请求已入队"。

### timer

- `TIME_MANAGE_CV` 用 `condition_variable`，`TIME_MANAGE_POSIX` 用 `pthread_cond_timedwait`。
- 两者都在析构时 `join` 工作线程，不再 `detach`；`stop()` 用原子交换避免重复停止与 lost wakeup。
- **平台差异**：winpthreads 的 `pthread_condattr_setclock(CLOCK_MONOTONIC)` 返回 `EINVAL`，
  此时条件变量仍按 `CLOCK_REALTIME` 计算超时，代码会打印提示并回退到 `CLOCK_REALTIME`。

### logger

- `logger_message` 是模板，定义**必须在头文件里**；放在某个 `.cpp` 里会导致其他翻译单元链接失败。
- `LOG_*` 宏是 `do { ... } while(0)`（不带结尾分号），可以安全放进 `if/else`。
- `LOG_MODULE_ON` 未定义时默认为 1（开启）；需要关闭时显式 `#define LOG_MODULE_ON 0`。
- 头文件不使用 `using namespace LOGGER;`，宏内部一律写全限定名。

### image

- 支持 1/4/8/24/32bpp 未压缩 BMP：≤8bpp 按位解开成"索引值"存入内部缓冲，保存时按 8bpp + 256 项调色板输出。
- 行数据按 BMP 规范做 4 字节对齐；支持负高度（top-down）；文件头、长度、截断都有校验。
- 不解压任何压缩格式（RLE 等），也不做 1bpp→8bpp 之外的色深转换。

### math_tools

- CRC8 查表法用 `std::call_once` 做惰性初始化，多线程首次并发调用只建表一次。
- `calc_md5` 返回实际写入的字节数（失败返回 0），并检查 OpenSSL 每一步的返回值。

### thread_pool / multi_queue

- `thread_pool::init()` 只生效一次，重复调用不会让线程数翻倍。
- `multi_queue` 里队列由 `mutex` 保护，信号量只负责"有数据"的通知；消息格式化使用 `snprintf` 并校验长度。

## 4. 已知限制

- `exec`、`timer` 依赖 POSIX 接口，MSVC 下不可直接编译。
- `logger` 依赖随目录提供的 `fmt`，不额外安装系统包。
- `multi_queue` 的示例进程持续运行，需要手动中断。
