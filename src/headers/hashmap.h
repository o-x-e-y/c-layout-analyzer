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
    vec_t store;
} hash_map_t;

size_t hash_hashmap(void* bytes, size_t len);

hash_map_t new_hashmap(size_t key_size, size_t value_size);
void insert_hashmap(hash_map_t* m, void* key, void* value);
void* get_hashmap(hash_map_t* m, void* key);

void print_hashmap(hash_map_t* m, void print_key(void*), void print_value(void*));
void debug_hashmap(hash_map_t* m);

#endif