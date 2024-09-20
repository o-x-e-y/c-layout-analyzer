#ifndef __OXEY_ANALYZER_H
#define __OXEY_ANALYZER_H

#include <stddef.h>
#include <stdio.h>

#include "data.h"
#include "layout.h"
#include "util.h"
#include "vec.h"

typedef struct {
    layout_t* layouts;
    data_t* data;
} analyzer_t;

analyzer_t new_analyzer(data_t* data);
void free_analyzer(analyzer_t* analyzer);

vec_t load_layouts_from_path(analyzer_t* analyzer, char* path);

double sfbs(analyzer_t* analyzer, layout_t* layout);
double sfs(analyzer_t* analyzer, layout_t* layout);

// static void __generate_sfb_indices() {
//     size_t finger_indices[3] = {0, 10, 20};
//     size_t fingers[6] = {0, 1, 2, 7, 8, 9};

//     for (size_t f = 0; f < 6; f++) {
//         for (size_t i = 0; i < 3; i++) {
//             for (size_t j = i + 1; j < 3; j++) {
//                 size_t l = finger_indices[i] + fingers[f];
//                 size_t r = finger_indices[j] + fingers[f];

//                 printf("    (pos_pair_t) { .l = %zu, .r = %zu },\n", l, r);
//             }
//         }
//         printf("\n");
//     }

//     size_t index_indices[6] = {3, 4, 13, 14, 23, 24};

//     for (size_t f = 0; f <= 2; f += 2) {
//         for (size_t i = 0; i < 6; i++) {
//             for (size_t j = i + 1; j < 6; j++) {
//                 size_t l = index_indices[i] + f;
//                 size_t r = index_indices[j] + f;

//                 printf("    (pos_pair_t) { .l = %zu, .r = %zu },\n", l, r);
//             }
//         }
//         printf("\n");
//     }
// }

#endif
