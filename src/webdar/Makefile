CFLAGS=-ggdb -pthread
CXXFLAGS=-ggdb -pthread
LDFLAGS=-ggdb -lstdc++ -ldar64 -pthread

all: webdar

clean:
	rm -f *.o webdar

webdar: webdar.o central_report.o connexion.o exceptions.o listener.o thread.o parser.o webdar_tools.o server.o req_ans.o mutex.o semaphore.o uri.o session.o date.o

webdar.o : webdar.cpp exceptions.hpp central_report.hpp listener.hpp webdar_tools.hpp server.hpp authentication.hpp
central_report.o : central_report.cpp central_report.hpp webdar_tools.hpp exceptions.hpp mutex.hpp
connexion.o : connexion.cpp connexion.hpp exceptions.hpp
exceptions.o : exceptions.cpp exceptions.hpp
listener.o : listener.cpp listener.hpp connexion.hpp server.hpp webdar_tools.hpp central_report.hpp thread.hpp exceptions.hpp authentication.hpp
thread.o : thread.cpp thread.hpp exceptions.hpp mutex.hpp
parser.o : parser.cpp parser.hpp webdar_tools.hpp exceptions.hpp connexion.hpp req_ans.hpp central_report.hpp date.hpp
webdar_tools.o : webdar_tools.cpp webdar_tools.hpp exceptions.hpp uri.hpp
server.o : server.cpp server.hpp exceptions.hpp central_report.hpp req_ans.hpp server.hpp webdar_tools.hpp cookies.hpp error_page.hpp
req_ans.o : req_ans.cpp req_ans.hpp exceptions.hpp webdar_tools.hpp uri.hpp
mutex.o : mutex.cpp mutex.hpp exceptions.hpp
semaphore.o : semaphore.cpp semaphore.hpp mutex.hpp exceptions.hpp
uri.o : uri.cpp uri.hpp
session.o : session.cpp session.hpp cookies.hpp webdar_tools.hpp req_ans.hpp semaphore.hpp
date.o : date.cpp date.hpp exceptions.hpp webdar_tools.hpp
