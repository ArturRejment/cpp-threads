CC=g++

all: main

main: main.cpp
	$(CC) main.cpp -pthread -lncurses -o main
