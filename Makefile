CC=g++

all: build_server

build_server: main.o server.o logsqueue.o
	$(CC) -o build_server main.o server.o logsqueue.o -lev -lpthread

main.o: main.cpp logsqueue.h server.h headers.h
	$(CC) -c main.cpp

server.o: server.cpp server.h logsqueue.h  headers.h
	$(CC) -c server.cpp -lev -lpthread

logsqueue.o: logsqueue.cpp logsqueue.h headers.h
	$(CC) -c -lpthread logsqueue.cpp

clean:
	rm -rf *.o build_server
