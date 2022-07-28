#define PY_SSIZE_T_CLEAN
#include <python3.10/Python.h>
#include <stdlib.h>
#include "RungeKutta4Py.h" // https://stackoverflow.com/a/33711076/16527499
#include "matrix.h"

#define UNUSED(x) (void)(x)

// https://stackoverflow.com/questions/53215786/when-writing-a-python-extension-in-c-how-does-one-pass-a-python-function-in-to

int malloc_save(void **ptr, size_t size, int size_of)
{
    void *tmp;

    tmp = malloc(size * size_of);

    if (tmp)
    {
        *ptr = tmp;
        return 0;
    }
    else
    {
        return -1;
    }
}

static PyObject *RK4(PyObject *self, PyObject *args)
{

    size_t n;                 // size of func array
    size_t size;              // amount of moments in calculated vectors
    double *t;                // pointer to vector elements of moments
    double *Y;                // pointer to matrix elements of function values at moments t
    double t0;                // first moment
    double tmax;              // last moment
    double h;                 // step-size between moments
    double *y0;               // inital array at position t=t0
    PyObject *PO_funcs;       // Python object of functions tuple
    PyObject *PO_y0;          // Python object of y0 tuple
    PyObject **PO_func_array; // Array of Python functions unpacked from Python tuple

    /*
    Parse arguments:
    d: double
    O: PyObject*
    */
    if (!PyArg_ParseTuple(args, "OddOd", &PO_funcs, &t0, &tmax, &PO_y0, &h))
        // TODO: write proper exception handling
        return PyUnicode_FromString("Error: Unpacking");

    // check for sequences in PO_funcs and PO_y0
    if (!PySequence_Check(PO_funcs) || !PySequence_Check(PO_y0))
        // TODO: write proper exception handling
        return PyUnicode_FromString("Error: Sequence checking");

    // check for equal length
    if (PySequence_Length(PO_funcs) == PySequence_Length(PO_y0))
        n = (size_t)PySequence_Length(PO_y0);
    else
        // TODO: write proper exception handling
        return PyUnicode_FromString("Error: Length comparison");

    // Create C array to unpack Python function tuple into
    if (malloc_save(&PO_func_array, n, sizeof(PyObject)))
        // TODO: write proper exception handling
        return PyUnicode_FromString("Error: malloc");

    // Unpack function tuple into array
    // https://stackoverflow.com/questions/25552315/python-tuple-to-c-array
    for (Py_ssize_t i = 0; i < (Py_ssize_t)n; i++)
        PO_func_array[i] = PySequence_GetItem(PO_funcs, i);

    //! TODO: unpack Py y0 into C array

    /*
    //! TODO: PyObject_CallObject / PyObject_CallFunction
    // convert python function to c function via a wrapper

    rv = PyObject_CallFunction(PO_func_array[0], "d", t0);
    if (rv)
        return rv;
    else
        return PyUnicode_FromString("Error: CallObject");
    */

    size = RK4vector(&t, &Y, PO_func_array, n, t0, tmax, y0, 0.1);

    // sanitize
    free(PO_func_array);
    UNUSED(y0);
    UNUSED(Y);
    UNUSED(t);

    return PyUnicode_FromString("Made it till the end");
};

/*
array of functions (methods) implemented in this module, terminated with a "null-function"
[0]: string of python function name
[1]: C function to call
[2]: macro whether function takes arguments (and of which type)
[3]: docstring
*/
static PyMethodDef numericalodes_methods[] = {
    {"RK4", RK4, METH_VARARGS, "Python interface for Runge-Kutta algorithms written as a C library function"},
    {NULL, NULL, 0, NULL}};

/*
bundle up the module
[0]: macro
[1]: module name
[2]: docstring
[3]: amount of memory needed to store the program state (negative value: no support for sub-interpreters)
[4]: array of fuctions
*/
static struct PyModuleDef numericalodes_module = {
    PyModuleDef_HEAD_INIT,
    "numericalodes",
    "Python interface for Runge-Kutta algorithms written as a C library function",
    -1,
    numericalodes_methods};

/*
Initialize the module with PyInit_MODULENAME
*/
PyMODINIT_FUNC PyInit_numericalodes(void)
{
    return PyModule_Create(&numericalodes_module);
}
