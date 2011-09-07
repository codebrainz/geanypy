/*
 * editor.c - See Geany's editor.h
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
Editor_dealloc(Editor *self)
{
	self->ob_type->tp_free((PyObject *) self);
}


static int
Editor_init(Editor *self, PyObject *args, PyObject *kwds)
{
    self->editor = NULL;
	return 0;
}


static PyObject *
Editor_create_widget(Editor *self, PyObject *args)
{
    g_debug("create_widget() not yet implemented, sorry");
    Py_RETURN_NONE;
}


static PyObject *
Editor_find_snippet(Editor *self, PyObject *args)
{
    gchar *name;
    const gchar *snippet;
    if (PyArg_ParseTuple(args, "s", &name))
    {
        if (name != NULL)
        {
            snippet = editor_find_snippet(self->editor, name);
            if (snippet != NULL)
                return Py_BuildValue("s", snippet);
        }
    }
    Py_RETURN_NONE;
}


static PyObject *
Editor__get_eol_char(Editor *self, PyObject *args)
{
    const gchar *eol_char = editor_get_eol_char(self->editor);
    if (eol_char != NULL)
        return Py_BuildValue("s", eol_char);
    Py_RETURN_NONE;
}


static PyObject *
Editor__get_eol_char_len(Editor *self, PyObject *args)
{
    return Py_BuildValue("i", editor_get_eol_char_len(self->editor));
}


static PyObject *
Editor__get_eol_char_mode(Editor *self, PyObject *args)
{
    return Py_BuildValue("i", editor_get_eol_char_mode(self->editor));
}


static PyObject *
Editor__get_eol_char_name(Editor *self, PyObject *args)
{
    const gchar *eol_char_name = editor_get_eol_char_name(self->editor);
    if (eol_char_name != NULL)
        return Py_BuildValue("s", eol_char_name);
    Py_RETURN_NONE;
}


static PyObject *
Editor__get_indent_prefs(Editor *self, PyObject *args)
{
    const GeanyIndentPrefs *indent_prefs;
    IndentPrefs *py_prefs;
    indent_prefs = editor_get_indent_prefs(self->editor);
    if (indent_prefs != NULL)
    {
        py_prefs = IndentPrefs_create_new_from_geany_indent_prefs((GeanyIndentPrefs *)indent_prefs);
        return (PyObject *) py_prefs;
    }
    Py_RETURN_NONE;
}


static PyObject *
Editor_get_word_at_position(Editor *self, PyObject *args)
{
    gint pos = -1;
    gchar *wordchars = NULL, *word = NULL;
    PyObject *py_word;

    if (PyArg_ParseTuple(args, "|iz", &pos, &wordchars))
    {
        word = editor_get_word_at_pos(self->editor, pos, wordchars);
        if (word != NULL)
        {
            py_word = Py_BuildValue("s", word);
            g_free(word);
            return py_word;
        }
    }

    Py_RETURN_NONE;
}


static PyObject *
Editor_goto_pos(Editor *self, PyObject *args)
{
    gint pos, mark = FALSE, result;
    if (PyArg_ParseTuple(args, "i|i", &pos, &mark))
    {
        result = editor_goto_pos(self->editor, pos, mark);
        if (result)
            Py_RETURN_TRUE;
        else
            Py_RETURN_FALSE;
    }
    Py_RETURN_NONE;
}


static PyObject *
Editor_indicator_clear(Editor *self, PyObject *args)
{
    gint indic;
    if (PyArg_ParseTuple(args, "i", &indic))
        editor_indicator_clear(self->editor, indic);
    Py_RETURN_NONE;
}


static PyObject *
Editor_indicator_set_on_line(Editor *self, PyObject *args)
{
    gint indic, line_num;
    if (PyArg_ParseTuple(args, "ii", &indic, &line_num))
        editor_indicator_set_on_line(self->editor, indic, line_num);
    Py_RETURN_NONE;
}


static PyObject *
Editor_indicator_set_on_range(Editor *self, PyObject *args)
{
    gint indic, start, end;
    if (PyArg_ParseTuple(args, "iii", &indic, &start, &end))
        editor_indicator_set_on_range(self->editor, indic, start, end);
    Py_RETURN_NONE;
}


static PyObject *
Editor_insert_snippet(Editor *self, PyObject *args)
{
    gint pos = 0;
    gchar *snippet = NULL;

    if (PyArg_ParseTuple(args, "is", &pos, &snippet))
        editor_insert_snippet(self->editor, pos, snippet);
    Py_RETURN_NONE;
}


static PyObject *
Editor_insert_text_block(Editor *self, PyObject *args)
{
    gchar *text = NULL;
    gint insert_pos, cursor_index = -1, newline_indent_size = -1;
    gint replace_newlines = 0;

    if (PyArg_ParseTuple(args, "si|iii", &text, &insert_pos, &cursor_index,
        &newline_indent_size, &replace_newlines))
    {
        editor_insert_text_block(self->editor, text, insert_pos, cursor_index,
            newline_indent_size, (gboolean) replace_newlines);
    }

    Py_RETURN_NONE;
}


static PyObject *
Editor__set_indent_type(Editor *self, PyObject *args)
{
    gint indent_type;
    if (PyArg_ParseTuple(args, "i", &indent_type))
        editor_set_indent_type(self->editor, indent_type);
    Py_RETURN_NONE;
}


static PyObject *
Editor__get_scintilla(Editor *self, PyObject *args)
{
    Scintilla *sci;
    if (self->editor != NULL)
    {
        sci = Scintilla_create_new_from_scintilla(self->editor->sci);
        return (PyObject *) sci;
    }
    Py_RETURN_NONE;
}


static PyMethodDef Editor_methods[] = {
    { "create_widget", (PyCFunction) Editor_create_widget, METH_VARARGS },
    { "find_snippet", (PyCFunction) Editor_find_snippet, METH_VARARGS },
    { "get_word_at_position", (PyCFunction) Editor_get_word_at_position, METH_VARARGS },
    { "goto_pos", (PyCFunction) Editor_goto_pos, METH_VARARGS },
    { "indicator_clear", (PyCFunction) Editor_indicator_clear, METH_VARARGS },
    { "indicator_set_on_line", (PyCFunction) Editor_indicator_set_on_line, METH_VARARGS },
    { "indicator_set_on_range", (PyCFunction) Editor_indicator_set_on_range, METH_VARARGS },
    { "insert_snippet", (PyCFunction) Editor_insert_snippet, METH_VARARGS },
    { "insert_text_block", (PyCFunction) Editor_insert_text_block, METH_VARARGS },
    { "_get_eol_char", (PyCFunction) Editor__get_eol_char, METH_VARARGS },
    { "_get_eol_char_len", (PyCFunction) Editor__get_eol_char_len, METH_VARARGS },
    { "_get_eol_char_mode", (PyCFunction) Editor__get_eol_char_mode, METH_VARARGS },
    { "_get_eol_char_name", (PyCFunction) Editor__get_eol_char_name, METH_VARARGS },
    { "_get_indent_prefs", (PyCFunction) Editor__get_indent_prefs, METH_VARARGS },
    { "_set_indent_type", (PyCFunction) Editor__set_indent_type, METH_VARARGS },
    { "_get_scintilla", (PyCFunction) Editor__get_scintilla, METH_VARARGS },
	{ NULL }
};


static PyTypeObject EditorType = {
	PyObject_HEAD_INIT(NULL)
    0,                          /*ob_size*/
    "_geany_editor.Editor",     /*tp_name*/
    sizeof(Editor),             /*tp_basicsize*/
    0,                          /*tp_itemsize*/
    (destructor)Editor_dealloc, /*tp_dealloc*/
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
    "Geany Editor",             /* tp_doc */
    0,		                    /* tp_traverse */
    0,		               	    /* tp_clear */
    0,		                    /* tp_richcompare */
    0,		                    /* tp_weaklistoffset */
    0,		                    /* tp_iter */
    0,		                    /* tp_iternext */
    Editor_methods,             /* tp_methods */
    0,                          /* tp_members */
    0,                          /* tp_getset */
    0,                          /* tp_base */
    0,                          /* tp_dict */
    0,                          /* tp_descr_get */
    0,                          /* tp_descr_set */
    0,                          /* tp_dictoffset */
    (initproc)Editor_init,      /* tp_init */
    0,                          /* tp_alloc */
    0,                          /* tp_new */

};


static PyObject *
Editor__find_snippet(PyObject *module, PyObject *args)
{
    gchar *name;
    const gchar *snippet;
    if (PyArg_ParseTuple(args, "s", &name))
    {
        if (name != NULL)
        {
            snippet = editor_find_snippet(NULL, name);
            if (snippet != NULL)
                return Py_BuildValue("s", snippet);
        }
    }
    Py_RETURN_NONE;
}


static PyObject *
Editor_get_default_eol_char(PyObject *module, PyObject *args)
{
    const gchar *eol_char = editor_get_eol_char(NULL);
    if (eol_char != NULL)
        return Py_BuildValue("s", eol_char);
    Py_RETURN_NONE;
}


static PyObject *
Editor_get_default_eol_char_len(PyObject *module, PyObject *args)
{
    return Py_BuildValue("i", editor_get_eol_char_len(NULL));
}


static PyObject *
Editor_get_default_eol_char_mode(PyObject *module, PyObject *args)
{
    return Py_BuildValue("i", editor_get_eol_char_mode(NULL));
}


static PyObject *
Editor_get_default_eol_char_name(PyObject *module, PyObject *args)
{
    const gchar *eol_char_name = editor_get_eol_char_name(NULL);
    if (eol_char_name != NULL)
        return Py_BuildValue("s", eol_char_name);
    Py_RETURN_NONE;
}


static PyObject *
Editor_get_default_indent_prefs(PyObject *module, PyObject *args)
{
    const GeanyIndentPrefs *indent_prefs;
    IndentPrefs *py_prefs;
    indent_prefs = editor_get_indent_prefs(NULL);
    if (indent_prefs != NULL)
    {
        py_prefs = IndentPrefs_create_new_from_geany_indent_prefs((GeanyIndentPrefs *)indent_prefs);
        return (PyObject *) py_prefs;
    }
    Py_RETURN_NONE;
}


static
PyMethodDef EditorModule_methods[] = {
    { "find_snippet", (PyCFunction) Editor__find_snippet, METH_VARARGS },
    { "get_default_eol_char", (PyCFunction) Editor_get_default_eol_char, METH_VARARGS },
    { "get_default_eol_char_len", (PyCFunction) Editor_get_default_eol_char_len, METH_VARARGS },
    { "get_default_eol_char_mode", (PyCFunction) Editor_get_default_eol_char_mode, METH_VARARGS },
    { "get_default_eol_char_name", (PyCFunction) Editor_get_default_eol_char_name, METH_VARARGS },
    { "get_default_indent_prefs", (PyCFunction) Editor_get_default_indent_prefs, METH_VARARGS },
    { NULL }
};


PyMODINIT_FUNC
init_geany_editor(void)
{
    PyObject *m;

    EditorType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&EditorType) < 0)
        return;

    m = Py_InitModule("_geany_editor", EditorModule_methods);

    Py_INCREF(&EditorType);
    PyModule_AddObject(m, "Editor", (PyObject *)&EditorType);
}


Editor *Editor_create_new_from_geany_editor(GeanyEditor *editor)
{
    Editor *self;
    self = (Editor *) PyObject_CallObject((PyObject *) &EditorType, NULL);
    self->editor = editor;
    return self;
}
