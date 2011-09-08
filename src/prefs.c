/*
 * prefs.c - See Geany's app.h and GeanyPy's app.py.
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

#include "geanypy.h"



static void
Prefs_dealloc(Prefs *self)
{
	self->ob_type->tp_free((PyObject *) self);
}


static int
Prefs_init(Prefs *self)
{
	self->prefs = geany_data->prefs;
	return 0;
}


static PyObject *
Prefs_get_default_open_path(Prefs *self)
{
	if (self->prefs != NULL)
		return PyString_FromString(self->prefs->default_open_path);
	Py_RETURN_NONE;
}
GEANYPY_WRAP_GET_ONLY(Prefs, default_open_path);


static PyMethodDef Prefs_methods[] = {
	{ "get_default_open_path",	(PyCFunction) Prefs_get_default_open_path,	METH_NOARGS },
	{ NULL }
};


static PyGetSetDef Prefs_getseters[] = {
	GEANYPY_GETSETDEF(Prefs, default_open_path),
	{ NULL }
};


static PyTypeObject PrefsType = {
	PyObject_HEAD_INIT(NULL)
	0,											/* ob_size */
	"geany.prefs.Prefs",						/* tp_name */
	sizeof(Prefs),								/* tp_basicsize */
	0,											/* tp_itemsize */
	(destructor)Prefs_dealloc,					/* tp_dealloc */
	0,											/* tp_print */
	0,											/* tp_getattr */
	0,											/* tp_setattr */
	0,											/* tp_compare */
	0,											/* tp_repr */
	0,											/* tp_as_number */
	0,											/* tp_as_sequence */
	0,											/* tp_as_mapping */
	0,											/* tp_hash  */
	0,											/* tp_call */
	0,											/* tp_str */
	0,											/* tp_getattro */
	0,											/* tp_setattro */
	0,											/* tp_as_buffer */
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,	/* tp_flags */
	"Geany Prefs",								/* tp_doc  */
	0,											/* tp_traverse  */
	0,											/* tp_clear  */
	0,											/* tp_richcompare */
	0,											/* tp_weaklistoffset */
	0,											/* tp_iter */
	0,											/* tp_iternext */
	Prefs_methods,								/* tp_methods */
	0,											/* tp_members */
	Prefs_getseters,							/* tp_getset */
	0,											/* tp_base */
	0,											/* tp_dict */
	0,											/* tp_descr_get */
	0,											/* tp_descr_set */
	0,											/* tp_dictoffset */
	(initproc)Prefs_init,						/* tp_init */
	0,											/* tp_alloc */
	0,											/* tp_new */
};


static PyMethodDef PrefsModule_methods[] = { { NULL } };


PyMODINIT_FUNC initprefs(void)
{
	PyObject *m;

	PrefsType.tp_new = PyType_GenericNew;
	if (PyType_Ready(&PrefsType) < 0)
		return;

	m = Py_InitModule("prefs", PrefsModule_methods);

	Py_INCREF(&PrefsType);
	PyModule_AddObject(m, "Prefs", (PyObject *)&PrefsType);
}
