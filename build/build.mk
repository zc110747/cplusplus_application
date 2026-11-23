################################################################################
#
# THIS FILE IS BASIC MAKE FILE FOR
# Copyright (c) 2020-2021
# Original Author: 1107473010@qq.com
#
################################################################################

# this place define the compile and flags, CC complie should be g++ and 
# option could be add in place
STD_VERSION ?= -std=c++20
CC = $(CROSS_COMPILE)g++

# 可执行文件后缀：mingw / msys / cygwin 目标下 g++ 实际产出 target.exe。
# 若规则里只写 target，make 每次都判定目标不存在而重复链接（增量构建失效）。
# 判据取 $(CC) -dumpmachine 而不是 $(OS)，这样在 Linux 上交叉编译出 .exe 也正确。
CC_MACHINE := $(shell $(CC) -dumpmachine 2>/dev/null)
ifneq (,$(or $(findstring mingw,$(CC_MACHINE)),$(findstring msys,$(CC_MACHINE)),$(findstring cygwin,$(CC_MACHINE))))
EXEEXT ?= .exe
else
EXEEXT ?=
endif

# 真实产物路径 = Makefile 声明的 executables + 平台后缀，直接链接到最终位置，
# 不再靠 mv 搬运（搬走会让目标在构建目录里消失，等于永远重链）。
# 去掉 executables_path 的尾随斜杠，避免出现 ./target.exe 之外的 .//target.exe
executables_dir := $(patsubst %/,%,$(executables_path))
executables_output := $(executables_dir)/$(executables)$(EXEEXT)

# 告警默认全开：示例代码里确属"故意演示"的写法用
# #pragma GCC diagnostic ignored "..." 就近关闭，不要退回全局静音
WARN_FLAGS ?= -Wall -Wextra

# 编译选项（不含任何链接库）
CFLAGS += $(STD_VERSION) $(WARN_FLAGS) $(gdef)

# 链接库（只出现在链接行）
LDLIBS += -lpthread -lm

# this is the rule how to complie file with type .cpp to middle file .o 
# then link the object and generate the executables.
all : $(executables_output)

%.o : %.cpp
	$(CC) $(CFLAGS) -c $< -o $@ $(include_path)

$(executables_output): $(objects)
	@mkdir -p $(executables_dir)
	$(CC) -o $@ $(objects) $(LIB) $(CFLAGS) $(LDLIBS)

tags :
	ctags -R *

# this is the rule how to clean all the file
# 同时清理旧命名（无后缀的 target）留下的残留
clean:
	rm -f $(objects) $(executables) $(executables_output)

# this tags let the make can execute the executabls.
# command: make execute option="-h"
# 注意：这里只删中间文件 .o，不能用 make clean——clean 会连产物一起删掉，就没得运行了
ex:
	@make -j4 gdef=$(gdef)
	@rm -f $(objects)
	@echo ---- run complier executables ----
	@$(executables_output) $(option)

start:
	@echo ---- run complier executables ----
	@$(executables_output) $(option)
	
build:
	@make