CFLAGS=-ggdb -pthread
CXXFLAGS=-ggdb -pthread
LDFLAGS=-ggdb -lstdc++ -ldar64 -pthread

all: webdar

clean:
	rm -f *.o webdar

webdar: webdar.o central_report.o connexion.o exceptions.o listener.o thread.o parser.o webdar_tools.o