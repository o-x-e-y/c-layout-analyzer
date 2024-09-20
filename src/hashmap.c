#include "headers/hashmap.h"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "headers/vec.h"

static const size_t K = 0x517cc1b727220a95;

static void __add_to_hash(size_t* hash, size_t val) { *hash = ((*hash << 5) ^ val) * K; }

hash_map_t new_hashmap(size_t key_size, size_t value_size) {
    hash_map_t res;

    res.key_size = key_size;
    res.value_size = value_size;

    res.store = new_vec(500, key_size + value_size);
    res.store.len = res.store.capacity;

    res.len = 0;

    return res;
}

size_t hash_hashmap(void* bytes, size_t len) {
    assert(sizeof(size_t) <= 8);

    size_t hash = 0;

    while (len >= sizeof(size_t)) {
        __add_to_hash(&hash, *(size_t*)bytes);
        bytes += sizeof(size_t);
        len -= sizeof(size_t);
    }

    if (len >= 4) {
        __add_to_hash(&hash, *(unsigned int*)bytes);
        len -= 4;
    }
    if (len >= 2) {
        __add_to_hash(&hash, *(unsigned short*)bytes);
        bytes -= 2;
    }
    if (len >= 1) {
        __add_to_hash(&hash, *(unsigned char*)bytes);
    }

    return hash;
}

void insert_hashmap(hash_map_t* m, void* key, void* value) {
    assert(key != NULL);

    size_t index = hash_hashmap(key, m->store.elem_size) % m->store.capacity;
    void* value_ptr = get_vec(&m->store, index);

    while (memcmp(value_ptr, key, m->key_size)) {
        value_ptr += m->store.elem_size;
    }

    memcpy(value_ptr, key, m->key_size);
    memcpy(value_ptr + m->key_size, value, m->value_size);

    m->len++;
}

void print_hashmap(hash_map_t* m, void print_key(void*), void print_value(void*)) {
    assert(m != NULL);

    printf("{");

    if (m->len > 0) {
        printf("\n");

        for (size_t i = 0; i < m->store.len; i++) {
        }
    }

    // if (vec->len > 0) {
    //     for (size_t i = 0; i < vec->len - 1; i++) {
    //         void* elem = get_vec(vec, i);
    //         (*print_elem)(elem);
    //         printf(", ");
    //     }

    //     void* elem = last_vec(vec);
    //     (*print_elem)(elem);
    // }

    printf("{\n");
}

void debug_hashmap(hash_map_t* m);