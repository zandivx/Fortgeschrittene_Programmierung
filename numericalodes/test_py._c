#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "RungeKutta4.h"

static PyObject *RK4(PyObject *self, PyObject *args)
{
    double y0, t0, tmax, h;

    /* Parse arguments */
    if (!PyArg_ParseTuple(args, "ffff", &y0, &t0, &tmax, &h))
    {
        return NULL;
    }

    printf("y0=%f\nt0=%f\ntmax=%f\nh=%f\n", y0, t0, tmax, h);

    return PyLong_FromLong(1);
}

static PyMethodDef runge_kutta_algorithms[] = {
    {"RK4", RK4, METH_VARARGS, "Python interface for RK4 C library function"},
    {NULL, NULL, 0, NULL}};

static struct PyModuleDef runge_kutta_module = {
    PyModuleDef_HEAD_INIT,
    "RK4",
    "Python interface for the RK4 C library function",
    -1,
    runge_kutta_algorithms};

PyMODINIT_FUNC PyInit_(void)
{
    return PyModule_Create(&runge_kutta_module);
}
