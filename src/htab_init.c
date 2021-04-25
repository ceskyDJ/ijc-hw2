// htab_init.c
// Solution of IJC-DU2, exercise 2), 24th April 2021
// Author: Michal Šmahel, FIT
// Compiler: gcc 10.2.1

#include <stdlib.h>
#include "htab_private.h"

/**
 * Initializes hash table
 * @param n Number of keys that can be stored in the table
 * @return Initialized hash table; NULL => malloc error
 */
htab_t *htab_init(size_t n) {
    htab_t *hash_table;
    if ((hash_table = malloc(sizeof(struct htab) + sizeof(struct htab_item *) * n)) == NULL) {
        return NULL;
    }

    hash_table->size = 0;
    hash_table->arr_size = n;

    for (size_t i = 0; i < n; i++) {
        hash_table->ptr_array[i] = NULL;
    }

    return hash_table;
}
