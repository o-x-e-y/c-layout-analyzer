#ifndef __OXEY_STRING_H
#define __OXEY_STRING_H

#include <stddef.h>

typedef struct {
    size_t len;
    size_t capacity;

    char* str;
} string_t;

typedef struct {
    char* ptr;
    const char* end;
} str_iter_t;

string_t new_str(size_t capacity);
string_t str_from(char* str);
void free_str(string_t* str);

char get_str(string_t* str, size_t index);
char first_str(string_t* str);
char last_str(string_t* str);

string_t clone_str(string_t* str);

void push_str(string_t* str, char c);
void push_str_str(string_t* str, char* s, size_t len);
char pop_str(string_t* str);

void set_str(string_t* str, char s, size_t index);

void insert_str(string_t* str, char s, size_t index);
void remove_str(string_t* str, size_t index);
void clear_str(string_t* str);

void reserve_str(string_t* str, size_t sents);
void resize_str(string_t* str, size_t len);

string_t reverse_str(string_t str);
char* reverse_str_str(char* str, size_t len);

void print_str(string_t* str);
void debug_str(string_t* str);

str_iter_t iter_from_cstr(char* str);
str_iter_t iter_from_str(const string_t* str);
char str_iter_peek(str_iter_t* iter);
char str_iter_peek_n(str_iter_t* iter, size_t n);
char str_iter_next(str_iter_t* iter);
void str_iter_skip(str_iter_t* iter, size_t skip);
void str_iter_skip_whitespace(str_iter_t* iter);

#endif