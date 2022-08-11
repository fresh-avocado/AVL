CPP = g++
CPPFLAGS = -std=c++2a -Wall -Wpedantic -Wextra -Wshadow -Wsign-conversion
DEBUGFLAGS = -DDEBUG

prod: main.o avl.o helpers.o
	$(CPP) $(CPPFLAGS) main.o avl.o helpers.o -o avl

main.o: main.cpp
	$(CPP) $(CPPFLAGS) -c main.cpp -o main.o

avl.o: avl.hpp
	$(CPP) $(CPPFLAGS) -c avl.hpp -o avl.o

helpers.o: helpers.hpp helpers.cpp
	$(CPP) $(CPPFLAGS) -c helpers.cpp -o helpers.o

debug: main.debug.o avl.debug.o helpers.debug.o
	$(CPP) $(CPPFLAGS) $(DEBUGFLAGS) main.debug.o avl.debug.o helpers.debug.o -o avl

main.debug.o: main.cpp
	$(CPP) $(CPPFLAGS) $(DEBUGFLAGS) -c main.cpp -o main.debug.o

avl.debug.o: avl.hpp
	$(CPP) $(CPPFLAGS) $(DEBUGFLAGS) -c avl.hpp -o avl.debug.o

helpers.debug.o: helpers.hpp helpers.cpp
	$(CPP) $(CPPFLAGS) $(DEBUGFLAGS) -c helpers.cpp -o helpers.debug.o

clean:
	rm *.o avl avltest