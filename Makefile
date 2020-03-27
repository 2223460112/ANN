PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

BUILD_MODE = run

ifeq ($(BUILD_MODE),debug)
	CFLAGS += -g
else ifeq ($(BUILD_MODE),run)
	CFLAGS += -O1 -O2 -O3
else
	$(error Build mode $(BUILD_MODE) not supported by this Makefile)
endif

CC=g++
 
CFLAGS += -std=c++11
CFLAGS += -W -Wall -Wno-unused-parameter

Dep = eigen3
PKG-FLAGS =	$$(pkg-config --libs --cflags $(Dep))

all:example_1_relu example_1_sigmoid

clean:
	rm ./example/1/relu/run
	rm ./example/1/sigmoid/run
	
example_1_relu:
	g++ ./debug/example1relu.cpp $(CFLAGS) $(PKG_FLAGS) -o ./examples/1/relu/run -v
example_1_sigmoid:
	g++ ./debug/example1sigmoid.cpp $(CFLAGS) $(PKG_FLAGS) -o ./examples/1/sigmoid/run -v