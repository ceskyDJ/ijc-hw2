// io.c
// Solution of IJC-DU2, exercise 2), 10th April 2021
// Author: Michal Šmahel, FIT
// Compiler: gcc 10.2.1

#include <stdio.h> // fgetc, ungetc, EOF
#include <ctype.h> // isspace
#include "io.h"

/**
 * Reads one word from the file
 * @param s Pointer to string where to save the word to
 * @param max Maximum length of the word
 * @param f File where to read from
 * @return Real word length (can be longer than max) if success of EOF if end of file has been reached
 */
int read_word(char *s, int max, FILE *f) {
    int c;
    int length = 0;
    // Read word (until isspace() char is read - spaces before first non-space character are ignored)
    while ((c = fgetc(f)) != EOF && !(isspace(c) && length > 0)) {
        // Spaces before word are ignored
        if(!isspace(c)) {
            // Chars are append until maximum length is achieved
            if (length < (max - 1)) {
                s[length] = (char)c;
            }

            length++;
        }
    }

    // Propagate EOF to the function output
    if (c == EOF) {
        return EOF;
    }

    // Return last char (space after word)
    if (isspace(c)) {
        ungetc(c, f);
    }

    return length;
}
