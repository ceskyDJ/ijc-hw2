// wordcount.c
// Solution of IJC-DU2, exercise 2), 10th April 2021
// Author: Michal Šmahel, FIT
// Compiler: gcc 10.2.1

#include <stdio.h> // printf, getchar
#include <ctype.h> // isspace
#include "../src/io.h"

int main(void) {
    char word[128];

    int length = read_word((char *)&word, 128, stdin);

    if (length != EOF) {
        printf("%s\n%d\n", word, length);

        if (isspace(getchar())) {
            printf("Y\n");
        } else {
            printf("N\n");
        }
    } else {
        printf("EOF\n");
    }

    return 0;
}
