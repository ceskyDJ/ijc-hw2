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

// Buffer with loaded lines
typedef struct buffer {
    char **data;
    int capacity; // Number of rows that can be inserted into the buffer
} buffer_t;

int main(int argc, char *argv[]) {
    // File to read from
    FILE *input_file = stdin;
    // Row offset from the beginning/end of the file (how many rows will be read)
    int row_offset = 10;
    // Row offset direction - true => from end of file; false => from the beginning of the file
    bool end_offset = true;

    // Load and parse input arguments
    for (int i = 1; i < argc; i++) {
        // Switch arguments
        // Row offset (from the beginning/end of the file)
        if (strcmp(argv[i], "-n") == 0) {
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
        }

        // Non-switch argument is path to the input file
        if ((input_file = fopen(argv[i], "r")) == NULL) {
            fprintf(stderr, "Argument bez přepínače musí být platná cesta k souboru\n"
                            "Pokud byla zadána platná cesta, soubor se nepodařilo otevřít\n");
            exit(1);
        }
    }

    // Prepare buffer with initial allocated memory
    buffer_t buffer = {.capacity = 1};
    if ((buffer.data = malloc(sizeof(char *))) == NULL) {
        fprintf(stderr, "Nepodařilo se alokovat paměť pro pomocný buffer\n");
        exit(1);
    }

    // Load rows into buffer
    int i = 0;
    bool warned = false;
    bool ignore_next = false;
    char read_line[ROW_SIZE];
    // +1 => '\0'
    while ((fgets(read_line, MAX_ROW_LENGTH + 1, input_file)) != NULL) {
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

        // Resizing buffer if needed
        if (i >= buffer.capacity) {
            char **tmp_ptr;
            if ((tmp_ptr = realloc(buffer.data, sizeof(char *) * buffer.capacity * 2)) == NULL) {
                fprintf(stderr, "Nepodařilo se přialokovat paměť pro pomocný buffer\n");
                free(tmp_ptr);
                exit(1);
            }

            buffer.data = tmp_ptr;
            buffer.capacity *= 2;
        }

        if ((buffer.data[i] = malloc(ROW_SIZE)) == NULL) {
            fprintf(stderr, "Nepodařilo se alokovat paměť pro uložení řádku do bufferu\n");
            exit(1);
        }

        memcpy(buffer.data[i], read_line, ROW_SIZE);
        i++;
    }

    // Set the start position for writing output by configured direction
    int start_row;
    if (end_offset) {
        start_row = (i + 1) - row_offset;
    } else {
        start_row = row_offset;
    }

    // Write output
    for (int j = start_row - 1; j < i; j++) {
        printf("%s", buffer.data[j]);
    }

    for (int j = 0; j < i; j++) {
        free(buffer.data[j]);
    }

    free(buffer.data);
    return 0;
}
