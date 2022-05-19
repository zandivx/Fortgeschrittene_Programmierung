#include "transpose.h"
#include <stddef.h>

/* m x n matrix*/
int transpose(double **matrix, size_t m, size_t n)
{
    size_t i, j;
    for (i = 0; i < m; i++)
    {
        for (j = 0; j < n; j++)
        {
            matrix[i][j] = matrix[j][i];
        }
    }
    return 0;
}