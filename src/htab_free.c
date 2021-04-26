// htab_free.c
// Solution of IJC-DU2, exercise 2), 24th April 2021
// Author: Michal Šmahel, FIT
// Compiler: gcc 10.2.1

#include <stdlib.h> // free
#include "htab_private.h" // htab_clear

/**
 * Deallocates a hash table
 * @param t The hash table to be deallocated
 */
void htab_free(htab_t * t) {
    htab_clear(t);
    free(t);
}
