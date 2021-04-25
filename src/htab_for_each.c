// htab_for_each.c
// Solution of IJC-DU2, exercise 2), 24th April 2021
// Author: Michal Šmahel, FIT
// Compiler: gcc 10.2.1

#include "htab_private.h"

/**
 * Iterates over all pairs (key, value) of a hash table and call a function on each pair
 * @param t The hash table to be iterated throw
 * @param f The function to call for each pair
 */
void htab_for_each(const htab_t * t, void (*f)(htab_pair_t *)) {
    // Iterate throw the hash table
    for (size_t i = 0; i < t->arr_size; i++) {
        struct htab_item *next = NULL;
        for (struct htab_item *item = t->ptr_array[i]; item != NULL; item = next) {
            // Create a backup of the next item (current item could be removed which cause
            // losing of the pointer to the next item)
            next = item->next;

            // Call the function on current item - with the stored pair, respectively
            f(&item->pair);
        }
    }
}
