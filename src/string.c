#include "headers/string.h"

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "headers/util.h"

static void __grow_str(string_t* str);

string_t new_str(size_t item_capacity) {
    return (string_t){
        .capacity = item_capacity,
        .len = 0,
        .str = malloc(item_capacity * sizeof(char)),
    };
}

string_t str_from(char* str, size_t len) {
    char* ptr = malloc(len + 1);

    strncpy(ptr, str, len);
    ptr[len] = '\0';

    return (string_t){
        .capacity = len,
        .len = len,
        .str = ptr,
    };
}

void free_str(string_t* str) {
    if (str) {
        free(str->str);
    }

    str = NULL;
}

char get_str(string_t* str, size_t index) {
    if (index >= str->capacity) {
        return 0;
    } else {
        return str->str[index];
    }
}

char first_str(string_t* str) {
    if (str->len == 0) {
        return 0;
    } else {
        return str->str[0];
    }
}

char last_str(string_t* str) {
    if (str->len == 0) {
        return 0;
    } else {
        return str->str[str->len - 1];
    }
}

string_t clone_str(string_t* str) { return str_from(str->str, str->len); }

void push_str(string_t* str, char c) {
    assert(str != NULL);

    str->str[str->len] = c;
    str->len++;

    __grow_str(str);

    str->str[str->len] = '\0';
}

void push_str_str(string_t* str, char* s, size_t len) {
    assert(str != NULL);
    assert(s != NULL);

    if (str->capacity <= str->len + len) {
        size_t new_capacity = (str->len + len + (size_t)(0.2 * (double)str->capacity));

        str->str = realloc(str->str, new_capacity * sizeof(char));
        str->capacity = new_capacity;
    }

    memcpy(str->str + str->len, s, len * sizeof(char));

    str->len += len;
    str->str[str->len] = '\0';
}

void set_str(string_t* str, char c, size_t index) {
    assert(str != NULL);
    assert(index < str->len);

    str->str[index] = c;
}

char pop_str(string_t* str) {
    assert(str != NULL);

    if (str->len == 0) {
        return 0;
    } else {
        str->len--;

        return str->str[str->len];
    }
}

void clear_str(string_t* str) { str->len = 0; }

void insert_str(string_t* str, char c, size_t index) {
    assert(str != NULL);
    assert(index <= str->len);

    if (index == str->len) {
        push_str(str, c);
    }

    str->len++;

    __grow_str(str);

    char* src_ptr = str->str + str->len;
    char* dst_ptr = str->str + str->len + 1;
    size_t copy_len = (str->len - index) * sizeof(char);

    memmove(dst_ptr, src_ptr, copy_len);
    src_ptr[index] = c;
}

void remove_str(string_t* str, size_t index) {
    assert(str != NULL);

    if (index < str->len) {
        char* src_ptr = str->str + str->len + 1;
        char* dst_ptr = str->str + str->len;
        size_t copy_len = (str->len - index) * sizeof(char);

        memmove(dst_ptr, src_ptr, copy_len);
        str->len--;
    }
}

void resize_str(string_t* str, size_t len) {
    assert(str != NULL);

    if (len == str->len) {
        return;
    }

    size_t new_size = len * sizeof(char);

    str->str = realloc(str->str, new_size);
    str->capacity = MIN(str->capacity, len);
    str->len = MIN(str->len, len);
}

void reserve_str(string_t* str, size_t chars) {
    assert(str != NULL);

    if (chars <= str->len) {
        return;
    } else if (chars < str->len * 2) {
        chars = str->len * 2;
    }

    resize_str(str, chars);
}

string_t reverse_str(string_t str) {
    char* ptr = str.str;

    for (size_t i = 0; i < str.len / 2; ++i) {
        char help = ptr[i];
        ptr[i] = ptr[str.len - i - 1];
        ptr[str.len - i - 1] = help;
    }

    return str;
}

char* reverse_str_str(char* str, size_t len) {
    for (size_t i = 0; i < len / 2; ++i) {
        char help = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = help;
    }

    return str;
}

void print_str(string_t* str) {
    assert(str != NULL);

    printf("\"");

    if (str->len > 0) {
        for (size_t i = 0; i < str->len - 1; i++) {
            char c = get_str(str, i);
            printf("%c", c);
        }

        char c = last_str(str);
        printf("%c", c);
    }

    printf("\"");
}

void debug_str(string_t* str) {
    assert(str != NULL);

    printf("string_t {    \n    capacity: %zu\n    len: %zu\n    ptr: %p\n}\n", str->capacity,
           str->len, str->str);
}

static void __grow_str(string_t* str) {
    if (str->len >= str->capacity) {
        size_t new_capacity = (str->capacity + 1) * 2;

        char* tmp = realloc(str->str, new_capacity * sizeof(char));
        if (tmp) {
            str->str = tmp;
            str->capacity = new_capacity;
        } else {
            exit(-1);
        }
    }
}

str_iter_t iter_from_str(string_t* str) {
    assert(str != NULL);

    char* end = str->str + (str->len - 1) * sizeof(char);

    return (str_iter_t){
        .ptr = str->str,
        .end = end,
    };
}

char str_iter_peek(str_iter_t* iter) {
    assert(iter != NULL);

    if (iter->ptr + 1 <= iter->end) {
        return iter->ptr[1];
    } else {
        return 0;
    }
}

char str_iter_next(str_iter_t* iter) {
    assert(iter != NULL);

    if (iter->ptr <= iter->end) {
        char tmp = iter->ptr[0];

        iter->ptr++;

        return tmp;
    } else {
        return 0;
    }
}

void str_iter_skip(str_iter_t* iter, size_t skip) {
    assert(iter != NULL);

    if (iter->ptr + skip <= iter->end) {
        iter->ptr += skip;
    } else {
        iter->ptr = iter->end + 1;
    }
}

void str_iter_skip_whitespace(str_iter_t* iter) {
    char curr;

    while (isspace(curr = str_iter_next(iter))) {
        continue;
    }

    iter->ptr--;
}