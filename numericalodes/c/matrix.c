#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "matrix.h"

// https://www.geeksforgeeks.org/dynamically-allocate-2d-array-c/
// https://codereview.stackexchange.com/questions/159301/matrices-in-c-implementation

void create_m(matrix *m)
{
    void *tmp = NULL;
    tmp = malloc(m->r * m->c * sizeof(double));
    if (tmp)
    {
        m->ptr = (double *)tmp;
    }
}

void delete_m(matrix m)
{
    free(m.ptr);
}

double get_e(matrix m, size_t i, size_t j)
{
    return *(m.ptr + i * m.c + j);
}

double *get_r(matrix m, size_t i)
{
    return m.ptr + i * m.c;
}

void set(matrix m, size_t i, size_t j, double value)
{
    *(m.ptr + i * m.c + j) = value;
}

void print_m(matrix m)
{
    for (size_t i = 0; i < m.r; i++)
    {
        for (size_t j = 0; j < m.c - 1; j++)
        {
            printf("%e\t", get_e(m, i, j));
        }

        // last entry in row: newline instead of tab
        printf("%e\n", get_e(m, i, m.c - 1));
    }
}

void fprint_m(matrix m, char *path)
{
    FILE *file = fopen(path, "w");

    // write header as "y0", "y1", ...
    for (size_t j = 0; j < m.c - 1; j++)
    {
        fprintf(file, "y%ld,", j);
    }

    // last header entry has to end with a newline instead of a comma
    fprintf(file, "y%ld\n", m.c - 1);

    for (size_t i = 0; i < m.r; i++)
    {
        for (size_t j = 0; j < m.c - 1; j++)
        {
            fprintf(file, "%e,", get_e(m, i, j));
        }

        // same thing: last entry with newline
        fprintf(file, "%e\n", get_e(m, i, m.c - 1));
    }
    fclose(file);
    printf("Wrote to path: %s\n", path);
}

void transpose(matrix *m)
{
    // matrix m = {m.ptr, m.r, m.c}
    matrix tmp = {NULL, m->c, m->r};
    create_m(&tmp);

    for (size_t i = 0; i < m->r; i++)
    {
        for (size_t j = 0; j < m->c; j++)
        {
            set(tmp, j, i, get_e(*m, i, j));
        }
    }

    delete_m(*m);
    *m = tmp;
}
