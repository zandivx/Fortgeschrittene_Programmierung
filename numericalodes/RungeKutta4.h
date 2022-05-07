#ifndef RUNGE_KUTTA_4_H
#define RUNGE_KUTTA_4_H
#include <stddef.h>

size_t RK4single(double **t, double **y, double (*func)(double, double), double t0, double tmax, double y0, double h);
size_t RK4vector(double **t, double **y, double (**func)(double, double), int n, double t0, double tmax, double y0, double h);

#endif