#ifndef __OXEY_VEC_H
#define __OXEY_VEC_H

#include <stddef.h>

typedef struct {
    size_t len;
    size_t capacity;
    size_t elem_size;

    void* ptr;
} Vec;

typedef struct {
    size_t elem_size;

    void* ptr;
    void* end;
} Iter;

Vec new_vec(size_t capacity, size_t elem_size);
void free_vec(Vec* vec, void elem_destructor(void*));

void* get_vec(Vec* vec, size_t index);
void* first_vec(Vec* vec);
void* last_vec(Vec* vec);

Vec clone_vec(Vec* vec);
size_t byte_capacity_vec(Vec* vec);

void push_vec(Vec* vec, void* elem);
void set_vec(Vec* vec, void* elem, size_t index);
void* pop_vec(Vec* vec);
void clear_vec(Vec* vec);

void insert_vec(Vec* vec, void* elem, size_t index);
void remove_vec(Vec* vec, size_t index);
void append_vec(Vec* vec, Vec* app);

void reserve_vec(Vec* vec, size_t elements);
void resize_vec(Vec* vec, size_t len);

void print_vec(Vec* vec, void print_elem(void*));
void debug_vec(Vec* vec);

Iter iter_from_vec(Vec* vec);
void* iter_peek(Iter* iter);
void* iter_next(Iter* iter);

void map_vec(Vec* vec, void map(void*));

#endif