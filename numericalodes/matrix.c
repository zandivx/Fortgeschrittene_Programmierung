#include "matrix.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

// https://www.geeksforgeeks.org/dynamically-allocate-2d-array-c/
void create_m(matrix m)
{
    m.ptr = malloc(m.r * sizeof(double *));
    m.ptr[0] = malloc(m.r * m.c * sizeof(double));

    for (size_t i = 0; i < m.c; i++)
    {
        m.ptr[i] = m.ptr[0] + m.c * i;
    }
}

void delete_m(matrix m)
{
    free(m.ptr);
}

// double get_m(matrix m, size_t i, size_t j)
// {
//     return *(m.ptr + i * m.r + j * m.c);
// }

// int set_m(matrix m, size_t i, size_t j, double value)
// {
//     m.ptr[i * m.r + j * m.c] = value;
//     return 0;
// }

void print_m(matrix m)
{
    for (size_t i = 0; i < m.r; i++)
    {
        for (size_t j = 0; j < m.c; j++)
        {
            printf("%f\t", m.ptr[i][j]);
        }
        printf("\n");
    }
}

void transpose_m(matrix m)
{
    size_t i, j;
    for (i = 0; i < m.r; i++)
    {
        for (j = 0; j < m.c; j++)
        {
            m.ptr[i][j] = m.ptr[j][i];
        }
    }
}