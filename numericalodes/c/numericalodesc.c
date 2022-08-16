#define PY_SSIZE_T_CLEAN
#include <python3.10/Python.h>
#include <stdlib.h>
#include "RungeKutta4Py.h" // https://stackoverflow.com/a/33711076/16527499
#include "matrix.h"

#define UNUSED(x) (void)(x)

/*
important concept:
https://stackoverflow.com/questions/53215786/when-writing-a-python-extension-in-c-how-does-one-pass-a-python-function-in-to
*/

int malloc_fail(void **ptr, size_t size, unsigned int size_of)
{
    void *tmp = NULL;

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

int unpack_fail(PyObject **dest, PyObject *sequence, Py_ssize_t loc)
{
    PyObject *tmp = NULL;

    tmp = PySequence_GetItem(sequence, loc);

    if (tmp)
    {
        *dest = tmp;
        return 0;
    }
    else
    {
        return -1;
    }
}

// Functions to call in Python ----------------------------------------------------------------------------

static PyObject *RK4(PyObject *self, PyObject *args)
{
    size_t n = 0;                    // size of func array
    size_t size = 0;                 // amount of moments in calculated vectors
    double *t = NULL;                // pointer to vector elements of moments
    double *y = NULL;                // pointer to matrix elements of function values at moments t
    double t0 = 0;                   // first moment
    double tmax = 0;                 // last moment
    double h = 0;                    // step-size between moments
    double *y0 = NULL;               // inital array at position t=t0
    PyObject *funcs = NULL;          // Python object of functions tuple
    PyObject *PO_y0 = NULL;          // Python object of y0 tuple
    PyObject **array_PO_func = NULL; // Array of Python functions unpacked from Python tuple
    PyObject *tuple_t = NULL;        // tuple of moments
    PyObject *tuple_y = NULL;        // tuple of calculated values
    PyObject *tuple_rv = NULL;       // tuple of return values
    PyObject *PO_tmp = NULL;         // temporary Python Object

    /*
    Parse arguments:
    d: double
    O: PyObject*
    */
    if (!PyArg_ParseTuple(args, "OddOd", &funcs, &t0, &tmax, &PO_y0, &h))
    {
        PyErr_SetString(PyExc_TypeError, "Given arguments do not match types: [PyObject, double, double, PyObject, double]");
        return NULL;
    }

    // check for sequences in funcs and PO_y0
    if (!PySequence_Check(funcs) || !PySequence_Check(PO_y0))
    {
        PyErr_SetString(PyExc_TypeError, "'funcs' and 'y0' are not of type 'Sequence'");
        return NULL;
    }

    // check for equal length
    if (PySequence_Length(funcs) == PySequence_Length(PO_y0))
    {
        n = (size_t)PySequence_Length(PO_y0);
        if (n == 0)
        {
            PyErr_SetString(PyExc_ValueError, "Sequences 'funcs' and 'y0' must not be of length 0");
            return NULL;
        }
    }
    else
    {
        PyErr_SetString(PyExc_ValueError, "Sequences 'funcs' and 'y0' are not of same length");
        return NULL;
    }

    // Create C array to unpack Python function tuple into
    if (malloc_fail((void **)&array_PO_func, n, sizeof(PyObject *)))
    {
        PyErr_SetString(PyExc_SystemError, "malloc array_PO_func");
        return NULL;
    }

    // Unpack function tuple into array
    // https://stackoverflow.com/questions/25552315/python-tuple-to-c-array
    for (Py_ssize_t i = 0; i < (Py_ssize_t)n; i++)
    {
        if (unpack_fail(&PO_tmp, funcs, i))
        {
            PyErr_SetString(PyExc_LookupError, "Unpacking of funcs to array_PO_func");
            return NULL;
        }
        else
        {
            // Check wheter unpacked elements are Callables
            if (PyCallable_Check(PO_tmp))
            {
                array_PO_func[i] = PO_tmp;
                PO_tmp = NULL;
            }
            else
            {
                PyErr_SetString(PyExc_TypeError, "Elements of sequence 'funcs' are not callables");
                return NULL;
            }
        }
    }

    // Create C array to unpack Python y0 tuple into
    if (malloc_fail((void **)&y0, n, sizeof(PyObject *)))
    {
        PyErr_SetString(PyExc_SystemError, "malloc y0");
        return NULL;
    }

    // Unpack y0 tuple into array
    for (Py_ssize_t i = 0; i < (Py_ssize_t)n; i++)
    {
        if (unpack_fail(&PO_tmp, PO_y0, i))
        {
            PyErr_SetString(PyExc_LookupError, "Unpacking of PO_y0 to y0");
            return NULL;
        }
        else
        {
            // Check wheter unpacked elements are numbers (float or int)
            if (PyFloat_Check(PO_tmp) || PyLong_Check(PO_tmp))
            {
                y0[i] = PyFloat_AsDouble(PO_tmp);
                PO_tmp = NULL;
            }
            else
            {
                PyErr_SetString(PyExc_TypeError, "Elements of sequence 'y0' are not numbers");
                return NULL;
            }
        }
    }

    // sanity checks for input numbers
    if (t0 >= tmax)
    {
        PyErr_SetString(PyExc_TypeError, "'tmax' has to be greater than 't0'");
        return NULL;
    }

    if (h <= 0)
    {
        PyErr_SetString(PyExc_TypeError, "'h' has to be greater than 0");
        return NULL;
    }

    // Calculation ------------------------------------------------------------------------

    size = RK4vector(&t, &y, array_PO_func, n, t0, tmax, y0, h);
    matrix m = {y, size, n};
    /*
    size x n-->
     |
     V
    */

    // Building return tuple ------------------------------------------------------------------------

    // https://stackoverflow.com/a/16401126/16527499
    // Return value: New (strong) reference
    // PyTuple_SetItem steals references, so no decref'ing needed
    tuple_t = PyTuple_New(size);
    tuple_y = PyTuple_New(n);
    tuple_rv = PyTuple_New(2);

    for (Py_ssize_t i = 0; i < (Py_ssize_t)size; i++)
    {
        if (PyTuple_SetItem(tuple_t, i, PyFloat_FromDouble(t[i])))
        {
            PyErr_Format(PyExc_IndexError, "Setting return vector t: out of bounds (i=%zu, size=%zu)", i, size);
            return NULL;
        }
    }

    for (Py_ssize_t i = 0; i < (Py_ssize_t)n; i++)
    {
        PO_tmp = PyTuple_New(size);
        for (Py_ssize_t j = 0; j < (Py_ssize_t)size; j++)
        {
            if (PyTuple_SetItem(PO_tmp, j, PyFloat_FromDouble(get_e(m, i, j))))
            {
                PyErr_Format(PyExc_IndexError, "Setting return matrix y: out of bounds (j=%zu, size=%zu)", j, size);
                return NULL;
            }
        }

        // i--> PO_tmp1, PO_tmp2, PO_tmp3, ...
        if (PyTuple_SetItem(tuple_y, i, PO_tmp))
        {
            PyErr_Format(PyExc_IndexError, "Setting return matrix y: out of bounds (i=%zu, n=%zu)", i, n);
            return NULL;
        }
    }

    if (PyTuple_SetItem(tuple_rv, 0, tuple_t) || PyTuple_SetItem(tuple_rv, 1, tuple_y))
    {
        PyErr_SetString(PyExc_IndexError, "Setting return tuple rv: out of bounds");
        return NULL;
    }

    // sanitize
    UNUSED(t);
    UNUSED(y);
    free(array_PO_func);
    free(y0);
    free(t);
    free(y);

    return tuple_rv;
}

// General setup -----------------------------------------------------------------------------------------------------------

/*
array of functions (methods) implemented in this module, terminated with a "null-function"
[0]: string of python function name
[1]: C function to call
[2]: macro whether function takes arguments (and of which type)
[3]: docstring
*/
static PyMethodDef numericalodesc_methods[] = {
    {"RK4", (PyCFunction)RK4, METH_VARARGS, "Python interface for Runge-Kutta algorithms written as a C library function"},
    {NULL}};

/*
bundle up the module
[0]: macro
[1]: module name
[2]: docstring
[3]: amount of memory needed to store the program state (negative value: no support for sub-interpreters)
[4]: array of fuctions
*/
static struct PyModuleDef numericalodesc_module = {
    PyModuleDef_HEAD_INIT,
    "numericalodesc",
    "Python interface for Runge-Kutta algorithms written as a C library function",
    -1,
    numericalodesc_methods};

/*
Initialize the module with PyInit_MODULENAME
*/
PyMODINIT_FUNC PyInit_numericalodesc(void)
{
    return PyModule_Create(&numericalodesc_module);
}
