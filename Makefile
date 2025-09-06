# Simple oblivion Makefile
build:
	g++ -std=c++17 -Iinclude -IC://libcurl/include include/Engine.cpp include/Conversation.cpp main.cpp -LC://libcurl/lib -lcurl -o oblivion.exe

run:
	./oblivion.exe

clean:
	del oblivion.exe

all: build run