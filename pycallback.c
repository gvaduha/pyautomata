#include <Python.h>

static PyObject *my_callback = NULL;

static PyObject *
my_set_callback(dummy, args)
    PyObject *dummy, *args;
{
    PyObject *result = NULL;
    PyObject *temp;

    if (PyArg_ParseTuple(args, "O:set_callback", &temp)) {
        if (!PyCallable_Check(temp)) {
            PyErr_SetString(PyExc_TypeError, "parameter must be callable");
            return NULL;
        }
        Py_XINCREF(temp);         /* Add a reference to new callback */
        Py_XDECREF(my_callback);  /* Dispose of previous callback */
        my_callback = temp;       /* Remember new callback */
        /* Boilerplate to return "None" */
        Py_INCREF(Py_None);
        result = Py_None;
    }
    return result;
}

static void
call py_call_callback(int arg)
{
	PyObject *arglist, *result;
        arglist = Py_BuildValue("(i)", arg);
        result = PyEval_CallObject(my_callback, arglist);
        Py_DECREF(arglist);
        if (result == NULL)
            return NULL; /* Pass error back */
        /* Here maybe use the result */
        Py_DECREF(result);
}
