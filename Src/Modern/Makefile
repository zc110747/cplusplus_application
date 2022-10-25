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
		modules/basic.o \
		modules/class.o \
		reference/reference.o \
		template/template.o \
		lambda/lambda.o \
		constexpr/constexpr.o \
		namespace/namespace.o \
		type/type.o \
		struct_bind/bind_process.o \
		type/noexcept_process.o \
		thread/thread_process.o

# path for header
include_path = -I"reference/" -I"modules/" -I"lambda/"

# This is the file name of the executables and path where 
# place the executables
executables = ModernCpp
executables_path = ../../../Executables
command = $(executables_path)/$(executables)

# Including this file allows access to the Build System.
include ../../Build/build.mk