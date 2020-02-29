PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

BUILD_MODE = debug

ifeq ($(BUILD_MODE),debug)
	CFLAGS += -g -W -Wall
else ifeq ($(BUILD_MODE),run)
	CFLAGS += -O3 -W -Wall
else
	$(error Build mode $(BUILD_MODE) not supported by this Makefile)
endif

CFLAGS += -std=c++11
CFLAGS += -W -Wall

precompile:
	g++ bin/Unit.hpp $(CFLAGS) $(CPPFLAGS) $(CXXFLAGS) -Wno-unused-parameter -o debug/Unit.gch
clean:
	rm -fr debug/Unit.gch
