#include "map.h"
#include "common.h"
#include "list.h"
#include "stdlib.h"

// Sets the bucket size of the map.
#define MAX_MAP_KEYS 10

/**
 * @typedef pair
 * @brief Stores a key-value pair.
 *
 */
typedef struct pair pair_t;

struct pair {
    void *key;
    void *value;
};

struct map {
    int max;
    hashfunc_t hash;
    cmpfunc_t cmp;
    list_t **bucket;
};

static pair_t *create_pair(void *key, void *value) {
    pair_t *pair = malloc(sizeof(pair_t));

    if (!pair)
        fatal_error("create_pair: Malloc failed!\n");

    pair->key = key;
    pair->value = value;

    return pair;
}

static void pair_destroy(pair_t *pair) { free(pair); }

static list_t **init_indices(size_t n, cmpfunc_t cmpfunc) {
    int i;

    // Allocate array of lists.
    list_t **out = calloc(n, sizeof(list_t *));

    if (!out)
        fatal_error("init_indices: Calloc failed!\n");

    // Set all lists to empty list.
    for (i = 0; i < n; i++) {
        out[i] = list_create(cmpfunc);
    }

    return out;
}

map_t *map_create(cmpfunc_t cmpfunc, hashfunc_t hashfunc) {
    int i;

    map_t *map = malloc(sizeof(map_t));

    if (!map)
        fatal_error("map_create: Malloc failed!\n");

    map->max = MAX_MAP_KEYS;
    map->hash = hashfunc;
    map->cmp = cmpfunc;
    map->bucket = init_indices(MAX_MAP_KEYS, cmpfunc);

    return map;
}

void map_destroy(map_t *map) {
    int i;
    list_t *inner;
    list_iter_t *inner_iter;

    // Loop over bucket indices.
    for (i = 0; i < map->max; i++) {
        inner = map->bucket[i];

        // If nothing to free.
        if (inner == NULL) {
            continue;
        }

        // Free inner key value pair at each index.
        inner_iter = list_createiter(inner);

        // Iterate over and free all pairs within.
        while (list_hasnext(inner_iter)) {
            pair_destroy(list_next(inner_iter));
        }
        list_destroyiter(inner_iter);
    }

    // Free the bucket array.
    free(map->bucket);
    free(map);
    return;
}

/**
 * @brief Get mapped hash value
 *
 * @param hash hashfunc_t
 * @param key void *
 * @param range int
 * @return unsigned long
 */
static unsigned long gethash(hashfunc_t hash, void *key, int range) {
    return hash(key) % range;
}

void map_put(map_t *map, void *key, void *value) {

    // Get appropriate index for key..
    unsigned long ind = gethash(map->hash, key, map->max);

    // Add kay value pair to end of list.
    list_addlast(map->bucket[ind], create_pair(key, value));
}

/**
 * @brief Static function to reduce boilerplate..
 *
 * @param map
 * @param key
 * @return pair_t *
 */
static pair_t *map_getpair(map_t *map, void *key) {
    int cmpval;
    pair_t *current_pair;
    unsigned long ind = gethash(map->hash, key, map->max);

    list_t *inner = map->bucket[ind];
    list_iter_t *inner_iter = list_createiter(inner);

    // Iterate over pairs, look for match.
    while (list_hasnext(inner_iter)) {
        current_pair = list_next(inner_iter);

        // Comparison.
        cmpval = map->cmp(current_pair->key, key);

        // Match
        if (cmpval == 0)
            return current_pair;
    }

    list_destroyiter(inner_iter);
    return NULL;
}

int map_haskey(map_t *map, void *key) {
    if (!map_getpair(map, key)) {
        return 0;
    }

    return 1;
}

void *map_get(map_t *map, void *key) {
    pair_t *pair = map_getpair(map, key);

    if (!pair)
        return NULL;

    return pair->value;
}

/**
 * @typedef Print functions
 * @brief Functions for printing specific types.
 */
typedef void (*printfunc_t)(void *);

void print_string(void *elem) { printf("%s", (char *)elem); }
void print_int(void *elem) { printf("%d", *(int *)elem); }
void print_uint(void *elem) { printf("%lu", *(unsigned long *)elem); }
void print_float(void *elem) { printf("%f", *(float *)elem); }

void map_print(map_t *map, printfunc_t print_key, printfunc_t print_value) {
    int i;
    list_t *inner;
    list_iter_t *inner_iter;
    pair_t *pair;

    printf("Map:\n");
    for (i = 0; i < map->max; i++) {

        // Get inner list.
        inner = map->bucket[i];

        if (!inner) {
            printf("\t‰d -: -\n", i);
        }

        printf("\t%d ", i);
        inner_iter = list_createiter(inner);

        while (list_hasnext(inner_iter)) {
            pair = list_next(inner_iter);

            print_key(pair->key);
            printf(": ");
            print_value(pair->value);

            if (list_hasnext(inner_iter))
                printf(" -> ");
        }
        list_destroyiter(inner_iter);
        printf("\n");
    }
}
