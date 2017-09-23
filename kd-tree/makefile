CC=gcc
CFLAGS=-g -c -Wall
OBJECTS2=kd-tree.o src-kd-tree.o
default: kd-tree
all: kd-tree

kd-tree: $(OBJECTS2)
	$(CC) $(OBJECTS2) -o kd-tree
kd-tree.o: kd-tree.c kd-tree.h
	$(CC) $(CFLAGS) kd-tree.c
src-kd-tree.o: src-kd-tree.c
	$(CC) $(CFLAGS) src-kd-tree.c

clean:
	rm -f $(OBJECTS2) kd-tree
