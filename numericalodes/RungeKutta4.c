#include "RungeKutta4.h"
#include <stdlib.h>
#include <stddef.h>
#include <math.h>

// source: https://en.wikipedia.org/wiki/Runge–Kutta_methods

size_t RK4single(double **t, double **y, double (*func)(double, double), double t0, double tmax, double y0, double h)
{
    // declare variables
    size_t size = ceil((tmax - t0) / h);
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