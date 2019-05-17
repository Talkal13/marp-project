SOURCES := src/main.cpp

all:
	g++ $(SOURCES) -Wall -std=c++17 -g -o bin/pracmarp -pthread