// htab_erase.c
// Solution of IJC-DU2, exercise 2), 24th April 2021
// Author: Michal Šmahel, FIT
// Compiler: gcc 10.2.1

#include <string.h> // strcmp
#include "htab_private.h" // htab_hash_function

/**
 * Removes a pair (key, value)  with a key from a hash table
 * @param t The hash table to be modified
 * @param key The key to remove
 * @return true => success, false => key wasn't found
 */
bool htab_erase(htab_t * t, htab_key_t key) {
    // Count index in the hash table
    size_t index = (htab_hash_function(key) % t->arr_size);

    // Search group of keys at the index for the key we are looking for
    struct htab_item *previous = NULL;
    for (struct htab_item *item = t->ptr_array[index]; item != NULL; item = item->next) {
        if (strcmp(item->pair.key, key) == 0) {
            // Change next pointer to the next item (item linked by current item)
            // If the next item doesn't exist, NULL will be set (item->next == NULL in that case)
            if (previous != NULL) {
                // Second and next key at the index
                previous->next = item->next;
            } else {
                // First key at the index
                t->ptr_array[index] = item->next;
            }

            // Table size is smaller now
            t->size--;

            // Free item and its data
            FREE_ITEM(item);

            return true;
        }

        previous = item;
    }

    return false;
}
