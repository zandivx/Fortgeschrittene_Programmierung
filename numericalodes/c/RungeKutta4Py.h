#ifndef RUNGE_KUTTA_4_PY_H
#define RUNGE_KUTTA_4_PY_H
#include <python3.10/Python.h>
#include <stddef.h>
#include "vector.h"

size_t RK4vector(double **t, double **y, PyObject **func, size_t n, double t0, double tmax, double *y0, double h);
void kn_loop(vector k, PyObject **func_array, double t, PyObject *tuple);

#endif
