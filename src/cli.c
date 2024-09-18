#include "string.h"

#include "headers/analyzer.h"
#include "headers/cli.h"

void print_usage(const char *program_name);
void layout_info(analyzer_t* analyzer, char* path);

void __free_layout(void* ptr) {
    layout_t* layout = (layout_t*)ptr;

    free_layout(layout);
}

size_t longest_layout_name(Vec* layouts) {
    layout_t* layout;
    size_t len = 0;
    size_t longest = 0;

    Iter layout_iter = iter_from_vec(layouts);

    while ((layout = iter_next(&layout_iter))) {
        len = strlen(layout->name);
        if (len > longest)
            longest = len;
    }

    return longest;
}

int cli(int argc, char** argv) {
    if (argc == 1) {
        print_usage(argv[0]);
        return -1;
    }
    
    data_t d = load_data("./data/shai.data", "shai");
    analyzer_t analyzer = new_analyzer(&d);

    int opt;
    char* filename = NULL;
    char* layout_path = NULL;
    int number = -1;

    while ((opt = getopt(argc, argv, "hl:r")) != -1) {
        switch (opt) {
            case 'h':
                print_usage(argv[0]);
                return 0;
            case 'l':
                layout_path = optarg;
                break;
            case 'r':
                Vec layouts = load_layouts_from_path(&analyzer, "./layouts/");

                size_t longest_name = longest_layout_name(&layouts);

                Iter layout_iter = iter_from_vec(&layouts);

                layout_t* layout;
                size_t name_len = 0;

                while ((layout = iter_next(&layout_iter))) {
                    name_len = strlen(layout->name);

                    printf("%s  ", layout->name);
                    for (size_t i = name_len; i < longest_name; ++i) {
                        printf(" ");
                    }
                    
                    double sfbs_c = sfbs(&analyzer, layout) * 100.0;
                    double sfs_c = sfs(&analyzer, layout) * 100.0;

                    printf("sfbs: %f%%   sfs: %f%%\n", sfbs_c, sfs_c);
                }

                free_vec(&layouts, __free_layout);
                break;
            default:
                fprintf(stderr, "Unknown option: %c\n", optopt);
                print_usage(argv[0]);
                return 1;
        }
    }
    if (layout_path) {
        layout_info(&analyzer, layout_path);
    }

    free_analyzer(&analyzer);
}

void layout_info(analyzer_t* analyzer, char* path) {
    layout_t layout = load_layout(path);

    print_layout(&layout);
    printf("sfbs: %f%%\n", sfbs(analyzer, &layout) * 100.0);
    printf("sfs:  %f%%\n\n", sfs(analyzer, &layout) * 100.0);
}

void print_usage(const char *program_name) {
    fprintf(stderr, "Usage: %s [-h] [-l layout] [-r]\n", program_name);
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "  -h           Display this help message\n");
    fprintf(stderr, "  -l path      Shows information about a layout\n");
    fprintf(stderr, "  -r           Ranks all layouts in './layouts', sorted by sfb\n");
    // fprintf(stderr, "  -f filename       Specify an input file\n");
    // fprintf(stderr, "  -n number         Specify a number\n");
}