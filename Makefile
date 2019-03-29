CC=g++

all: build_server

build_server: main.o errors.o
	$(CC) -o build_server main.o errors.o

main.o: main.cpp errors.h
	$(CC) -c main.cpp

errors.o: errors.cpp errors.h headers.h
	$(CC) -c errors.cpp

clean:
	rm -rf *.o build_server
