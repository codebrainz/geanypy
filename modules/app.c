#include <Python.h>
#include <structmember.h>
#include <geanyplugin.h>


extern GeanyPlugin		*geany_plugin;
extern GeanyData		*geany_data;
extern GeanyFunctions	*geany_functions;


typedef struct
{
	PyObject_HEAD
    PyObject *proj;
} App;


static void
App_dealloc(App *self)
{
    Py_XDECREF(self->proj);
	self->ob_type->tp_free((PyObject *) self);
}


static int
App_init(App *self)
{
    self->proj = NULL;
    return 0;
}


static PyObject *
App_get_pointer(App *self)
{
    return PyLong_FromVoidPtr((void *) geany_data->app);
}


static PyObject *
App_get_configdir(App *self)
{
    return PyString_FromString(geany_data->app->configdir);
}


static PyObject *
App_get_datadir(App *self)
{
    return PyString_FromString(geany_data->app->datadir);
}


static PyObject *
App_get_docdir(App *self)
{
    return PyString_FromString(geany_data->app->docdir);
}


static PyObject *
App_get_debug_mode(App *self)
{
    if (geany_data->app->debug_mode)
        Py_RETURN_TRUE;
    else
        Py_RETURN_FALSE;
}


static PyObject *
App_get_project(App *self)
{
    if (geany_data->app->project == NULL)
        Py_RETURN_NONE;

    if (self->proj == NULL)
    {
        PyObject *mod, *cls;

        if (mod = PyImport_ImportModule("project"))
        {
            cls = PyObject_GetAttrString(mod, "Project");
            Py_DECREF(mod);
            if (cls && PyCallable_Check(cls))
            {
                self->proj = PyObject_CallObject(cls, NULL);
                Py_DECREF(cls);
                return self->proj;
            }
            else if (PyErr_Occurred())
                PyErr_Print();
        }
        else if (PyErr_Occurred())
            PyErr_Print();

        Py_RETURN_NONE;
    }
    else
        return self->proj;
}


static PyMethodDef App_methods[] = {

    /* Private methods */
    { "_get_pointer", (PyCFunction) App_get_pointer, METH_NOARGS, NULL },

    /* Public methods */
	{
        "get_configdir",
        (PyCFunction) App_get_configdir,
        METH_NOARGS,
        "User configuration directory, usually `~/.config/geany`.\n"
    },
    {
        "get_datadir",
        (PyCFunction) App_get_datadir,
        METH_NOARGS,
        "Directory where Geany stores its data.\n"
    },
    {
        "get_docdir",
        (PyCFunction) App_get_docdir,
        METH_NOARGS,
        "Directory where Geany stores its documentation.\n"
    },
    {
        "get_debug_mode",
        (PyCFunction) App_get_debug_mode,
        METH_NOARGS,
        "Whether debug messages should be printed or not.\n"
    },
    {
        "get_project",
        (PyCFunction) App_get_project,
        METH_NOARGS,
        "Currently active :class:`Project` instance or :data:`None`.\n\n"
        ".. see:: :class:`Project`\n"
    },
	{ NULL }
};


static PyTypeObject AppType = {
	PyObject_HEAD_INIT(NULL)
    0,                          /* ob_size */
    "geany.app.App",            /* tp_name */
    sizeof(App),                /* tp_basicsize */
    0,                          /* tp_itemsize */
    (destructor) App_dealloc,   /* tp_dealloc */
    0,                          /* tp_print */
    0,                          /* tp_getattr */
    0,                          /* tp_setattr */
    0,                          /* tp_compare */
    0,                          /* tp_repr */
    0,                          /* tp_as_number */
    0,                          /* tp_as_sequence */
    0,                          /* tp_as_mapping */
    0,                          /* tp_hash */
    0,                          /* tp_call */
    0,                          /* tp_str */
    0,                          /* tp_getattro */
    0,                          /* tp_setattro */
    0,                          /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags */
    "Wrapper class around Geany's :c:type:`GeanyApp` structure.  This class "
    "should not be directly initialized, instead access a single instance of "
    "it through :attr:`geany.app`.",             /* tp_doc */
    0,		                    /* tp_traverse */
    0,		               	    /* tp_clear */
    0,		                    /* tp_richcompare */
    0,		                    /* tp_weaklistoffset */
    0,		                    /* tp_iter */
    0,		                    /* tp_iternext */
    App_methods,                /* tp_methods */
    0,                          /* tp_members */
    0,                          /* tp_getset */
    0,                          /* tp_base */
    0,                          /* tp_dict */
    0,                          /* tp_descr_get */
    0,                          /* tp_descr_set */
    0,                          /* tp_dictoffset */
    (initproc) App_init,        /* tp_init */
    0,                          /* tp_alloc */
    0,                          /* tp_new */
};


static PyMethodDef AppModule_methods[] = { { NULL } };


PyMODINIT_FUNC initapp(void)
{
    PyObject *m;

    AppType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&AppType) < 0)
        return;

    m = Py_InitModule3("app", AppModule_methods,
            "The :mod:`geany.app` module provides a single wrapper class "
            "around a :c:type:`GeanyApp` structure.");

    Py_INCREF(&AppType);
    PyModule_AddObject(m, "App", (PyObject *)&AppType);
}
