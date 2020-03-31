PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

BUILD_MODE = run

ifeq ($(BUILD_MODE),debug)
	CFLAGS += -g
else ifeq ($(BUILD_MODE),run)
	CFLAGS += -O1 -O2 -O3
else
	$(error Build mode $(BUILD_MODE) not supported by this Makefile)
endif

CC=/usr/local/bin/g++-9
 
CFLAGS += -std=c++11
CFLAGS += -W -Wall -Wno-unused-parameter

Dep = eigen3
PKG-FLAGS =	$$(pkg-config --libs --cflags $(Dep))

all:header example_relu example_sigmoid example_mnist
	
header:
	$(CC) lib/Net.hpp $(CFLAGS) $(PKG_CONFIG) -o lib/Net.hpp.gch

example_relu:
	$(CC) examples/examplerelu.cpp $(CFLAGS) $(PKG_FLAGS) -o examples/1/relu
	$(CC) examples/examplerelucontinue.cpp $(CFLAGS) $(PKG_FLAGS) -o examples/1/relu_c
example_sigmoid:
	$(CC) examples/examplesigmoid.cpp $(CFLAGS) $(PKG_FLAGS) -o examples/1/sigmoid
	$(CC) examples/examplesigmoidcontinue.cpp $(CFLAGS) $(PKG_FLAGS) -o examples/1/sigmoid_c

example_mnist:
	$(CC) examples/examplemnist.cpp $(CFLAGS) $(PKG_FLAGS) -o examples/2/mnist
	$(CC) examples/examplemnistcontinue.cpp $(CFLAGS) $(PKG_FLAGS) -o examples/2/mnist_c
	
clean:
	-rm -r examples/1/relu*
	-rm -r examples/1/sigmoid*
	-rm -r examples/1/1_relu*
	-rm -r examples/1/1_sigmoid*
	-rm -r examples/2/mnist*
	-rm -r lib/*.gch