CC=g++
LDFLAGS=-lpthread -ggdb -ldar64 -L$(HOME)/usr/lib
CFLAGS=-ggdb -I../webdar -I$(HOME)/usr/include
CXXFLAGS=-ggdb  -I../webdar -I$(HOME)/usr/include

All: pthread_test test_thread test_events
clean:
	rm -f *.o pthread_test test_thread test_events

pthread_test: pthread_test.o

test_thread: test_thread.o ../webdar/thread.o ../webdar/exceptions.o ../webdar/mutex.o

test_events: test_events.o ../webdar/exceptions.o ../webdar/reference.o ../webdar/events.o
