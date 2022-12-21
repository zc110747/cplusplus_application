################################################################################
#
# THIS FILE IS BASIC MAKE FILE FOR
# Copyright (c) 2020-2021
# Original Author: 1107473010@qq.com
#
################################################################################

# This is object should be build and will link to generate
# the executables
objects = main.o \
		  asio_server.o

include_path = -I asio/include

CFLAGS = -lws2_32 -lpthread -lwsock32
# This is the file name of the executables and path where 
# place the executables
executables = studyserver
executables_path = ../../Executables/

# Including this file allows access to the Build System.
include ../../Build/build.mk