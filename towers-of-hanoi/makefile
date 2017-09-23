CC=gcc
CFLAGS=-g -c -Wall
OBJECTS3=tower.o src-tower.o

default: tower
all: tower

tower: $(OBJECTS3)
	$(CC) $(OBJECTS3) -o prog3-cs1524
tower.o: tower.c tower.h
	$(CC) $(CFLAGS) tower.c
src-tower.o: src-tower.c
	$(CC) $(CFLAGS) src-tower.c

clean:
	rm -f $(OBJECTS3) tower
