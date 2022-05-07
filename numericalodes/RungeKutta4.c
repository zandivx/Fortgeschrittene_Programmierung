#include "RungeKutta4.h"
#include <stdlib.h>
#include <math.h>

/* source: https://en.wikipedia.org/wiki/Runge–Kutta_methods */

double **RK4single(double (*func)(double, double), double t0, double tmax, double y0, double h)
{
    int size = ceil((tmax - t0) / h);
    double k1, k2, k3, k4;
    static double *t, *y, *ret[3];

    return ret;
}