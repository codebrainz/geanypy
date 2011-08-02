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

#include <Python.h>
#include <structmember.h>
#include <gtk/gtk.h>
#include <geanyplugin.h>
#include "plugin.h"


extern GeanyPlugin		*geany_plugin;
extern GeanyData		*geany_data;
extern GeanyFunctions	*geany_functions;


static void
Document_dealloc(Document *self)
{
	self->ob_type->tp_free((PyObject *) self);
}


static PyObject *
Document_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	Document *self;
	self = (Document *)type->tp_alloc(type, 0);
	if (self != NULL)
	{
		self->doc = NULL;
	}
	return (PyObject *) self;
}


static int
Document_init(Document *self, PyObject *args, PyObject *kwds)
{
    self->doc = NULL;
	return 0;
}


static PyObject*
Document_close(Document *self, PyObject *args)
{
    if (document_close(self->doc))
        Py_RETURN_TRUE;
    else
        Py_RETURN_FALSE;
}


static PyObject*
Document__get_basename_for_display(Document *self, PyObject *args)
{
    gchar *res;

    res = document_get_basename_for_display(self->doc, -1);

    if (res == NULL)
        Py_RETURN_NONE;

    return Py_BuildValue("s", res);
}


static PyObject*
Document__get_notebook_page(Document *self, PyObject *args)
{
    gint res;

    res = document_get_notebook_page(self->doc);

    return Py_BuildValue("i", res);
}


static PyObject*
Document__get_status_color(Document *self, PyObject *args)
{
    const GdkColor *color;

    color = document_get_status_color(self->doc);

    if (color == NULL)
        Py_RETURN_NONE;

    return Py_BuildValue("iii", color->red, color->green, color->blue);

}


static PyObject*
Document_reload_file(Document *self, PyObject *args)
{
    gchar *forced_enc = NULL;

    if (PyArg_ParseTuple(args, "|z", &forced_enc))
    {
        if (document_reload_file(self->doc, forced_enc))
            Py_RETURN_TRUE;
        else
            Py_RETURN_FALSE;
    }

    Py_RETURN_NONE;
}


static PyObject*
Document_rename_file(Document *self, PyObject *args)
{
    gchar *new_fn = NULL;

    if (PyArg_ParseTuple(args, "s", &new_fn))
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
Document_save_file(Document *self, PyObject *args)
{
    gboolean result;
    gint force = 0;

    if (PyArg_ParseTuple(args, "|i", &force))
    {
        result = document_save_file(self->doc, force);
        if (result)
            Py_RETURN_TRUE;
        else
            Py_RETURN_FALSE;
    }
    Py_RETURN_NONE;
}


static PyObject*
Document_save_file_as(Document *self, PyObject *args)
{
    gboolean result;
    gchar *filename = NULL;

    if (PyArg_ParseTuple(args, "s", &filename))
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
Document__set_encoding(Document *self, PyObject *args)
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
Document__get_encoding(Document *self, PyObject *args)
{
    if (DOC_VALID(self->doc))
        return PyString_FromString(self->doc->encoding);
    Py_RETURN_NONE;
}


static PyObject*
Document__set_filetype(Document *self, PyObject *args)
{
    PyObject *py_ft = NULL;
    Filetype *filetype = NULL;

    if (PyArg_ParseTuple(args, "O", &py_ft))
    {
        if (py_ft != Py_None)
        {
            filetype = (Filetype *) py_ft;
            if (filetype->ft != NULL)
                document_set_filetype(self->doc, filetype->ft);
        }
    }
    Py_RETURN_NONE;
}


static PyObject*
Document__get_filetype(Document *self, PyObject *args)
{
    if (DOC_VALID(self->doc))
        return (PyObject *) Filetype_create_new_from_geany_filetype(self->doc->file_type);
    Py_RETURN_NONE;
}


static PyObject*
Document__set_text_changed(Document *self, PyObject *args)
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
Document__get_text_changed(Document *self, PyObject *args)
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
Document__get_file_name(Document *self, PyObject *args)
{
    if (DOC_VALID(self->doc) && self->doc->file_name != NULL)
        return PyString_FromString(self->doc->file_name);
    Py_RETURN_NONE;
}


static PyObject*
Document__get_has_bom(Document *self, PyObject *args)
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
Document__get_has_tags(Document *self, PyObject *args)
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
Document__get_index(Document *self, PyObject *args)
{
    if (DOC_VALID(self->doc))
        return Py_BuildValue("i", self->doc->index);
    Py_RETURN_NONE;

}


static PyObject*
Document__get_is_valid(Document *self, PyObject *args)
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
Document__get_readonly(Document *self, PyObject *args)
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
Document__get_real_path(Document *self, PyObject *args)
{
    if (DOC_VALID(self->doc))
        return PyString_FromString(self->doc->real_path);
    Py_RETURN_NONE;
}


static PyObject *
Document__get_editor(Document *self, PyObject *args)
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
    { "close", (PyCFunction)Document_close, METH_VARARGS },
    { "_get_basename_for_display", (PyCFunction)Document__get_basename_for_display, METH_VARARGS },
    { "_get_notebook_page", (PyCFunction)Document__get_notebook_page, METH_VARARGS },
    { "_get_status_color", (PyCFunction)Document__get_status_color, METH_VARARGS },
    { "reload_file", (PyCFunction)Document_reload_file, METH_VARARGS },
    { "rename_file", (PyCFunction)Document_rename_file, METH_VARARGS },
    { "save_file", (PyCFunction)Document_save_file, METH_VARARGS },
    { "save_file_as", (PyCFunction)Document_save_file_as, METH_VARARGS },
    { "_set_encoding", (PyCFunction)Document__set_encoding, METH_VARARGS },
    { "_get_encoding", (PyCFunction)Document__get_encoding, METH_VARARGS },
    { "_set_file_type", (PyCFunction)Document__set_filetype, METH_VARARGS },
    { "_get_file_type", (PyCFunction)Document__get_filetype, METH_VARARGS },
    { "_set_text_changed", (PyCFunction)Document__set_text_changed, METH_VARARGS },
    { "_get_text_changed", (PyCFunction)Document__get_text_changed, METH_VARARGS },
    { "_get_file_name", (PyCFunction)Document__get_file_name, METH_VARARGS },
    { "_get_has_bom", (PyCFunction)Document__get_has_bom, METH_VARARGS },
    { "_get_has_tags", (PyCFunction)Document__get_has_tags, METH_VARARGS },
    { "_get_index", (PyCFunction)Document__get_index, METH_VARARGS },
    { "_get_is_valid", (PyCFunction)Document__get_is_valid, METH_VARARGS },
    { "_get_read_only", (PyCFunction)Document__get_readonly, METH_VARARGS },
    { "_get_real_path", (PyCFunction)Document__get_real_path, METH_VARARGS },
    { "_get_editor", (PyCFunction)Document__get_editor, METH_VARARGS },
	{NULL}
};


static PyMemberDef Document_members[] = {
	//{"active_profile", T_STRING, offsetof(Document, active_profile), 0,
		//"The profile used by Zen Coding when performing actions."},
	//{"context", T_LONG, offsetof(Document, context), 0,
		//"The current editor context used by Zen Coding when performing actions."},
	{NULL}
};


static PyTypeObject DocumentType = {
	PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "_geany_document.Document",  /*tp_name*/
    sizeof(Document),         /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)Document_dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    "Geany document",          /* tp_doc */
    0,		                   /* tp_traverse */
    0,		               	   /* tp_clear */
    0,		                   /* tp_richcompare */
    0,		                   /* tp_weaklistoffset */
    0,		                   /* tp_iter */
    0,		                   /* tp_iternext */
    Document_methods,   /* tp_methods */
    Document_members,   /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)Document_init,  /* tp_init */
    0,                         /* tp_alloc */
    Document_new,       /* tp_new */

};


static PyObject*
Document_compare_by_display_name(PyObject *self, PyObject *args)
{
    gint res;
    PyObject *a, *b;

    if (PyArg_ParseTuple(args, "OO", &a, &b))
    {
        Document *doc_a, *doc_b;
        doc_a = (Document *) a;
        doc_b = (Document *) b;
        res = document_compare_by_display_name(&(doc_a->doc), &(doc_b->doc));
        return Py_BuildValue("i", res);
    }

    Py_RETURN_NONE;
}


static PyObject*
Document_compare_by_tab_order(PyObject *self, PyObject *args)
{
    gint res, reversed = 0;
    PyObject *a, *b;

    if (PyArg_ParseTuple(args, "OO|i", &a, &b, &reversed))
    {
        Document *doc_a, *doc_b;

        doc_a = (Document *) a;
        doc_b = (Document *) b;

        if (!reversed)
            res = document_compare_by_tab_order(&(doc_a->doc), &(doc_b->doc));
        else
            res = document_compare_by_tab_order_reverse(&(doc_a->doc), &(doc_b->doc));

        return Py_BuildValue("i", res);
    }

    Py_RETURN_NONE;
}


static PyObject*
Document_find_by_filename(PyObject *self, PyObject *args)
{
    gchar *fn;
    Document *py_doc;
    GeanyDocument *doc;

    if (PyArg_ParseTuple(args, "s", &fn))
    {
        doc = document_find_by_filename(fn);
        if (DOC_VALID(doc))
        {
            py_doc = (Document *) PyObject_CallObject((PyObject *)&DocumentType, NULL);
            py_doc->doc = doc;
            return (PyObject *) py_doc;
        }
    }
    Py_RETURN_NONE;
}


static PyObject*
Document_find_by_real_path(PyObject *self, PyObject *args)
{
    gchar *fn;
    Document *py_doc;
    GeanyDocument *doc;

    if (PyArg_ParseTuple(args, "s", &fn))
    {
        doc = document_find_by_real_path(fn);
        if (DOC_VALID(doc))
        {
            py_doc = (Document *) PyObject_CallObject((PyObject *)&DocumentType, NULL);
            py_doc->doc = doc;
            return (PyObject *) py_doc;
        }
    }
    Py_RETURN_NONE;
}


static PyObject*
Document_get_current(PyObject *self, PyObject *args)
{
    Document *py_doc;
    GeanyDocument *doc;

    doc = document_get_current();
    if (DOC_VALID(doc))
    {
        py_doc = (Document *) PyObject_CallObject((PyObject *)&DocumentType, NULL);
        py_doc->doc = doc;
        return (PyObject *) py_doc;
    }
    Py_RETURN_NONE;
}


static PyObject*
Document_get_from_page(PyObject *self, PyObject *args)
{
    gint page_num;
    Document *py_doc;
    GeanyDocument *doc;

    if (PyArg_ParseTuple(args, "i", &page_num))
    {
        doc = document_get_from_page(page_num);
        if (DOC_VALID(doc))
        {
            py_doc = (Document *) PyObject_CallObject((PyObject *)&DocumentType, NULL);
            py_doc->doc = doc;
            return (PyObject *) py_doc;
        }
    }
    Py_RETURN_NONE;
}


static PyObject*
Document_index(PyObject *self, PyObject *args)
{
    gint index;
    Document *py_doc;
    GeanyDocument *doc;

    if (PyArg_ParseTuple(args, "i", &index))
    {
        doc = document_index(index);
        if (DOC_VALID(doc))
        {
            py_doc = (Document *) PyObject_CallObject((PyObject *)&DocumentType, NULL);
            py_doc->doc = doc;
            return (PyObject *) py_doc;
        }
    }
    Py_RETURN_NONE;
}


static PyObject*
Document_new_file(PyObject *self, PyObject *args)
{
    gchar *filename = NULL;
    gchar *initial_text = NULL;
    Document *py_doc;
    Filetype *filetype = NULL;
    PyObject *py_ft = NULL;
    GeanyDocument *doc;
    GeanyFiletype *ft = NULL;

    if (PyArg_ParseTuple(args, "|zOz", &filename, &py_ft, &initial_text))
    {
        if (py_ft != NULL  && py_ft != Py_None)
        {
            filetype = (Filetype *) py_ft;
            if (filetype->ft != NULL)
                ft = filetype->ft;
        }
        doc = document_new_file(filename, ft, initial_text);
        if (DOC_VALID(doc))
        {
            py_doc = (Document *) PyObject_CallObject((PyObject *)&DocumentType, NULL);
            py_doc->doc = doc;
            return (PyObject *) py_doc;
        }
    }
    Py_RETURN_NONE;
}


static PyObject*
Document_open_file(PyObject *self, PyObject *args)
{
    gchar *filename = NULL;
    gint read_only = 0;
    gchar *forced_encoding = NULL;
    Document *py_doc;
    Filetype *filetype = NULL;
    PyObject *py_ft = NULL;
    GeanyDocument *doc;
    GeanyFiletype *ft = NULL;

    if (PyArg_ParseTuple(args, "s|iOz", &filename, &read_only, &py_ft, &forced_encoding))
    {
        if (py_ft != NULL && py_ft != Py_None)
        {
            filetype = (Filetype *) py_ft;
            if (filetype->ft != NULL)
                ft = filetype->ft;
        }
        doc = document_open_file(filename, read_only, ft, forced_encoding);
        if (DOC_VALID(doc))
        {
            py_doc = (Document *) PyObject_CallObject((PyObject *)&DocumentType, NULL);
            py_doc->doc = doc;
            return (PyObject *) py_doc;
        }
    }
    Py_RETURN_NONE;
}


static PyObject*
Document_remove_page(PyObject *self, PyObject *args)
{
    gboolean res;
    guint page_num;

    if (PyArg_ParseTuple(args, "i", &page_num))
    {
        res = document_remove_page(page_num);

        if (res)
            Py_RETURN_TRUE;
        else
            Py_RETURN_FALSE;
    }
    Py_RETURN_NONE;
}


static PyObject *
Document_get_documents_list(PyObject *module, PyObject *args)
{
    GeanyDocument *doc;
    Document *py_doc;
    gint i;
    PyObject *list;

    list = PyList_New(0);

    for (i = 0; i < geany_data->documents_array->len; i++)
    {
        doc = g_ptr_array_index(geany_data->documents_array, i);
        py_doc = (Document *) PyObject_CallObject((PyObject *)&DocumentType, NULL);
        py_doc->doc = doc;
        PyList_Append(list, (PyObject *) py_doc);
    }

    return list;
}


static
PyMethodDef DocumentModule_methods[] = {
    { "compare_by_display_name", (PyCFunction)Document_compare_by_display_name, METH_VARARGS },
    { "compare_by_tab_order", (PyCFunction)Document_compare_by_tab_order, METH_VARARGS },
    { "find_by_filename", (PyCFunction)Document_find_by_filename, METH_VARARGS },
    { "find_by_real_path", (PyCFunction)Document_find_by_real_path, METH_VARARGS },
    { "get_current", (PyCFunction)Document_get_current, METH_VARARGS },
    { "get_from_page", (PyCFunction)Document_get_from_page, METH_VARARGS },
    { "index", (PyCFunction)Document_index, METH_VARARGS },
    { "new_file", (PyCFunction)Document_new_file, METH_VARARGS },
    { "open_file", (PyCFunction)Document_open_file, METH_VARARGS },
    { "remove_page", (PyCFunction)Document_remove_page, METH_VARARGS },
    { "get_documents_list", (PyCFunction)Document_get_documents_list, METH_VARARGS },
    { NULL }
};


PyMODINIT_FUNC
init_geany_document(void)
{
    PyObject *m;

    DocumentType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&DocumentType) < 0)
        return;

    m = Py_InitModule("_geany_document", DocumentModule_methods);

    Py_INCREF(&DocumentType);
    PyModule_AddObject(m, "Document", (PyObject *)&DocumentType);
}
