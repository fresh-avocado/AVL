CPP = g++
CPPFLAGS = -std=c++2a -Wall -Wpedantic -Wextra -Wshadow -Wsign-conversion
DEBUGFLAGS = -DDEBUG
VPATH = ./src:./src/avl:./src/utils:./tests

prod: main.o avl.o helpers.o
	$(CPP) $(CPPFLAGS) main.o avl.o helpers.o -o avl

main.o: main.cpp
	$(CPP) $(CPPFLAGS) -c ./src/main.cpp -o main.o

avl.o: avl.cpp
	$(CPP) $(CPPFLAGS) -c ./src/avl/avl.cpp -o avl.o

helpers.o: helpers.hpp helpers.cpp
	$(CPP) $(CPPFLAGS) -c ./src/utils/helpers.cpp -o helpers.o

debug: main.debug.o avl.debug.o helpers.debug.o
	$(CPP) $(CPPFLAGS) $(DEBUGFLAGS) main.debug.o avl.debug.o helpers.debug.o -o avl

main.debug.o: main.cpp
	$(CPP) $(CPPFLAGS) $(DEBUGFLAGS) -c ./src/main.cpp -o main.debug.o

avl.debug.o: avl.cpp
	$(CPP) $(CPPFLAGS) $(DEBUGFLAGS) -c ./src/avl/avl.cpp -o avl.debug.o

helpers.debug.o: helpers.hpp helpers.cpp
	$(CPP) $(CPPFLAGS) $(DEBUGFLAGS) -c ./src/utils/helpers.cpp -o helpers.debug.o

test: tests.o avl.debug.o helpers.debug.o
	$(CPP) $(CPPFLAGS) tests.o avl.debug.o helpers.debug.o -o avltest

tests.o: tests.cpp
	$(CPP) $(CPPFLAGS) $(DEBUGFLAGS) -c ./tests/tests.cpp -o tests.o

clean:
	rm *.o avl avltest