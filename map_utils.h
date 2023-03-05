#ifndef MAP_UTILS_H
#define MAP_UTILS_H

#include "map.h"
#include <stdio.h>

struct map;

typedef void (*printfunc_t)(void *);

void map_print(struct map *map, printfunc_t print_key, printfunc_t print_value);

void print_string(void *elem);

void print_int(void *elem);

void print_uint(void *elem);

void print_float(void *elem);

#endif
