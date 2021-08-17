# Makefile for project "emumice"

CC = gcc
CXX = g++
CXXFLAGS = -g
FRMWRKS = -framework Carbon -framework Foundation -framework IOKit
OBJS = mouse.o main.o gamepad.o emumice.o

.PHONY: all
all: emumice

emumice: $(OBJS)
	$(CXX) $(OBJS) $(CXXFLAGS) $(FRMWRKS) -o emumice

main.o: test.cpp emumice.hpp
	$(CXX) -c test.cpp $(CXXFLAGS) -o main.o

gamepad.o: gamepad.c
	$(CC) -c gamepad.c $(CXXFLAGS)

emumice.o: emumice.cpp emumice.hpp
	$(CXX) -c emumice.cpp $(CXXFLAGS)

mouse.o: mouse.cpp emumice.hpp
	$(CXX) -c mouse.cpp $(CXXFLAGS)

.PHONY: clean
clean:
	rm -f *.o emumice

# all:
# 	gcc -DDEBUG -o test test.cpp tiny-json.c gamepad.c -framework Foundation -framework IOKit
