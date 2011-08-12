#include <Python.h>
#include <structmember.h>
#include <geanyplugin.h>

#include "modules-common.h"


typedef struct
{
	PyObject_HEAD
} Project;


static void
Project_dealloc(Project *self)
{
	self->ob_type->tp_free((PyObject *) self);
}


static int
Project_init(Project *self)
{
	return 0;
}


static PyObject *
Project_get_pointer(Project *self)
{
    if (geany_data->app->project == NULL)
        Py_RETURN_NONE;
    return PyLong_FromVoidPtr((void *) geany_data->app->project);
}


static PyObject *
Project_get_base_path(Project *self)
{
    if (geany_data->app->project != NULL)
        return PyString_FromString(geany_data->app->project->base_path);
    Py_RETURN_NONE;
}


static PyObject *
Project_get_description(Project *self)
{
    if (geany_data->app->project != NULL)
        return PyString_FromString(geany_data->app->project->description);
    Py_RETURN_NONE;
}


static PyObject *
Project_get_file_name(Project *self)
{
    if (geany_data->app->project != NULL)
        return PyString_FromString(geany_data->app->project->file_name);
    Py_RETURN_NONE;
}


static PyObject *
Project_get_file_patterns(Project *self)
{
    guint i, len;
    PyObject *list, *item;

    if (geany_data->app->project != NULL)
    {
        len = g_strv_length(geany_data->app->project->file_patterns);
        list = PyList_New(0);
        for (i = 0; i < len; i++)
        {
            item = PyString_FromString(geany_data->app->project->file_patterns[i]);
            PyList_Append(list, item);
        }
        return list;
    }
    Py_RETURN_NONE;
}


static PyObject *
Project_get_name(Project *self)
{
    if (geany_data->app->project != NULL)
        return PyString_FromString(geany_data->app->project->name);
    Py_RETURN_NONE;
}


static PyObject *
Project_get_type(Project *self)
{
    if (geany_data->app->project != NULL)
        return Py_BuildValue("i", geany_data->app->project->type);
    Py_RETURN_NONE;
}


static PyObject *
Project_get_is_open(Project *self)
{
    if (geany_data->app->project == NULL)
        Py_RETURN_FALSE;
    else
        Py_RETURN_TRUE;
}


static PyMethodDef Project_methods[] = {
    /* Private methods */
    { "_get_pointer", (PyCFunction) Project_get_pointer, METH_NOARGS },

    /* Public methods */
	{ "get_base_path", (PyCFunction) Project_get_base_path, METH_NOARGS },
    { "get_description", (PyCFunction) Project_get_description, METH_NOARGS },
    { "get_file_name", (PyCFunction) Project_get_file_name, METH_NOARGS },
    { "get_file_patterns", (PyCFunction) Project_get_file_patterns, METH_NOARGS },
    { "get_name", (PyCFunction) Project_get_name, METH_NOARGS },
    { "get_type", (PyCFunction) Project_get_type, METH_NOARGS },
    { "get_is_open", (PyCFunction) Project_get_is_open, METH_NOARGS },
	{ NULL }
};


static PyTypeObject ProjectType = {
	PyObject_HEAD_INIT(NULL)
    0,                          /*ob_size*/
    "geany.project.Project",  /*tp_name*/
    sizeof(Project),        /*tp_basicsize*/
    0,                          /*tp_itemsize*/
    (destructor)Project_dealloc, /*tp_dealloc*/
    0,                          /*tp_print*/
    0,                          /*tp_getattr*/
    0,                          /*tp_setattr*/
    0,                          /*tp_compare*/
    0,                          /*tp_repr*/
    0,                          /*tp_as_number*/
    0,                          /*tp_as_sequence*/
    0,                          /*tp_as_mapping*/
    0,                          /*tp_hash */
    0,                          /*tp_call*/
    0,                          /*tp_str*/
    0,                          /*tp_getattro*/
    0,                          /*tp_setattro*/
    0,                          /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    "Geany Project", /* tp_doc */
    0,		                    /* tp_traverse */
    0,		               	    /* tp_clear */
    0,		                    /* tp_richcompare */
    0,		                    /* tp_weaklistoffset */
    0,		                    /* tp_iter */
    0,		                    /* tp_iternext */
    Project_methods,        /* tp_methods */
    0,                          /* tp_members */
    0,                          /* tp_getset */
    0,                          /* tp_base */
    0,                          /* tp_dict */
    0,                          /* tp_descr_get */
    0,                          /* tp_descr_set */
    0,                          /* tp_dictoffset */
    (initproc)Project_init, /* tp_init */
    0,                          /* tp_alloc */
    0,                          /* tp_new */
};


static PyMethodDef ProjectModule_methods[] = { { NULL } };


PyMODINIT_FUNC initproject(void)
{
    PyObject *m;

    ProjectType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&ProjectType) < 0)
        return;

    m = Py_InitModule("project", ProjectModule_methods);

    Py_INCREF(&ProjectType);
    PyModule_AddObject(m, "Project", (PyObject *)&ProjectType);
}

