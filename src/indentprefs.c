/*
 * indentprefs.c - See Geany's editor.h
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
IndentPrefs_dealloc(IndentPrefs *self)
{
	self->ob_type->tp_free((PyObject *) self);
}


static int
IndentPrefs_init(IndentPrefs *self, PyObject *args, PyObject *kwds)
{
    self->indent_prefs = NULL;
	return 0;
}


static PyObject *
IndentPrefs_get_width(IndentPrefs *self, PyObject *args)
{
    if (self->indent_prefs != NULL)
        return Py_BuildValue("i", self->indent_prefs->width);
    Py_RETURN_NONE;
}


static PyObject *
IndentPrefs_get_type(IndentPrefs *self, PyObject *args)
{
    if (self->indent_prefs != NULL)
        return Py_BuildValue("i", (gint) self->indent_prefs->type);
    Py_RETURN_NONE;
}


static PyObject *
IndentPrefs_get_hard_tab_width(IndentPrefs *self, PyObject *args)
{
    if (self->indent_prefs != NULL)
        return Py_BuildValue("i", self->indent_prefs->hard_tab_width);
    Py_RETURN_NONE;
}


static PyObject *
IndentPrefs_get_auto_indent_mode(IndentPrefs *self, PyObject *args)
{
    if (self->indent_prefs != NULL)
        return Py_BuildValue("i", self->indent_prefs->auto_indent_mode);
    Py_RETURN_NONE;
}


static PyObject *
IndentPrefs_get_detect_type(IndentPrefs *self, PyObject *args)
{
    if (self->indent_prefs != NULL)
        return Py_BuildValue("i", (gint) self->indent_prefs->detect_type);
    Py_RETURN_NONE;
}


static PyObject *
IndentPrefs_get_detect_width(IndentPrefs *self, PyObject *args)
{
    if (self->indent_prefs != NULL)
        return Py_BuildValue("i", self->indent_prefs->detect_width);
    Py_RETURN_NONE;
}


static PyMethodDef IndentPrefs_methods[] = {
    { "get_width", (PyCFunction)IndentPrefs_get_width, METH_VARARGS },
    { "get_type", (PyCFunction)IndentPrefs_get_type, METH_VARARGS },
    { "get_hard_tab_width", (PyCFunction)IndentPrefs_get_hard_tab_width, METH_VARARGS },
    { "get_auto_indent_mode", (PyCFunction)IndentPrefs_get_auto_indent_mode, METH_VARARGS },
    { "get_detect_type", (PyCFunction)IndentPrefs_get_detect_type, METH_VARARGS },
    { "get_detect_width", (PyCFunction)IndentPrefs_get_detect_width, METH_VARARGS },
    { NULL }
};


static PyTypeObject IndentPrefsType = {
	PyObject_HEAD_INIT(NULL)
    0,                          /*ob_size*/
    "geany.indentprefs.IndentPrefs",  /*tp_name*/
    sizeof(IndentPrefs),        /*tp_basicsize*/
    0,                          /*tp_itemsize*/
    (destructor)IndentPrefs_dealloc, /*tp_dealloc*/
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
    "Geany indent prefs",          /* tp_doc */
    0,		                    /* tp_traverse */
    0,		               	    /* tp_clear */
    0,		                    /* tp_richcompare */
    0,		                    /* tp_weaklistoffset */
    0,		                    /* tp_iter */
    0,		                    /* tp_iternext */
    IndentPrefs_methods,        /* tp_methods */
    0,                          /* tp_members */
    0,                          /* tpgetset */
    0,                          /* tp_base */
    0,                          /* tp_dict */
    0,                          /* tp_descrget */
    0,                          /* tp_descrset */
    0,                          /* tp_dictoffset */
    (initproc)IndentPrefs_init, /* tp_init */
    0,                          /* tp_alloc */
    0,                          /* tp_new */
};


static
PyMethodDef IndentPrefsModule_methods[] = {
    { NULL }
};


PyMODINIT_FUNC
initindentprefs(void)
{
    PyObject *m;

    IndentPrefsType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&IndentPrefsType) < 0)
        return;

    m = Py_InitModule("indentprefs", IndentPrefsModule_methods);

    Py_INCREF(&IndentPrefsType);
    PyModule_AddObject(m, "IndentPrefs", (PyObject *)&IndentPrefsType);
}


IndentPrefs *IndentPrefs_create_new_from_geany_indent_prefs(GeanyIndentPrefs *indent_prefs)
{
    IndentPrefs *self;
    self = (IndentPrefs *) PyObject_CallObject((PyObject *) &IndentPrefsType, NULL);
    self->indent_prefs = indent_prefs;
    return self;
}
