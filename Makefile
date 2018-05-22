all: Main Queues

Main: main.cpp
	g++ -c main.cpp

Queues: Queues.cpp
	g++ -c Queues.cpp

compile: main.o Queues.o
	g++ main.o Queues.o -o main

run:
	./main	