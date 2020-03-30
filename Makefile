PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

BUILD_MODE = debug

ifeq ($(BUILD_MODE),debug)
	CFLAGS += -g
else ifeq ($(BUILD_MODE),run)
	CFLAGS += -O1 -O2 -O3
else
	$(error Build mode $(BUILD_MODE) not supported by this Makefile)
endif

CC=g++-9
 
CFLAGS += -std=c++11
CFLAGS += -W -Wall -Wno-unused-parameter

Dep = eigen3
PKG-FLAGS =	$$(pkg-config --libs --cflags $(Dep))

all:example_1_relu example_1_sigmoid

clean:
	rm -r debug/1_relu*
	rm -r debug/1_sigmoid*
	
example_1_relu:
	g++ ./debug/example1relu.cpp $(CFLAGS) $(PKG_FLAGS) -o debug/1_relu
	g++ ./debug/example1relucontinue.cpp $(CFLAGS) $(PKG_FLAGS) -o debug/1_relu_c
example_1_sigmoid:
	g++ ./debug/example1sigmoid.cpp $(CFLAGS) $(PKG_FLAGS) -o debug/1_sigmoid
	g++ ./debug/example1sigmoidcontinue.cpp $(CFLAGS) $(PKG_FLAGS) -o debug/1_sigmoid_c