// htab_clear.c
// Solution of IJC-DU2, exercise 2), 24th April 2021
// Author: Michal Šmahel, FIT
// Compiler: gcc 10.2.1

#include <stdlib.h> // free
#include "htab_private.h"

/**
 * Clears pairs (key, value) from a hash table
 * @param t The hash table to be changed
 */
void htab_clear(htab_t * t) {
    // Go throw all keys and empty contained linked-lists
    for (size_t i = 0; i < t->arr_size; i++) {
        // Deallocate items stored in the linked-list at the key i
        struct htab_item *item = t->ptr_array[i];
        struct htab_item *next;
        while(item != NULL) {
            next = item->next;
            free(item);
            item = next;
        }

        // Remove pointer to the first item (the item don't exists)
        t->ptr_array[i] = NULL;
    }

    // The table is empty now
    t->size = 0;
}
