#include "RungeKutta4.h"
#include "vector.h"
#include "matrix.h"
#include <stdio.h>  // printf
#include <stdlib.h> // malloc, free
#include <stddef.h> // size_t
#include <string.h> // memcpy
#include <math.h>   // ceil

#define UNUSED(x) (void)(x)

// source: https://en.wikipedia.org/wiki/Runge–Kutta_methods

size_t RK4single(double **t, double **y, double (*func)(double, double), double t0, double tmax, double y0, double h)
{
    // declare variables
    const size_t size = ceil((tmax - t0) / h);
    double k1, k2, k3, k4;

    // allocate memory on the heap
    *t = malloc(size * sizeof(double));
    *y = malloc(size * sizeof(double));

    // partially initialize arrays
    (*t)[0] = t0;
    (*t)[size - 1] = tmax;
    (*y)[0] = y0;

    for (size_t n = 1; n < size; n++)
    {
        // calculate k's
        k1 = func((*t)[n - 1], (*y)[n - 1]);
        k2 = func((*t)[n - 1] + h / 2, (*y)[n - 1] + h * k1 / 2);
        k3 = func((*t)[n - 1] + h / 2, (*y)[n - 1] + h * k2 / 2);
        k4 = func((*t)[n - 1] + h, (*y)[n - 1] + h * k3);

        // calculate next t and y
        (*t)[n] = (*t)[n - 1] + h;
        (*y)[n] = (*y)[n - 1] + h / 6 * (k1 + 2 * (k2 + k3) + k4);
    }

    return size;
}

size_t RK4vector(double **t, double **y, double (**func)(double, double *), size_t n, double t0, double tmax, double *y0, double h)
{
    /*
    t: pointer to array
    y: pointer to 2-dim array (matrix)
    */

    // declare variables
    const size_t size = ceil((tmax - t0) / h);
    vector k1 = {NULL, n};
    vector k2 = {NULL, n};
    vector k3 = {NULL, n};
    vector k4 = {NULL, n};
    vector tmp = {NULL, n};
    vector row = {NULL, n};
    vector t_ = {NULL, size};
    matrix m = {NULL, size, n};

    // create size x n matrix
    create_m(&m);

    // create size-dim vector
    create_v(&t_);

    // create n-dim vectors
    create_v(&k1);
    create_v(&k2);
    create_v(&k3);
    create_v(&k4);
    create_v(&tmp);

    // partially initialize t vector
    t_.ptr[0] = t0;
    t_.ptr[size - 1] = tmax;

    // partially initialize matrix
    memcpy((void *)m.ptr, (void *)y0, n * sizeof(double));

    // axis 0
    for (size_t i = 1; i < size; i++)
    {
        // axis 1 loops
        row.ptr = get_r(m, i - 1);

        // k1
        for (size_t j = 0; j < n; j++)
        {
            k1.ptr[j] = func[j](t_.ptr[i - 1], get_r(m, i - 1));
        }

        // k2
        v_add_v_factor_tmp(&tmp, row, k1, h / 2);
        for (size_t j = 0; j < n; j++)
        {
            k2.ptr[j] = func[j](t_.ptr[i - 1] + h / 2, tmp.ptr);
        }

        // k3
        v_add_v_factor_tmp(&tmp, row, k2, h / 2);
        for (size_t j = 0; j < n; j++)
        {
            k3.ptr[j] = func[j](t_.ptr[i - 1] + h / 2, tmp.ptr);
        }

        // k4
        v_add_v_factor_tmp(&tmp, row, k3, h);
        for (size_t j = 0; j < n; j++)
        {
            k3.ptr[j] = func[j](t_.ptr[i - 1] + h, tmp.ptr);
        }

        // calculate next y
        for (size_t j = 0; j < n; j++)
        {
            set(m, i, j, get_e(m, i - 1, j) + h / 6 * (k1.ptr[j] + 2 * (k2.ptr[j] + k3.ptr[j]) + k4.ptr[j]));
        }

        // calculate next t
        t_.ptr[i] = t_.ptr[i - 1] + h;
    }

    delete_v(&k1);
    delete_v(&k2);
    delete_v(&k3);
    delete_v(&k4);
    delete_v(&tmp);
    // no free of row!!!

    // point input pointers to matrix m and array t_
    *t = t_.ptr;
    *y = m.ptr;
    UNUSED(t);
    UNUSED(y);

    return size;
}