#include <Python.h>
#include <structmember.h>
#include <gtk/gtk.h>
#include <geanyplugin.h>
#include <pygtk/pygtk.h>
#include "plugin.h"
#include "document.h"


static PyObject *
Navqueue_goto_line(PyObject *module, PyObject *args)
{
    gint line = 1;
    PyObject *py_old, *py_new;
    Document *py_doc_old, *py_doc_new;
    GeanyDocument *old_doc, *new_doc;

    /* TODO: find out why navqueue_goto_line() is segfaulting */
    g_debug("navqueue.goto_line() disabled");
    Py_RETURN_NONE;

    if (PyArg_ParseTuple(args, "OOi", &py_old, &py_new, &line))
    {
        if (py_old == Py_None)
            old_doc = NULL;
        else
        {
            py_doc_old = (Document *) py_old;
            old_doc = py_doc_old->doc;
        }
        if (py_new == Py_None)
            Py_RETURN_NONE;
        else
        {
            py_doc_new = (Document *) py_new;
            new_doc = py_doc_new->doc;
        }
        if ( (old_doc != NULL && !DOC_VALID(old_doc)) || !DOC_VALID(new_doc) )
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
init_geany_navqueue(void)
{
    PyObject *m;
    m = Py_InitModule("_geany_navqueue", NavqueueModule_methods);
}
