#include "RungeKutta4.h"
#include "broadcast.h"
#include "matrix.h"
#include <stdio.h>  // printf
#include <stdlib.h> // malloc, free
#include <stddef.h> // size_t
#include <stdarg.h> // va_list
#include <string.h> // memcpy
#include <math.h>   // ceil

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
    double *k1, *k2, *k3, *k4, *y_temp, *v = *t;
    matrix m = {y, size, n};

    // create size x n matrix
    create_m(m);

    // size-dim vector
    v = malloc(size * sizeof(double));

    // n-dim vectors
    k1 = malloc(n * sizeof(double));
    k2 = malloc(n * sizeof(double));
    k3 = malloc(n * sizeof(double));
    k4 = malloc(n * sizeof(double));
    y_temp = malloc(n * sizeof(double));

    // partially initialize t vector
    v[0] = t0;
    v[size - 1] = tmax;

    // partially initialize matrix
    memcpy(*m.pptr, y0, n * sizeof(double));

    // axis 0
    for (size_t i = 1; i < size; i++)
    {
        // axis 1
        for (size_t j = 0; j < n; j++)
        {
            // calculate k's
            k1[j] = func[j](v[i - 1], get_r(m, i - 1));
            broadcast_add(y_temp, get_r(m, i - 1), n, h * k1[j]);
            k2[j] = func[j](v[i - 1] + h / 2, y_temp);
            broadcast_add(y_temp, get_r(m, i - 1), n, h * k2[j]);
            k3[j] = func[j](v[i - 1] + h / 2, y_temp);
            broadcast_add(y_temp, get_r(m, i - 1), n, h * k3[j]);
            k4[j] = func[j](v[i - 1] + h, y_temp);

            // calculate next t and y
            v[i] = v[i - 1] + h;
            set(m, i, j, get_e(m, i - 1, j) + h / 6 * (k1[j] + 2 * (k2[j] + k3[j]) + k4[j]));
        }
    }

    free(k1);
    free(k2);
    free(k3);
    free(k4);
    free(y_temp);

    return size;
}