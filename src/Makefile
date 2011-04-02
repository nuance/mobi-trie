CC=llvm-gcc -std=c99
CFLAGS=-Wall

all: test

test: mobitrie.o mobitrie_test.o
	$(CC) -o test mobitrie.o mobitrie_test.o
	./test

clean:
	rm -f test mt mobitrie.o mobitrie_test.o
