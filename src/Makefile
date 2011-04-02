CC=gcc
CFLAGS=-Wall -std=c99 -ggdb

all: test

test: mobitrie.o mobitrie_test.o
	$(CC) -o test -ggdb mobitrie.o mobitrie_test.o

clean:
	rm -f test mt mobitrie.o mobitrie_test.o
