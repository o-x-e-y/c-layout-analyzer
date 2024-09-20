#include "headers/vec.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "headers/util.h"

static void __grow_vec(Vec* vec);

Vec new_vec(size_t item_capacity, size_t elem_size) {
    Vec res = (Vec){
        .capacity = item_capacity,
        .elem_size = elem_size,
        .len = 0,
        .ptr = calloc(item_capacity, elem_size),
    };

    return res;
}

void free_vec(Vec* vec, void elem_destructor(void*)) {
    if (vec != NULL) {
        if (elem_destructor != NULL) {
            for (void* ptr = vec->ptr; ptr < vec->ptr + vec->capacity; ptr += vec->elem_size) {
                elem_destructor(ptr);
            }
        }

        free(vec->ptr);
    }
}

void* get_vec(Vec* vec, size_t index) {
    if (index >= vec->capacity) {
        return NULL;
    } else {
        size_t offset = index * vec->elem_size;
        return vec->ptr + offset;
    }
}

void* first_vec(Vec* vec) {
    if (vec->len == 0) {
        return NULL;
    } else {
        return vec->ptr;
    }
}

void* last_vec(Vec* vec) {
    if (vec->len == 0) {
        return NULL;
    } else {
        return get_vec(vec, vec->len - 1);
    }
}

Vec clone_vec(Vec* vec) {
    Vec res = new_vec(vec->capacity, vec->elem_size);

    res.len = vec->len;

    memcpy(res.ptr, vec->ptr, vec->len * vec->elem_size);

    return res;
}

size_t byte_capacity_vec(Vec* vec) { return vec->capacity * vec->elem_size; }

void push_vec(Vec* vec, void* elem) {
    assert(vec != NULL);
    assert(elem != NULL);

    __grow_vec(vec);

    size_t offset = vec->len * vec->elem_size;

    memcpy(vec->ptr + offset, elem, vec->elem_size);

    vec->len++;
}

void set_vec(Vec* vec, void* elem, size_t index) {
    assert(vec != NULL);
    assert(elem != NULL);
    assert(index < vec->len);

    size_t offset = index * vec->elem_size;
    void* dst_ptr = vec->ptr + offset;

    memcpy(dst_ptr, elem, vec->elem_size);
}

void* pop_vec(Vec* vec) {
    assert(vec != NULL);

    if (vec->len == 0) {
        return NULL;
    } else {
        vec->len--;
        size_t offset = vec->len * vec->elem_size;

        return vec->ptr + offset;
    }
}

void clear_vec(Vec* vec) { vec->len = 0; }

void insert_vec(Vec* vec, void* elem, size_t index) {
    assert(vec != NULL);
    assert(elem != NULL);
    assert(index <= vec->len);

    if (index == vec->len) {
        push_vec(vec, elem);
    }

    vec->len++;

    __grow_vec(vec);

    size_t offset = index * vec->elem_size;

    void* src_ptr = vec->ptr + offset;
    void* dst_ptr = vec->ptr + offset + vec->elem_size;
    size_t copy_len = (vec->len - index) * vec->elem_size;

    memmove(dst_ptr, src_ptr, copy_len);
    memcpy(src_ptr, elem, vec->elem_size);
}

void remove_vec(Vec* vec, size_t index) {
    assert(vec != NULL);

    if (index < vec->len) {
        size_t offset = index * vec->elem_size;
        void* src_ptr = vec->ptr + offset + vec->elem_size;
        void* dst_ptr = vec->ptr + offset;
        size_t copy_len = (vec->len - index) * vec->elem_size;

        memcpy(dst_ptr, src_ptr, copy_len);
        vec->len--;
    }
}

void append_vec(Vec* vec, Vec* app) {
    assert(vec != NULL);
    assert(app != NULL);
    assert(vec->elem_size == app->elem_size);

    for (size_t i = 0; i < app->len; ++i) {
        void* elem = get_vec(vec, i);
        push_vec(vec, elem);
    }
}

void resize_vec(Vec* vec, size_t len) {
    assert(vec != NULL);

    if (len == vec->len) {
        return;
    }

    size_t new_size = len * vec->elem_size;

    vec->ptr = realloc(vec->ptr, new_size);
    vec->capacity = MIN(vec->capacity, len);
    vec->len = MIN(vec->len, len);
}

void reserve_vec(Vec* vec, size_t elements) {
    assert(vec != NULL);

    if (elements <= vec->len) {
        return;
    } else if (elements < vec->len * 2) {
        elements = vec->len * 2;
    }

    resize_vec(vec, elements);
}

void print_vec(Vec* vec, void print_elem(void*)) {
    assert(vec != NULL);

    printf("[");

    if (vec->len > 0) {
        for (size_t i = 0; i < vec->len - 1; i++) {
            void* elem = get_vec(vec, i);
            (*print_elem)(elem);
            printf(", ");
        }

        void* elem = last_vec(vec);
        (*print_elem)(elem);
    }

    printf("]");
}

void debug_vec(Vec* vec) {
    assert(vec != NULL);

    printf("Vec {    \n    capacity: %zu\n    len: %zu\n    elem_size: %zu\n    ptr: %p\n}\n",
           vec->capacity, vec->len, vec->elem_size, vec->ptr);
}

static void __grow_vec(Vec* vec) {
    if (vec->len >= vec->capacity) {
        size_t new_capacity = (vec->capacity + 1) * 2;

        vec->ptr = realloc(vec->ptr, new_capacity * vec->elem_size);
        vec->capacity = new_capacity;
    }
}

Iter iter_from_vec(Vec* vec) {
    assert(vec != NULL);

    void* end = vec->ptr + (vec->len - 1) * vec->elem_size;

    return (Iter){
        .elem_size = vec->elem_size,
        .ptr = vec->ptr,
        .end = end,
    };
}

void* iter_peek(Iter* iter) {
    assert(iter != NULL);

    if (iter->ptr <= iter->end) {
        return iter->ptr;
    } else {
        return NULL;
    }
}

void* iter_next(Iter* iter) {
    assert(iter != NULL);

    if (iter->ptr <= iter->end) {
        void* tmp = iter->ptr;

        iter->ptr += iter->elem_size;

        return tmp;
    } else {
        return NULL;
    }
}

void map_vec(Vec* vec, void map(void*)) {
    assert(vec != NULL);

    Iter iter = iter_from_vec(vec);

    while (iter_next(&iter)) {
        map(iter.ptr);
    }
}