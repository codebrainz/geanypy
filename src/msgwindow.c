#include <Python.h>
#include <structmember.h>
#include <gtk/gtk.h>
#include <geanyplugin.h>
#include <pygtk/pygtk.h>
#include "plugin.h"
#include "document.h"


static PyObject *
Msgwin_clear_tab(PyObject *module, PyObject *args)
{
    gint tab_num = 0;

    if (PyArg_ParseTuple(args, "i", &tab_num))
        msgwin_clear_tab(tab_num);

    Py_RETURN_NONE;
}


static PyObject *
Msgwin_compiler_add(PyObject *module, PyObject *args)
{
    gint msg_color = COLOR_BLACK;
    gchar *msg = NULL;

    if (PyArg_ParseTuple(args, "s|i", &msg, &msg_color))
        msgwin_compiler_add(msg_color, "%s", msg);

    Py_RETURN_NONE;
}


static PyObject *
Msgwin_msg_add(PyObject *module, PyObject *args)
{
    gint msg_color = COLOR_BLACK, line = -1;
    PyObject *obj = NULL;
    Document *py_doc = NULL;
    GeanyDocument *doc = NULL;
    gchar *msg = NULL;

    if (PyArg_ParseTuple(args, "s|iiO", &msg, &msg_color, &line, &obj))
    {
        if (obj == NULL || obj == Py_None)
            doc = NULL;
        else
        {
            py_doc = (Document *) obj;
            doc = py_doc->doc;
        }
        msgwin_msg_add(msg_color, line, doc, "%s", msg);
    }

    Py_RETURN_NONE;
}


static PyObject *
Msgwin_set_messages_dir(PyObject *module, PyObject *args)
{
    gchar *msgdir = NULL;

    if (PyArg_ParseTuple(args, "s", &msgdir))
    {
        if (msgdir != NULL)
            msgwin_set_messages_dir(msgdir);
    }

    Py_RETURN_NONE;
}


static PyObject *
Msgwin_status_add(PyObject *module, PyObject *args)
{
    gchar *msg = NULL;

    if (PyArg_ParseTuple(args, "s", &msg))
    {
        if (msg != NULL)
            msgwin_status_add("%s", msg);
    }

    Py_RETURN_NONE;
}


static PyObject *
Msgwin_switch_tab(PyObject *module, PyObject *args)
{
    gint tabnum = 0, show = 0;

    if (PyArg_ParseTuple(args, "i|i", &tabnum, &show))
        msgwin_switch_tab(tabnum, show);

    Py_RETURN_NONE;
}


static
PyMethodDef MsgwinModule_methods[] = {
    { "clear_tab", (PyCFunction) Msgwin_clear_tab, METH_VARARGS },
    { "compiler_add", (PyCFunction) Msgwin_compiler_add, METH_VARARGS },
    { "msg_add", (PyCFunction) Msgwin_msg_add, METH_VARARGS },
    { "set_messages_dir", (PyCFunction) Msgwin_set_messages_dir, METH_VARARGS },
    { "status_add", (PyCFunction) Msgwin_status_add, METH_VARARGS },
    { "switch_tab", (PyCFunction) Msgwin_switch_tab, METH_VARARGS },
    { NULL }
};


PyMODINIT_FUNC
init_geany_msgwin(void)
{
    PyObject *m;

    m = Py_InitModule("_geany_msgwindow", MsgwinModule_methods);
}
