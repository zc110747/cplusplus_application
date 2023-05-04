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
		Chapter-01/base_type.o \
		Chapter-02/namespace.o \
		Chapter-03/auto.o \
		Chapter-04/decltype.o \
		Chapter-05/postf_return.o \
		Chapter-06/reference.o \
		Chapter-07/lambda.o \
		Chapter-08/initialization.o \
		Chapter-09/initializer_list.o \
		Chapter-10/construct.o \
		Chapter-11/union.o \
		Chapter-12/delegation.o \
		Chapter-13/inherit.o \
		Chapter-14/strong_enum.o \
		Chapter-15/aggregate.o \
		Chapter-16/override.o \
		Chapter-17/for_loop.o \
		Chapter-18/choose_init.o \
		Chapter-19/static_assert.o \
		Chapter-20/struct_bind.o \
		Chapter-21/noexcept.o \
		Chapter-22/template_alias.o \
		Chapter-23/nullptr.o \
		Chapter-24/spaceship.o \
		Chapter-26/inline.o \
		Chapter-27/constexpr.o \
		Chapter-28/eval_order.o \
		Chapter-29/literal.o \
		Chapter-30/align.o \
		Chapter-32/preprocess.o \
		Chapter-36/typename.o \
		Chapter-39/derivation.o \
		Chapter-42/template_optimize.o

# path for header
include_path = -I"reference/" -I"modules/" -I"lambda/"

# This is the file name of the executables and path where 
# place the executables
executables = ModernCpp
executables_path = ../../../Executables
command = $(executables_path)/$(executables)

# Including this file allows access to the Build System.
include ../../Build/build.mk