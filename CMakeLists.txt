cmake_minimum_required(VERSION 3.19)
project(IJC\ |\ 2nd\ homework LANGUAGES C CXX)
set(CMAKE_SOURCE_DIR src/)
set(PROJECT_BINARY_DIR ../src/)
set(CMAKE_BINARY_DIR ../src/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../src/)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -pedantic -Wall -Wextra -fsanitize=address")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

add_library(libhtab.a src/htab.h src/htab_private.h src/htab_bucket_count.c src/htab_clear.c src/htab_erase.c src/htab_find.c src/htab_for_each.c src/htab_free.c src/htab_hash_function.c src/htab_init.c src/htab_lookup_add.c src/htab_move.c src/htab_size.c)
add_library(libhtab.so src/htab.h src/htab_private.h src/htab_bucket_count.c src/htab_clear.c src/htab_erase.c src/htab_find.c src/htab_for_each.c src/htab_free.c src/htab_hash_function.c src/htab_init.c src/htab_lookup_add.c src/htab_move.c src/htab_size.c)

add_executable(tail src/tail.c)
add_executable(wordcount-cc src/wordcount.cc)
add_executable(wordcount src/wordcount.c src/htab.h src/io.c src/io.h)
add_executable(wordcount-dynamic src/wordcount.c src/htab.h src/io.c src/io.h)
