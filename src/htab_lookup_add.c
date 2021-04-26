// htab_lookup_add.c
// Solution of IJC-DU2, exercise 2), 24th April 2021
// Author: Michal Šmahel, FIT
// Compiler: gcc 10.2.1

#include <stdlib.h> // malloc
#include "htab_private.h"

/**
 * Tries to find a pair (key, value) with a key in a hash table, if it's not found, the pair is added
 * @param t The hash table where to search from
 * @param key The key to search for
 * @return Pointer to the found or newly added pair (key, value); NULL => malloc error
 */
htab_pair_t * htab_lookup_add(htab_t * t, htab_key_t key) {
    htab_pair_t *pair;

    // The pair has already been in the hash table
    if ((pair = htab_find(t, key)) != NULL) {
        return pair;
    }

    // The pair isn't in the table --> add it
    // Allocate memory for a new item
    struct htab_item *new_item = NULL;
    if ((new_item = malloc(sizeof(struct htab_item))) == NULL) {
        return NULL;
    }

    // Allocate memory for the key (provided pointer in key parameter cannot be used)
    char *key_copy;
    // +1 --> \0 at the end of the string (strlen doesn't count with it)
    if ((key_copy = malloc(strlen(key) + 1)) == NULL) {
        return NULL;
    }

    // Create a copy of the key
    // +1 --> \0 at the end of the string (strlen doesn't count with it)
    memcpy(key_copy, key, strlen(key) + 1);

    // Fill the new item with default values and the key
    new_item->next = NULL;
    new_item->pair.key = key_copy;
    new_item->pair.value = 0;

    // Count index in the hash table
    size_t index = (htab_hash_function(key) % t->arr_size);

    // Find the last key in the linked-list at the counted index
    struct htab_item *last_item = NULL;
    for (struct htab_item *item = t->ptr_array[index]; item != NULL; item = item->next) {
        last_item = item;
    }

    // Add the new item at the end of the table at the index
    if (last_item != NULL) {
        // There is at least one item at the index
        last_item->next = new_item;
    } else {
        // The index is empty
        t->ptr_array[index] = new_item;
    }

    // Table size is bigger now
    t->size++;

    return &new_item->pair;
}
