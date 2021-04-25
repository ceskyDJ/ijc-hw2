# Project: IJC - 2nd homework
# Author:  Michal Šmahel (xsmahe01)
# Date:    April 2021
#
# Usage:
#   - compile:             make
#   - pack to archive:     make pack
#   - clean:               make clean

CC=gcc
CFLAGS=-g -std=c99 -pedantic -Wall -Wextra -lm -O2

.PHONY: all run pack clean

# make
all: tail wordcount

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

## ## ##
io.o: io.c io.h
## ## ##

# Compiling programs with only one module included
tail: tail.c
	$(CC) $(CFLAGS) tail.c -o tail

wordcount: wordcount.c io.o
	$(CC) $(CFLAGS) wordcount.c io.o -o wordcount
# Linking .o files into final binaries

# make pack
pack:
	zip xsmahe01.zip *.c *.h Makefile

# make clean
clean:
	rm -f tail wordcount wordcount-dynamic *.o