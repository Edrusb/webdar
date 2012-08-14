CFLAGS=-ggdb
CXXFLAGS=-ggdb
LDFLAGS=-ggdb -lstdc++ -ldar64

all: webdar

clean:
	rm -f *.o webdar

webdar: webdar.o central_report.o connexion.o exceptions.o listener.o thread.o parser.o