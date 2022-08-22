#ifndef C_H
#define C_H
#include <python3.10/Python.h>
#include <stddef.h>

int malloc_fail(void **ptr, size_t size, unsigned int size_of);
int unpack_fail(PyObject **dest, PyObject *sequence, Py_ssize_t loc);
static PyObject *RK4c(PyObject *self, PyObject *args);

#endif