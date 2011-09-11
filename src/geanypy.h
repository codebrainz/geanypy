/*
 * geanypy.h
 *
 * Copyright 2011 Matthew Brush <mbrush@codebrainz.ca>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301  USA.
 *
 */

#ifndef GEANYPY_H__
#define GEANYPY_H__
#ifdef __cplusplus
extern "C" {
#endif


#include <Python.h>
#ifndef PyMODINIT_FUNC
#  define PyMODINIT_FUNC void
#endif
#include <structmember.h>


/* Defines a setter that throws an attribute exception when called. */
#define GEANYPY_PROPS_READONLY(cls) \
	static int \
	cls ## _set_property(cls *self, PyObject *value, const gchar *prop_name) { \
		PyErr_SetString(PyExc_AttributeError, "can't set attribute"); \
		return -1; }

/* Defines a getter/setter for use in the tp_getset table. */
#define GEANYPY_GETSETDEF(cls, prop_name, doc) \
	{ prop_name, \
		(getter) cls ## _get_property, \
		(setter) cls ## _set_property, \
		doc, \
		prop_name }


/* Initializes a new `cls` type object. */
#define GEANYPY_NEW(cls) \
	(cls *) PyObject_CallObject((PyObject *) &(cls ## Type), NULL);


/* Returns a new py string or py none if string is NULL. */
#define GEANYPY_RETURN_STRING(memb) \
	{ \
		if (memb != NULL) \
			return PyString_FromString(memb); \
		else \
			Py_RETURN_NONE; \
	}


#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <dlfcn.h>

#include <gtk/gtk.h>
#include <pygobject.h>
#include <pygtk/pygtk.h>
#include <geanyplugin.h>

#ifndef GTK
#  define GTK
#endif
#include <Scintilla.h>
#include <ScintillaWidget.h>

#include "plugin-config.h"
#include "document.h"
#include "editor.h"
#include "filetypes.h"
#include "indentprefs.h"
#include "plugin.h"
#include "project.h"
#include "scintilla.h"
#include "signalmanager.h"


#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* GEANYPY_H__ */
