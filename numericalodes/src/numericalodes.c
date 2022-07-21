#define PY_SSIZE_T_CLEAN
#include <python3.10/Python.h>
#include <stdlib.h>
#include "RungeKutta4.h" // https://stackoverflow.com/a/33711076/16527499

#define UNUSED(x) (void)(x)

// https://stackoverflow.com/questions/53215786/when-writing-a-python-extension-in-c-how-does-one-pass-a-python-function-in-to

static PyObject *RK4(PyObject *self, PyObject *args)
{

    size_t n;                            // size of func array (to be determined)
    double *t, *Y;                       // pointers to yet to be allocated adresses
    double (**func)(double, double *);   // function pointer array
    double t0, tmax, h;                  // 1st t, last t and step-size h
    double *y0;                          // inital array at position t=t0
    PyObject *PO_funcs, *PO_y0, *rv;     // objects to store the **func and *y0 python objects (PO's)
    PyObject **PO_func_array, **tmp_ptr; // array of unpacked PO_funcs

    /*
    Parse arguments:
    d: double
    O: PyObject*
    */
    if (!PyArg_ParseTuple(args, "OddOd", &PO_funcs, &t0, &tmax, &PO_y0, &h))
        // TODO:
        return PyUnicode_FromString("Error: Unpacking");

    // check for sequences in PO_funcs and PO_y0
    if (!PySequence_Check(PO_funcs) || !PySequence_Check(PO_y0))
        // TODO:
        return PyUnicode_FromString("Error: Sequence checking");

    // check for equal length
    if (PySequence_Length(PO_funcs) == PySequence_Length(PO_y0))
        n = (size_t)PySequence_Length(PO_y0);
    else
        // TODO:
        return PyUnicode_FromString("Error: Length comparison");

    // unpack python tuple into C array
    tmp_ptr = malloc(n * sizeof(PyObject *));

    // chech if malloc assignment was valid
    if (tmp_ptr)
        PO_func_array = (PyObject **)tmp_ptr;
    else
        // TODO:
        return PyUnicode_FromString("Error: malloc");

    // https://stackoverflow.com/questions/25552315/python-tuple-to-c-array
    for (Py_ssize_t i = 0; i < (Py_ssize_t)n; i++)
        PO_func_array[i] = PySequence_GetItem(PO_funcs, i);

    //! TODO: PyObject_CallObject
    // convert python function to c function via a wrapper

    rv = PyObject_CallFunction(PO_func_array[0], "d", 1);
    if (rv)
        return rv;
    else
        return PyUnicode_FromString("Error: CallObject");

    // sanitize
    free(PO_func_array);
    UNUSED(y0);
    UNUSED(func);
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
