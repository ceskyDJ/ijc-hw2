// htab_private.h
// Solution of IJC-DU2, exercise 2), 24th April 2021
// Author: Michal Šmahel, FIT
// Compiler: gcc 10.2.1

#include "htab.h"

// Structure for creating linked-list of pairs stored in hash table
struct htab_item {
    struct htab_pair * next; // Next pair
    struct htab_pair pair;   // Data of the actual pair
};

// Full definition of hash table structure for using by library functions
struct htab {
    size_t size;                 // Number of items stored in the hash table
    size_t arr_size;             // Number of keys used in the hash table (size of the item_arr array)
    struct htab_item item_arr[]; // Array with linked-lists that contains (key, value) pairs
};
