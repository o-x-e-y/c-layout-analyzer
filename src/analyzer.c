#include "headers/analyzer.h"

#include <assert.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

#include "headers/vec.h"

const static PosPair SFB_INDICES[48] = {
    (PosPair){.l = 0, .r = 10},  (PosPair){.l = 0, .r = 20},  (PosPair){.l = 10, .r = 20},
    (PosPair){.l = 1, .r = 11},  (PosPair){.l = 1, .r = 21},  (PosPair){.l = 11, .r = 21},
    (PosPair){.l = 2, .r = 12},  (PosPair){.l = 2, .r = 22},  (PosPair){.l = 12, .r = 22},
    (PosPair){.l = 7, .r = 17},  (PosPair){.l = 7, .r = 27},  (PosPair){.l = 17, .r = 27},
    (PosPair){.l = 8, .r = 18},  (PosPair){.l = 8, .r = 28},  (PosPair){.l = 18, .r = 28},
    (PosPair){.l = 9, .r = 19},  (PosPair){.l = 9, .r = 29},  (PosPair){.l = 19, .r = 29},

    (PosPair){.l = 3, .r = 4},   (PosPair){.l = 3, .r = 13},  (PosPair){.l = 3, .r = 14},
    (PosPair){.l = 3, .r = 23},  (PosPair){.l = 3, .r = 24},  (PosPair){.l = 4, .r = 13},
    (PosPair){.l = 4, .r = 14},  (PosPair){.l = 4, .r = 23},  (PosPair){.l = 4, .r = 24},
    (PosPair){.l = 13, .r = 14}, (PosPair){.l = 13, .r = 23}, (PosPair){.l = 13, .r = 24},
    (PosPair){.l = 14, .r = 23}, (PosPair){.l = 14, .r = 24}, (PosPair){.l = 23, .r = 24},

    (PosPair){.l = 5, .r = 6},   (PosPair){.l = 5, .r = 15},  (PosPair){.l = 5, .r = 16},
    (PosPair){.l = 5, .r = 25},  (PosPair){.l = 5, .r = 26},  (PosPair){.l = 6, .r = 15},
    (PosPair){.l = 6, .r = 16},  (PosPair){.l = 6, .r = 25},  (PosPair){.l = 6, .r = 26},
    (PosPair){.l = 15, .r = 16}, (PosPair){.l = 15, .r = 25}, (PosPair){.l = 15, .r = 26},
    (PosPair){.l = 16, .r = 25}, (PosPair){.l = 16, .r = 26}, (PosPair){.l = 25, .r = 26},
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

Vec load_layouts_from_path(analyzer_t* analyzer, char* path) {
    DIR* dir;
    struct dirent* ent;
    Vec layouts = new_vec(10, sizeof(layout_t));

    size_t path_len = strlen(path);

    if ((dir = opendir(path)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            int name_len = strlen(ent->d_name);
            if (name_len < 3 || strncmp(&ent->d_name[name_len - 3], ".md", 3) == 0) continue;

            String full_path = str_from(path, path_len);

            if (last_str(&full_path) != '\\' && last_str(&full_path) != '/') {
                push_str(&full_path, '/');
            }
            push_str_str(&full_path, ent->d_name, strlen(ent->d_name));

            // layout_t layout = load_layout(full_path.str);
            // layout.name[name_len - 3] = '\0';
            // layout.score = sfbs(analyzer, &layout);

            // push_vec(&layouts, &layout);
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
        PosPair p = SFB_INDICES[i];
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
        PosPair p = SFB_INDICES[i];
        char a = layout->matrix[p.l];
        char b = layout->matrix[p.r];

        total += get_skipgram(analyzer->data, a, b);
        total += get_skipgram(analyzer->data, b, a);
    }

    return total;
}