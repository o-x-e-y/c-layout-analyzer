#ifndef __OXEY_DATA_H
#define __OXEY_DATA_H

#include <stdbool.h>
#include <stddef.h>

#include "string.h"

typedef struct {
    char* language;
    // len should be 128
    double* chars;
    // len should be 128 * 128
    double* bigrams;
    // len should be 128 * 128
    double* skipgrams;
} data_t;

data_t generate_data(char* str, char* include_chars, char* language);
data_t load_data(char* path, char* language);
void save_data(data_t* data, char* path);
void free_data(data_t* data);

double get_char(data_t* data, char c);
double get_bigram(data_t* data, char a, char b);
double get_skipgram(data_t* data, char a, char b);

#endif