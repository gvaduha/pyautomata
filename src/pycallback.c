#include <stdio.h>
#include <Python.h>

static PyObject* test(PyObject *self, PyObject *args) {
    printf("Test extension module\n");
    Py_RETURN_NONE;
}

static PyObject *my_callback = NULL;

static PyObject* set_callback(PyObject *self, PyObject *args) {
    PyObject *temp;

    if (PyArg_ParseTuple(args, "O:set_callback", &temp)) {
        if (!PyCallable_Check(temp)) {
            PyErr_SetString(PyExc_TypeError, "parameter must be callable");
            return NULL;
        }
        Py_XINCREF(temp);         /* Add a reference to new callback */
        Py_XDECREF(my_callback);  /* Dispose of previous callback */
        my_callback = temp;       /* Remember new callback */
    }
    Py_RETURN_NONE;
}

static PyMethodDef mod_methods[] = { 
    {   
        "test", test, METH_NOARGS,
        "Print test message from a method defined in a C extension."
    },  
    {   
        "set_callback", set_callback, METH_VARARGS,
        "Set function callback in python code."
    },  
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef mod_definition = { 
    PyModuleDef_HEAD_INIT,
    "pycallback",
    "A Python callback extension module.",
    -1, 
    mod_methods
};

PyMODINIT_FUNC PyInit_pycallback(void) {
    Py_Initialize();
    return PyModule_Create(&mod_definition);
}

// END OF PYTHON INTERFACE

static void
exec_callback(int arg) {
	PyObject *arglist, *result;
        arglist = Py_BuildValue("(i)", arg);
        result = PyEval_CallObject(my_callback, arglist);
        Py_DECREF(arglist);
        if (result == NULL)
            return NULL; /* Pass error back */
        /* Here maybe use the result */
        Py_DECREF(result);
}
