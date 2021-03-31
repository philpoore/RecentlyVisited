
TARGET=src/main.cpp src/socket.cpp
CPP_FLAGS=-std=c++2a -l pthread
CPP_FLAGS_RELEASE=-O2
CPP_FLAGS_DEBUG=-g
BIN=RecentlyVisited

all: compile

compile:
	g++ $(TARGET) $(CPP_FLAGS) $(CPP_FLAGS_RELEASE) -o $(BIN)

compile-debug:
	g++ $(TARGET) $(CPP_FLAGS) $(CPP_FLAGS_DEBUG) -o $(BIN)_debug

benchmark:
	python3 test/benchmark.py