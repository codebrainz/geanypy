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


/* Set by Geany when plugin is loaded */
extern GeanyPlugin		*geany_plugin;
extern GeanyData		*geany_data;
extern GeanyFunctions	*geany_functions;


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
EditorPrefs__get_indentation (EditorPrefs *self, PyObject *args)
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
EditorPrefs__set_indentation (EditorPrefs *self, PyObject *args)
{
    IndentPrefs *prefs = NULL;
    if (PyArg_ParseTuple(args, "O", &prefs))
    {
        if (prefs != NULL)
            self->editor_prefs->indentation = (GeanyIndentPrefs *) prefs->indent_prefs;
    }
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs__get_show_white_space (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->show_white_space);
    Py_RETURN_NONE;
}

static PyObject *
EditorPrefs__set_show_white_space (EditorPrefs *self, PyObject *args)
{
    gboolean value;
    if (PyArg_ParseTuple(args, "i", &value))
        self->editor_prefs->show_white_space = (gboolean) value;
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs__get_show_indent_guide (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->show_indent_guide);
    Py_RETURN_NONE;
}

static PyObject *
EditorPrefs__set_show_indent_guide (EditorPrefs *self, PyObject *args)
{
    gboolean value;
    if (PyArg_ParseTuple(args, "i", &value))
        self->editor_prefs->show_indent_guide = (gboolean) value;
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs__get_show_line_endings (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->show_line_endings);
    Py_RETURN_NONE;
}

static PyObject *
EditorPrefs__set_show_line_endings (EditorPrefs *self, PyObject *args)
{
    gboolean value;
    if (PyArg_ParseTuple(args, "i", &value))
        self->editor_prefs->show_line_endings = (gboolean) value;
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs__get_long_line_type (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->long_line_type);
    Py_RETURN_NONE;
}

static PyObject *
EditorPrefs__set_long_line_type (EditorPrefs *self, PyObject *args)
{
    gint value;
    if (PyArg_ParseTuple(args, "i", &value))
        self->editor_prefs->long_line_type = (gint) value;
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs__get_long_line_column (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->long_line_column);
    Py_RETURN_NONE;
}

static PyObject *
EditorPrefs__set_long_line_column (EditorPrefs *self, PyObject *args)
{
    gint value;
    if (PyArg_ParseTuple(args, "i", &value))
        self->editor_prefs->long_line_column = (gint) value;
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs__get_long_line_color (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("s", self->editor_prefs->long_line_color);
    Py_RETURN_NONE;
}

static PyObject *
EditorPrefs__set_long_line_color (EditorPrefs *self, PyObject *args)
{
    gchar * value;
    if (PyArg_ParseTuple(args, "s", &value))
        self->editor_prefs->long_line_color = (gchar *) value;
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs__get_show_markers_margin (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->show_markers_margin);
    Py_RETURN_NONE;
}

static PyObject *
EditorPrefs__set_show_markers_margin (EditorPrefs *self, PyObject *args)
{
    gboolean value;
    if (PyArg_ParseTuple(args, "i", &value))
        self->editor_prefs->show_markers_margin = (gboolean) value;
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs__get_show_linenumber_margin (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->show_linenumber_margin);
    Py_RETURN_NONE;
}

static PyObject *
EditorPrefs__set_show_linenumber_margin (EditorPrefs *self, PyObject *args)
{
    gboolean value;
    if (PyArg_ParseTuple(args, "i", &value))
        self->editor_prefs->show_linenumber_margin = (gboolean) value;
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs__get_show_scrollbars (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->show_scrollbars);
    Py_RETURN_NONE;
}

static PyObject *
EditorPrefs__set_show_scrollbars (EditorPrefs *self, PyObject *args)
{
    gboolean value;
    if (PyArg_ParseTuple(args, "i", &value))
        self->editor_prefs->show_scrollbars = (gboolean) value;
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs__get_scroll_stop_at_last_line (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->scroll_stop_at_last_line);
    Py_RETURN_NONE;
}

static PyObject *
EditorPrefs__set_scroll_stop_at_last_line (EditorPrefs *self, PyObject *args)
{
    gboolean value;
    if (PyArg_ParseTuple(args, "i", &value))
        self->editor_prefs->scroll_stop_at_last_line = (gboolean) value;
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs__get_line_wrapping (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->line_wrapping);
    Py_RETURN_NONE;
}

static PyObject *
EditorPrefs__set_line_wrapping (EditorPrefs *self, PyObject *args)
{
    gboolean value;
    if (PyArg_ParseTuple(args, "i", &value))
        self->editor_prefs->line_wrapping = (gboolean) value;
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs__get_use_indicators (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->use_indicators);
    Py_RETURN_NONE;
}

static PyObject *
EditorPrefs__set_use_indicators (EditorPrefs *self, PyObject *args)
{
    gboolean value;
    if (PyArg_ParseTuple(args, "i", &value))
        self->editor_prefs->use_indicators = (gboolean) value;
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs__get_folding (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->folding);
    Py_RETURN_NONE;
}

static PyObject *
EditorPrefs__set_folding (EditorPrefs *self, PyObject *args)
{
    gboolean value;
    if (PyArg_ParseTuple(args, "i", &value))
        self->editor_prefs->folding = (gboolean) value;
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs__get_unfold_all_children (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->unfold_all_children);
    Py_RETURN_NONE;
}

static PyObject *
EditorPrefs__set_unfold_all_children (EditorPrefs *self, PyObject *args)
{
    gboolean value;
    if (PyArg_ParseTuple(args, "i", &value))
        self->editor_prefs->unfold_all_children = (gboolean) value;
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs__get_disable_dnd (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->disable_dnd);
    Py_RETURN_NONE;
}

static PyObject *
EditorPrefs__set_disable_dnd (EditorPrefs *self, PyObject *args)
{
    gboolean value;
    if (PyArg_ParseTuple(args, "i", &value))
        self->editor_prefs->disable_dnd = (gboolean) value;
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs__get_use_tab_to_indent (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->use_tab_to_indent);
    Py_RETURN_NONE;
}

static PyObject *
EditorPrefs__set_use_tab_to_indent (EditorPrefs *self, PyObject *args)
{
    gboolean value;
    if (PyArg_ParseTuple(args, "i", &value))
        self->editor_prefs->use_tab_to_indent = (gboolean) value;
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs__get_smart_home_key (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->smart_home_key);
    Py_RETURN_NONE;
}

static PyObject *
EditorPrefs__set_smart_home_key (EditorPrefs *self, PyObject *args)
{
    gboolean value;
    if (PyArg_ParseTuple(args, "i", &value))
        self->editor_prefs->smart_home_key = (gboolean) value;
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs__get_newline_strip (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->newline_strip);
    Py_RETURN_NONE;
}

static PyObject *
EditorPrefs__set_newline_strip (EditorPrefs *self, PyObject *args)
{
    gboolean value;
    if (PyArg_ParseTuple(args, "i", &value))
        self->editor_prefs->newline_strip = (gboolean) value;
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs__get_auto_complete_symbols (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->auto_complete_symbols);
    Py_RETURN_NONE;
}

static PyObject *
EditorPrefs__set_auto_complete_symbols (EditorPrefs *self, PyObject *args)
{
    gboolean value;
    if (PyArg_ParseTuple(args, "i", &value))
        self->editor_prefs->auto_complete_symbols = (gboolean) value;
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs__get_auto_close_xml_tags (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->auto_close_xml_tags);
    Py_RETURN_NONE;
}

static PyObject *
EditorPrefs__set_auto_close_xml_tags (EditorPrefs *self, PyObject *args)
{
    gboolean value;
    if (PyArg_ParseTuple(args, "i", &value))
        self->editor_prefs->auto_close_xml_tags = (gboolean) value;
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs__get_complete_snippets (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->complete_snippets);
    Py_RETURN_NONE;
}

static PyObject *
EditorPrefs__set_complete_snippets (EditorPrefs *self, PyObject *args)
{
    gboolean value;
    if (PyArg_ParseTuple(args, "i", &value))
        self->editor_prefs->complete_snippets = (gboolean) value;
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs__get_symbolcompletion_min_chars (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->symbolcompletion_min_chars);
    Py_RETURN_NONE;
}

static PyObject *
EditorPrefs__set_symbolcompletion_min_chars (EditorPrefs *self, PyObject *args)
{
    gint value;
    if (PyArg_ParseTuple(args, "i", &value))
        self->editor_prefs->symbolcompletion_min_chars = (gint) value;
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs__get_symbolcompletion_max_height (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->symbolcompletion_max_height);
    Py_RETURN_NONE;
}

static PyObject *
EditorPrefs__set_symbolcompletion_max_height (EditorPrefs *self, PyObject *args)
{
    gint value;
    if (PyArg_ParseTuple(args, "i", &value))
        self->editor_prefs->symbolcompletion_max_height = (gint) value;
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs__get_brace_match_ltgt (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->brace_match_ltgt);
    Py_RETURN_NONE;
}

static PyObject *
EditorPrefs__set_brace_match_ltgt (EditorPrefs *self, PyObject *args)
{
    gboolean value;
    if (PyArg_ParseTuple(args, "i", &value))
        self->editor_prefs->brace_match_ltgt = (gboolean) value;
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs__get_use_gtk_word_boundaries (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->use_gtk_word_boundaries);
    Py_RETURN_NONE;
}

static PyObject *
EditorPrefs__set_use_gtk_word_boundaries (EditorPrefs *self, PyObject *args)
{
    gboolean value;
    if (PyArg_ParseTuple(args, "i", &value))
        self->editor_prefs->use_gtk_word_boundaries = (gboolean) value;
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs__get_complete_snippets_whilst_editing (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->complete_snippets_whilst_editing);
    Py_RETURN_NONE;
}

static PyObject *
EditorPrefs__set_complete_snippets_whilst_editing (EditorPrefs *self, PyObject *args)
{
    gboolean value;
    if (PyArg_ParseTuple(args, "i", &value))
        self->editor_prefs->complete_snippets_whilst_editing = (gboolean) value;
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs__get_line_break_column (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->line_break_column);
    Py_RETURN_NONE;
}

static PyObject *
EditorPrefs__set_line_break_column (EditorPrefs *self, PyObject *args)
{
    gint value;
    if (PyArg_ParseTuple(args, "i", &value))
        self->editor_prefs->line_break_column = (gint) value;
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs__get_auto_continue_multiline (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->auto_continue_multiline);
    Py_RETURN_NONE;
}

static PyObject *
EditorPrefs__set_auto_continue_multiline (EditorPrefs *self, PyObject *args)
{
    gboolean value;
    if (PyArg_ParseTuple(args, "i", &value))
        self->editor_prefs->auto_continue_multiline = (gboolean) value;
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs__get_comment_toggle_mark (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("s", self->editor_prefs->comment_toggle_mark);
    Py_RETURN_NONE;
}

static PyObject *
EditorPrefs__set_comment_toggle_mark (EditorPrefs *self, PyObject *args)
{
    gchar * value;
    if (PyArg_ParseTuple(args, "s", &value))
        self->editor_prefs->comment_toggle_mark = (gchar *) value;
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs__get_autocompletion_max_entries (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("?", self->editor_prefs->autocompletion_max_entries);
    Py_RETURN_NONE;
}

static PyObject *
EditorPrefs__set_autocompletion_max_entries (EditorPrefs *self, PyObject *args)
{
    guint value;
    if (PyArg_ParseTuple(args, "?", &value))
        self->editor_prefs->autocompletion_max_entries = (guint) value;
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs__get_autoclose_chars (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("?", self->editor_prefs->autoclose_chars);
    Py_RETURN_NONE;
}

static PyObject *
EditorPrefs__set_autoclose_chars (EditorPrefs *self, PyObject *args)
{
    guint value;
    if (PyArg_ParseTuple(args, "?", &value))
        self->editor_prefs->autoclose_chars = (guint) value;
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs__get_autocomplete_doc_words (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->autocomplete_doc_words);
    Py_RETURN_NONE;
}

static PyObject *
EditorPrefs__set_autocomplete_doc_words (EditorPrefs *self, PyObject *args)
{
    gboolean value;
    if (PyArg_ParseTuple(args, "i", &value))
        self->editor_prefs->autocomplete_doc_words = (gboolean) value;
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs__get_completion_drops_rest_of_word (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->completion_drops_rest_of_word);
    Py_RETURN_NONE;
}

static PyObject *
EditorPrefs__set_completion_drops_rest_of_word (EditorPrefs *self, PyObject *args)
{
    gboolean value;
    if (PyArg_ParseTuple(args, "i", &value))
        self->editor_prefs->completion_drops_rest_of_word = (gboolean) value;
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs__get_color_scheme (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("s", self->editor_prefs->color_scheme);
    Py_RETURN_NONE;
}

static PyObject *
EditorPrefs__set_color_scheme (EditorPrefs *self, PyObject *args)
{
    gchar * value;
    if (PyArg_ParseTuple(args, "s", &value))
        self->editor_prefs->color_scheme = (gchar *) value;
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs__get_show_virtual_space (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->show_virtual_space);
    Py_RETURN_NONE;
}

static PyObject *
EditorPrefs__set_show_virtual_space (EditorPrefs *self, PyObject *args)
{
    gint value;
    if (PyArg_ParseTuple(args, "i", &value))
        self->editor_prefs->show_virtual_space = (gint) value;
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs__get_long_line_enabled (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->long_line_enabled);
    Py_RETURN_NONE;
}

static PyObject *
EditorPrefs__set_long_line_enabled (EditorPrefs *self, PyObject *args)
{
    gboolean value;
    if (PyArg_ParseTuple(args, "i", &value))
        self->editor_prefs->long_line_enabled = (gboolean) value;
    Py_RETURN_NONE;
}


static PyObject *
EditorPrefs__get_autocompletion_update_freq (EditorPrefs *self, PyObject *args)
{
    if (self->editor_prefs != NULL)
        return Py_BuildValue("i", self->editor_prefs->autocompletion_update_freq);
    Py_RETURN_NONE;
}

static PyObject *
EditorPrefs__set_autocompletion_update_freq (EditorPrefs *self, PyObject *args)
{
    gint value;
    if (PyArg_ParseTuple(args, "i", &value))
        self->editor_prefs->autocompletion_update_freq = (gint) value;
    Py_RETURN_NONE;
}



static PyMethodDef EditorPrefs_methods[] = {
	{ "_get_indentation;", (PyCFunction) EditorPrefs__get_indentation, METH_VARARGS },
	{ "_set_indentation;", (PyCFunction) EditorPrefs__set_indentation, METH_VARARGS },
	{ "_get_show_white_space;", (PyCFunction) EditorPrefs__get_show_white_space, METH_VARARGS },
	{ "_set_show_white_space;", (PyCFunction) EditorPrefs__set_show_white_space, METH_VARARGS },
	{ "_get_show_indent_guide;", (PyCFunction) EditorPrefs__get_show_indent_guide, METH_VARARGS },
	{ "_set_show_indent_guide;", (PyCFunction) EditorPrefs__set_show_indent_guide, METH_VARARGS },
	{ "_get_show_line_endings;", (PyCFunction) EditorPrefs__get_show_line_endings, METH_VARARGS },
	{ "_set_show_line_endings;", (PyCFunction) EditorPrefs__set_show_line_endings, METH_VARARGS },
	{ "_get_long_line_type;", (PyCFunction) EditorPrefs__get_long_line_type, METH_VARARGS },
	{ "_set_long_line_type;", (PyCFunction) EditorPrefs__set_long_line_type, METH_VARARGS },
	{ "_get_long_line_column;", (PyCFunction) EditorPrefs__get_long_line_column, METH_VARARGS },
	{ "_set_long_line_column;", (PyCFunction) EditorPrefs__set_long_line_column, METH_VARARGS },
	{ "_get_long_line_color;", (PyCFunction) EditorPrefs__get_long_line_color, METH_VARARGS },
	{ "_set_long_line_color;", (PyCFunction) EditorPrefs__set_long_line_color, METH_VARARGS },
	{ "_get_show_markers_margin;", (PyCFunction) EditorPrefs__get_show_markers_margin, METH_VARARGS },
	{ "_set_show_markers_margin;", (PyCFunction) EditorPrefs__set_show_markers_margin, METH_VARARGS },
	{ "_get_show_linenumber_margin;", (PyCFunction) EditorPrefs__get_show_linenumber_margin, METH_VARARGS },
	{ "_set_show_linenumber_margin;", (PyCFunction) EditorPrefs__set_show_linenumber_margin, METH_VARARGS },
	{ "_get_show_scrollbars;", (PyCFunction) EditorPrefs__get_show_scrollbars, METH_VARARGS },
	{ "_set_show_scrollbars;", (PyCFunction) EditorPrefs__set_show_scrollbars, METH_VARARGS },
	{ "_get_scroll_stop_at_last_line;", (PyCFunction) EditorPrefs__get_scroll_stop_at_last_line, METH_VARARGS },
	{ "_set_scroll_stop_at_last_line;", (PyCFunction) EditorPrefs__set_scroll_stop_at_last_line, METH_VARARGS },
	{ "_get_line_wrapping;", (PyCFunction) EditorPrefs__get_line_wrapping, METH_VARARGS },
	{ "_set_line_wrapping;", (PyCFunction) EditorPrefs__set_line_wrapping, METH_VARARGS },
	{ "_get_use_indicators;", (PyCFunction) EditorPrefs__get_use_indicators, METH_VARARGS },
	{ "_set_use_indicators;", (PyCFunction) EditorPrefs__set_use_indicators, METH_VARARGS },
	{ "_get_folding;", (PyCFunction) EditorPrefs__get_folding, METH_VARARGS },
	{ "_set_folding;", (PyCFunction) EditorPrefs__set_folding, METH_VARARGS },
	{ "_get_unfold_all_children;", (PyCFunction) EditorPrefs__get_unfold_all_children, METH_VARARGS },
	{ "_set_unfold_all_children;", (PyCFunction) EditorPrefs__set_unfold_all_children, METH_VARARGS },
	{ "_get_disable_dnd;", (PyCFunction) EditorPrefs__get_disable_dnd, METH_VARARGS },
	{ "_set_disable_dnd;", (PyCFunction) EditorPrefs__set_disable_dnd, METH_VARARGS },
	{ "_get_use_tab_to_indent;", (PyCFunction) EditorPrefs__get_use_tab_to_indent, METH_VARARGS },
	{ "_set_use_tab_to_indent;", (PyCFunction) EditorPrefs__set_use_tab_to_indent, METH_VARARGS },
	{ "_get_smart_home_key;", (PyCFunction) EditorPrefs__get_smart_home_key, METH_VARARGS },
	{ "_set_smart_home_key;", (PyCFunction) EditorPrefs__set_smart_home_key, METH_VARARGS },
	{ "_get_newline_strip;", (PyCFunction) EditorPrefs__get_newline_strip, METH_VARARGS },
	{ "_set_newline_strip;", (PyCFunction) EditorPrefs__set_newline_strip, METH_VARARGS },
	{ "_get_auto_complete_symbols;", (PyCFunction) EditorPrefs__get_auto_complete_symbols, METH_VARARGS },
	{ "_set_auto_complete_symbols;", (PyCFunction) EditorPrefs__set_auto_complete_symbols, METH_VARARGS },
	{ "_get_auto_close_xml_tags;", (PyCFunction) EditorPrefs__get_auto_close_xml_tags, METH_VARARGS },
	{ "_set_auto_close_xml_tags;", (PyCFunction) EditorPrefs__set_auto_close_xml_tags, METH_VARARGS },
	{ "_get_complete_snippets;", (PyCFunction) EditorPrefs__get_complete_snippets, METH_VARARGS },
	{ "_set_complete_snippets;", (PyCFunction) EditorPrefs__set_complete_snippets, METH_VARARGS },
	{ "_get_symbolcompletion_min_chars;", (PyCFunction) EditorPrefs__get_symbolcompletion_min_chars, METH_VARARGS },
	{ "_set_symbolcompletion_min_chars;", (PyCFunction) EditorPrefs__set_symbolcompletion_min_chars, METH_VARARGS },
	{ "_get_symbolcompletion_max_height;", (PyCFunction) EditorPrefs__get_symbolcompletion_max_height, METH_VARARGS },
	{ "_set_symbolcompletion_max_height;", (PyCFunction) EditorPrefs__set_symbolcompletion_max_height, METH_VARARGS },
	{ "_get_brace_match_ltgt;", (PyCFunction) EditorPrefs__get_brace_match_ltgt, METH_VARARGS },
	{ "_set_brace_match_ltgt;", (PyCFunction) EditorPrefs__set_brace_match_ltgt, METH_VARARGS },
	{ "_get_use_gtk_word_boundaries;", (PyCFunction) EditorPrefs__get_use_gtk_word_boundaries, METH_VARARGS },
	{ "_set_use_gtk_word_boundaries;", (PyCFunction) EditorPrefs__set_use_gtk_word_boundaries, METH_VARARGS },
	{ "_get_complete_snippets_whilst_editing;", (PyCFunction) EditorPrefs__get_complete_snippets_whilst_editing, METH_VARARGS },
	{ "_set_complete_snippets_whilst_editing;", (PyCFunction) EditorPrefs__set_complete_snippets_whilst_editing, METH_VARARGS },
	{ "_get_line_break_column;", (PyCFunction) EditorPrefs__get_line_break_column, METH_VARARGS },
	{ "_set_line_break_column;", (PyCFunction) EditorPrefs__set_line_break_column, METH_VARARGS },
	{ "_get_auto_continue_multiline;", (PyCFunction) EditorPrefs__get_auto_continue_multiline, METH_VARARGS },
	{ "_set_auto_continue_multiline;", (PyCFunction) EditorPrefs__set_auto_continue_multiline, METH_VARARGS },
	{ "_get_comment_toggle_mark;", (PyCFunction) EditorPrefs__get_comment_toggle_mark, METH_VARARGS },
	{ "_set_comment_toggle_mark;", (PyCFunction) EditorPrefs__set_comment_toggle_mark, METH_VARARGS },
	{ "_get_autocompletion_max_entries;", (PyCFunction) EditorPrefs__get_autocompletion_max_entries, METH_VARARGS },
	{ "_set_autocompletion_max_entries;", (PyCFunction) EditorPrefs__set_autocompletion_max_entries, METH_VARARGS },
	{ "_get_autoclose_chars;", (PyCFunction) EditorPrefs__get_autoclose_chars, METH_VARARGS },
	{ "_set_autoclose_chars;", (PyCFunction) EditorPrefs__set_autoclose_chars, METH_VARARGS },
	{ "_get_autocomplete_doc_words;", (PyCFunction) EditorPrefs__get_autocomplete_doc_words, METH_VARARGS },
	{ "_set_autocomplete_doc_words;", (PyCFunction) EditorPrefs__set_autocomplete_doc_words, METH_VARARGS },
	{ "_get_completion_drops_rest_of_word;", (PyCFunction) EditorPrefs__get_completion_drops_rest_of_word, METH_VARARGS },
	{ "_set_completion_drops_rest_of_word;", (PyCFunction) EditorPrefs__set_completion_drops_rest_of_word, METH_VARARGS },
	{ "_get_color_scheme;", (PyCFunction) EditorPrefs__get_color_scheme, METH_VARARGS },
	{ "_set_color_scheme;", (PyCFunction) EditorPrefs__set_color_scheme, METH_VARARGS },
	{ "_get_show_virtual_space;", (PyCFunction) EditorPrefs__get_show_virtual_space, METH_VARARGS },
	{ "_set_show_virtual_space;", (PyCFunction) EditorPrefs__set_show_virtual_space, METH_VARARGS },
	{ "_get_long_line_enabled;", (PyCFunction) EditorPrefs__get_long_line_enabled, METH_VARARGS },
	{ "_set_long_line_enabled;", (PyCFunction) EditorPrefs__set_long_line_enabled, METH_VARARGS },
	{ "_get_autocompletion_update_freq;", (PyCFunction) EditorPrefs__get_autocompletion_update_freq, METH_VARARGS },
	{ "_set_autocompletion_update_freq;", (PyCFunction) EditorPrefs__set_autocompletion_update_freq, METH_VARARGS },
	{ NULL }
};


static PyTypeObject EditorPrefsType = {
	PyObject_HEAD_INIT(NULL)
    0,                          /*ob_size*/
    "_geany_editor_prefs.EditorPrefs",  /*tp_name*/
    sizeof(EditorPrefs),        /*tp_basicsize*/
    0,                          /*tp_itemsize*/
    (destructor)EditorPrefs_dealloc, /*tp_dealloc*/
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
    "Geany Editor preferences", /* tp_doc */
    0,		                    /* tp_traverse */
    0,		               	    /* tp_clear */
    0,		                    /* tp_richcompare */
    0,		                    /* tp_weaklistoffset */
    0,		                    /* tp_iter */
    0,		                    /* tp_iternext */
    EditorPrefs_methods,        /* tp_methods */
    0,                          /* tp_members */
    0,                          /* tp_getset */
    0,                          /* tp_base */
    0,                          /* tp_dict */
    0,                          /* tp_descr_get */
    0,                          /* tp_descr_set */
    0,                          /* tp_dictoffset */
    (initproc)EditorPrefs_init, /* tp_init */
    0,                          /* tp_alloc */
    0,                          /* tp_new */
};


static
PyMethodDef EditorPrefsModule_methods[] = {
    { NULL }
};


PyMODINIT_FUNC
init_geany_editor_prefs(void)
{
    PyObject *m;

    EditorPrefsType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&EditorPrefsType) < 0)
        return;

    m = Py_InitModule("_geany_editor_prefs", EditorPrefsModule_methods);

    Py_INCREF(&EditorPrefsType);
    PyModule_AddObject(m, "EditorPrefs", (PyObject *)&EditorPrefsType);
}
