CC=gcc
CFLAGS=-g -std=c99 -pedantic -Wall -Wextra -lm -O2

.PHONY: all clean

# make
all: io-test

# Build test binaries
io-test: io-test.c ../src/io.o
	$(CC) $(CFLAGS) io-test.c ../src/io.o -o io-test

# make clean
clean:
	rm -f tail wordcount wordcount-dynamic *.o