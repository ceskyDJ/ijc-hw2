CC=gcc
CFLAGS=-g -std=c99 -pedantic -Wall -Wextra -lm -O2 -fPIC
CXX=g++
CXX_FLAGS=-std=c++11 -O2

.PHONY: all clean

# make
all: io-test htab-test

# Build dependencies for some tests
wordcount-cc: ../src/wordcount.cc
	$(CXX) $(CXX_FLAGS) ../src/wordcount.cc -o wordcount-cc

# Build test binaries
io-test: io-test.c ../src/io.o
	$(CC) $(CFLAGS) io-test.c ../src/io.o -o io-test
htab-test: htab-test.c ../src/libhtab.so ../src/htab_private.h
	$(CC) $(CFLAGS) htab-test.c -o htab-test -L../src -lhtab

# make clean
clean:
	rm -f io-test htab-test *.o