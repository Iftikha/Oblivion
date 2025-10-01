# Simple oblivion Makefile (Windows focused)

CXX = g++
CXXFLAGS = -std=c++17
INCLUDES = -Iinclude -IC:/libcurl/include
LIBS = -LC:/libcurl/lib -lcurl -lwinmm
SOURCES = include/Engine.cpp include/CommandParser.cpp include/ResponseHandler.cpp include/Conversation.cpp main.cpp
TARGET = oblivion.exe

build:
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SOURCES) $(LIBS) -o $(TARGET)

run: 
	./$(TARGET)

clean:
	del /f /q $(TARGET)

debug: CXXFLAGS += -g -DDEBUG
debug: build

all: build

.PHONY: build run clean debug all