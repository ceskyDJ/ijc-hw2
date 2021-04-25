// htab_move.c
// Solution of IJC-DU2, exercise 2), 24th April 2021
// Author: Michal Šmahel, FIT
// Compiler: gcc 10.2.1

#include "htab_private.h"

/**
 * Moves data from some hash table to a newly created one
 * @param n Size of the new hash table
 * @param from Pointer to the source hash table (where to copy from)
 * @return The new hash table; NULL => n < size of the source hash table
 */
htab_t *htab_move(size_t n, htab_t *from) {
    // The new hash table can has the same size or be bigger only
    if (n < from->size) {
        return NULL;
    }

    // Create new hash table
    htab_t *new_table = htab_init(n);

    // Move data from old hash table to the new one
    for (size_t i = 0; i < from->arr_size; i++) {
        new_table->ptr_array[i] = from->ptr_array[i];
        from->ptr_array[i] = NULL;
    }

    // All items of the old table were moved to the new table, so sizes will be changed
    new_table->size = from->size;
    from->size = 0;

    return new_table;
}
