#ifndef UTIL_H
#define UTIL_H

#include "alloc_string.h"

#define MAX(v1, v2) v1 > v2 ? v1 : v2
#define MIN(v1, v2) v1 < v2 ? v1 : v2

string_t read_file(char* path);
void write_file(char* path, char* content);

#endif