// htab_find.c
// Solution of IJC-DU2, exercise 2), 24th April 2021
// Author: Michal Šmahel, FIT
// Compiler: gcc 10.2.1

#include <string.h> // strcmp
#include "htab_private.h"

/**
 * Finds an pair (key, value) by a key in a hash table
 * @param t The hash table where to search
 * @param key The key to search for
 * @return Found pair (key, value) or NULL
 */
htab_pair_t * htab_find(htab_t * t, htab_key_t key) {
    // Count index in the hash table
    size_t index = (htab_hash_function(key) % t->arr_size);

    // Search group of keys at the index for the key we are looking for
    for (struct htab_item *item = t->ptr_array[index]; item != NULL; item = item->next) {
        if (strcmp(item->pair.key, key) == 0) {
            return &item->pair;
        }
    }

    // Item with the key wasn't found
    return NULL;
}
