#include "vector.h"
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

void create_v(vector *v)
{
    void *tmp = NULL;
    tmp = malloc(v->n * sizeof(double));
    if (tmp)
    {
        v->ptr = (double *)tmp;
    }
}

void delete_v(vector *v)
{
    free(v->ptr);
}

void print_v(vector v)
{
    for (size_t i = 0; i < v.n - 1; i++)
    {
        printf("% 05.8f, ", v.ptr[i]);
    }
    printf("% 05.8f\n", v.ptr[v.n - 1]);
}

void v_add_v_factor_tmp(vector dest, vector src1, vector src2, double factor)
{
    if (src1.n != src2.n)
    {
        printf("Error in v_add_v_factor_tmp\n");
        return;
    }
    else
    {
        for (size_t i = 0; i < src1.n; i++)
        {
            dest.ptr[i] = src1.ptr[i] + factor * src2.ptr[i];
        }
    }
}