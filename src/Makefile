# Project: IJC - 2nd homework
# Author:  Michal Šmahel (xsmahe01)
# Date:    April 2021
#
# Usage:
#   - compile:             make
#   - pack to archive:     make pack
#   - clean:               make clean

CC=gcc
CFLAGS=-g -std=c99 -pedantic -Wall -Wextra -O2 -fPIC

.PHONY: all pack clean

# make
all: tail wordcount wordcount-dynamic

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

## ## ##
io.o: io.c io.h
wordcount.o: wordcount.c htab.h
htab_bucket_count.o: htab_bucket_count.c htab.h htab_private.h
htab_clear.o: htab_clear.c htab.h htab_private.h
htab_erase.o: htab_erase.c htab.h htab_private.h
htab_find.o: htab_find.c htab.h htab_private.h
htab_for_each.o: htab_for_each.c htab.h htab_private.h
htab_free.o: htab_free.c htab.h htab_private.h
htab_hash_function.o: htab_hash_function.c htab.h htab_private.h
htab_init.o: htab_init.c htab.h htab_private.h
htab_lookup_add.o: htab_lookup_add.c htab.h htab_private.h
htab_move.o: htab_move.c htab.h htab_private.h
htab_size.o: htab_size.c htab.h htab_private.h
## ## ##

# Compiling libraries
libhtab.a: htab_bucket_count.o htab_clear.o htab_erase.o htab_find.o htab_for_each.o htab_free.o htab_hash_function.o htab_init.o htab_lookup_add.o htab_move.o htab_size.o
	ar crs libhtab.a htab_bucket_count.o htab_clear.o htab_erase.o htab_find.o htab_for_each.o htab_free.o htab_hash_function.o htab_init.o htab_lookup_add.o htab_move.o htab_size.o
libhtab.so: htab_bucket_count.o htab_clear.o htab_erase.o htab_find.o htab_for_each.o htab_free.o htab_hash_function.o htab_init.o htab_lookup_add.o htab_move.o htab_size.o
	$(CC) $(CFLAGS) -shared htab_bucket_count.o htab_clear.o htab_erase.o htab_find.o htab_for_each.o htab_free.o htab_hash_function.o htab_init.o htab_lookup_add.o htab_move.o htab_size.o -o libhtab.so

# Compiling programs with only one module included
tail: tail.c
	$(CC) $(CFLAGS) tail.c -o tail

# Compiling programs composited of multiple modules
wordcount: wordcount.o io.o libhtab.a
	$(CC) wordcount.o io.o -o wordcount -static -L. -lhtab
wordcount-dynamic: wordcount.o io.o libhtab.so
	$(CC) wordcount.o io.o -o wordcount-dynamic -L. -lhtab

# make pack
pack:
	zip xsmahe01.zip *.c *.h Makefile

# make clean
clean:
	rm -f tail wordcount wordcount-dynamic *.o