CC=g++

all: build_server

build_server: main.o server.o
	$(CC) -o build_server main.o server.o -lev

main.o: main.cpp server.h
	$(CC) -c main.cpp

server.o: server.cpp server.h headers.h
	$(CC) -c server.cpp -lev

clean:
	rm -rf *.o build_server
