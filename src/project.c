/*
 * project.c - See Geany's project.h
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
Project_dealloc(Project *self)
{
	self->ob_type->tp_free((PyObject *) self);
}


static int
Project_init(Project *self, PyObject *args, PyObject *kwds)
{
    self->project = geany_data->app->project;
	return 0;
}


static PyObject *
Project__get_base_path(Project *self, PyObject *args)
{
    if (self->project != NULL)
        return PyString_FromString(self->project->base_path);
    Py_RETURN_NONE;
}


static PyObject *
Project__get_description(Project *self, PyObject *args)
{
    if (self->project != NULL)
        return PyString_FromString(self->project->description);
    Py_RETURN_NONE;
}


static PyObject *
Project__get_file_name(Project *self, PyObject *args)
{
    if (self->project != NULL)
        return PyString_FromString(self->project->file_name);
    Py_RETURN_NONE;
}


static PyObject *
Project__get_file_patterns(Project *self, PyObject *args)
{
    guint i, len;
    PyObject *list, *item;

    if (self->project != NULL)
    {
        len = g_strv_length(self->project->file_patterns);
        list = PyList_New(0);
        for (i = 0; i < len; i++)
        {
            item = PyString_FromString(self->project->file_patterns[i]);
            PyList_Append(list, item);
        }
        return list;
    }
    Py_RETURN_NONE;
}


static PyObject *
Project__get_name(Project *self, PyObject *args)
{
    if (self->project != NULL)
        return PyString_FromString(self->project->name);
    Py_RETURN_NONE;
}


static PyObject *
Project__get_type(Project *self, PyObject *args)
{
    if (self->project != NULL)
        return Py_BuildValue("i", self->project->type);
    Py_RETURN_NONE;
}


static PyObject *
Project__get_is_open(Project *self, PyObject *args)
{
    if (self->project == NULL)
        Py_RETURN_FALSE;
    else
        Py_RETURN_TRUE;
}


static PyMethodDef Project_methods[] = {
	{ "_get_base_path", (PyCFunction) Project__get_base_path, METH_VARARGS },
    { "_get_description", (PyCFunction) Project__get_description, METH_VARARGS },
    { "_get_file_name", (PyCFunction) Project__get_file_name, METH_VARARGS },
    { "_get_file_patterns", (PyCFunction) Project__get_file_patterns, METH_VARARGS },
    { "_get_name", (PyCFunction) Project__get_name, METH_VARARGS },
    { "_get_type", (PyCFunction) Project__get_type, METH_VARARGS },
    { "_get_is_open", (PyCFunction) Project__get_is_open, METH_VARARGS },
	{ NULL }
};


static PyTypeObject ProjectType = {
	PyObject_HEAD_INIT(NULL)
    0,                          /*ob_size*/
    "_geany_project.Project",  /*tp_name*/
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


static
PyMethodDef ProjectModule_methods[] = {
    { NULL }
};


PyMODINIT_FUNC
init_geany_project(void)
{
    PyObject *m;

    ProjectType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&ProjectType) < 0)
        return;

    m = Py_InitModule("_geany_project", ProjectModule_methods);

    Py_INCREF(&ProjectType);
    PyModule_AddObject(m, "Project", (PyObject *)&ProjectType);
}


Project *Project_create_new(void)
{
    Project *self;
    self = (Project *) PyObject_CallObject((PyObject *) &ProjectType, NULL);
    return self;
}

