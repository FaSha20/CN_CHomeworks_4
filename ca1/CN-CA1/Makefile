CC = g++
BUILD_DIR = build
SRC_DIR = src
INCLUDE_DIR = include
CONFIGURATION_DIR = configuration
CFLAGS = -std=c++11 -Wall -Werror -I$(INCLUDE_DIR)

OBJECTS = \
	$(BUILD_DIR)/Server.o \
	$(BUILD_DIR)/Parser.o \
	$(BUILD_DIR)/User.o \
	$(BUILD_DIR)/Room.o \
	$(BUILD_DIR)/Reservation.o \


UserSensitivityList = \
	$(SRC_DIR)/User.cpp \
	$(INCLUDE_DIR)/User.hpp \
	$(INCLUDE_DIR)/IncludeAndDefine.hpp \

ParserSensitivityList = \
	$(SRC_DIR)/Parser.cpp \
	$(INCLUDE_DIR)/Parser.hpp \

ServerSensitivityList = \
	$(SRC_DIR)/Server.cpp \
	$(INCLUDE_DIR)/Server.hpp \
	$(INCLUDE_DIR)/User.hpp \
	$(INCLUDE_DIR)/Room.hpp \
	$(INCLUDE_DIR)/Reservation.hpp \
	$(INCLUDE_DIR)/IncludeAndDefine.hpp \
	$(INCLUDE_DIR)/Date.hpp \
	$(CONFIGURATION_DIR)/RoomsInfo.json \
	$(CONFIGURATION_DIR)/UsersInfo.json \

RoomSensitivityList = \
	$(SRC_DIR)/Room.cpp \
	$(INCLUDE_DIR)/Server.hpp \
	$(INCLUDE_DIR)/User.hpp \
	$(INCLUDE_DIR)/Room.hpp \
	$(INCLUDE_DIR)/Reservation.hpp \
	$(INCLUDE_DIR)/IncludeAndDefine.hpp \

ReservationSensitivityList = \
	$(SRC_DIR)/Reservation.cpp \
	$(INCLUDE_DIR)/Server.hpp \
	$(INCLUDE_DIR)/User.hpp \
	$(INCLUDE_DIR)/Room.hpp \
	$(INCLUDE_DIR)/Reservation.hpp \
	$(INCLUDE_DIR)/IncludeAndDefine.hpp \

NetworkSensitivityList = \
	$(SRC_DIR)/Network.cpp \
	$(INCLUDE_DIR)/Server.hpp \
	$(INCLUDE_DIR)/Parser.hpp \
	$(INCLUDE_DIR)/IncludeAndDefine.hpp \
	$(CONFIGURATION_DIR)/config.json \

ClientSensitivityList = \
	$(SRC_DIR)/Client.cpp \
	$(INCLUDE_DIR)/Parser.hpp \
	$(INCLUDE_DIR)/IncludeAndDefine.hpp \
	$(CONFIGURATION_DIR)/config.json \

all: $(BUILD_DIR) server.out client.out

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

server.out : $(BUILD_DIR)/Network.o $(OBJECTS)
	$(CC) -o server.out $(BUILD_DIR)/Network.o $(OBJECTS)

client.out : $(BUILD_DIR)/Client.o $(OBJECTS)
	$(CC) -o client.out $(BUILD_DIR)/Client.o $(OBJECTS)

$(BUILD_DIR)/User.o: $(UserSensitivityList)
	$(CC) -c $(SRC_DIR)/User.cpp -o $(BUILD_DIR)/User.o

$(BUILD_DIR)/Parser.o: $(ParserSensitivityList)
	$(CC) -c $(SRC_DIR)/Parser.cpp -o $(BUILD_DIR)/Parser.o

$(BUILD_DIR)/Server.o: $(ServerSensitivityList)
	$(CC) -c $(SRC_DIR)/Server.cpp -o $(BUILD_DIR)/Server.o

$(BUILD_DIR)/Room.o: $(RoomSensitivityList)
	$(CC) -c $(SRC_DIR)/Room.cpp -o $(BUILD_DIR)/Room.o

$(BUILD_DIR)/Reservation.o: $(ReservationSensitivityList)
	$(CC) -c $(SRC_DIR)/Reservation.cpp -o $(BUILD_DIR)/Reservation.o

$(BUILD_DIR)/Network.o: $(NetworkSensitivityList)
	$(CC) -c $(SRC_DIR)/Network.cpp -o $(BUILD_DIR)/Network.o

$(BUILD_DIR)/Client.o: $(ClientSensitivityList)
	$(CC) -c $(SRC_DIR)/Client.cpp -o $(BUILD_DIR)/Client.o

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) *.o *.out