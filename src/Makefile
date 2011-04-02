CC=llvm-gcc -std=c99
CFLAGS=-Wall

all: mt

test: mobitrie.o mobitrie_test.o
	$(CC) -o test mobitrie.o mobitrie_test.o

clean:
	rm -f test mt mobitrie.o mobitrie_test.o
