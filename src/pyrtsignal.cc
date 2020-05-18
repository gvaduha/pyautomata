#include <stdio.h>
#include <signal.h>
#include <Python.h>
//#include <stdlib.h>
//#include <string.h>
//#include <unistd.h>


// RT signalling code
void rt_handler(int signal, siginfo_t *info, void *arg __attribute__ ((__unused__)))
{
    int val = info->si_value.sival_int;
    printf("signal arrived: %d=%d\n", signal, val);
}

bool subscribe_signal(int signal, bool unsubscribe = false)
{
    struct sigaction action;
    sigset_t mask;

    if (unsubscribe) {
	    action.sa_flags = SA_RESETHAND;
	    action.sa_handler = SIG_DFL;
    }
    else {
	    action.sa_flags = SA_SIGINFO;
	    action.sa_sigaction = rt_handler;
    }
    sigemptyset(&action.sa_mask);
    if (sigaction(signal, &action, 0) < 0)
        return false;

    sigemptyset(&mask);
    sigaddset(&mask, signal);
    sigprocmask(unsubscribe ? SIG_UNBLOCK : SIG_BLOCK, &mask, 0);
}

bool send_rt_signal(int signo, int value)
{
    union sigval sivalue;
    sivalue.sival_int = value;

    if (sigqueue(getpid(), signo, sivalue) < 0) {
    	fprintf(stderr, "sigqueue failed: %s\n", strerror(errno));
	return false;
    }
    return true;
}

// Python wrapper code
static PyObject *sighandlers_map[32];

static PyObject* set_sighandler(PyObject *self, PyObject *args) {
    PyObject *temp;
    int signal;

    if (PyArg_ParseTuple(args, "iO:set_sighandler", &signal, &temp)) {
        if (signal < 0 || signal > 31) {
            PyErr_SetString(PyExc_TypeError, "signal number should be in 0-31 range");
            return 0;
        }
        if (!PyCallable_Check(temp)) {
            PyErr_SetString(PyExc_TypeError, "parameter must be callable (int,int) function");
            return 0;
        }
        Py_XINCREF(temp);
        Py_XDECREF(sighandlers_map[signal]);
        sighandlers_map[signal] = temp;

	//subscribe_signal(SIGRTMIN+signal);
    }
    Py_RETURN_NONE;
}

static PyObject* test_sighandler(PyObject *self, PyObject *args) {
	PyObject *arglist, *result;
	int signal, val;
	if (PyArg_ParseTuple(args, "ii", &signal, &val)) {
        arglist = Py_BuildValue("ii", signal, val);
        if (signal < 0 || signal > 31) {
            PyErr_SetString(PyExc_TypeError, "signal number should be in 0-31 range");
            return 0;
        }
        if (0 == sighandlers_map[signal]) {
            PyErr_SetString(PyExc_TypeError, "signal handler is null for this signal");
            return 0; /* Pass error back */
	    }
       	result = PyEval_CallObject(sighandlers_map[signal], arglist);
        Py_DECREF(arglist);
        if (result == 0) {
            PyErr_SetString(PyExc_TypeError, "handler execution result is null");
            return 0; /* Pass error back */
	    }
	}
        Py_DECREF(result);
	Py_RETURN_NONE;
}

static PyMethodDef mod_methods[] = { 
    {   
        "set_sighandler", set_sighandler, METH_VARARGS,
        "Set real time singal handler in python code. Call (int signal_number, pyfunc handler), handler is (int signum, int value)"
    },  
    {
        "test_sighandler", test_sighandler, METH_VARARGS,
        "Test real time hadler callback. Call (int signum, int value)"
    },  
    {0, 0, 0, 0}
};

static struct PyModuleDef mod_definition = { 
    PyModuleDef_HEAD_INIT,
    "pyrtsignal",
    "A Python real time signals extension module.",
    -1, 
    mod_methods
};

PyMODINIT_FUNC PyInit_pyrtsignal(void) {
    memset(&sighandlers_map, 0, sizeof(PyObject*)*32);
    Py_Initialize();
    PyObject *m = PyModule_Create(&mod_definition);
    /* Adding module globals */
    if (PyModule_AddIntConstant(m, "SIGRTMIN", 0)) {
        goto except;
    }
    if (PyModule_AddIntConstant(m, "SIGRTMAX", 31)) {
        goto except;
    }
    goto finally;
except:
    Py_XDECREF(m);
    m = 0;
finally:
    return m;
}
