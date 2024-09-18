#include "headers/util.h"

#include <assert.h>
#include <stdio.h>

String read_file(char* path) {
    assert(path != NULL);

    FILE* fp = fopen(path, "r");

    if (fp) {
        String res = new_str(100);

        char c;
        while ((c = fgetc(fp)) != -1) {
            push_str(&res, c);
        }

        fclose(fp);

        return res;
    }

    return new_str(0);
}

void write_file(char* path, char* content) {
    assert(path != NULL);
    assert(content != NULL);

    FILE* fp = fopen(path, "w");

    fprintf(fp, "%s", content);

    fclose(fp);
}