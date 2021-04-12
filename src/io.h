// io.h
// Solution of IJC-DU2, exercise 2), 10th April 2021
// Author: Michal Šmahel, FIT
// Compiler: gcc 10.2.1

#ifndef IJC_2ND_HOMEWORK_IO_H
#define IJC_2ND_HOMEWORK_IO_H

#include <stdio.h>

/**
 * Reads one word from the file
 * @param s Pointer to string where to save the word to
 * @param max Maximum length of the word
 * @param f File where to read from
 * @return 0 if success of EOF if end of file has been reached
 */
int read_word(char *s, int max, FILE *f);

#endif //IJC_2ND_HOMEWORK_IO_H
