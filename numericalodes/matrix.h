#ifndef MATRIX_H
#define MATRIX_H
#include <stddef.h>

typedef struct matrix
{
    double **pptr;
    size_t r;
    size_t c;
} matrix;

void create_m(matrix m);
void delete_m(matrix m);
double get_e(matrix m, size_t i, size_t j);
double *get_r(matrix m, size_t i);
void set(matrix m, size_t i, size_t j, double value);
void transpose(matrix m);
void print_m(matrix m);
void fprint_m(matrix m, char *path);

#endif