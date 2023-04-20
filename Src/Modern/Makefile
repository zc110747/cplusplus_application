################################################################################
#
# THIS FILE IS BASIC MAKE FILE FOR
# Copyright (c) 2020-2021
# Original Author: 1107473010@qq.com
#
################################################################################

# This is object should be build and will link to generate
# the executables
# object for compiler
objects = main.o 	\
		Chapter-05/postf_return.cpp \
		Chapter-06/reference.o \
		Chapter-14/strong_enum.o \
		Chapter-27/constexpr.o \
		Chapter-28/eval_order.o \
		Chapter-29/literal.o \
		Chapter-30/align.o \
		basic/basic.o \
		basic/class.o \
		basic/other_improve.o \
		template/template.o \
		template/temp_improve.o \
		template/temp_arg.o \
		template/temp_deduction.o \
		template/template_SFINAE.o \
		lambda/lambda.o \
		namespace/namespace.o \
		type/type.o \
		struct_bind/bind_process.o \
		type/noexcept_process.o \
		thread/thread_process.o \
		thread/semaphore.o \
		attribute/attribute.o 

# path for header
include_path = -I"reference/" -I"modules/" -I"lambda/"

# This is the file name of the executables and path where 
# place the executables
executables = ModernCpp
executables_path = ../../../Executables
command = $(executables_path)/$(executables)

# Including this file allows access to the Build System.
include ../../Build/build.mk