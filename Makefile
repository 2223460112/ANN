PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

BUILD_MODE = debug

ifeq ($(BUILD_MODE),debug)
	CFLAGS += -g
else ifeq ($(BUILD_MODE),run)
	CFLAGS += -O3
else
	$(error Build mode $(BUILD_MODE) not supported by this Makefile)
endif

CFLAGS += -std=c++11
CFLAGS += -Wall

precompile:
	g++ -c debug/compile.cpp $(CFLAGS) $(CPPFLAGS) $(CXXFLAGS) -Wno-unused-parameter -o debug/compile.o
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
	g++ debug/test.cpp $(CFLAGS) $(CPPFLAGS) $(CXXFLAGS) -o debug/test