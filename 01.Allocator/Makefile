CC=g++
FLAGS=-std=c++20 -Wall -pedantic -Wextra -Werror
TEST_LIBS=-lgtest_main -lgtest -lpthread -I /opt/homebrew/Cellar/googletest/1.11.0/include -L/opt/homebrew/lib/

all: clean main run test

main: main.o
	$(CC) $(FLAGS) main.o -o main

main.o:
	$(CC) $(FLAGS) main.cpp main.hpp -c

test: test_bin
	./test_bin

test_bin:
	$(CC) $(FLAGS) test.cpp -o test_bin $(TEST_LIBS)

run: main
	./main

clean:
	rm -f *.o *gch main test_bin
