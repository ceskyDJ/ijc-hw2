#include <stdbool.h>
#include <stdio.h> // printf, fprintf
#include <stdlib.h> // free
#include "../src/htab_private.h"

int test_count;
static htab_t *global_table;
static char loaded_strings[6][3];
static int loaded_string_id;

typedef bool (*test_fun_t)();

void print_debug_info(htab_t *table);
bool test(test_fun_t fun, char *test_name);
void mock_erase(htab_pair_t *pair);
void mock_printf(htab_pair_t *pair);
bool test_added_item(htab_t *table, char *key);

bool test_htab_init() {
    size_t arr_size = 11;
    htab_t *table = htab_init(arr_size);

    print_debug_info(table);

    // Initial values
    if (table->size != 0) {
        free(table);
        return false;
    }
    if(table->arr_size != arr_size) {
        free(table);
        return false;
    }

    for (size_t i = 0; i < table->arr_size; i++) {
        if (table->ptr_array[i] != NULL) {
            free(table);
            return false;
        }
    }

    free(table);
    return true;
}

bool test_htab_free() {
    size_t arr_size = 7;
    htab_t *table = htab_init(arr_size);

    htab_free(table);

    // If there is no segfault, it's ok
    return true;
}

bool test_htab_bucket_count() {
    size_t arr_size = 23;
    htab_t *table = htab_init(arr_size);

    if (htab_bucket_count(table) != arr_size) {
        htab_free(table);
        return false;
    }

    htab_free(table);
    return true;
}

bool test_htab_find() {
    size_t arr_size = 11;
    htab_t *table = htab_init(arr_size);

    char *item_name = "test";
    if (htab_find(table, item_name) != NULL) {
        fprintf(stderr, "\tFailed before inserting");
        htab_free(table);
        return false;
    }

    struct htab_item *item;
    if ((item = malloc(sizeof(struct htab_item))) == NULL) {
        exit(1);
    }

    char *key_copy = malloc(strlen(item_name) + 1);
    memcpy(key_copy, item_name, strlen(item_name) + 1);

    item->next = NULL;
    item->pair.key = key_copy;
    item->pair.value = 0;

    size_t index = (htab_hash_function(item_name) % table->arr_size);
    table->ptr_array[index] = item;
    table->size++;

    print_debug_info(table);

    if (htab_find(table, item_name) == NULL) {
        fprintf(stderr, "\tFailed after inserting");
        htab_free(table);
        return false;
    }

    htab_free(table);
    return true;
}

bool test_htab_lookup_add() {
    size_t arr_size = 11;
    htab_t *table = htab_init(arr_size);

    char items[8][8] = {"super", "veta", "se", "super", "slovy", "se", "super", "pismeny"};
    for (size_t i = 0; i < 8; i++) {
        htab_lookup_add(table, items[i]);
        if (!test_added_item(table, items[i])) {
            fprintf(stderr, "Item '%s' with number %zu wasn't added successfully", items[i], i);
            return false;
        }

        print_debug_info(table);
    }

    if (table->size != 5) {
        htab_free(table);
        return false;
    }

    htab_free(table);
    return true;
}

bool test_htab_size() {
    size_t arr_size = 5;
    htab_t *table = htab_init(arr_size);

    fprintf(stderr, "\tBefore fill:\n");
    print_debug_info(table);

    if (table->size != htab_size(table)) {
        htab_free(table);
        return false;
    }

    fprintf(stderr, "\tAfter fill:\n");
    print_debug_info(table);

    htab_lookup_add(table, "htab");
    htab_lookup_add(table, "hello");
    htab_lookup_add(table, "hi");
    htab_lookup_add(table, "test");
    htab_lookup_add(table, "world");

    if (table->size != htab_size(table)) {
        htab_free(table);
        return false;
    }

    htab_free(table);
    return true;
}

bool test_htab_move() {
    size_t arr_size = 7;
    htab_t *old_table = htab_init(arr_size);

    size_t inserted_items = 5;
    htab_lookup_add(old_table, "htab");
    htab_lookup_add(old_table, "hello");
    htab_lookup_add(old_table, "hi");
    htab_lookup_add(old_table, "test");
    htab_lookup_add(old_table, "world");
    fprintf(stderr, "\tOld table:\n");
    print_debug_info(old_table);

    // Move with the same size
    htab_t *new_table = htab_move(arr_size, old_table);
    fprintf(stderr, "\tNew table:\n");
    print_debug_info(new_table);

    if (old_table->arr_size != new_table->arr_size || new_table->arr_size != arr_size) {
        htab_free(old_table);
        htab_free(new_table);
        fprintf(stderr, "\tOld table has different arr_size than new table\n");
        return false;
    }

    if (old_table->size != 0 || new_table->size != inserted_items) {
        htab_free(old_table);
        htab_free(new_table);
        fprintf(stderr, "\tOld table hasn't size == 0 or new table hasn't size of the old table\n");
        return false;
    }

    for (size_t i = 0; i < old_table->arr_size; i++) {
        if (old_table->ptr_array[i] != NULL) {
            htab_free(old_table);
            htab_free(new_table);
            fprintf(stderr, "\tOld table isn't empty\n");
            return false;
        }
    }

    bool empty = true;
    for (size_t i = 0; i < new_table->arr_size; i++) {
        if (new_table->ptr_array[i] != NULL) {
            empty = false;
            break;
        }
    }

    if (empty) {
        htab_free(old_table);
        htab_free(new_table);
        fprintf(stderr, "\tNew table is empty\n");
        return false;
    }

    // Move with the bigger size
    htab_free(old_table);
    htab_t *newest_table = htab_move(arr_size + 3, new_table);
    fprintf(stderr, "\tNewest table:\n");
    print_debug_info(newest_table);

    if (new_table->size != 0 || newest_table->size != inserted_items) {
        htab_free(new_table);
        htab_free(newest_table);
        fprintf(stderr, "\tNew table hasn't size == 0 or newest table hasn't the original size of new table\n");
        return false;
    }

    for (size_t i = 0; i < new_table->arr_size; i++) {
        if (new_table->ptr_array[i] != NULL) {
            htab_free(new_table);
            htab_free(newest_table);
            fprintf(stderr, "\tNew table isn't empty\n");
            return false;
        }
    }

    empty = true;
    for (size_t i = 0; i < newest_table->arr_size; i++) {
        if (newest_table->ptr_array[i] != NULL) {
            empty = false;
            break;
        }
    }

    if (empty) {
        htab_free(new_table);
        htab_free(newest_table);
        fprintf(stderr, "\tNewest table is empty\n");
        return false;
    }

    htab_free(new_table);
    htab_free(newest_table);
    return true;
}

bool test_htab_clear() {
    size_t arr_size = 7;
    htab_t *table = htab_init(arr_size);

    htab_lookup_add(table, "htab");
    htab_lookup_add(table, "hello");
    htab_lookup_add(table, "hi");
    htab_lookup_add(table, "test");
    htab_lookup_add(table, "world");
    fprintf(stderr, "\tBefore clear:\n");
    print_debug_info(table);

    htab_clear(table);
    fprintf(stderr, "\tAfter clear:\n");
    print_debug_info(table);

    for (size_t i = 0; i < table->arr_size; i++) {
        if (table->ptr_array[i] != NULL) {
            htab_free(table);
            fprintf(stderr, "\tTable isn't empty\n");
            return false;
        }
    }

    if (table->size != 0) {
        htab_free(table);
        fprintf(stderr, "\tTable size isn't 0\n");
        return false;
    }

    htab_free(table);
    return true;
}

bool test_htab_erase() {
    size_t arr_size = 4;
    htab_t *table = htab_init(arr_size);

    size_t inserted_items = 3;
    char *item_for_removing = "hello";
    htab_lookup_add(table, "htab");
    htab_lookup_add(table, item_for_removing);
    htab_lookup_add(table, "hi");
    fprintf(stderr, "\tBefore erase:\n");
    print_debug_info(table);

    htab_erase(table, item_for_removing);
    fprintf(stderr, "\tAfter erase:\n");
    print_debug_info(table);

    if (table->size != (inserted_items - 1)) {
        htab_free(table);
        fprintf(stderr, "\tTable size isn't lower by 1 than original size after removing one item\n");
        return false;
    }

    if (htab_find(table, item_for_removing) != NULL) {
        htab_free(table);
        fprintf(stderr, "\tThe right item wasn't removed\n");
        return false;
    }

    htab_free(table);
    return true;
}

bool test_htab_for_each() {
    size_t arr_size = 4;
    htab_t *table = htab_init(arr_size);

    htab_lookup_add(table, "htab");
    htab_lookup_add(table, "hello");
    htab_lookup_add(table, "hi");
    fprintf(stderr, "\tTable before for each:\n");
    print_debug_info(table);

    // Non-changing operation
    for (int i = 0; i < 3; i++) {
        memset(loaded_strings[i], '\0', 6);
    }

    htab_for_each(table, mock_printf);

    fprintf(stderr, "\tLoaded strings:\n");
    for (int i = 0; i < 3; i++) {
        fprintf(stderr, "\t\t[%d] => %s\n", i, loaded_strings[i]);
    }

    for (int i = 0; i < 3; i++) {
        if (strlen(loaded_strings[i]) == 0) {
            fprintf(stderr, "\tString at index %d wasn't printed\n", i);
            return false;
        }
    }

    // Changing operation
    global_table = table;
    htab_for_each(table, mock_erase);
    fprintf(stderr, "\tTable after for each:\n");
    print_debug_info(table);

    if (table->size != 0) {
        htab_free(table);
        fprintf(stderr, "\tTable size isn't 0\n");
        return false;
    }

    for (size_t i = 0; i < table->arr_size; i++) {
        if (table->ptr_array[i] != NULL) {
            htab_free(table);
            fprintf(stderr, "\tTable isn't empty\n");
            return false;
        }
    }

    htab_free(table);
    return true;
}

int main(void) {
    int score = 0;

    score += !test(test_htab_init, "htab_init()");
    score += !test(test_htab_free, "htab_free()");
    score += !test(test_htab_clear, "htab_clear()");
    score += !test(test_htab_bucket_count, "htab_bucket_count()");
    // score += !test(test_htab_has_function, "htab_hash_function()") - that's not my code, so I hope it's working
    score += !test(test_htab_find, "htab_find()");
    score += !test(test_htab_lookup_add, "htab_lookup_add()");
    score += !test(test_htab_size, "htab_size()");
    score += !test(test_htab_move, "htab_move()");
    score += !test(test_htab_erase, "htab_erase()");
    score += !test(test_htab_for_each, "htab_for_each()");

    // All tests ran well => score == 0 (number of error == 0)
    // Exit code has different meaning - 1 (and bigger) => error, 0 => success
    // We need to convert score to two-value state with exit code meaning
    // score == 0 => int(false) --> int(0) --> 0
    // score != 0 => int(true)  --> int(1) --> 1
    // True can be more than one but there is not too bad situation and it will be working
    return (int)(score != 0);
}

bool test(test_fun_t fun, char *test_name) {
    if (fun()) {
        // Test completed successfully
        printf("\033[92m[%.2d] Successful\033[39m\n", test_count++);

        return true;
    } else {
        // Test completed with error
        printf("\033[91m[%.2d] Failed: %s\033[39m\n", test_count++, test_name);

        return false;
    }
}

// For debugging in case of error
void print_debug_info(htab_t *table) {
    fprintf(stderr, "\tTable size: %zu\n", table->size);
    fprintf(stderr, "\tArray size: %zu\n", table->arr_size);
    fprintf(stderr, "\tItems:\n");

    for (size_t i = 0; i < table->arr_size; i++) {
        for (struct htab_item *item = table->ptr_array[i]; item != NULL; item = item->next) {
            fprintf(stderr, "\t\t[%zu][%s] = %d\n", i, item->pair.key, item->pair.value);
        }
    }
}

void mock_erase(htab_pair_t *pair) {
    if (!htab_erase(global_table, pair->key)) {
        fprintf(stderr, "\tmock_erase(): htab_erase() error\n");
    }
}

void mock_printf(htab_pair_t *pair) {
    fprintf(stderr, "\tmock_printf(): Processed key [%d]: %s", loaded_string_id, pair->key);
    fprintf(stderr, "\t\tActual value: %s", loaded_strings[loaded_string_id]);
    memcpy(loaded_strings[loaded_string_id], pair->key, 6);
    fprintf(stderr, "\t\tNew value: %s\n", loaded_strings[loaded_string_id]);

    loaded_string_id++;
}

bool test_added_item(htab_t *table, char *key) {
    htab_lookup_add(table, key);
    size_t index = (htab_hash_function(key) % table->arr_size);

    for (struct htab_item *item = table->ptr_array[index]; item != NULL; item = item->next) {
        if (strcmp(item->pair.key, key) == 0) {
            return true;
        }
    }

    return false;
}
