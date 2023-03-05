/* Author: Steffen Viken Valvaag <steffenv@cs.uit.no> */
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "list.h"
#include "map.h"

/**
 * @typedef key_value
 * @brief Key-Value pair data structure
 */
typedef struct key_value kv_t;

struct key_value {
    char *key;
    int value;
};

/**
 * @brief Initialize a key-value pair
 *
 * @param key char *
 * @param value int
 * @return kv_t *
 */
kv_t *kv_create(char *key, int value) {
    kv_t *pair = malloc(sizeof(kv_t *));

    if (!pair)
        fatal_error("kv_create: Malloc failed!\n");

    pair->key = key;
    pair->value = value;

    return pair;
}

/**
 * @brief Update value of pair
 *
 * @param pair kv_t *
 * @param new_value int
 */
void kv_update_value(kv_t *pair, int new_value) { pair->value = new_value; }

/**
 * @brief Update all values of a linked list storing kv_t pointers.
 *
 * @param pairs list_t * - List to update values of.
 * @param data map_t * - Map to fetch new values from.
 */
void update_list(list_t *pairs, map_t *data) {
    kv_t *pair;
    list_iter_t *i = list_createiter(pairs);

    while (list_hasnext(i)) {
        pair = list_next(i);

        // Update pairs values from the data map.
        kv_update_value(pair, *(int *)map_get(data, pair->key));
    }

    list_destroyiter(i);
}

/**
 * @brief comparison function which only compares on keys in list of kv_t
 * pointers
 *
 * @param elem1 void *
 * @param elem2 void *
 * @return int
 */
int compare_by_value(void *elem1, void *elem2) {
    kv_t *pair1 = (kv_t *)elem1;
    kv_t *pair2 = (kv_t *)elem2;

    return pair2->value - pair1->value;
}

int main(int argc, char **argv) {
    int i;
    int *count;
    char *curr;
    kv_t *curr_pair;
    FILE *f;
    list_iter_t *iter;
    map_t *word_counts = map_create(compare_strings, hash_string);
    list_t *tokens = list_create(compare_strings);
    list_t *unique_tokens = list_create(compare_by_value);

    // Tokenize all input files...
    for (i = 1; i < argc; i++) {

        // Read file contents
        f = fopen(argv[i], "r");

        // Tokenize file
        tokenize_file(f, tokens);

        // Close file buffer.
        fclose(f);
    }

    iter = list_createiter(tokens);

    // Iterate over tokens...
    while (list_hasnext(iter)) {
        curr = list_next(iter);

        // If token is not in map...
        if (!map_haskey(word_counts, curr)) {

            // Allocate space for reference count.
            count = malloc(sizeof(int));
            *count = 1;

            // Create new key-value pair with token, and count of 1.
            map_put(word_counts, curr, count);

            curr_pair = kv_create(curr, *count);

            // Add first occurance of word to unique list as key-value pair.
            list_addlast(unique_tokens, curr_pair);
            continue;
        }

        // Cast to int pointer, dereference and increment.
        (*(int *)map_get(word_counts, curr))++;
    }

    // Update counts.
    update_list(unique_tokens, word_counts);

    // Sort unique list by the values of each pair entry (using cmpfunc).
    list_sort(unique_tokens);

    // Write to stdout.
    iter = list_createiter(unique_tokens);
    printf("Unique sorted words with count from sample Trump tweets:\n");
    i = 0;
    while (list_hasnext(iter)) {
        curr_pair = list_next(iter);

        // Count number of times count is 1.
        if (curr_pair->value == 1) {
            i++;
        }

        // For very large inputs, dont display all unique words with count of 1.
        if (i > 5) {
            break;
        }

        printf("%d - %s\n", curr_pair->value, curr_pair->key);

        free(curr_pair);
    }

    list_destroyiter(iter);
    list_destroy(tokens);
    map_destroy(word_counts);
    list_destroy(unique_tokens);

    return 0;
}
