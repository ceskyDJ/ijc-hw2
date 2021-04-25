// htab_size.c
// Solution of IJC-DU2, exercise 2), 24th April 2021
// Author: Michal Šmahel, FIT
// Compiler: gcc 10.2.1

#include "htab_private.h"

/**
 * Returns current size of a hash table
 * @param t The hash table
 * @return Number of pairs (key, value) stored in the hash table
 */
size_t htab_size(const htab_t * t) {
    return t->size;
}
