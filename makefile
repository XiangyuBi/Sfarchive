CXX=g++
CXXFLAGS=-g -std=c++11 -Wall -pedantic
BIN=sfarchiver

SRC=$(wildcard main.cpp)
OBJ=$(SRC:%.cpp=%.o)

	
all: $(OBJ)
	$(CXX) -o $(BIN) $^;

%.o: %.c
	$(CXX) $@ -c $

clean:
	rm -f *.o
	rm $(BIN)