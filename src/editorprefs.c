/*
 * editorprefs.c - See Geany's editor.h
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
EditorPrefs_dealloc(EditorPrefs *self)
{
	self->ob_type->tp_free((PyObject *) self);
}


static int
EditorPrefs_init(EditorPrefs *self, PyObject *args, PyObject *kwds)
{
    self->editor_prefs = geany_data->editor_prefs;
	return 0;
}


static PyObject *
EditorPrefs_get_indentation (EditorPrefs *self, PyObject *args)
{
    IndentPrefs *prefs;
    if (self->editor_prefs != NULL)
    {
        prefs = IndentPrefs_create_new_from_geany_indent_prefs(self->editor_prefs->indentation);
        return Py_BuildValue("O", (PyObject *) prefs);
    }
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs_get_show_white_space (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->show_white_space);
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs_get_show_indent_guide (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->show_indent_guide);
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs_get_show_line_endings (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->show_line_endings);
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs_get_long_line_type (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->long_line_type);
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs_get_long_line_column (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->long_line_column);
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs_get_long_line_color (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("s", self->editor_prefs->long_line_color);
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs_get_show_markers_margin (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->show_markers_margin);
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs_get_show_linenumber_margin (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->show_linenumber_margin);
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs_get_show_scrollbars (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->show_scrollbars);
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs_get_scroll_stop_at_last_line (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->scroll_stop_at_last_line);
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs_get_line_wrapping (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->line_wrapping);
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs_get_use_indicators (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->use_indicators);
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs_get_folding (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->folding);
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs_get_unfold_all_children (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->unfold_all_children);
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs_get_disable_dnd (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->disable_dnd);
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs_get_use_tab_to_indent (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->use_tab_to_indent);
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs_get_smart_home_key (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->smart_home_key);
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs_get_newline_strip (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->newline_strip);
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs_get_auto_complete_symbols (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->auto_complete_symbols);
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs_get_auto_close_xml_tags (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->auto_close_xml_tags);
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs_get_complete_snippets (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->complete_snippets);
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs_get_symbolcompletion_min_chars (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->symbolcompletion_min_chars);
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs_get_symbolcompletion_max_height (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->symbolcompletion_max_height);
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs_get_brace_match_ltgt (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->brace_match_ltgt);
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs_get_use_gtk_word_boundaries (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->use_gtk_word_boundaries);
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs_get_complete_snippets_whilst_editing (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->complete_snippets_whilst_editing);
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs_get_line_break_column (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->line_break_column);
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs_get_auto_continue_multiline (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->auto_continue_multiline);
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs_get_comment_toggle_mark (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("s", self->editor_prefs->comment_toggle_mark);
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs_get_autocompletion_max_entries (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("?", self->editor_prefs->autocompletion_max_entries);
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs_get_autoclose_chars (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("?", self->editor_prefs->autoclose_chars);
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs_get_autocomplete_doc_words (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->autocomplete_doc_words);
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs_get_completion_drops_rest_of_word (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->completion_drops_rest_of_word);
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs_get_color_scheme (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("s", self->editor_prefs->color_scheme);
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs_get_show_virtual_space (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->show_virtual_space);
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs_get_long_line_enabled (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->long_line_enabled);
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs_get_autocompletion_update_freq (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->autocompletion_update_freq);
    Py_RETURN_NONE;
}



static PyMethodDef EditorPrefs_methods[] = {
	{ "get_indentation", (PyCFunction) EditorPrefs_get_indentation, METH_VARARGS },
	{ "get_show_white_space", (PyCFunction) EditorPrefs_get_show_white_space, METH_VARARGS },
	{ "get_show_indent_guide", (PyCFunction) EditorPrefs_get_show_indent_guide, METH_VARARGS },
	{ "get_show_line_endings", (PyCFunction) EditorPrefs_get_show_line_endings, METH_VARARGS },
	{ "get_long_line_type", (PyCFunction) EditorPrefs_get_long_line_type, METH_VARARGS },
	{ "get_long_line_column", (PyCFunction) EditorPrefs_get_long_line_column, METH_VARARGS },
	{ "get_long_line_color", (PyCFunction) EditorPrefs_get_long_line_color, METH_VARARGS },
	{ "get_show_markers_margin", (PyCFunction) EditorPrefs_get_show_markers_margin, METH_VARARGS },
	{ "get_show_linenumber_margin", (PyCFunction) EditorPrefs_get_show_linenumber_margin, METH_VARARGS },
	{ "get_show_scrollbars", (PyCFunction) EditorPrefs_get_show_scrollbars, METH_VARARGS },
	{ "get_scroll_stop_at_last_line", (PyCFunction) EditorPrefs_get_scroll_stop_at_last_line, METH_VARARGS },
	{ "get_line_wrapping", (PyCFunction) EditorPrefs_get_line_wrapping, METH_VARARGS },
	{ "get_use_indicators", (PyCFunction) EditorPrefs_get_use_indicators, METH_VARARGS },
	{ "get_folding", (PyCFunction) EditorPrefs_get_folding, METH_VARARGS },
	{ "get_unfold_all_children", (PyCFunction) EditorPrefs_get_unfold_all_children, METH_VARARGS },
	{ "get_disable_dnd", (PyCFunction) EditorPrefs_get_disable_dnd, METH_VARARGS },
	{ "get_use_tab_to_indent", (PyCFunction) EditorPrefs_get_use_tab_to_indent, METH_VARARGS },
	{ "get_smart_home_key", (PyCFunction) EditorPrefs_get_smart_home_key, METH_VARARGS },
	{ "get_newline_strip", (PyCFunction) EditorPrefs_get_newline_strip, METH_VARARGS },
	{ "get_auto_complete_symbols", (PyCFunction) EditorPrefs_get_auto_complete_symbols, METH_VARARGS },
	{ "get_auto_close_xml_tags", (PyCFunction) EditorPrefs_get_auto_close_xml_tags, METH_VARARGS },
	{ "get_complete_snippets", (PyCFunction) EditorPrefs_get_complete_snippets, METH_VARARGS },
	{ "get_symbolcompletion_min_chars", (PyCFunction) EditorPrefs_get_symbolcompletion_min_chars, METH_VARARGS },
	{ "get_symbolcompletion_max_height", (PyCFunction) EditorPrefs_get_symbolcompletion_max_height, METH_VARARGS },
	{ "get_brace_match_ltgt", (PyCFunction) EditorPrefs_get_brace_match_ltgt, METH_VARARGS },
	{ "get_use_gtk_word_boundaries", (PyCFunction) EditorPrefs_get_use_gtk_word_boundaries, METH_VARARGS },
	{ "get_complete_snippets_whilst_editing", (PyCFunction) EditorPrefs_get_complete_snippets_whilst_editing, METH_VARARGS },
	{ "get_line_break_column", (PyCFunction) EditorPrefs_get_line_break_column, METH_VARARGS },
	{ "get_auto_continue_multiline", (PyCFunction) EditorPrefs_get_auto_continue_multiline, METH_VARARGS },
	{ "get_comment_toggle_mark", (PyCFunction) EditorPrefs_get_comment_toggle_mark, METH_VARARGS },
	{ "get_autocompletion_max_entries", (PyCFunction) EditorPrefs_get_autocompletion_max_entries, METH_VARARGS },
	{ "get_autoclose_chars", (PyCFunction) EditorPrefs_get_autoclose_chars, METH_VARARGS },
	{ "get_autocomplete_doc_words", (PyCFunction) EditorPrefs_get_autocomplete_doc_words, METH_VARARGS },
	{ "get_completion_drops_rest_of_word", (PyCFunction) EditorPrefs_get_completion_drops_rest_of_word, METH_VARARGS },
	{ "get_color_scheme", (PyCFunction) EditorPrefs_get_color_scheme, METH_VARARGS },
	{ "get_show_virtual_space", (PyCFunction) EditorPrefs_get_show_virtual_space, METH_VARARGS },
	{ "get_long_line_enabled", (PyCFunction) EditorPrefs_get_long_line_enabled, METH_VARARGS },
	{ "get_autocompletion_update_freq", (PyCFunction) EditorPrefs_get_autocompletion_update_freq, METH_VARARGS },
	{ NULL }
};


static PyTypeObject EditorPrefsType = {
	PyObject_HEAD_INIT(NULL)
    0,                          /*ob_size*/
    "geany.editorprefs.EditorPrefs",  /*tp_name*/
    sizeof(EditorPrefs),        /*tp_basicsize*/
    0,                          /*tp_itemsize*/
    (destructor)EditorPrefs_dealloc, /*tp_dealloc*/
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
    "Wrapper class around Geany's `GeanyEditorPrefs` structure.  This class "
    "should not be directly initialized, instead access a single instance of "
    "it through `geany.editor_prefs`.", /* tp_doc */
    0,		                    /* tp_traverse */
    0,		               	    /* tp_clear */
    0,		                    /* tp_richcompare */
    0,		                    /* tp_weaklistoffset */
    0,		                    /* tp_iter */
    0,		                    /* tp_iternext */
    EditorPrefs_methods,        /* tp_methods */
    0,                          /* tp_members */
    0,                          /* tpgetset */
    0,                          /* tp_base */
    0,                          /* tp_dict */
    0,                          /* tp_descrget */
    0,                          /* tp_descrset */
    0,                          /* tp_dictoffset */
    (initproc)EditorPrefs_init, /* tp_init */
    0,                          /* tp_alloc */
    0,                          /* tp_new */
};


static PyMethodDef EditorPrefsModule_methods[] = { { NULL } };


PyMODINIT_FUNC
initeditorprefs(void)
{
    PyObject *m;

    EditorPrefsType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&EditorPrefsType) < 0)
        return;

    m = Py_InitModule3("editorprefs", EditorPrefsModule_methods,
            "The `editorprefs` module provides a single wrapper class around a  "
            "`GeanyEditorPrefs` structure.");

    Py_INCREF(&EditorPrefsType);
    PyModule_AddObject(m, "EditorPrefs", (PyObject *)&EditorPrefsType);
}
