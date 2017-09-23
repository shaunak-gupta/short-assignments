CC=gcc
CFLAGS=-g -c -Wall
OBJECTS=trie.o src-trie.o
default: trie
all: trie

trie: $(OBJECTS)
	$(CC) $(OBJECTS) -o trie
trie.o: trie.c trie.h
	$(CC) $(CFLAGS) trie.c
src-trie.o: src-trie.c
	$(CC) $(CFLAGS) src-trie.c

clean:
	rm -f $(OBJECTS) trie
