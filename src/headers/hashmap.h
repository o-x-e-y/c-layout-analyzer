#ifndef __OXEY_HASHMAP_H
#define __OXEY_HASHMAP_H

#include <stdbool.h>

#include "vec.h"

typedef struct {
    void* value;
} Bucket;

typedef struct {
    size_t key_size;
    size_t value_size;
    size_t len;
    Vec store;
} HashMap;

size_t hash_hashmap(void* bytes, size_t len);

HashMap new_hashmap(size_t key_size, size_t value_size);
void insert_hashmap(HashMap* m, void* key, void* value);
void* get_hashmap(HashMap* m, void* key);

void print_hashmap(HashMap* m, void print_key(void*), void print_value(void*));
void debug_hashmap(HashMap* m);

#endif