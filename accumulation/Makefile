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
		basic_any.o \
		enable_share.o \
		enable_if.o \
		singleton.o \
		lambda.o

# path for header
include_path = -Iincludes/

# This is the file name of the executables and path where 
# place the executables
executables = ModernCpp
executables_path = ../executables
command = $(executables_path)/$(executables)

# Including this file allows access to the Build System.
include ../build/build.mk