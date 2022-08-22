#include <python3.10/Python.h>
// #include <stdio.h>  // printf
#include <stdlib.h> // malloc, free
#include <stddef.h> // size_t
#include <string.h> // memcpy
#include <math.h>   // ceil
#include "vector.h"
#include "matrix.h"
#include "RungeKutta4Py.h"

/*
    source for algorithm:
    https://en.wikipedia.org/wiki/Runge–Kutta_methods
*/

size_t RK4vector(double **t, double **y, PyObject **func, size_t n, double t0, double tmax, double *y0, double h)
{
    // declare variables
    const size_t size = ceil((tmax - t0) / h);
    double matrix_element = 0;
    double current_t = 0;
    PyObject *PO_tmp = NULL;
    PyObject *PO_tuple = NULL;
    vector k1 = {NULL, n};
    vector k2 = {NULL, n};
    vector k3 = {NULL, n};
    vector k4 = {NULL, n};
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

    // partially initialize t vector
    t_.ptr[0] = t0;
    t_.ptr[size - 1] = tmax;

    // partially initialize matrix
    memcpy(m.ptr, y0, n * sizeof(double));

    // create a new tuple to store arguments for function calls of Python code in
    // * NEW REFERENCE *
    PO_tuple = PyTuple_New(n);

    // axis 0 loop
    for (size_t i = 1; i < size; i++)
    {
        // axis 1 loops
        row.ptr = get_r(m, i - 1);
        current_t = t_.ptr[i - 1];

        // k1
        // convert array to call Python-functions with into a tuple
        // array_to_tuple(PO_tuple, row.ptr, n);
        vector_to_tuple(PO_tuple, row);
        kn_loop(k1, func, current_t, PO_tuple);

        // k2
        v_add_v_factor_to_tuple(PO_tuple, row, k1, h / 2);
        kn_loop(k2, func, current_t + h / 2, PO_tuple);

        // k3
        v_add_v_factor_to_tuple(PO_tuple, row, k2, h / 2);
        kn_loop(k3, func, current_t + h / 2, PO_tuple);

        // k4
        v_add_v_factor_to_tuple(PO_tuple, row, k3, h);
        kn_loop(k4, func, current_t + h, PO_tuple);

        // calculate next y
        for (size_t j = 0; j < n; j++)
        {
            matrix_element = get_e(m, i - 1, j) + h / 6 * (k1.ptr[j] + 2 * (k2.ptr[j] + k3.ptr[j]) + k4.ptr[j]);
            set(m, i, j, matrix_element);
        }

        // calculate next t
        t_.ptr[i] = current_t + h;
    }

    // memory management
    delete_v(&k1);
    delete_v(&k2);
    delete_v(&k3);
    delete_v(&k4);

    // reference counting
    // * DECREF *
    Py_DECREF(PO_tuple);

    // point input pointers to array of matrix m and vector t_
    *t = t_.ptr;
    *y = m.ptr;

    return size;
}

void kn_loop(vector k, PyObject **func_array, double t, PyObject *tuple)
{
    PyObject *PO_tmp = NULL; // temporary PyObject

    for (size_t j = 0; j < k.n; j++)
    {
        /*
            Calling the functions using PyObject_CallFunction
            signature: (PyObject*)rv = PyObject_CallFunction((PyObject *)function, (char *)format_string, ...(arguments));
        */

        // * NEW REFERENCE *
        PO_tmp = PyObject_CallFunction(func_array[j], "dO", t, tuple);

        // Check whether calculated object is a number (float or int)
        if (!PyFloat_Check(PO_tmp) && !PyLong_Check(PO_tmp))
        {
            PyErr_SetString(PyExc_TypeError, "Result of 'PyObject_Callfunction' in function 'kn_loop' did not return a number!");
            return NULL;
        }
        else
        {
            k.ptr[j] = PyFloat_AsDouble(PO_tmp);

            // * DECREF *
            Py_DECREF(PO_tmp);
            PO_tmp = NULL;
        }
    }
}
