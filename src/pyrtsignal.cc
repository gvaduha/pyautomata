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
static PyObject *my_callback = 0;

static PyObject* set_sighandler(PyObject *self, PyObject *args) {
    PyObject *temp;
    int signal;

    if (PyArg_ParseTuple(args, "iO:set_callback", &signal, &temp)) {
        if (!PyCallable_Check(temp)) {
            PyErr_SetString(PyExc_TypeError, "parameter must be callable");
            return 0;
        }
        Py_XINCREF(temp);
        Py_XDECREF(my_callback);
        my_callback = temp;

	//subscribe_signal(SIGRTMIN+signal);
    }
    Py_RETURN_NONE;
}

static PyObject* test_callback(PyObject *self, PyObject *args) {
	printf("TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT\n");
	PyObject *arglist, *result;
	int argval;
	if (PyArg_ParseTuple(args, "i", &argval)) {
			printf("Calling with: %d\n", argval);//////////////////////////////////////////////////////
            arglist = Py_BuildValue("i", argval);
       	    result = PyEval_CallObject(my_callback, arglist);
			printf("%x : %x : %d \n", my_callback, &my_callback, result);//////////////////////////////////////////////////////
            Py_DECREF(arglist);
            if (result == 0) {
                PyErr_SetString(PyExc_TypeError, "result is null");
                return 0; /* Pass error back */
	    }
	}
        Py_DECREF(result);
	Py_RETURN_NONE;
}

static PyMethodDef mod_methods[] = { 
    {   
        "set_signalhandler", set_sighandler, METH_VARARGS,
        "Set real time singal handler in python code."
    },  
    {
        "test_callback", test_callback, METH_VARARGS,
        "Test rt hadler callback"
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
