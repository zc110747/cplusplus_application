################################################################################
#
# THIS FILE IS BASIC MAKE FILE FOR
# Copyright (c) 2020-2021
# Original Author: 1107473010@qq.com
#
################################################################################

# this place define the compile and flags, CC complie should be g++ and 
# option could be add in place
CC = g++
CFLAGS  := -O3 -std=c++11 -lpthread

# this is the rule how to complie file with type .cpp to middle file .o 
# then link the object and generate the executables.
all : $(executables)

%.o : %.cpp
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE_PATH)

$(executables): $(objects)
	$(CC) -o $(executables) $(objects) $(LIB) $(CFLAGS)
	rm -f $(objects)
	mv $(executables) $(executables_path)
tags :
	ctags -R *

# this is the rule how to clean all the file
clean:
	rm -f $(objects) $(executables_path)/$(executables)

# this tags let the make can execute the executabls.
# command: make execute option="-h"
execute:
	$(executables_path)/$(executables) $(option)