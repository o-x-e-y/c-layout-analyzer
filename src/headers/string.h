#ifndef __OXEY_STRING_H
#define __OXEY_STRING_H

#include <stddef.h>

typedef struct {
    size_t len;
    size_t capacity;

    char* str;
} String;

typedef struct {
    char* ptr;
    char* end;
} StrIter;

String new_str(size_t capacity);
String str_from(char* str, size_t len);
void free_str(String* str);

char get_str(String* str, size_t index);
char first_str(String* str);
char last_str(String* str);

String clone_str(String* str);

void push_str(String* str, char c);
void push_str_str(String* str, char* s, size_t len);
char pop_str(String* str);

void set_str(String* str, char s, size_t index);

void insert_str(String* str, char s, size_t index);
void remove_str(String* str, size_t index);
void clear_str(String* str);

void reserve_str(String* str, size_t sents);
void resize_str(String* str, size_t len);

String reverse_str(String str);
char* reverse_str_str(char* str, size_t len);

void print_str(String* str);
void debug_str(String* str);

StrIter iter_from_str(String* str);
char str_iter_peek(StrIter* iter);
char str_iter_next(StrIter* iter);
void str_iter_skip(StrIter* iter, size_t skip);
void str_iter_skip_whitespace(StrIter* iter);

#endif