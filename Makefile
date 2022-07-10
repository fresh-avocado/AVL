CPP17 = g++
CPPFLAGS = -std=c++17 -Wall -Wpedantic -Wextra -Wshadow -Wsign-conversion

all:
	$(CPP17) $(CPPFLAGS) main.cpp

main: main.o avl.o 
	$(CPP17) $(CPPFLAGS) main.o avl.o -o avl

main.o: ./src/main.cpp
	$(CPP17) $(CPPFLAGS) -c ./src/main.cpp -o main.o

avl.o: ./src/avl.hpp
	$(CPP17) $(CPPFLAGS) -c ./src/avl.hpp -o avl.o

avltest.o: ./test/main.cpp
	$(CPP17) $(CPPFLAGS) -c ./test/main.cpp -o avltest.o

avltest: avltest.o avl.o
	$(CPP17) $(CPPFLAGS) avl.o -o avltest

clean:
	rm *.o avl avltest