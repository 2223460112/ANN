PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

BUILD_MODE = debug

ifeq ($(BUILD_MODE),debug)
	CFLAGS += -g
else ifeq ($(BUILD_MODE),run)
	CFLAGS += -O3
else
	$(error Build mode $(BUILD_MODE) not supported by this Makefile)
endif

CC=g++
ifneq ($(OS),Windows_NT)
    CC=/usr/local/bin/g++-9
endif
 
CFLAGS += -std=c++11
CFLAGS += -W -Wall

Dep = eigen3
PKG-FLAGS =	$$(/usr/local/bin/pkg-config --libs --cflags $(Dep))

precompile:
	$(CC) -c debug/compile.cpp $(PKG-FLAGS) $(CFLAGS) $(CPPFLAGS) $(CXXFLAGS) -Wno-unused-parameter -o debug/compile.o
clean:
 ifeq ($(OS),Windows_NT)
 	del debug/compile.o
 	del debug/test.exe
 else
	rm -fr debug/compile.o
	rm -fr debug/test
	rm -fr test.dSYM
 endif
 
test:
	$(CC) debug/test.cpp $(CFLAGS) $(CPPFLAGS) $(CXXFLAGS) -o debug/test