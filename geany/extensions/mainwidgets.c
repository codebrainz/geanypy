#include <Python.h>
#include <structmember.h>
#include <pygobject.h>
#include <pygtk/pygtk.h>
#include <geanyplugin.h>

#include "modules-common.h"


typedef struct
{
	PyObject_HEAD
	GeanyMainWidgets *main_widgets;
} MainWidgets;

static PyTypeObject *PyGObject_Type = NULL;

static void
MainWidgets_dealloc(MainWidgets *self)
{
	self->ob_type->tp_free((PyObject *) self);
}


static int
MainWidgets_init(MainWidgets *self, PyObject *args, PyObject *kwds)
{
    self->main_widgets = NULL;
	return 0;
}


static PyObject *
MainWidgets_get_editor_menu(MainWidgets *self, PyObject *args)
{
    PyObject *py_gobject;
    py_gobject = pygobject_new((GObject *) geany_data->main_widgets->editor_menu);
    return py_gobject;
}


static PyObject *
MainWidgets_get_message_window_notebook(MainWidgets *self, PyObject *args)
{
    PyObject *py_gobject;
    py_gobject = pygobject_new((GObject *) geany_data->main_widgets->message_window_notebook);
    return py_gobject;
}


static PyObject *
MainWidgets_get_notebook(MainWidgets *self, PyObject *args)
{
    PyObject *py_gobject;
    py_gobject = pygobject_new((GObject *) geany_data->main_widgets->notebook);
    return py_gobject;
}


static PyObject *
MainWidgets_get_progressbar(MainWidgets *self, PyObject *args)
{
    PyObject *py_gobject;
    py_gobject = pygobject_new((GObject *) geany_data->main_widgets->progressbar);
    return py_gobject;
}


static PyObject *
MainWidgets_get_project_menu(MainWidgets *self, PyObject *args)
{
    PyObject *py_gobject;
    py_gobject = pygobject_new((GObject *) geany_data->main_widgets->project_menu);
    return py_gobject;
}


static PyObject *
MainWidgets_get_sidebar_notebook(MainWidgets *self, PyObject *args)
{
    PyObject *py_gobject;
    py_gobject = pygobject_new((GObject *) geany_data->main_widgets->sidebar_notebook);
    return py_gobject;
}


static PyObject *
MainWidgets_get_toolbar(MainWidgets *self, PyObject *args)
{
    PyObject *py_gobject;
    py_gobject = pygobject_new((GObject *) geany_data->main_widgets->toolbar);
    return py_gobject;
}


static PyObject *
MainWidgets_get_tools_menu(MainWidgets *self, PyObject *args)
{
    PyObject *py_gobject;
    py_gobject = pygobject_new((GObject *) geany_data->main_widgets->tools_menu);
    return py_gobject;
}


static PyObject *
MainWidgets_get_window(MainWidgets *self, PyObject *args)
{
    PyObject *py_gobject;
    py_gobject = pygobject_new((GObject *) geany_data->main_widgets->window);
    return py_gobject;
}


static PyMethodDef MainWidgets_methods[] = {
    { "get_editor_menu", (PyCFunction) MainWidgets_get_editor_menu, METH_VARARGS },
    { "get_message_window_notebook", (PyCFunction) MainWidgets_get_message_window_notebook, METH_VARARGS },
    { "get_notebook", (PyCFunction) MainWidgets_get_notebook, METH_VARARGS },
    { "get_progressbar", (PyCFunction) MainWidgets_get_progressbar, METH_VARARGS },
    { "get_project_menu", (PyCFunction) MainWidgets_get_project_menu, METH_VARARGS },
    { "get_sidebar_notebook", (PyCFunction) MainWidgets_get_sidebar_notebook, METH_VARARGS },
    { "get_toolbar", (PyCFunction) MainWidgets_get_toolbar, METH_VARARGS },
    { "get_tools_menu", (PyCFunction) MainWidgets_get_tools_menu, METH_VARARGS },
    { "get_window", (PyCFunction) MainWidgets_get_window, METH_VARARGS },
	{ NULL }
};


static PyTypeObject MainWidgetsType = {
	PyObject_HEAD_INIT(NULL)
    0,                          /*ob_size*/
    "geany.mainwidgets.MainWidgets",     /*tp_name*/
    sizeof(MainWidgets),             /*tp_basicsize*/
    0,                          /*tp_itemsize*/
    (destructor)MainWidgets_dealloc, /*tp_dealloc*/
    0,                          /*tp_print*/
    0,                          /*tpgetattr*/
    0,                          /*tpsetattr*/
    0,                          /*tp_compare*/
    0,                          /*tp_repr*/
    0,                          /*tp_as_number*/
    0,                          /*tp_as_sequence*/
    0,                          /*tp_as_mapping*/
    0,                          /*tp_hash */
    0,                          /*tp_call*/
    0,                          /*tp_str*/
    0,                          /*tpgetattro*/
    0,                          /*tpsetattro*/
    0,                          /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    "Geany main widgets",             /* tp_doc */
    0,		                    /* tp_traverse */
    0,		               	    /* tp_clear */
    0,		                    /* tp_richcompare */
    0,		                    /* tp_weaklistoffset */
    0,		                    /* tp_iter */
    0,		                    /* tp_iternext */
    MainWidgets_methods,             /* tp_methods */
    0,                          /* tp_members */
    0,                          /* tpgetset */
    0,                          /* tp_base */
    0,                          /* tp_dict */
    0,                          /* tp_descrget */
    0,                          /* tp_descrset */
    0,                          /* tp_dictoffset */
    (initproc)MainWidgets_init,      /* tp_init */
    0,                          /* tp_alloc */
    0,                          /* tp_new */

};


static PyMethodDef MainWidgetsModule_methods[] = { { NULL } };


PyMODINIT_FUNC
initmainwidgets(void)
{
    PyObject *m;

    MainWidgetsType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&MainWidgetsType) < 0)
        return;

    m = Py_InitModule("mainwidgets", MainWidgetsModule_methods);

    Py_INCREF(&MainWidgetsType);
    PyModule_AddObject(m, "MainWidgets", (PyObject *)&MainWidgetsType);


    init_pygobject();
    init_pygtk();
    m = PyImport_ImportModule("gobject");

    if (m)
    {
        PyGObject_Type = (PyTypeObject *) PyObject_GetAttrString(m, "GObject");
        Py_XDECREF(m);
    }

}
