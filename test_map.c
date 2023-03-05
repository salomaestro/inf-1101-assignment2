#include "common.h"
#include "map.h"
#include "map_utils.h"
#include <stdlib.h>
#include <string.h>

int main(void) {
    int i, n = 6;
    int *age;
    char *name;
    char *names[6] = {"Christian", "Johanne", "Signe",
                      "Benny",     "Tobias",  "Sigurd"};
    int ages[6] = {24, 22, 21, 12, 21, 24};
    char **narr = malloc(sizeof(char *) * n);
    int **aarr = malloc(sizeof(int *) * n);

    map_t *m = map_create(compare_strings, hash_string);

    for (i = 0; i < n; i++) {
        name = strdup(names[i]);
        age = malloc(sizeof(int *));
        *age = ages[i];
        map_put(m, name, age);
        narr[i] = name;
        aarr[i] = age;
    }

    map_print(m, print_string, print_int);

    printf("map has %s: %d\n", names[1], map_haskey(m, names[1]));

    printf("Age of %s: %d\n", names[3], *(int *)map_get(m, names[3]));

    map_destroy(m);
    for (i = 0; i < n; i++) {
        free(narr[i]);
        free(aarr[i]);
    }
    free(narr);
    free(aarr);
    return 0;
}
