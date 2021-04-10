cmake_minimum_required(VERSION 3.19)
project(IJC\ |\ 2nd\ homework C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -pedantic -Wall -Wextra -fsanitize=address -O2")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

#add_library(libhtab.a htab.h)
#add_library(libhtab.so htab.h)

add_executable(tail tail.c)
add_executable(wordcount-cc wordcount.cc)
add_executable(wordcount wordcount.c htab.h io.c io.h)
add_executable(wordcount-dynamic wordcount.c htab.h io.c io.h)