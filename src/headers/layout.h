#ifndef __OXEY_LAYOUT_H
#define __OXEY_LAYOUT_H

#include <stddef.h>

const static size_t LAYOUT_CHARS = 30;

typedef struct {
    char* name;
    char matrix[30];
    size_t mapping[128];
    double score;
} layout_t;

typedef struct {
    size_t l;
    size_t r;
} pos_pair_t;

layout_t new_layout(char chars[LAYOUT_CHARS], char* name);
layout_t load_layout(char* path);
void free_layout(layout_t* layout);
void print_layout(layout_t* layout);

#endif