#include <python3.10/Python.h>
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

void array_to_tuple(PyObject *tuple, double *array, size_t n)
{
    PyObject *tmp = NULL;

    if (!PyTuple_Check(tuple))
    {
        PyErr_SetString(PyExc_TypeError, "Argument 'tuple' in function 'array_to_tuple' is not a Python tuple");
        return NULL;
    }
    for (size_t i = 0; i < n; i++)
    {
        // * NEW REFERENCE *
        tmp = PyFloat_FromDouble(array[i]);
        if (tmp)
        {
            // * DECREF *
            PyTuple_SetItem(tuple, (Py_ssize_t)i, tmp);
            tmp = NULL;
        }
        else
        {
            PyErr_Format(PyExc_ValueError, "Argument 'array[%zu]' in function 'array_to_tuple' is not convertibel to a Python float", i);
            return NULL;
        }
    }
}

void v_add_v_factor_to_tuple(PyObject *tuple, vector src1, vector src2, double factor)
{
    PyObject *tmp = NULL;

    if (!PyTuple_Check(tuple))
    {
        PyErr_SetString(PyExc_TypeError, "Argument 'tuple' in function 'v_add_v_factor_to_tuple' is not a Python tuple");
        return NULL;
    }

    if (src1.n != src2.n)
    {
        PyErr_SetString(PyExc_ValueError, "Input vectors are not of the same size");
        return NULL;
    }

    for (size_t i = 0; i < src1.n; i++)
    {
        // * NEW REFERENCE *
        tmp = PyFloat_FromDouble(src1.ptr[i] + factor * src2.ptr[i]);

        // check if conversion was valid
        if (tmp)
        {
            // * DECREF *
            PyTuple_SetItem(tuple, (Py_ssize_t)i, tmp);
            tmp = NULL;
        }
        else
        {
            PyErr_Format(PyExc_ValueError, "Argument 'array[%zu]' in function 'v_add_v_factor_to_tuple' is not convertible to a Python float", i);
            return NULL;
        }
    }
}
void vector_to_tuple(PyObject *tuple, vector vec)
{
    PyObject *tmp = NULL;

    if (!PyTuple_Check(tuple))
    {
        PyErr_SetString(PyExc_TypeError, "Argument 'tuple' in function 'array_to_tuple' is not a Python tuple");
        return NULL;
    }
    for (size_t i = 0; i < vec.n; i++)
    {
        // * NEW REFERENCE *
        tmp = PyFloat_FromDouble(vec.ptr[i]);
        if (tmp)
        {
            // * DECREF *
            PyTuple_SetItem(tuple, (Py_ssize_t)i, tmp);
            tmp = NULL;
        }
        else
        {
            PyErr_Format(PyExc_ValueError, "Argument 'array[%zu]' in function 'array_to_tuple' is not convertibel to a Python float", i);
            return NULL;
        }
    }
}