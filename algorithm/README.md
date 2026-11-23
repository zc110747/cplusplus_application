# algorithm

手写数据结构与算法练习题，全部以 `00N.xxx.hpp` 单头文件形式给出，每个文件自带
`namespace` 与 `test()` 入口，由 `main.cpp` 统一调度。代码源自公众号「嵌入式技术总结」的题解，
保留原题思路注释。

## 文件索引

| 文件 | 命名空间 | 内容 | 关键点 |
| --- | --- | --- | --- |
| `001.megre.hpp` | `ALG_MEGRE` | 合并两个有序链表 | 双指针，取较小者接到新链上 |
| `002.reserved.hpp` | `ALG_RESERVED` | 反转单链表 | `pre`/`cur`/`node` 三指针迭代 |
| `003.linelist.hpp` | `ALG_LINE_LIST` | 线性表基础操作 | 顺序存储与链式存储对照 |
| `004.singlelist.hpp` | `ALG_SINGAL_LIST` | 单链表模板类 | 析构、深拷贝、移动、`clear()`/`swap()`、越界删除判空 |
| `005.doublelist.hpp` | `ALG_DOUBLE_LIST` | 双向链表模板类 | 同单链表，另需维护 `tail`；删掉唯一节点后 `tail` 必须置空 |
| `006.mostgive.hpp` | `ALG_MOSTGIVE` | 数位 DP：统计 ≤ n 且各位数字都来自给定集合的整数个数 | 分「位数更少」与「位数相同」两段累加 |
| `007.mediatwo.hpp` | `ALG_MEDIATWO` | 两个正序数组的中位数 | 双指针归并，`O(m+n)` 时间、`O(1)` 空间 |

## 构建

```bash
cd algorithm
make          # 产物：../executables/ModernCpp
make ex       # 构建并直接运行
make clean    # 只清理 .o
```

## 说明

- 004/005 的链表类是**可拷贝、可移动的值语义容器**：拷贝为深拷贝，移动后源对象置空，
  析构负责逐节点释放。删除下标越界时返回错误码而不是崩溃。
- 006 的 `atMostNGivenDigitSet` 里循环下标用 `std::size_t`，与 `std::string::size()` 同类型，
  避免 `-Wsign-compare`。
- 007 的注释给出的是归并思路；若需要 `O(log(min(m,n)))` 的二分写法，可另加实现。
