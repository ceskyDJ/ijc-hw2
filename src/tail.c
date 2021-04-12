// tail.c
// Solution of IJC-DU2, exercise 1), 10th April 2021
// Author: Michal Šmahel, FIT
// Compiler: gcc 10.2.1

#include <stdio.h> // FILE, fprintf, fopen, printf
#include <string.h> // strcmp
#include <stdlib.h> // strtol, exit, malloc, realloc
#include <stdbool.h>

// Maximum row length in number of chars (without \n)
#define MAX_ROW_LENGTH 511
// Maximum row size in memory (MAX_ROW_LENGTH +1 => '\n', +1 => '\0')
#define ROW_SIZE MAX_ROW_LENGTH + 2

int main(int argc, char *argv[]) {
    // File to read from
    FILE *input_file = stdin;
    bool default_file = true;
    // Row offset from the beginning/end of the file (how many rows will be read)
    int row_offset = 10;
    // Row offset direction - true => from end of file; false => from the beginning of the file
    bool end_offset = true;

    // Load and parse input arguments
    for (int i = 1; i < argc; i++) {
        // Switch arguments
        // Row offset (from the beginning/end of the file)
        if (strcmp(argv[i], "-n") == 0) {
            // Types: -n 10; -n +10
            if ((i + 1) >= argc || (row_offset = (int)strtol(argv[i + 1], NULL, 10)) <= 0) {
                fprintf(stderr, "Po přepínači -n musí následovat kladné celé číslo případně s prefixem '+'\n");
                exit(1);
            }

            // Change row offset direction to: from the beginning of the file
            if (argv[i + 1][0] == '+') {
                end_offset = false;
            }

            // There is the already processed value of -n switch at next index
            i++;
            // Move to processing another argument
            continue;
        } else if(strstr(argv[i], "-n") != NULL) {
            // Types: -n10; -n+10
            // "-n" => +2
            if ((row_offset = (int)strtol(argv[i] + 2, NULL, 10)) <= 0) {
                fprintf(stderr, "Hodnota přepínače -n musí být kladné celé číslo případně s prefixem '+'\n");
                exit(1);
            }

            // Change row offset direction to: from the beginning of the file
            // [0] => '-', [1] => 'n'
            if (argv[i][2] == '+') {
                end_offset = false;
            }

            // Move to processing another argument
            continue;
        }

        // Non-switch argument is path to the input file
        if ((input_file = fopen(argv[i], "r")) == NULL) {
            fprintf(stderr, "Argument bez přepínače musí být platná cesta k souboru\n"
                            "Pokud byla zadána platná cesta, soubor se nepodařilo otevřít\n");
            exit(1);
        } else {
            default_file = false;
        }
    }

    // Prepare buffer with initial allocated memory
    char **buffer;
    if (end_offset) {
        if ((buffer = malloc(row_offset * sizeof(char *))) == NULL) {
            fprintf(stderr, "Nepodařilo se alokovat paměť pro buffer\n");
            exit(1);
        }

        for (int i = 0; i < row_offset; i++) {
            if ((buffer[i] = malloc(ROW_SIZE)) == NULL) {
                fprintf(stderr, "Nepodařilo se alokovat paměť pro řádek bufferu\n");
                exit(1);
            }

            memset(buffer[i], '\0', ROW_SIZE);
        }
    }

    // Load rows into buffer or write rows
    int i = 0;
    bool warned = false;
    bool ignore_next = false;
    char read_line[ROW_SIZE];
    // +1 => '\0'
    while ((fgets(read_line, ROW_SIZE, input_file)) != NULL) {
        // Ignore this part of the file (for skipping parts outside maximum file length)
        if (ignore_next) {
            // Don't ignore next part if the whole row has been read yet
            if (read_line[strlen(read_line) - 1] == '\n') {
                ignore_next = false;
            }

            continue;
        }

        // Row is bigger than maximum length
        if (read_line[strlen(read_line) - 1] != '\n') {
            // Warn about it but only once
            if (!warned) {
                fprintf(stderr, "Soubor obsahuje delší řádek než je maximum (511 znaků bez \\n)\n");
                warned = true;
            }

            // Explicitly add \n char at the end of the row
            read_line[MAX_ROW_LENGTH] = '\n';

            // Next output of fgets() will be the rest of the row => ignore it
            ignore_next = true;
        }

        if (end_offset) {
            // Shift buffer rows
            for (int j = 0; j < row_offset - 1; j++) {
                memcpy(buffer[j], buffer[j + 1], ROW_SIZE);
            }

            // Put the content to the end row of the buffer
            memcpy(buffer[row_offset - 1], read_line, ROW_SIZE);
        } else if (i >= row_offset - 1) {
            printf("%s", read_line);
        }

        i++;
    }

    // Close file if it isn't stdin
    if (!default_file) {
        fclose(input_file);
    }

    if (end_offset) {
        // Write output of the buffer
        for (int j = 0; j < row_offset; j++) {
            printf("%s", buffer[j]);
            free(buffer[j]);
        }

        free(buffer);
    }

    return 0;
}
