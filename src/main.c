#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "headers/analyzer.h"
#include "headers/data.h"
#include "headers/vec.h"
#include "headers/cli.h"

int main(int argc, char** argv) {
    cli(argc, argv);
    
    // char* path = "./corpora/monkeyracer.txt";

    // String mr = read_file(path);

    // printf("len of file: %zu bytes\n", mr.len);

    // data_t d_generate = generate_data(mr.str,
    //                          "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%%^"
    //                          "&*()',.;\"<>:[]{}/=?+-\\_|`~",
    //                          "shai");

    // save_data(&d_generate, "./data");

    // data_t d = load_data("./data/shai.data", "shai");

    // analyzer_t analyzer = new_analyzer(&d);

    // Vec layouts = load_layouts_from_path(&analyzer, "./layouts/");
    
    // Iter layout_iter = iter_from_vec(&layouts);

    // layout_t* layout;

    // while ((layout = iter_next(&layout_iter))) {
    //     print_layout(layout);
    //     printf("sfbs: %f%%\n", sfbs(&analyzer, layout) * 100.0);
    //     printf("sfs:  %f%%\n\n", sfs(&analyzer, layout) * 100.0);
    // }

    return 0;
}