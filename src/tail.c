// tail.c
// Solution of IJC-DU2, exercise 1), 10th April 2021
// Author: Michal Šmahel, FIT
// Compiler: gcc 10.2.1

#include <stdio.h> // FILE, fprintf, fopen, fclose, printf, stderr
#include <string.h> // strcmp, memset, strstr
#include <stdlib.h> // strtol, exit, malloc, realloc
#include <stdbool.h>

// Maximum row length in number of chars (without \n)
#define MAX_ROW_LENGTH 511
// Maximum row size in memory (MAX_ROW_LENGTH +1 => '\n', +1 => '\0')
#define ROW_SIZE MAX_ROW_LENGTH + 2

// Buffer for storing cached rows
typedef struct buffer {
    unsigned size;
    char *(data)[];
} buffer_t;

/**
 * Allocates memory for a new buffer and initialize the structure
 * @param buffer_size Size of the buffer (number of stored rows)
 * @return Pointer to prepared buffer
 */
buffer_t *alloc_buffer(unsigned buffer_size) {
    buffer_t *buffer;
    if ((buffer = malloc(sizeof(buffer_t) + buffer_size * sizeof(char *))) == NULL) {
        fprintf(stderr, "Nepodařilo se alokovat paměť pro buffer\n");
        exit(1);
    }

    for (int i = 0; i < (int)buffer_size; i++) {
        if ((buffer->data[i] = malloc(ROW_SIZE)) == NULL) {
            fprintf(stderr, "Nepodařilo se alokovat paměť pro řádek bufferu\n");
            exit(1);
        }

        memset(buffer->data[i], '\0', ROW_SIZE);
    }

    buffer->size = buffer_size;

    return buffer;
}

/**
 * Shifts buffer items to create free space at the end of the buffer
 * @param buffer Buffer to modify
 */
void shift_buffer_items(buffer_t *buffer) {
    for (int j = 0; j < (int)buffer->size - 1; j++) {
        memcpy(buffer->data[j], buffer->data[j + 1], ROW_SIZE);
    }
}

/**
 * Adds a new row at the end of the buffer
 * @param buffer Buffer to modify
 * @param read_line Row to insert
 */
void push_row_to_buffer(buffer_t *buffer, char *read_line) {
    // Shift buffer rows
    shift_buffer_items(buffer);

    // Put the content to the end row of the buffer
    memcpy(buffer->data[buffer->size - 1], read_line, ROW_SIZE);
}

/**
 * Writes buffer content
 * @param buffer Buffer to write
 */
void write_buffer(buffer_t *buffer) {
    for (int j = 0; j < (int)buffer->size; j++) {
        printf("%s", buffer->data[j]);
    }
}

/**
 * Deallocates buffer allocated in memory
 * @param buffer Buffer to deallocate
 */
void deallocate_buffer(buffer_t *buffer) {
    for (int j = 0; j < (int)buffer->size; j++) {
        free(buffer->data[j]);
    }

    free(buffer);
}

/**
 * Program for writing last (or first) N lines of the file
 * @param argc Number of input arguments
 * @param argv Array of input arguments. Format: [0] => "-n", [1] => N, [2] => [file]. All arguments are optional.
 * @return Exit code. 0 if success, 1 if something failed
 */
int main(int argc, char *argv[]) {
    // File to read from
    FILE *input_file = stdin;
    bool default_file = true;
    // Row offset from the beginning/end of the file (how many rows will be read)
    unsigned row_offset = 10;
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
    // Buffer is not needed if the opposite direction mode is activated (end_offset == true)
    buffer_t *buffer = NULL;
    if (end_offset) {
        buffer = alloc_buffer(row_offset);
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

        // Process input
        if (end_offset) {
            push_row_to_buffer(buffer, read_line);
        } else if (i >= (int)row_offset - 1) {
            printf("%s", read_line);
        }

        i++;
    }

    // Close file if it isn't stdin
    if (!default_file) {
        fclose(input_file);
    }

    // Write buffer content to stdout and deallocate buffer if it has been used
    if (end_offset) {
        write_buffer(buffer);
        deallocate_buffer(buffer);
    }

    return 0;
}
