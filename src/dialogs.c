#include <Python.h>
#include <structmember.h>
#include <gtk/gtk.h>
#include <geanyplugin.h>
#include <pygtk/pygtk.h>
#include "plugin.h"


static PyObject *
Dialogs_show_input(PyObject *self, PyObject *args)
{
    const gchar *title = NULL;
    const gchar *label_text = NULL;
    const gchar *default_text = NULL;
    const gchar *result = NULL;
    PyObject *py_win_obj;
    PyGObject *py_win_gobj;
    GtkWindow *win;

    if (PyArg_ParseTuple(args, "|zOzz", &title, &py_win_obj, &label_text, &default_text))
    {
        if (title == NULL)
            title = "";

        if (py_win_obj != Py_None)
        {
            py_win_gobj = (PyGObject *) py_win_obj;
            win = GTK_WINDOW((GObject *) py_win_gobj->obj);
        }
        else
            win = GTK_WINDOW(geany->main_widgets->window);

        result = dialogs_show_input(title, win, label_text, default_text);
        if (result != NULL)
            return PyString_FromString(result);
    }

    Py_RETURN_NONE;
}


static PyObject *
Dialogs_show_input_numeric(PyObject *self, PyObject *args)
{
    const gchar *title = NULL;
    const gchar *label_text = NULL;
    gdouble value = 0.0, min = 0.0, max = 0.0, step = 0.0;

    if (PyArg_ParseTuple(args, "|zzdddd", &title, &label_text, &value, &min, &max, &step))
    {
        if (title == NULL)
            title = "";

        if (label_text == NULL)
            label_text = "";

        if (dialogs_show_input_numeric(title, label_text, &value, min, max, step))
            return PyFloat_FromDouble(value);
    }

    Py_RETURN_NONE;
}


static PyObject *
Dialogs_show_msgbox(PyObject *self, PyObject *args)
{
    gchar *text = NULL;
    gint msgtype = (gint) GTK_MESSAGE_INFO;

    if (PyArg_ParseTuple(args, "s|i", &text, &msgtype))
    {
        if (text != NULL)
        {
            dialogs_show_msgbox((GtkMessageType) msgtype, "%s", text);
            Py_RETURN_TRUE;
        }
    }
    Py_RETURN_NONE;
}


static PyObject *
Dialogs_show_question(PyObject *self, PyObject *args)
{
    gchar *text = NULL;

    if (PyArg_ParseTuple(args, "s", &text))
    {
        if (text != NULL)
        {
            if (dialogs_show_question("%s", text))
                Py_RETURN_TRUE;
            else
                Py_RETURN_FALSE;
        }
    }
    Py_RETURN_NONE;
}


static PyObject *
Dialogs_show_save_as(PyObject *self, PyObject *args)
{
    if (dialogs_show_save_as())
        Py_RETURN_TRUE;
    else
        Py_RETURN_FALSE;
}


static
PyMethodDef DialogsModule_methods[] = {
    { "show_input", (PyCFunction)Dialogs_show_input, METH_VARARGS },
    { "show_input_numeric", (PyCFunction)Dialogs_show_input_numeric, METH_VARARGS },
    { "show_msgbox", (PyCFunction)Dialogs_show_msgbox, METH_VARARGS },
    { "show_question", (PyCFunction)Dialogs_show_question, METH_VARARGS },
    { "show_save_as", (PyCFunction)Dialogs_show_save_as, METH_VARARGS },
    { NULL }
};


PyMODINIT_FUNC
init_geany_dialogs(void)
{
    PyObject *m;

    m = Py_InitModule("_geany_dialogs", DialogsModule_methods);
}
