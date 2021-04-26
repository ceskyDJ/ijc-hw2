// htab_private.h
// Solution of IJC-DU2, exercise 2), 24th April 2021
// Author: Michal Šmahel, FIT
// Compiler: gcc 10.2.1

#include "htab.h"

// Structure for creating linked-list of pairs stored in hash table
struct htab_item {
    struct htab_item * next; // Next pair
    struct htab_pair pair;   // Data of the actual pair
};

// Full definition of hash table structure for using by library functions
struct htab {
    size_t size;                 // Number of items stored in the hash table
    size_t arr_size;             // Number of keys used in the hash table (size of the ptr_array array)
    struct htab_item *ptr_array[]; // Array with linked-lists that contains (key, value) pairs
};

// Item (struct htab_item) has complicated free (not only free(item)),
// so this macro solves problem with code redundancy
#define FREE_ITEM(item) do {                                                                                           \
    free((char *)(item)->pair.key);                                                                                    \
    free((item));                                                                                                      \
} while(0);
