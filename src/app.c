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
#include <geanyplugin.h>
#include "plugin.h"


static void
App_dealloc(App *self)
{
	self->ob_type->tp_free((PyObject *) self);
}


static int
App_init(App *self)
{
    self->app = geany_data->app;
	return 0;
}


static PyObject *
App_get_configdir(App *self)
{
    if (self->app != NULL && self->app->configdir != NULL)
        return PyString_FromString(self->app->configdir);
    Py_RETURN_NONE;
}


static PyObject *
App_get_datadir(App *self)
{
    if (self->app != NULL && self->app->datadir != NULL)
        return PyString_FromString(self->app->datadir);
    Py_RETURN_NONE;
}


static PyObject *
App_get_docdir(App *self)
{
    if (self->app != NULL && self->app->docdir != NULL)
        return PyString_FromString(self->app->docdir);
    Py_RETURN_NONE;
}


static PyObject *
App_get_debug_mode(App *self)
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
App_get_project(App *self)
{
    Project *proj;
    proj = Project_create_new();
    if (proj != NULL)
        return (PyObject *) proj;
    Py_RETURN_NONE;
}


static PyMethodDef App_methods[] = {
	{ "get_configdir", (PyCFunction) App_get_configdir, METH_NOARGS,
        "User configuration directory, usually `~/.config/geany`."},
    { "get_datadir", (PyCFunction) App_get_datadir, METH_NOARGS,
        "Directory where Geany stores its data." },
    { "get_docdir", (PyCFunction) App_get_docdir, METH_NOARGS,
        "Directory where Geany stores its documentation." },
    { "get_debug_mode", (PyCFunction) App_get_debug_mode, METH_NOARGS,
        "Whether debug messages should be printed or not." },
    { "get_project", (PyCFunction) App_get_project, METH_NOARGS,
        "Currently active project or `None`." },
	{ NULL }
};


static PyTypeObject AppType = {
	PyObject_HEAD_INIT(NULL)
    0,                          /*ob_size*/
    "geany.app.App",            /*tp_name*/
    sizeof(App),                /*tp_basicsize*/
    0,                          /*tp_itemsize*/
    (destructor)App_dealloc,    /*tp_dealloc*/
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
    "Wrapper class around Geany's `GeanyApp` structure.  This class should not "
    "be directly initialized, instead access a single instance of it through "
    "`geany.app`.",         /* tp_doc */
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
    (initproc)App_init,         /* tp_init */
    0,                          /* tp_alloc */
    0,                          /* tp_new */
};


static PyMethodDef AppModule_methods[] = { { NULL } };


PyMODINIT_FUNC
initapp(void)
{
    PyObject *m;

    AppType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&AppType) < 0)
        return;

    m = Py_InitModule3("app", AppModule_methods,
            "The `app` module provides a single wrapper class around a  "
            "`GeanyApp` structure.");

    Py_INCREF(&AppType);
    PyModule_AddObject(m, "App", (PyObject *)&AppType);
}
