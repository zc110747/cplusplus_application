
################################################################################
#
# THIS FILE IS BASIC MAKE FILE FOR
# Copyright (c) 2020-2021
# Original Author: 1107473010@qq.com
#
################################################################################

#
mkfile_path := $(shell pwd)
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
build_path := $(foreach path,$(container_path),$(mkfile_path)/$(path))

# this is the rule how to complie file in the program, all the makefile will
# execute, and finally could be see in $(root)/Executables
all:
	@echo ==============================================================================
	@echo
	@echo ********************start build containers c++ executables********************
	@echo
	@echo ==============================================================================
	for x_path in $(build_path); do \
		cd $$x_path && make; \
	done

# this is the rule how to clean all the file
clean:
	rm -f ../Executables/*

$(phony all): 
	