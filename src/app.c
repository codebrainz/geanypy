/*
 * app.c - See Geany's app.h
 *
 * Copyright 2011 Matthew Brush <mbrush@codebrainz.ca>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#include <Python.h>
#include <structmember.h>
#include <gtk/gtk.h>
#include <geanyplugin.h>
#include "plugin.h"


static void
App_dealloc(App *self)
{
	self->ob_type->tp_free((PyObject *) self);
}


static int
App_init(App *self, PyObject *args, PyObject *kwds)
{
    self->app = geany_data->app;
	return 0;
}


static PyObject *
App__get_configdir(App *self, PyObject *args)
{
    if (self->app != NULL)
        return PyString_FromString(self->app->configdir);
    Py_RETURN_NONE;
}


static PyObject *
App__get_debug_mode(App *self, PyObject *args)
{
    if (self->app != NULL)
    {
        if (self->app->debug_mode)
            Py_RETURN_TRUE;
        else
            Py_RETURN_FALSE;
    }
    Py_RETURN_NONE;
}


static PyObject *
App__get_project(App *self, PyObject *args)
{
    Project *proj;
    proj = Project_create_new();
    if (proj != NULL)
        return (PyObject *) proj;
    Py_RETURN_NONE;
}


static PyMethodDef App_methods[] = {
	{ "_get_configdir", (PyCFunction) App__get_configdir, METH_VARARGS },
    { "_get_debug_mode", (PyCFunction) App__get_debug_mode, METH_VARARGS },
    { "_get_project", (PyCFunction) App__get_project, METH_VARARGS },
	{ NULL }
};


static PyTypeObject AppType = {
	PyObject_HEAD_INIT(NULL)
    0,                          /*ob_size*/
    "_geany_app.App",  /*tp_name*/
    sizeof(App),        /*tp_basicsize*/
    0,                          /*tp_itemsize*/
    (destructor)App_dealloc, /*tp_dealloc*/
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
    "Geany App", /* tp_doc */
    0,		                    /* tp_traverse */
    0,		               	    /* tp_clear */
    0,		                    /* tp_richcompare */
    0,		                    /* tp_weaklistoffset */
    0,		                    /* tp_iter */
    0,		                    /* tp_iternext */
    App_methods,        /* tp_methods */
    0,                          /* tp_members */
    0,                          /* tp_getset */
    0,                          /* tp_base */
    0,                          /* tp_dict */
    0,                          /* tp_descr_get */
    0,                          /* tp_descr_set */
    0,                          /* tp_dictoffset */
    (initproc)App_init, /* tp_init */
    0,                          /* tp_alloc */
    0,                          /* tp_new */
};


static
PyMethodDef AppModule_methods[] = {
    { NULL }
};


PyMODINIT_FUNC
init_geany_app(void)
{
    PyObject *m;

    AppType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&AppType) < 0)
        return;

    m = Py_InitModule("_geany_app", AppModule_methods);

    Py_INCREF(&AppType);
    PyModule_AddObject(m, "App", (PyObject *)&AppType);
}
