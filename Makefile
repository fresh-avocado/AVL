CPP17 = g++
CPPFLAGS = -std=c++17 -Wall -Wpedantic -Wextra -Wshadow -Wsign-conversion

all: main.o avl.o 
	$(CPP17) $(CPPFLAGS) main.o avl.o -o avl

main.o: main.cpp
	$(CPP17) $(CPPFLAGS) -c main.cpp -o main.o

avl.o: avl.hpp avl.cpp
	$(CPP17) $(CPPFLAGS) -c avl.cpp -o avl.o

clean:
	rm *.o avl avltest