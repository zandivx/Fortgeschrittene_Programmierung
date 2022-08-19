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
    PyObject *PO_tmp = NULL;
    PyObject *PO_tuple = NULL;
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

    // create new tuple to store arguments for function calls of Python code in
    // * NEW REFERENCE *
    PO_tuple = PyTuple_New(n);
    /* also interesting:
    https://stackoverflow.com/questions/57101350/calling-python-function-with-arbitrary-number-not-known-at-compilation-time-of */

    // axis 0
    for (size_t i = 1; i < size; i++)
    {
        // axis 1 loops
        row.ptr = get_r(m, i - 1);
        current_t = t_.ptr[i - 1];

        // k1
        // convert array to call Python-functions with into a tuple
        array_to_tuple(PO_tuple, row.ptr, n);
        for (size_t j = 0; j < n; j++)
        {
            // Calling the functions using PyObject_CallFunction
            // (PyObject*)rv = PyObject_CallFunction(PyObject *function, char *format_string, ...(arguments));
            // * NEW REFERENCE *
            PO_tmp = PyObject_CallFunction(func[j], "dO", current_t, PO_tuple);

            // Check wheter calculated object is a number (float or int)
            if (!PyFloat_Check(PO_tmp) && !PyLong_Check(PO_tmp))
            {
                PyErr_Format(PyExc_TypeError, "Result of 'PyObject_Callfunction' in loop 'k1' at index j=%zu did not return a number!", j);
                return NULL;
            }
            else
            {
                k1.ptr[j] = PyFloat_AsDouble(PO_tmp);

                // * DECREF *
                Py_DECREF(PO_tmp);
                PO_tmp = NULL;
            }
        }

        // k2
        v_add_v_factor_tmp(tmp, row, k1, h / 2);
        array_to_tuple(PO_tuple, tmp.ptr, n);
        for (size_t j = 0; j < n; j++)
        {
            // * NEW REFERENCE *
            PO_tmp = PyObject_CallFunction(func[j], "dO", current_t + h / 2, PO_tuple);
            if (!PyFloat_Check(PO_tmp) && !PyLong_Check(PO_tmp))
            {
                PyErr_Format(PyExc_TypeError, "Result of 'PyObject_Callfunction' in loop 'k2' at index j=%zu did not return a number!", j);
                return NULL;
            }
            else
            {
                k2.ptr[j] = PyFloat_AsDouble(PO_tmp);

                // * DECREF *
                Py_DECREF(PO_tmp);
                PO_tmp = NULL;
            }
        }

        // k3
        v_add_v_factor_tmp(tmp, row, k2, h / 2);
        array_to_tuple(PO_tuple, tmp.ptr, n);
        for (size_t j = 0; j < n; j++)
        {
            // * NEW REFERENCE *
            PO_tmp = PyObject_CallFunction(func[j], "dO", current_t + h / 2, PO_tuple);
            if (!PyFloat_Check(PO_tmp) && !PyLong_Check(PO_tmp))
            {
                PyErr_Format(PyExc_TypeError, "Result of 'PyObject_Callfunction' in loop 'k3' at index j=%zu did not return a number!", j);
                return NULL;
            }
            else
            {
                k3.ptr[j] = PyFloat_AsDouble(PO_tmp);

                // * DECREF *
                Py_DECREF(PO_tmp);
                PO_tmp = NULL;
            }
        }

        // k4
        v_add_v_factor_tmp(tmp, row, k3, h);
        array_to_tuple(PO_tuple, tmp.ptr, n);
        for (size_t j = 0; j < n; j++)
        {
            // * NEW REFERENCE *
            PO_tmp = PyObject_CallFunction(func[j], "dO", current_t + h, PO_tuple);
            if (!PyFloat_Check(PO_tmp) && !PyLong_Check(PO_tmp))
            {
                PyErr_Format(PyExc_TypeError, "Result of 'PyObject_Callfunction' in loop 'k4' at index j=%zu did not return a number!", j);
                return NULL;
            }
            else
            {
                k4.ptr[j] = PyFloat_AsDouble(PO_tmp);

                // * DECREF *
                Py_DECREF(PO_tmp);
                PO_tmp = NULL;
            }
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

    // memory management
    delete_v(&k1);
    delete_v(&k2);
    delete_v(&k3);
    delete_v(&k4);
    delete_v(&tmp);
    // no free of row!!!

    // reference counting
    // * DECREF *
    Py_DECREF(PO_tuple);

    // point input pointers to array of matrix m and vector t_
    *t = t_.ptr;
    *y = m.ptr;

    return size;
}
