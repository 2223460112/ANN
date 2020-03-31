PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

BUILD_MODE = run

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
	
example_1_relu:
	g++ ./examples/examplerelu.cpp $(CFLAGS) $(PKG_FLAGS) -o examples/1/relu
	g++ ./examples/examplerelucontinue.cpp $(CFLAGS) $(PKG_FLAGS) -o examples/1/relu_c
example_1_sigmoid:
	g++ ./examples/examplesigmoid.cpp $(CFLAGS) $(PKG_FLAGS) -o examples/1/sigmoid
	g++ ./examples/examplesigmoidcontinue.cpp $(CFLAGS) $(PKG_FLAGS) -o examples/1/sigmoid_c

clean:
	-rm -r examples/1/relu*
	-rm -r examples/1/sigmoid*
	-rm -r examples/1/1_relu*
	-rm -r examples/1/1_sigmoid*