CPP17 = g++
CPPFLAGS = -std=c++2a -Wall -Wpedantic -Wextra -Wshadow -Wsign-conversion
DEBUGFLAGS = -DDEBUG

prod: main.o avl.o 
	$(CPP17) $(CPPFLAGS) main.o avl.o -o avl

main.o: main.cpp
	$(CPP17) $(CPPFLAGS) -c main.cpp -o main.o

avl.o: avl.hpp avl.cpp
	$(CPP17) $(CPPFLAGS) -c avl.cpp -o avl.o

debug: main.debug.o avl.debug.o 
	$(CPP17) $(CPPFLAGS) main.debug.o avl.debug.o -o avl

main.debug.o: main.cpp
	$(CPP17) $(CPPFLAGS) -c main.cpp -o main.debug.o

avl.debug.o: avl.hpp avl.cpp
	$(CPP17) $(CPPFLAGS) $(DEBUGFLAGS) -c avl.cpp -o avl.debug.o

clean:
	rm *.o avl avltest