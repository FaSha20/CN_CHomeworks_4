CC = g++
FLAGS = -std=c++11
COMPILE_FLAGS = $(FLAGS) -c
BUILD_DIR = build
SOURCE_DIR = src
LIBRARY_DIR = header

OBJECT_FILES = tcp.o

all: Routing.out

Routing.out: $(OBJECT_FILES)
	$(CC) $(OBJECT_FILES) $(FLAGS) -o Routing.out

tcp.o: tcp.cpp 
	$(CC) $(COMPILE_FLAGS) tcp.cpp -o tcp.o


clean:
	rm *.o
	rm *.out