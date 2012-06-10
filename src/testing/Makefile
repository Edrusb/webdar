CC=g++
LDFLAGS=-lpthread -ggdb -ldar64
CFLAGS=-ggdb -I../webdar
CXXFLAGS=-ggdb  -I../webdar

all: pthread_test
clean:
	rm -f *.o pthread_test

pthread_test: pthread_test.o

test_thread: test_thread.o ../webdar/thread.o ../webdar/exceptions.o

