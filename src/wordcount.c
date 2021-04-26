// wordcount.c
// Solution of IJC-DU2, exercise 2), 10th April 2021
// Author: Michal Šmahel, FIT
// Compiler: gcc 10.2.1

#include <stdio.h> // stdin, stderr, EOF, fprintf, printf
#include <time.h> // clock_t, clock
#include <inttypes.h> // uint32_t
#include "htab.h" // htab_t, htab_init, htab_lookup_add, htab_pair_t, htab_free, htab_move
#include "io.h" // read_word

#ifdef HASHTEST
/**
 * Counts hash for a provided string
 * @param str The provided string to count hash from
 * @return Counted hash
 */
size_t htab_hash_function(htab_key_t str) {
    // Source: https://stackoverflow.com/a/57960443
    uint32_t h = 3323198485ul;

    for (; *str; ++str) {
        h ^= *str;
        h *= 0x5bd1e995;
        h ^= h >> 15;
    }

    return h;
}
#endif

// Number of "rows" in a hash table
// 512 is good for texts with < 10^4 words (~ 20 words in the linked list)
// with more words the speed will be increased (we need to iterate over many linked-lists)
#define HASH_TABLE_ARR_SIZE 512
// Maximum length of loaded word (without \0)
#define MAX_WORD_LENGTH 127

/**
 * Prints word statistics (word and number of its occurrences)
 * @param data The pair with the data for printing
 */
void print_word_statistics(htab_pair_t *data) {
    printf("%s\t%d\n", data->key, data->value);
}

/**
 * Program for counting words in typed sentence (input is gotten from stdin)
 * @return Exit code. 0 if success, 1 if something failed
 */
int main(void) {
    clock_t start = clock();

    // Prepare hash table for storing words
    htab_t *hash_table;
    if ((hash_table = htab_init(HASH_TABLE_ARR_SIZE)) == NULL) {
        fprintf(stderr, "Došlo k chybě při alokaci paměti pro rozptýlenou tabulku");

        return 1;
    }

    // Loading words into the hash table
    char word[MAX_WORD_LENGTH + 1] = ""; // +1 --> \0
    bool warned = false;
    int loaded_length;
    while ((loaded_length = read_word(word, MAX_WORD_LENGTH, stdin)) != EOF) {
        // There is an implementation limit for word length, longer words are cut to the maximum length
        // and user is warned by this alert (but only once)
        if (loaded_length > MAX_WORD_LENGTH && !warned) {
            fprintf(stderr, "Byla překročena maximální délka slova (%d znaků)\n", MAX_WORD_LENGTH);
            warned = true;
        }

        // Add/load a pair to/from the hash table (add in case the word hasn't been in the table yet)
        htab_pair_t *pair;
        if ((pair = htab_lookup_add(hash_table, word)) == NULL) {
            fprintf(stderr, "Došlo k chybě při alokaci paměti pro nový prvek rozptýlené tabulky\n");

            htab_free(hash_table);
            return 1;
        }

        // Increment number of occurrences (for the "fresh added" word there is 0,
        // so after incrementation there will be 1)
        pair->value++;

        // Clear the word
        memset(word, '\0', MAX_WORD_LENGTH + 1); // +1 --> \0 at the end of string
    }

#ifdef MOVETEST
    // Test of the htab_move function
    htab_t *new_hash_table;
    if ((new_hash_table = htab_move(HASH_TABLE_ARR_SIZE, hash_table)) == NULL) {
        fprintf(stderr, "Došlo k chybě při alokaci paměti pro novou rozptýlenou tabulku\n");

        htab_free(hash_table);
        return 1;
    }

    // Old table can be freed now
    htab_free(hash_table);

    // Print statistics for each of the loaded words
    htab_for_each(new_hash_table, print_word_statistics);

    // Clean allocated memory
    htab_free(new_hash_table);
#else // MOVETEST
    // Print statistics for each of the loaded words
    htab_for_each(hash_table, print_word_statistics);

    // Clean allocated memory
    htab_free(hash_table);
#endif // MOVETEST

    // Measure script execution time
    fprintf(stderr, "Doba trvání = %.3g\n", (double)(clock() - start) / CLOCKS_PER_SEC);

    return 0;
}
