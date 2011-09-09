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


#define GEANYPY_WRAP_GET_ONLY(cls, prop_name) \
	static PyObject * \
	cls ## _ ## prop_name ##_getter(PyObject *self, void *closure) { \
		return cls ## _get_ ## prop_name ( ( cls * ) self); \
	} \
	static int \
	cls ## _ ## prop_name ## _setter(PyObject *self, PyObject *value, void *closure) { \
		PyErr_SetString(PyExc_AttributeError, \
			"property '" #prop_name "' is read-only."); \
		return -1; \
	}

#define GEANYPY_WRAP_SET_ONLY(cls, prop_name) \
	static PyObject * \
	cls ## _ ## prop_name ## _getter(PyObject *self, void *closure) { \
		PyErr_SetString(PyExc_AttributeError, \
			"property '" #prop_name "' is write-only."); \
		return -1; \
	} \
	static int \
	cls ## _ ## prop_name ## _setter(PyObject *self, PyObject *value, void *closure) { \
		return cls ## _set_ ## prop_name( ( cls * ) self, value); \
	}

#define GEANYPY_WRAP_GET_SET(cls, prop_name) \
	static PyObject * \
	cls ## _ ## prop_name ## _getter(PyObject *self, void *closure) { \
		return  cls ## _get_ ## prop_name( ( cls * ) self); \
	} \
	static int \
	cls ## _ ## prop_name ## _setter(PyObject *self, PyObject *value, void *closure) { \
		return cls ## _set_ ## prop_name( ( cls * ) self, value); \
	}

#define GEANYPY_GETSETDEF(cls, prop_name) \
	{ #prop_name, \
		(getter) cls ## _ ## prop_name ## _getter, \
		(setter) cls ## _ ## prop_name ## _setter, \
		NULL, NULL }


#define GEANYPY_NEW(cls) \
	(cls *) PyObject_CallObject((PyObject *) &(cls ## Type), NULL);


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
#include "dialogs.h"
#include "document.h"
#include "editor.h"
#include "plugin.h"
#include "prefs.h"
#include "project.h"
#include "signalmanager.h"


#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* GEANYPY_H__ */
