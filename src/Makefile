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
all: tail

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

## ## ##
## ## ##

# Compiling programs with only one module included
tail: tail.c
	$(CC) $(CFLAGS) tail.c -o tail

# Linking .o files into final binaries

# make pack
pack:
	zip xsmahe01.zip *.c *.h Makefile

# make clean
clean:
	rm -f tail xsmahe01.zip *.o