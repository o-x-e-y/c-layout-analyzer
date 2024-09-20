#include "headers/layout.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "headers/util.h"

layout_t new_layout(char chars[LAYOUT_CHARS], char* name) {
    assert(name != NULL);
    assert(strlen(chars) == LAYOUT_CHARS);

    layout_t res;

    for (size_t i = 0; i < LAYOUT_CHARS; ++i) {
        char c = chars[i];
        res.mapping[(size_t)c] = i;
        res.matrix[i] = c;
    }

    size_t name_len = strlen(name) + 1;
    res.name = malloc(name_len * sizeof(char));
    strncpy(res.name, name, name_len);
    res.score = 0.0;

    return res;
}

layout_t load_layout(char* path) {
    assert(path != NULL);

    String s = read_file(path);
    String layout = new_str(31);

    for (size_t i = 0; i < s.len; ++i) {
        char c = s.str[i];
        if (c != ' ' && c != '\t' && c != '\n' && c != '\r') {
            push_str(&layout, c);
        }
    }

    String name = new_str(15);

    for (size_t i = strlen(path) - 1; i >= 0; --i) {
        char c = path[i];
        if (c == '/' || c == '\\') break;
        push_str(&name, c);
    }

    layout_t res = new_layout(layout.str, reverse_str(name).str);

    free_str(&layout);
    free_str(&name);

    return res;
}

void free_layout(layout_t* layout) {
    if (layout) {
        free(layout->name);
        layout = NULL;
    }
}

void print_layout(layout_t* layout) {
    printf("%s\n", layout->name);

    for (size_t i = 0; i < LAYOUT_CHARS; ++i) {
        printf("%c ", layout->matrix[i]);

        if ((i - 4) % 10 == 0)
            printf(" ");
        else if ((i + 1) % 10 == 0)
            printf("\n");
    }
}