#include "headers/analyzer.h"

#include <assert.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

#include "headers/vec.h"

const static pos_pair_t SFB_INDICES[48] = {
    (pos_pair_t){.l = 0, .r = 10},  (pos_pair_t){.l = 0, .r = 20},  (pos_pair_t){.l = 10, .r = 20},
    (pos_pair_t){.l = 1, .r = 11},  (pos_pair_t){.l = 1, .r = 21},  (pos_pair_t){.l = 11, .r = 21},
    (pos_pair_t){.l = 2, .r = 12},  (pos_pair_t){.l = 2, .r = 22},  (pos_pair_t){.l = 12, .r = 22},
    (pos_pair_t){.l = 7, .r = 17},  (pos_pair_t){.l = 7, .r = 27},  (pos_pair_t){.l = 17, .r = 27},
    (pos_pair_t){.l = 8, .r = 18},  (pos_pair_t){.l = 8, .r = 28},  (pos_pair_t){.l = 18, .r = 28},
    (pos_pair_t){.l = 9, .r = 19},  (pos_pair_t){.l = 9, .r = 29},  (pos_pair_t){.l = 19, .r = 29},

    (pos_pair_t){.l = 3, .r = 4},   (pos_pair_t){.l = 3, .r = 13},  (pos_pair_t){.l = 3, .r = 14},
    (pos_pair_t){.l = 3, .r = 23},  (pos_pair_t){.l = 3, .r = 24},  (pos_pair_t){.l = 4, .r = 13},
    (pos_pair_t){.l = 4, .r = 14},  (pos_pair_t){.l = 4, .r = 23},  (pos_pair_t){.l = 4, .r = 24},
    (pos_pair_t){.l = 13, .r = 14}, (pos_pair_t){.l = 13, .r = 23}, (pos_pair_t){.l = 13, .r = 24},
    (pos_pair_t){.l = 14, .r = 23}, (pos_pair_t){.l = 14, .r = 24}, (pos_pair_t){.l = 23, .r = 24},

    (pos_pair_t){.l = 5, .r = 6},   (pos_pair_t){.l = 5, .r = 15},  (pos_pair_t){.l = 5, .r = 16},
    (pos_pair_t){.l = 5, .r = 25},  (pos_pair_t){.l = 5, .r = 26},  (pos_pair_t){.l = 6, .r = 15},
    (pos_pair_t){.l = 6, .r = 16},  (pos_pair_t){.l = 6, .r = 25},  (pos_pair_t){.l = 6, .r = 26},
    (pos_pair_t){.l = 15, .r = 16}, (pos_pair_t){.l = 15, .r = 25}, (pos_pair_t){.l = 15, .r = 26},
    (pos_pair_t){.l = 16, .r = 25}, (pos_pair_t){.l = 16, .r = 26}, (pos_pair_t){.l = 25, .r = 26},
};

analyzer_t new_analyzer(data_t* data) {
    data_t* data_ptr = malloc(sizeof(data_t));
    data_ptr = memcpy(data_ptr, data, sizeof(data_t));

    return (analyzer_t){.data = data_ptr, .layouts = NULL};
}

void free_analyzer(analyzer_t* analyzer) {
    if (analyzer) {
        if (analyzer->layouts) free(analyzer->layouts);

        if (analyzer->data) {
            free_data(analyzer->data);
            free(analyzer->data);
        }
    }
}

int __cmp_layouts(const void* layout1, const void* layout2) {
    return (int)((((layout_t*)layout2)->score - ((layout_t*)layout1)->score) * 10000000.0);
}

vec_t load_layouts_from_path(analyzer_t* analyzer, char* path) {
    DIR* dir;
    struct dirent* ent;
    vec_t layouts = new_vec(10, sizeof(layout_t));

    size_t path_len = strlen(path);

    if ((dir = opendir(path)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            int name_len = strlen(ent->d_name);
            if (name_len < 3 || strncmp(&ent->d_name[name_len - 3], ".md", 3) == 0) continue;

            string_t full_path = str_from(path);

            if (last_str(&full_path) != '\\' && last_str(&full_path) != '/') {
                push_str(&full_path, '/');
            }
            push_str_str(&full_path, ent->d_name, strlen(ent->d_name));

            layout_t layout = load_layout(full_path.str);
            layout.name[name_len - 3] = '\0';
            layout.score = sfbs(analyzer, &layout);

            push_vec(&layouts, &layout);
        }

        closedir(dir);
    } else {
        // could not open directory
        perror("Couldn't open directory");
        // return EXIT_FAILURE;
    }

    qsort(layouts.ptr, layouts.len, sizeof(layout_t), __cmp_layouts);

    return layouts;
}

double sfbs(analyzer_t* analyzer, layout_t* layout) {
    double total = 0;

    for (size_t i = 0; i < 48; ++i) {
        pos_pair_t p = SFB_INDICES[i];
        char a = layout->matrix[p.l];
        char b = layout->matrix[p.r];

        total += get_bigram(analyzer->data, a, b);
        total += get_bigram(analyzer->data, b, a);
    }

    return total;
}

double sfs(analyzer_t* analyzer, layout_t* layout) {
    double total = 0;

    for (size_t i = 0; i < 48; ++i) {
        pos_pair_t p = SFB_INDICES[i];
        char a = layout->matrix[p.l];
        char b = layout->matrix[p.r];

        total += get_skipgram(analyzer->data, a, b);
        total += get_skipgram(analyzer->data, b, a);
    }

    return total;
}