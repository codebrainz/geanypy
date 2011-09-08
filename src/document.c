/*
 * document.c - see Geany's document.h
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
Document_dealloc(Document *self)
{
	self->ob_type->tp_free((PyObject *) self);
}


static int
Document_init(Document *self)
{
	self->doc = NULL;
	return 0;
}


static PyObject*
Document_close(Document *self)
{
	if (document_close(self->doc))
		Py_RETURN_TRUE;
	else
		Py_RETURN_FALSE;
}


static PyObject*
Document_get_basename_for_display(Document *self)
{
	gchar *res;

	res = document_get_basename_for_display(self->doc, -1);

	if (res == NULL)
		Py_RETURN_NONE;

	return Py_BuildValue("s", res);
}


static PyObject*
Document_get_notebook_page(Document *self)
{
	gint res;

	res = document_get_notebook_page(self->doc);

	return Py_BuildValue("i", res);
}


static PyObject*
Document_get_status_color(Document *self)
{
	const GdkColor *color;

	color = document_get_status_color(self->doc);

	if (color == NULL)
		Py_RETURN_NONE;

	return Py_BuildValue("iii", color->red, color->green, color->blue);

}


static PyObject*
Document_reload_file(Document *self, PyObject *args, PyObject *kwargs)
{
	gchar *forced_enc = NULL;
	static gchar *kwlist[] = { "forced_enc", NULL };

	if (PyArg_ParseTupleAndKeywords(args, kwargs, "|z", kwlist, &forced_enc))
	{
		if (document_reload_file(self->doc, forced_enc))
			Py_RETURN_TRUE;
		else
			Py_RETURN_FALSE;
	}

	Py_RETURN_NONE;
}


static PyObject*
Document_rename_file(Document *self, PyObject *args, PyObject *kwargs)
{
	gchar *new_fn = NULL;
	static gchar *kwlist[] = { "new_filename", NULL };

	if (PyArg_ParseTupleAndKeywords(args, kwargs, "s", kwlist, &new_fn))
	{
		if (new_fn != NULL)
			document_rename_file(self->doc, new_fn);
	}

	if (DOC_VALID(self->doc) && self->doc->file_name == new_fn)
		Py_RETURN_TRUE;
	else
		Py_RETURN_FALSE;

	Py_RETURN_NONE;
}


static PyObject*
Document_save_file(Document *self, PyObject *args, PyObject *kwargs)
{
	gboolean result;
	gint force = 0;
	static gchar *kwlist[] = { "force", NULL };

	if (PyArg_ParseTupleAndKeywords(args, kwargs, "|i", kwlist, &force))
	{
		result = document_save_file(self->doc, (gboolean) force);
		if (result)
			Py_RETURN_TRUE;
		else
			Py_RETURN_FALSE;
	}
	Py_RETURN_NONE;
}


static PyObject*
Document_save_file_as(Document *self, PyObject *args, PyObject *kwargs)
{
	gboolean result;
	gchar *filename = NULL;
	static gchar *kwlist[] = { "new_filename", NULL };

	if (PyArg_ParseTupleAndKeywords(args, kwargs, "s", kwlist, &filename))
	{
		if (filename != NULL)
		{
			result = document_save_file_as(self->doc, filename);
			if (result)
				Py_RETURN_TRUE;
			else
				Py_RETURN_FALSE;
		}
	}
	Py_RETURN_NONE;
}


static PyObject*
Document_set_encoding(Document *self, PyObject *args)
{
	gchar *encoding = NULL;

	if (PyArg_ParseTuple(args, "s", &encoding))
	{
		if (encoding != NULL)
			document_set_encoding(self->doc, encoding);
	}
	Py_RETURN_NONE;
}


static PyObject*
Document_get_encoding(Document *self)
{
	if (DOC_VALID(self->doc))
		return PyString_FromString(self->doc->encoding);
	Py_RETURN_NONE;
}


static PyObject*
Document_set_filetype(Document *self, PyObject *ft)
{
	Filetype *filetype = NULL;

	if (ft != NULL && ft != Py_None)
	{
		filetype = (Filetype *) ft;
		if (filetype->ft != NULL)
			document_set_filetype(self->doc, filetype->ft);
	}
	Py_RETURN_NONE;
}


static PyObject*
Document_get_filetype(Document *self)
{
	if (DOC_VALID(self->doc))
		return (PyObject *) Filetype_create_new_from_geany_filetype(self->doc->file_type);
	Py_RETURN_NONE;
}


static PyObject*
Document_set_text_changed(Document *self, PyObject *args)
{
	gint changed;
	if (DOC_VALID(self->doc))
	{
		changed = self->doc->changed;
		if (PyArg_ParseTuple(args, "i", &changed))
			document_set_text_changed(self->doc, changed);
	}
	Py_RETURN_NONE;
}


static PyObject*
Document_get_text_changed(Document *self)
{
	if (DOC_VALID(self->doc))
	{
		if (self->doc->changed)
			Py_RETURN_TRUE;
		else
			Py_RETURN_FALSE;
	}
	Py_RETURN_NONE;
}


static PyObject*
Document_get_file_name(Document *self)
{
	if (DOC_VALID(self->doc) && self->doc->file_name != NULL)
		return PyString_FromString(self->doc->file_name);
	Py_RETURN_NONE;
}


static PyObject*
Document_get_has_bom(Document *self)
{
	if (DOC_VALID(self->doc))
	{
		if (self->doc->has_bom)
			Py_RETURN_TRUE;
		else
			Py_RETURN_FALSE;
	}
	Py_RETURN_NONE;
}


static PyObject*
Document_get_has_tags(Document *self)
{
	if (DOC_VALID(self->doc))
	{
		if (self->doc->has_tags)
			Py_RETURN_TRUE;
		else
			Py_RETURN_FALSE;
	}
	Py_RETURN_NONE;
}


static PyObject*
Document_get_index(Document *self)
{
	if (DOC_VALID(self->doc))
		return Py_BuildValue("i", self->doc->index);
	Py_RETURN_NONE;

}


static PyObject*
Document_get_is_valid(Document *self)
{
	if (DOC_VALID(self->doc))
	{
		if (self->doc->is_valid)
			Py_RETURN_TRUE;
		else
			Py_RETURN_FALSE;
	}
	Py_RETURN_NONE;
}


static PyObject*
Document_get_readonly(Document *self)
{
	if (DOC_VALID(self->doc))
	{
		if (self->doc->readonly)
			Py_RETURN_TRUE;
		else
			Py_RETURN_FALSE;
	}
	Py_RETURN_NONE;
}


static PyObject*
Document_get_real_path(Document *self)
{
	if (DOC_VALID(self->doc))
		return PyString_FromString(self->doc->real_path);
	Py_RETURN_NONE;
}


static PyObject *
Document_get_editor(Document *self)
{
	Editor *editor;
	if (DOC_VALID(self->doc) && self->doc->editor != NULL)
	{
		editor = Editor_create_new_from_geany_editor(self->doc->editor);
		return (PyObject *) editor;
	}
	Py_RETURN_NONE;
}


static PyMethodDef Document_methods[] = {
	{ "close",						(PyCFunction)Document_close, 					METH_NOARGS },
	{ "get_basename_for_display",	(PyCFunction)Document_get_basename_for_display,	METH_NOARGS },
	{ "get_notebook_page",			(PyCFunction)Document_get_notebook_page,			METH_NOARGS },
	{ "get_status_color",			(PyCFunction)Document_get_status_color,			METH_NOARGS },
	{ "reload_file",				(PyCFunction)Document_reload_file,				METH_KEYWORDS },
	{ "rename_file",				(PyCFunction)Document_rename_file,				METH_KEYWORDS },
	{ "save_file",					(PyCFunction)Document_save_file,				METH_KEYWORDS },
	{ "save_file_as",				(PyCFunction)Document_save_file_as,				METH_KEYWORDS },
	{ "set_encoding",				(PyCFunction)Document_set_encoding,				METH_VARARGS },
	{ "get_encoding",				(PyCFunction)Document_get_encoding,				METH_NOARGS },
	{ "set_file_type",				(PyCFunction)Document_set_filetype,				METH_O },
	{ "get_file_type",				(PyCFunction)Document_get_filetype,				METH_NOARGS },
	{ "set_text_changed",			(PyCFunction)Document_set_text_changed,			METH_VARARGS },
	{ "get_text_changed",			(PyCFunction)Document_get_text_changed,			METH_NOARGS },
	{ "get_file_name",				(PyCFunction)Document_get_file_name,				METH_NOARGS },
	{ "get_has_bom",				(PyCFunction)Document_get_has_bom,				METH_NOARGS },
	{ "get_has_tags",				(PyCFunction)Document_get_has_tags,				METH_NOARGS },
	{ "get_index",					(PyCFunction)Document_get_index,					METH_NOARGS },
	{ "get_is_valid",				(PyCFunction)Document_get_is_valid,				METH_NOARGS },
	{ "get_read_only",				(PyCFunction)Document_get_readonly,				METH_NOARGS },
	{ "get_real_path",				(PyCFunction)Document_get_real_path,				METH_NOARGS },
	{ "get_editor",					(PyCFunction)Document_get_editor,					METH_NOARGS },
	{ NULL }
};


static PyMemberDef Document_members[] = { {NULL} };


static PyTypeObject DocumentType = {
	PyObject_HEAD_INIT(NULL)
	0,											/* ob_size */
	"geany_document.Document",					/* tp_name */
	sizeof(Document),							/* tp_basicsize */
	0,											/* tp_itemsize */
	(destructor)Document_dealloc,				/* tp_dealloc */
	0,											/* tp_print */
	0,											/* tp_getattr */
	0,											/* tp_setattr */
	0,											/* tp_compare */
	0,											/* tp_repr */
	0,											/* tp_as_number */
	0,											/* tp_as_sequence */
	0,											/* tp_as_mapping */
	0,											/* tp_hash */
	0,											/* tp_call */
	0,											/* tp_str */
	0,											/* tp_getattro */
	0,											/* tp_setattro */
	0,											/* tp_as_buffer */
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,	/* tp_flags */
	"Geany document",							/* tp_doc */
	0,											/* tp_traverse */
	0,											/* tp_clear */
	0,											/* tp_richcompare */
	0,											/* tp_weaklistoffset */
	0,											/* tp_iter */
	0,											/* tp_iternext */
	Document_methods,							/* tp_methods */
	Document_members,							/* tp_members */
	0,											/* tp_getset */
	0,											/* tp_base */
	0,											/* tp_dict */
	0,											/* tp_descr_get */
	0,											/* tp_descr_set */
	0,											/* tp_dictoffset */
	(initproc)Document_init,					/* tp_init */
	0,											/* tp_alloc */
	0,											/* tp_new */

};


static PyObject*
Document_find_by_filename(PyObject *self, PyObject *args, PyObject *kwargs)
{
	gchar *fn;
	GeanyDocument *doc;
	static gchar *kwlist[] = { "filename", NULL };

	if (PyArg_ParseTupleAndKeywords(args, kwargs, "s", kwlist, &fn))
	{
		doc = document_find_by_filename(fn);
		if (DOC_VALID(doc))
			return (PyObject *) Document_create_new_from_geany_document(doc);
	}
	Py_RETURN_NONE;
}


static PyObject*
Document_find_by_real_path(PyObject *self, PyObject *args, PyObject *kwargs)
{
	gchar *fn;
	GeanyDocument *doc;
	static gchar *kwlist[] = { "real_path", NULL };

	if (PyArg_ParseTupleAndKeywords(args, kwargs, "s", kwlist, &fn))
	{
		doc = document_find_by_real_path(fn);
		if (DOC_VALID(doc))
			return (PyObject *) Document_create_new_from_geany_document(doc);
	}
	Py_RETURN_NONE;
}


static PyObject*
Document_get_current(PyObject *self)
{
	Document *py_doc;
	GeanyDocument *doc;

	doc = document_get_current();
	if (DOC_VALID(doc))
		return (PyObject *) Document_create_new_from_geany_document(doc);
	Py_RETURN_NONE;
}


static PyObject*
Document_get_from_page(PyObject *self, PyObject *args, PyObject *kwargs)
{
	gint page_num;
	GeanyDocument *doc;
	static gchar *kwlist[] = { "page_num", NULL };

	if (PyArg_ParseTupleAndKeywords(args, kwargs, "i", kwlist, &page_num))
	{
		doc = document_get_from_page(page_num);
		if (DOC_VALID(doc))
			return (PyObject *) Document_create_new_from_geany_document(doc);
	}
	Py_RETURN_NONE;
}


static PyObject*
Document_get_from_index(PyObject *self, PyObject *args, PyObject *kwargs)
{
	gint idx;
	GeanyDocument *doc;
	static gchar *kwlist[] = { "index", NULL };

	if (PyArg_ParseTupleAndKeywords(args, kwargs, "i", kwlist, &idx))
	{
		doc = document_index(idx);
		if (DOC_VALID(doc))
			return (PyObject *) Document_create_new_from_geany_document(doc);
	}
	Py_RETURN_NONE;
}


static PyObject*
Document_new_file(PyObject *self, PyObject *args, PyObject *kwargs)
{
	gchar *filename = NULL, *initial_text = NULL;
	Filetype *filetype = NULL;
	PyObject *py_ft = NULL;
	GeanyDocument *doc;
	GeanyFiletype *ft = NULL;
	static gchar *kwlist[] = { "filename", "filetype", "initial_text", NULL };

	if (PyArg_ParseTupleAndKeywords(args, kwargs, "|zOz", kwlist,
		&filename, &py_ft, &initial_text))
	{
		if (py_ft != NULL  && py_ft != Py_None)
		{
			filetype = (Filetype *) py_ft;
			if (filetype->ft != NULL)
				ft = filetype->ft;
		}
		doc = document_new_file(filename, ft, initial_text);
		if (DOC_VALID(doc))
			return (PyObject *) Document_create_new_from_geany_document(doc);
	}
	Py_RETURN_NONE;
}


static PyObject*
Document_open_file(PyObject *self, PyObject *args, PyObject *kwargs)
{
	gint read_only = 0;
	gchar *filename = NULL, *forced_encoding = NULL;
	GeanyDocument *doc;
	GeanyFiletype *ft = NULL;
	Filetype *filetype = NULL;
	PyObject *py_ft = NULL;
	static gchar *kwlist[] = { "filename", "read_only", "filetype",
		"forced_enc", NULL };

	if (PyArg_ParseTupleAndKeywords(args, kwargs, "s|iOz", &filename,
		&read_only, &py_ft, &forced_encoding))
	{
		if (py_ft != NULL && py_ft != Py_None)
		{
			filetype = (Filetype *) py_ft;
			if (filetype->ft != NULL)
				ft = filetype->ft;
		}
		doc = document_open_file(filename, read_only, ft, forced_encoding);
		if (DOC_VALID(doc))
			return (PyObject *) Document_create_new_from_geany_document(doc);
	}
	Py_RETURN_NONE;
}


static PyObject*
Document_remove_page(PyObject *self, PyObject *args, PyObject *kwargs)
{
	guint page_num;
	static gchar *kwlist[] = { "page_num", NULL };

	if (PyArg_ParseTupleAndKeywords(args, kwargs, "i", kwlist, &page_num))
	{
		if (document_remove_page(page_num))
			Py_RETURN_TRUE;
		else
			Py_RETURN_FALSE;
	}
	Py_RETURN_NONE;
}


static PyObject *
Document_get_documents_list(PyObject *module)
{
	guint i;
	GeanyDocument *doc;
	PyObject *list;

	list = PyList_New(0);

	for (i = 0; i < geany_data->documents_array->len; i++)
	{
		doc = g_ptr_array_index(geany_data->documents_array, i);
		if (DOC_VALID(doc))
		{
			PyList_Append(list,
				(PyObject *) Document_create_new_from_geany_document(doc));
		}
	}

	return list;
}


static
PyMethodDef DocumentModule_methods[] = {
	{ "find_by_filename",	(PyCFunction) Document_find_by_filename,	METH_KEYWORDS },
	{ "find_by_real_path",	(PyCFunction) Document_find_by_real_path,	METH_KEYWORDS },
	{ "get_current",		(PyCFunction) Document_get_current,			METH_NOARGS },
	{ "get_from_page",		(PyCFunction) Document_get_from_page,		METH_KEYWORDS },
	{ "index",				(PyCFunction) Document_get_from_index,		METH_KEYWORDS },
	{ "new_file",			(PyCFunction) Document_new_file,			METH_KEYWORDS },
	{ "open_file",			(PyCFunction) Document_open_file,			METH_KEYWORDS },
	{ "remove_page",		(PyCFunction) Document_remove_page,			METH_KEYWORDS },
	{ "get_documents_list",	(PyCFunction) Document_get_documents_list,	METH_NOARGS },
	{ NULL }
};


PyMODINIT_FUNC init_geany_document(void)
{
	PyObject *m;

	DocumentType.tp_new = PyType_GenericNew;
	if (PyType_Ready(&DocumentType) < 0)
		return;

	m = Py_InitModule("_document", DocumentModule_methods);

	Py_INCREF(&DocumentType);
	PyModule_AddObject(m, "Document", (PyObject *)&DocumentType);
}


Document *Document_create_new_from_geany_document(GeanyDocument *doc)
{
	Document *self;
	self = (Document *) PyObject_CallObject((PyObject *) &DocumentType, NULL);
	self->doc = doc;
	return self;
}
