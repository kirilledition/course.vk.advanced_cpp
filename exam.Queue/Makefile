CC=g++
FLAGS=-std=c++20

all: clean main run

main: main.o
	$(CC) $(FLAGS) main.o -o main

main.o:
	$(CC) $(FLAGS) main.cpp -c

run: main
	./main

clean:
	rm -f *.o main
