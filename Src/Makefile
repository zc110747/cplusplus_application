
################################################################################
#
# THIS FILE IS BASIC MAKE FILE FOR
# Copyright (c) 2020-2021
# Original Author: 1107473010@qq.com
#
################################################################################

#define the directory for the compile system
mkfile_path := $(shell pwd)

#path for container
container_path := Containers/array \
				Containers/deque \
				Containers/forward_list \
				Containers/list \
				Containers/map \
				Containers/multimap \
				Containers/multiset \
				Containers/queue \
				Containers/set \
				Containers/stack \
				Containers/unordered_map \
				Containers/unordered_multimap \
				Containers/unordered_multiset \
				Containers/unordered_set \
				Containers/vector
build_container_path := $(foreach path,$(container_path),$(mkfile_path)/$(path))

#path for multithreading
multithreading_path := MultiThreading/thread \
				MultiThreading/mutex
build_multithreading_path := $(foreach path,$(multithreading_path),$(mkfile_path)/$(path))

# this is the rule how to complie file in the program, all the makefile will
# execute, and finally could be see in $(root)/Executables
all: build_container build_multithreading

build_container:
	@echo ==============================================================================
	@echo
	@echo ********************start build containers c++ executables********************
	@echo
	@echo ==============================================================================
	for x_path in $(build_container_path); do \
		cd $$x_path && make; \
	done

build_multithreading:
	@echo ==============================================================================
	@echo
	@echo ********************start build multithreading c++ executables********************
	@echo
	@echo ==============================================================================
	for x_path in $(build_multithreading_path); do \
		cd $$x_path && make; \
	done

# this is the rule how to clean all the file
clean:
	rm -f ../Executables/*

$(phony all): 
	