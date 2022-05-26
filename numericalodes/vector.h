#ifndef VECTOR_H
#define VECTOR_H
#include <stddef.h>

typedef struct vector
{
    double *ptr; // address of array
    size_t n;    // dimension
} vector;

void create_v(vector *v);
void delete_v(vector *v);
void print_v(vector v);
void v_add_v_factor_tmp(vector dest, vector src1, vector src2, double factor);

#endif