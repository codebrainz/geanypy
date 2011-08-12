#include <Python.h>
#include <geanyplugin.h>

#include "modules-common.h"


static PyObject *
Navqueue_goto_line(PyObject *module, PyObject *args)
{
    gint line = 1;
    PyObject *py_old, *py_new, *ptr_func, *pylong;
    GeanyDocument *old_doc = NULL, *new_doc = NULL;

    /* TODO: find out why navqueue_goto_line() is segfaulting */
    g_debug("navqueue.goto_line() disabled");
    Py_RETURN_NONE;

    if (PyArg_ParseTuple(args, "OOi", &py_old, &py_new, &line))
    {
        if (py_old != NULL && py_old != Py_None)
            old_doc = GET_POINTER(py_old, GeanyDocument);

        if (py_new != NULL && py_new != Py_None)
            new_doc = GET_POINTER(py_new, GeanyDocument);

        if ( (old_doc != NULL && new_doc != NULL && !DOC_VALID(old_doc)) || !DOC_VALID(new_doc) )
            Py_RETURN_NONE;

        if (navqueue_goto_line(old_doc, new_doc, line))
            Py_RETURN_TRUE;
        else
            Py_RETURN_FALSE;
    }

    Py_RETURN_NONE;
}


static
PyMethodDef NavqueueModule_methods[] = {
    { "goto_line", (PyCFunction) Navqueue_goto_line, METH_VARARGS },
    { NULL }
};


PyMODINIT_FUNC
initnavqueue(void)
{
    PyObject *m;
    m = Py_InitModule("navqueue", NavqueueModule_methods);
}
