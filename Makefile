CC=g++

all: build_server

build_server: main.o
	$(CC) -o build_server main.o

main.o: main.cpp
	$(CC) -c main.cpp

clean:
	rm -rf *.o build_server
