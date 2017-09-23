CC=gcc
CFLAGS=-g -c -Wall
OBJECTS1=tree.o src-tree.o
default: tree 
all: tree 

tree: $(OBJECTS1)
	$(CC) $(OBJECTS1) -o tree
tree.o: tree.c tree.h
	$(CC) $(CFLAGS) tree.c
src-tree.o: src-tree.c
	$(CC) $(CFLAGS) src-tree.c

clean:
	rm -f $(OBJECTS1) tree
