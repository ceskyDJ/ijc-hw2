// Original source (comment changed only): http://www.fit.vutbr.cz/study/courses/IJC/public/DU2.html
// htab.h -- interface for htab library
// Licence: žádná (Public domain)

#ifndef __HTAB_H__
#define __HTAB_H__

#include <string.h>     // size_t
#include <stdbool.h>    // bool

// Structure for a hash table
// It has incomplete declaration, because the user of this library
// don't has to know about internal structure (some kind of encapsulation)
struct htab;
// Type for simpler usage of the hash table structure
typedef struct htab htab_t;

// Key of the pair
typedef const char * htab_key_t;
// Value of the pair
typedef int htab_value_t;

// Pair stored in the hash table
// This type (structure, resp.) is public for library users
typedef struct htab_pair {
    // Indexation key
    htab_key_t    key;
    // Associated value
    htab_value_t  value;
} htab_pair_t;

/**
 * Counts hash for a provided string
 * @param str The provided string to count hash from
 * @return Counted hash
 */
size_t htab_hash_function(htab_key_t str);
/**
 * Initializes hash table
 * @param n Number of keys that can be stored in the table
 * @return Initialized hash table; NULL => malloc error
 */
htab_t *htab_init(size_t n);
/**
 * Moves data from some hash table to a newly created one
 * @param n Size of the new hash table
 * @param from Pointer to the source hash table (where to copy from)
 * @return The new hash table; NULL => n < size of the source hash table
 */
htab_t *htab_move(size_t n, htab_t *from);
/**
 * Returns current size of a hash table
 * @param t The hash table
 * @return Number of pairs (key, value) stored in the hash table
 */
size_t htab_size(const htab_t * t);
/**
 * Returns number of keys that can be stored in a hash table
 * @param t Pointer to the hash table
 * @return Number of keys
 */
size_t htab_bucket_count(const htab_t * t);
/**
 * Finds an pair (key, value) by a key in a hash table
 * @param t The hash table where to search
 * @param key The key to search for
 * @return Found pair (key, value) or NULL
 */
htab_pair_t * htab_find(htab_t * t, htab_key_t key);
/**
 * Tries to find a pair (key, value) with a key in a hash table, if it's not found, the pair is added
 * @param t The hash table where to search from
 * @param key The key to search for
 * @return Pointer to the found or newly added pair (key, value); NULL => malloc error
 */
htab_pair_t * htab_lookup_add(htab_t * t, htab_key_t key);
/**
 * Removes a pair (key, value)  with a key from a hash table
 * @param t The hash table to be modified
 * @param key The key to remove
 * @return true => success, false => key wasn't found
 */
bool htab_erase(htab_t * t, htab_key_t key);
/**
 * Iterates over all pairs (key, value) of a hash table and call a function on each pair
 * @param t The hash table to be iterated throw
 * @param f The function to call for each pair
 */
void htab_for_each(const htab_t * t, void (*f)(htab_pair_t *data));
/**
 * Clears pairs (key, value) from a hash table
 * @param t The hash table to be changed
 */
void htab_clear(htab_t * t);
/**
 * Deallocates a hash table
 * @param t The hash table to be deallocated
 */
void htab_free(htab_t * t);

#endif // __HTAB_H__
