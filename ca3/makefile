CC = g++
FLAGS = -std=c++11
COMPILE_FLAGS = $(FLAGS) -c
BUILD_DIR = build
SOURCE_DIR = src
LIBRARY_DIR = header

OBJECT_FILES = $(BUILD_DIR)/CommandHandler.o $(BUILD_DIR)/Network.o  $(BUILD_DIR)/main.o

all: Routing.out

Routing.out: $(OBJECT_FILES)
	$(CC) $(OBJECT_FILES) $(FLAGS) -o Routing.out

$(BUILD_DIR)/main.o: $(SOURCE_DIR)/main.cpp $(LIBRARY_DIR)/CommandHandler.h $(LIBRARY_DIR)/Network.h
	$(CC) $(COMPILE_FLAGS) $(SOURCE_DIR)/main.cpp -o $(BUILD_DIR)/main.o

$(BUILD_DIR)/CommandHandler.o: $(LIBRARY_DIR)/CommandHandler.h $(LIBRARY_DIR)/Exception.h $(LIBRARY_DIR)/Network.h $(SOURCE_DIR)/CommandHandler.cpp
	$(CC) $(COMPILE_FLAGS) $(SOURCE_DIR)/CommandHandler.cpp -o $(BUILD_DIR)/CommandHandler.o

$(BUILD_DIR)/Network.o: $(LIBRARY_DIR)/Network.h $(SOURCE_DIR)/Network.cpp
	$(CC) $(COMPILE_FLAGS) $(SOURCE_DIR)/Network.cpp -o $(BUILD_DIR)/Network.o

.PHONY: clean

clean:
	rm $(BUILD_DIR)/*.o
	rm *.out
