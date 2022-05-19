#ifndef MATRIX_H
#define MATRIX_H
#include <stddef.h>

typedef double *vectorptr;
typedef vectorptr *matrixptr;

typedef struct matrix
{
    matrixptr ptr;
    size_t r;
    size_t c;
} matrix;

void create_m(matrix m);
void delete_m(matrix m);
// double get_m(matrix m, size_t i, size_t j);
// int set_m(matrix m, size_t i, size_t j, double value);
void print_m(matrix m);
void transpose_m(matrix m);

#endif