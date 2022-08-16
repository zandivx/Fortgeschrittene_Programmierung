#include <python3.10/Python.h>
#include <stdio.h>  // printf
#include <stdlib.h> // malloc, free
#include <stddef.h> // size_t
#include <string.h> // memcpy
#include <math.h>   // ceil
#include "RungeKutta4Py.h"
#include "vector.h"
#include "matrix.h"

#define UNUSED(x) (void)(x)

// source: https://en.wikipedia.org/wiki/Runge–Kutta_methods

size_t RK4vector(double **t, double **y, PyObject **func, size_t n, double t0, double tmax, double *y0, double h)
{
    // declare variables
    const size_t size = ceil((tmax - t0) / h);
    double matrix_element = 0;
    double current_t = 0;
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
    memcpy(m.ptr, y0, n * sizeof(double));

    // axis 0
    for (size_t i = 1; i < size; i++)
    {
        // axis 1 loops
        row.ptr = get_r(m, i - 1);
        current_t = t_.ptr[i - 1];

        // k1
        for (size_t j = 0; j < n; j++)
        {
            // PyObject_CallFunction
            // rv = PyObject_CallFunction(PO_func_array[0], "d", t0);
            k1.ptr[j] = PyFloat_AsDouble(PyObject_CallFunction(func[j], "dd", current_t, row.ptr));
        }

        // k2
        v_add_v_factor_tmp(tmp, row, k1, h / 2);
        for (size_t j = 0; j < n; j++)
        {
            k2.ptr[j] = PyFloat_AsDouble(PyObject_CallFunction(func[j], "dd", current_t + h / 2, tmp.ptr));
        }

        // k3
        v_add_v_factor_tmp(tmp, row, k2, h / 2);
        for (size_t j = 0; j < n; j++)
        {
            k3.ptr[j] = PyFloat_AsDouble(PyObject_CallFunction(func[j], "dd", current_t + h / 2, tmp.ptr));
        }

        // k4
        v_add_v_factor_tmp(tmp, row, k3, h);
        for (size_t j = 0; j < n; j++)
        {
            k4.ptr[j] = PyFloat_AsDouble(PyObject_CallFunction(func[j], "dd", current_t + h, tmp.ptr));
        }

        // calculate next y
        for (size_t j = 0; j < n; j++)
        {
            matrix_element = get_e(m, i - 1, j) + h / 6 * (k1.ptr[j] + 2 * (k2.ptr[j] + k3.ptr[j]) + k4.ptr[j]);
            set(m, i, j, matrix_element);
        }

        // calculate next t
        t_.ptr[i] = current_t + h;
    }

    delete_v(&k1);
    delete_v(&k2);
    delete_v(&k3);
    delete_v(&k4);
    delete_v(&tmp);
    // no free of row!!!

    // point input pointers to array of matrix m and vector t_
    *t = t_.ptr;
    *y = m.ptr;
    UNUSED(t);
    UNUSED(y);

    return size;
}
