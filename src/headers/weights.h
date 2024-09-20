#ifndef __OXEY_WEIGHTS_H
#define __OXEY_WEIGHTS_H

typedef struct {
    // len should be 30
    double* char_map;
    // len should be 30 * 30
    double* bigram_map;
} weights_t;

#endif