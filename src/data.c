#include "headers/data.h"

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "headers/alloc_string.h"
#include "headers/util.h"

data_t generate_data(char* str, char* include_chars, char* language) {
    double* chars = calloc(128, sizeof(double));
    double* bigrams = calloc(128 * 128, sizeof(double));
    double* skipgrams = calloc(128 * 128, sizeof(double));

    bool include[255] = {0};

    for (size_t c = *include_chars; c != '\0'; c = *++include_chars) {
        include[c] = true;
    }

    size_t total_chars = 0;
    size_t total_bigrams = 0;
    size_t total_skipgrams = 0;
    size_t c1 = tolower(str[0]);

    if (c1) {
        chars[c1]++;
        str++;
        total_chars++;

        size_t c2 = tolower(str[1]);

        if (c2) {
            chars[c2]++;
            bigrams[c1 * 128 + c2]++;
            str++;
            total_chars++;

            for (size_t c3 = tolower(*str); c3 != '\0'; c3 = tolower(*++str)) {
                if (include[c3]) {
                    chars[c3]++;
                    total_chars++;

                    if (include[c2]) {
                        bigrams[c2 * 128 + c3]++;
                        total_bigrams++;
                    } else if (include[c1]) {
                        skipgrams[c1 * 128 + c3]++;
                        total_skipgrams++;
                    }
                }

                c1 = c2;
                c2 = c3;
            }
        }
    }

    for (size_t i = 0; i < 128; ++i) {
        chars[i] /= (double)total_chars;
    }

    for (size_t i = 0; i < 128 * 128; ++i) {
        bigrams[i] /= (double)total_bigrams;
        skipgrams[i] /= (double)total_skipgrams;
    }

    size_t lang_len = strlen(language);
    char* lang_alloc = malloc((lang_len + 1) * sizeof(char));
    strncpy(lang_alloc, language, lang_len + 1);

    return (data_t){
        .language = lang_alloc, .chars = chars, .bigrams = bigrams, .skipgrams = skipgrams};
}

data_t load_data(char* path, char* language) {
    assert(path != NULL);
    assert(language != NULL);

    double* chars = calloc(128, sizeof(double));
    double* bigrams = calloc(128 * 128, sizeof(double));
    double* skipgrams = calloc(128 * 128, sizeof(double));

    string_t content = read_file(path);

    str_iter_t iter = iter_from_str(&content);
    char curr;

    while (str_iter_peek(&iter)) {
        str_iter_skip_whitespace(&iter);

        size_t c = str_iter_next(&iter);

        str_iter_skip_whitespace(&iter);

        double freq = atof(iter.ptr);

        while ((curr = str_iter_next(&iter)) == '.' || isdigit(curr)) {
            continue;
        }

        chars[c] = freq;

        if (strncmp(iter.ptr - 1, "\n\n\n", 3) == 0) break;
    }

    while (str_iter_peek(&iter)) {
        str_iter_skip_whitespace(&iter);

        char a = str_iter_next(&iter);
        char b = str_iter_next(&iter);

        str_iter_skip_whitespace(&iter);

        double freq = atof(iter.ptr);

        while ((curr = str_iter_next(&iter)) == '.' || isdigit(curr)) {
            continue;
        }

        bigrams[a * 128 + b] = freq;

        if (strncmp(iter.ptr - 1, "\n\n\n", 3) == 0) break;
    }

    while (str_iter_peek(&iter)) {
        str_iter_skip_whitespace(&iter);

        char a = str_iter_next(&iter);
        char b = str_iter_next(&iter);

        str_iter_skip_whitespace(&iter);

        double freq = atof(iter.ptr);

        while ((curr = str_iter_next(&iter)) == '.' || isdigit(curr)) {
            continue;
        }

        skipgrams[a * 128 + b] = freq;
    }

    size_t lang_len = strlen(language);
    char* lang_alloc = malloc((lang_len + 1) * sizeof(char));
    strncpy(lang_alloc, language, lang_len + 1);

    free_str(&content);

    return (data_t){
        .language = lang_alloc, .chars = chars, .bigrams = bigrams, .skipgrams = skipgrams};
}

void save_data(data_t* data, char* folder) {
    assert(data != NULL);

    string_t path;
    if (folder == NULL) {
        path = str_from("./", strlen("./"));
    } else {
        path = str_from(folder, strlen(folder));
    }

    if (last_str(&path) != '\\' && last_str(&path) != '/') {
        push_str(&path, '/');
    }

    push_str_str(&path, data->language, strlen(data->language));
    push_str_str(&path, ".data", 5);

    FILE* fp = fopen(path.str, "wb+");

    if (fp) {
        for (size_t i = 0; i < 128; i++) {
            char c = i;
            double f = data->chars[i];
            if (f > 0.0) {
                fprintf(fp, "%c %.14lf\n", c, f);
            }
        }

        fprintf(fp, "\n\n");

        for (size_t i = 0; i < 128; i++) {
            char a = i;
            for (size_t j = 0; j < 128; ++j) {
                char b = j;
                double f = data->bigrams[i * 128 + j];
                if (f > 0.0) {
                    fprintf(fp, "%c%c %.14lf\n", a, b, f);
                }
            }
        }

        fprintf(fp, "\n\n");

        for (size_t i = 0; i < 128; i++) {
            char a = i;
            for (size_t j = 0; j < 128; ++j) {
                char b = j;
                double f = data->skipgrams[i * 128 + j];
                if (f > 0.0) {
                    fprintf(fp, "%c%c %.14lf\n", a, b, f);
                }
            }
        }

        fclose(fp);

        printf("saved data at: %s\n", path.str);
    } else {
        printf("Couldn't save data due to a faulty file pointer");
    }

    free_str(&path);
}

void free_data(data_t* data) {
    if (data) {
        if (data->chars) {
            free(data->chars);
        }

        if (data->bigrams) {
            free(data->bigrams);
        }

        if (data->skipgrams) {
            free(data->skipgrams);
        }

        if (data->language) {
            free(data->language);
        }

        data = NULL;
    }
}

double get_char(data_t* data, char c) { return data->chars[(size_t)c]; }

double get_bigram(data_t* data, char a, char b) {
    return data->bigrams[(size_t)a * 128 + (size_t)b];
}

double get_skipgram(data_t* data, char a, char b) {
    return data->skipgrams[(size_t)a * 128 + (size_t)b];
}