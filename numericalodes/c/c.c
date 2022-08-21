#define PY_SSIZE_T_CLEAN
#include <python3.10/Python.h>
#include <stdlib.h>
#include "RungeKutta4Py.h" // https://stackoverflow.com/a/33711076/16527499
#include "matrix.h"
#include "vector.h"

/*
important concept:
https://stackoverflow.com/questions/53215786/when-writing-a-python-extension-in-c-how-does-one-pass-a-python-function-in-to
*/

/*
    info: references
    only three within this code used functions return new references:
    - PySequence_GetItem
    - PyTuple_New
    - PyObject_CallFunction

    As long as they are not stolen by
    - PyTuple_SetItem
    they have to be decresed manually
*/

/*
    Ask the kernel for a memory block and check whether the call was valid.
    Return 0 on success or -1 on failure.
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

/*
    Get the element at position 'loc' from the Python sequence 'sequence' and
    store it at the location 'dest'.
    Return 0 on success or -1 on failure.
    New reference in 'dest'!
*/
int unpack_fail(PyObject **dest, PyObject *sequence, Py_ssize_t loc)
{
    PyObject *tmp = NULL; // temporary Python object

    // new reference
    tmp = PySequence_GetItem(sequence, loc);

    if (tmp)
    {
        // point dest pointer to address of obtained object
        *dest = tmp;
        return 0;
    }
    else
    {
        return -1;
    }
}

// Functions to call in Python ------------------------------------------------------------------

const char DOC_RK4c[] =
    "Calculate the solution to the explicit system of differential equations y'=f(t,y) (with y and f as vectors) numerically"
    "using the explicit Runge-Kutta algorithm of order 4 written in C.\n"
    "The function f has to take EXACTLY TWO input parameters: a scalar t and a vector y.\n"
    "'Sequence' is a Python object that supports the sequence protocol (e.g. a list or a tuple)\n"
    "funcs: sequence\tentries of the vector f(t,y) (Callables)\n"
    "t0: float\tleft border of the domain to calculate the solution on\n"
    "tmax: float\tright border of the domain to calculate the solution on\n"
    "y0: sequence\tentries of the vector with initial conditions at the moment t0\n"
    "h: float\tstep size (distance between moments t)\n";

static PyObject *
RK4c(PyObject *self, PyObject *args)
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
        PyErr_SetString(PyExc_TypeError, "Given arguments do not match types: [PyObject*, double, double, PyObject*, double]");
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

    // Unpack function-tuple into array
    // https://stackoverflow.com/questions/25552315/python-tuple-to-c-array
    for (Py_ssize_t i = 0; i < (Py_ssize_t)n; i++)
    {
        // * NEW REFERENCE *
        if (unpack_fail(&PO_tmp, funcs, i))
        {
            PyErr_SetString(PyExc_LookupError, "Unpacking of funcs to array_PO_func");
            return NULL;
        }
        else
        {
            // Check whether unpacked elements are Callables
            if (PyCallable_Check(PO_tmp))
            {
                array_PO_func[i] = PO_tmp;
                PO_tmp = NULL;
            }
            else
            {
                PyErr_Format(PyExc_TypeError, "Element at index %zu of sequence 'funcs' is not a Callable", i);
                return NULL;
            }
        }
    }

    // Create C array to unpack Python tuple y0 into
    if (malloc_fail((void **)&y0, n, sizeof(PyObject *)))
    {
        PyErr_SetString(PyExc_SystemError, "malloc y0");
        return NULL;
    }

    // Unpack tuple y0 into array
    for (Py_ssize_t i = 0; i < (Py_ssize_t)n; i++)
    {
        // * NEW REFERENCE *
        if (unpack_fail(&PO_tmp, PO_y0, i))
        {
            PyErr_SetString(PyExc_LookupError, "Unpacking of PO_y0 to y0");
            return NULL;
        }
        else
        {
            // Check whether unpacked elements are numbers (float or int)
            if (PyFloat_Check(PO_tmp) || PyLong_Check(PO_tmp))
            {
                y0[i] = PyFloat_AsDouble(PO_tmp);

                // float entries from initial conditions vector y0 are no longer needed as seperate references
                // * DECREF *
                Py_DECREF(PO_tmp);
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

    // Calculation ----------------------------------------------------------------------------------

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
    // PyTuple_SetItem steals a reference, so no decref-ing needed

    // * NEW REFERENCE *
    tuple_t = PyTuple_New(size);
    // * NEW REFERENCE *
    tuple_y = PyTuple_New(n);
    // * NEW REFERENCE *
    tuple_rv = PyTuple_New(2);

    for (Py_ssize_t i = 0; i < (Py_ssize_t)size; i++)
    {
        // reference counting:
        //  directly stolen <---------- new reference
        //  * DECREF *                  * NEW REFERENCE *
        if (PyTuple_SetItem(tuple_t, i, PyFloat_FromDouble(t[i])))
        {
            PyErr_Format(PyExc_IndexError, "Setting return vector t: out of bounds (i=%zu, size=%zu)", i, size);
            return NULL;
        }
    }

    for (Py_ssize_t i = 0; i < (Py_ssize_t)n; i++)
    {
        // * NEW REFERENCE *
        PO_tmp = PyTuple_New(size);
        for (Py_ssize_t j = 0; j < (Py_ssize_t)size; j++)
        {
            //  * DECREF *                 * NEW REFERENCE *
            if (PyTuple_SetItem(PO_tmp, j, PyFloat_FromDouble(get_e(m, i, j))))
            {
                PyErr_Format(PyExc_IndexError, "Setting return matrix y: out of bounds (j=%zu, size=%zu)", j, size);
                return NULL;
            }
        }

        // i--> PO_tmp1, PO_tmp2, PO_tmp3, ...
        //  * DECREF *
        if (PyTuple_SetItem(tuple_y, i, PO_tmp))
        {
            PyErr_Format(PyExc_IndexError, "Setting return matrix y: out of bounds (i=%zu, n=%zu)", i, n);
            return NULL;
        }
    }

    //  * DECREF *                               * DECREF *
    if (PyTuple_SetItem(tuple_rv, 0, tuple_t) || PyTuple_SetItem(tuple_rv, 1, tuple_y))
    {
        PyErr_SetString(PyExc_IndexError, "Setting return tuple rv: out of bounds");
        return NULL;
    }

    // memory management
    free(array_PO_func);
    free(y0);
    free(t);
    free(y);

    return tuple_rv;
}

static PyObject *
tmp(PyObject *self, PyObject *args)
{
    double d1, d2, d3, array[3];
    PyObject *tuple;

    PyArg_ParseTuple(args, "ddd", &d1, &d2, &d3);
    array[0] = d1;
    array[1] = d2;
    array[2] = d3;
    tuple = PyTuple_New(3);

    array_to_tuple(tuple, array, 3);

    return tuple;
}

// General setup -----------------------------------------------------------------------------------------------------------

/*
array of functions (methods) implemented in this module, terminated with a "null-function"
https://docs.python.org/3/c-api/structures.html#c.PyMethodDef
[0]: ml_name    string    function name in Python
[1]: ml_meth    pointer   to C function
[2]: ml_flags   int       calling convention bit (see documentation)
[3]: ml_doc     string    docstring
*/
static PyMethodDef c_methods[] = {
    {"RK4c", (PyCFunction)RK4c, METH_VARARGS, DOC_RK4c},
    {"tmp", (PyCFunction)tmp, METH_VARARGS, ""},
    {NULL, NULL, 0, NULL}};

/*
bundle up the module
[0]: m_base      macro        always PyModuleDef_HEAD_INIT
[1]: m_name      string       module name
[2]: m_doc       string       docstring
[3]: m_size      Py_ssize_t   amount of memory needed to store the program state (-1: no support for sub-interpreters)
[4]: m_methods   array  	  array of fuctions (PyMethodDef[])
[5]: m_slots
[6]: m_traverse
[7]: m_clear
[8]: m_free
*/

const char DOC_c[] = "C extension module with a function to calculate the solution of explicit systems of differential equations numerically.\n";

static struct PyModuleDef c_module = {
    PyModuleDef_HEAD_INIT,
    "c",
    DOC_c,
    -1,
    c_methods};

/*
Initialize the module with PyInit_MODULENAME
*/
PyMODINIT_FUNC PyInit_c(void)
{
    return PyModule_Create(&c_module);
}
