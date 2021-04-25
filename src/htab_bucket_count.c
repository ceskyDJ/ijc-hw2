// htab_bucket_count.c
// Solution of IJC-DU2, exercise 2), 24th April 2021
// Author: Michal Šmahel, FIT
// Compiler: gcc 10.2.1

#include "htab_private.h"

/**
 * Returns number of keys that can be stored in a hash table
 * @param t Pointer to the hash table
 * @return Number of keys
 */
size_t htab_bucket_count(const htab_t * t) {
    return t->arr_size;
}
