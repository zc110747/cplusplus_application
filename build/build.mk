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

# 告警默认全开：示例代码里确属"故意演示"的写法用
# #pragma GCC diagnostic ignored "..." 就近关闭，不要退回全局静音
WARN_FLAGS ?= -Wall -Wextra

# 编译选项（不含任何链接库）
CFLAGS += $(STD_VERSION) $(WARN_FLAGS) $(gdef)

# 链接库（只出现在链接行）
LDLIBS += -lpthread -lm

# this is the rule how to complie file with type .cpp to middle file .o 
# then link the object and generate the executables.
all : $(executables)

%.o : %.cpp
	$(CC) $(CFLAGS) -c $< -o $@ $(include_path)

$(executables): $(objects)
	$(CC) -o $(executables) $(objects) $(LIB) $(CFLAGS) $(LDLIBS)
	$(shell if [ -d $(executables_path) ]; then echo; else mkdir $(executables_path); fi)
ifneq ($(executables_path),./)
	mv $(executables) $(executables_path)
endif
tags :
	ctags -R *

# this is the rule how to clean all the file
clean:
	rm -f $(objects)

# this tags let the make can execute the executabls.
# command: make execute option="-h"
ex:
	@make -j4 gdef=$(gdef)
	@make clean
	@echo ---- run complier executables ----
	@$(executables_path)/$(executables) $(option)

start:
	@echo ---- run complier executables ----
	@$(executables_path)/$(executables) $(option)
	
build:
	@make