#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

// https://www.geeksforgeeks.org/dynamically-allocate-2d-array-c/
// https://codereview.stackexchange.com/questions/159301/matrices-in-c-implementation

void create_m(matrix m)
{
    *m.pptr = malloc(m.r * m.c * sizeof(double));
}

void delete_m(matrix m)
{
    free(*m.pptr);
}

double get_e(matrix m, size_t i, size_t j)
{
    return *(*m.pptr + i * m.c + j);
}

double *get_r(matrix m, size_t i)
{
    return *m.pptr + i * m.c;
}

void set(matrix m, size_t i, size_t j, double value)
{
    *(*m.pptr + i * m.c + j) = value;
}

void transpose(matrix m)
{
    for (size_t i = 0; i < m.r; i++)
    {
        for (size_t j = 0; j < m.c; j++)
        {
            set(m, i, j, get_e(m, j, i));
        }
    }
}

void print_m(matrix m)
{
    for (size_t i = 0; i < m.r; i++)
    {
        for (size_t j = 0; j < m.c; j++)
        {
            printf("%e\t", get_e(m, i, j));
        }
        printf("\n");
    }
}

void fprint_m(matrix m, char *path)
{
    FILE *file = fopen(path, "w");

    for (size_t j = 0; j < m.c; j++)
    {
        fprintf(file, "y%ld,", j);
    }
    fprintf(file, "\n");

    for (size_t i = 0; i < m.r; i++)
    {
        for (size_t j = 0; j < m.c; j++)
        {
            fprintf(file, "%e,", get_e(m, i, j));
        }
        fprintf(file, "\n");
    }
    fclose(file);
    printf("Wrote to path: %s\n", path);
}