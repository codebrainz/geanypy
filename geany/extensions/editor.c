#include <Python.h>
#include <structmember.h>
#include <geanyplugin.h>

#ifndef GTK
#  define GTK
#endif
#include <Scintilla.h>
#include <ScintillaWidget.h>

#include "modules-common.h"


typedef struct
{
	PyObject_HEAD
	GeanyEditorPrefs *editor_prefs;
    PyObject *indent_prefs;
} EditorPrefs;
static PyTypeObject EditorPrefsType;


typedef struct
{
	PyObject_HEAD
	GeanyIndentPrefs *indent_prefs;
} IndentPrefs;
static PyTypeObject IndentPrefsType;


typedef struct
{
	PyObject_HEAD
	GeanyEditor *editor;
    PyObject *sci;
    PyObject *indent_prefs;
} Editor;
static PyTypeObject EditorType;


static void
EditorPrefs_dealloc(EditorPrefs *self)
{
    Py_XDECREF(self->indent_prefs);
	self->ob_type->tp_free((PyObject *) self);
}


static int
EditorPrefs_init(EditorPrefs *self, PyObject *args, PyObject *kwds)
{
    self->editor_prefs = geany_data->editor_prefs;
    self->indent_prefs = NULL;
	return 0;
}


static PyObject *
EditorPrefs_get_indentation (EditorPrefs *self, PyObject *args)
{
    PyObject *pylong, *mod, *cls, *indent_prefs;
    const GeanyIndentPrefs *ip;

    if (self->editor_prefs == NULL || self->editor_prefs->indentation == NULL)
        Py_RETURN_NONE;

    if (self->indent_prefs == NULL)
    {
        self->indent_prefs = NEW_FROM_POINTER("editor.IndentPrefs",
                                self->editor_prefs->indentation);
        if (self->indent_prefs == NULL)
            Py_RETURN_NONE;
        else
            return self->indent_prefs;
    }
    else
    {
        SET_POINTER(self->indent_prefs, self->editor_prefs->indentation);
        return self->indent_prefs;
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
    "geany.editor.EditorPrefs",  /*tp_name*/
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
IndentPrefs_get_pointer(IndentPrefs *self)
{
    if (self->indent_prefs == NULL)
        Py_RETURN_NONE;
    return PyLong_FromVoidPtr((void *) self->indent_prefs);
}


static PyObject *
IndentPrefs_set_pointer(IndentPrefs *self, PyObject *ptr)
{
    if (ptr != NULL && ptr != Py_None)
        self->indent_prefs = (GeanyIndentPrefs *) PyLong_AsVoidPtr(ptr);
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
    /* Private members */
    { "_get_pointer", (PyCFunction) IndentPrefs_get_pointer, METH_NOARGS },
    { "_set_pointer", (PyCFunction) IndentPrefs_set_pointer, METH_O },

    /* Public members */
    { "get_width", (PyCFunction) IndentPrefs_get_width, METH_VARARGS },
    { "get_type", (PyCFunction) IndentPrefs_get_type, METH_VARARGS },
    { "get_hard_tab_width", (PyCFunction) IndentPrefs_get_hard_tab_width, METH_VARARGS },
    { "get_auto_indent_mode", (PyCFunction) IndentPrefs_get_auto_indent_mode, METH_VARARGS },
    { "get_detect_type", (PyCFunction) IndentPrefs_get_detect_type, METH_VARARGS },
    { "get_detect_width", (PyCFunction) IndentPrefs_get_detect_width, METH_VARARGS },
    { NULL }
};


static PyTypeObject IndentPrefsType = {
	PyObject_HEAD_INIT(NULL)
    0,                          /*ob_size*/
    "geany.editor.IndentPrefs", /*tp_name*/
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
    "Wrapper class around Geany's :c:type:`IndentPrefs` structure. "
    "This class should not be directly initialized, instead retrieve instances "
    "of it using through the :func:`Editor.get_indent_prefs` method, or "
    ":func:`editor.get_default_indent_prefs`.",          /* tp_doc */
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


static PyObject *
Editor_new(GeanyDocument *doc)
{
    PyObject *l, *p;
    if (l = PyLong_FromVoidPtr((void *) doc))
    {
        p = PyObject_CallObject((PyObject *) &EditorType, l);
        Py_DECREF(l);
        return p;
    }
    Py_RETURN_NONE;
}

static void
Editor_dealloc(Editor *self)
{
    Py_XDECREF(self->sci);
	self->ob_type->tp_free((PyObject *) self);
}


static int
Editor_init(Editor *self, PyObject *ptr)
{
    self->editor = NULL;
    self->sci = NULL;
    if (ptr != NULL && ptr != Py_None)
        self->editor = GET_POINTER(ptr, GeanyEditor);
	return 0;
}


static PyObject *
Editor_get_pointer(Editor *self)
{
    if (self->editor == NULL)
        Py_RETURN_NONE;
    return PyLong_FromVoidPtr((void *) self->editor);
}


static PyObject *
Editor_set_pointer(Editor *self, PyObject *ptr)
{
    if (ptr != NULL && ptr != Py_None)
        self->editor = (GeanyEditor *) PyLong_AsVoidPtr(ptr);
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
Editor_get_eol_char(Editor *self)
{
    const gchar *eol_char = editor_get_eol_char(self->editor);
    if (eol_char != NULL)
        return Py_BuildValue("s", eol_char);
    Py_RETURN_NONE;
}


static PyObject *
Editor_get_eol_char_len(Editor *self)
{
    return Py_BuildValue("i", editor_get_eol_char_len(self->editor));
}


static PyObject *
Editor_get_eol_char_mode(Editor *self)
{
    return Py_BuildValue("i", editor_get_eol_char_mode(self->editor));
}


static PyObject *
Editor_get_eol_char_name(Editor *self)
{
    const gchar *eol_char_name = editor_get_eol_char_name(self->editor);
    if (eol_char_name != NULL)
        return Py_BuildValue("s", eol_char_name);
    Py_RETURN_NONE;
}


static PyObject *
Editor_get_indent_prefs(Editor *self)
{
    PyObject *pylong;
    const GeanyIndentPrefs *ip;

    if (self->editor == NULL)
        Py_RETURN_NONE;

    ip = editor_get_indent_prefs(self->editor);
    if (ip == NULL)
        Py_RETURN_NONE;

    if (self->indent_prefs == NULL)
    {
        self->indent_prefs = NEW_FROM_POINTER("indentprefs.IndentPrefs", ip);
        if (self->indent_prefs == NULL)
            Py_RETURN_NONE;
        else
            return self->indent_prefs;
    }
    else
    {
        SET_POINTER(self->indent_prefs, ip);
        return self->indent_prefs;
    }
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
Editor_insert_snippet(Editor *self, PyObject *args, PyObject *kwargs)
{
    gint pos = 0;
    gchar *snippet = NULL;
    static gchar *kwlist[] = { "pos", "snippet_name", NULL };

    if (PyArg_ParseTupleAndKeywords(args, kwargs, "is", kwlist, &pos, &snippet))
        editor_insert_snippet(self->editor, pos, snippet);
    Py_RETURN_NONE;
}


static PyObject *
Editor_insert_text_block(Editor *self, PyObject *args, PyObject *kwargs)
{
    gchar *text = NULL;
    gint insert_pos, cursor_index = -1;
    gint newline_indent_size = -1;
    gint replace_newlines = 0;
    static gchar *kwlist[] = { "text", "insert_pos", "cursor_index",
        "newline_indent_size", "replace_newlines", NULL };

    if (PyArg_ParseTupleAndKeywords(args, kwargs, "si|iii", kwlist, &text,
        &insert_pos, &cursor_index, &newline_indent_size, &replace_newlines))
    {
        editor_insert_text_block(self->editor, text, insert_pos, cursor_index,
            newline_indent_size, (gboolean) replace_newlines);
    }

    Py_RETURN_NONE;
}


static PyObject *
Editor_set_indent_type(Editor *self, PyObject *args)
{
    gint indent_type;
    if (PyArg_ParseTuple(args, "i", &indent_type))
        editor_set_indent_type(self->editor, indent_type);
    Py_RETURN_NONE;
}


static PyObject *
Editor_get_scintilla(Editor *self, PyObject *args)
{
    PyObject *pylong;

    if (self->editor == NULL && self->editor->sci == NULL)
        Py_RETURN_NONE;

    if (self->sci == NULL)
    {
        self->sci = NEW_FROM_POINTER("scintilla.Scintilla", self->editor->sci);
        if (self->sci == NULL)
            Py_RETURN_NONE;
        else
            return self->sci;
    }
    else
    {
        SET_POINTER(self->sci, self->editor->sci);
        return self->sci;
    }
}


static PyMethodDef Editor_methods[] = {
    /* Private methods */
    { "_get_pointer", (PyCFunction) Editor_get_pointer, METH_NOARGS },
    { "_set_pointer", (PyCFunction) Editor_set_pointer, METH_NOARGS },

    /* Public methods */
    { "create_widget", (PyCFunction) Editor_create_widget, METH_VARARGS },
    { "find_snippet", (PyCFunction) Editor_find_snippet, METH_VARARGS },
    { "get_word_at_position", (PyCFunction) Editor_get_word_at_position, METH_VARARGS },
    { "goto_pos", (PyCFunction) Editor_goto_pos, METH_VARARGS },
    { "indicator_clear", (PyCFunction) Editor_indicator_clear, METH_VARARGS },
    { "indicator_set_on_line", (PyCFunction) Editor_indicator_set_on_line, METH_VARARGS },
    { "indicator_set_on_range", (PyCFunction) Editor_indicator_set_on_range, METH_VARARGS },
    { "insert_snippet", (PyCFunction) Editor_insert_snippet, METH_VARARGS | METH_KEYWORDS },
    { "insert_text_block", (PyCFunction) Editor_insert_text_block, METH_VARARGS | METH_KEYWORDS },
    { "get_eol_char", (PyCFunction) Editor_get_eol_char, METH_VARARGS },
    { "get_eol_char_len", (PyCFunction) Editor_get_eol_char_len, METH_VARARGS },
    { "get_eol_char_mode", (PyCFunction) Editor_get_eol_char_mode, METH_VARARGS },
    { "get_eol_char_name", (PyCFunction) Editor_get_eol_char_name, METH_VARARGS },
    { "get_indent_prefs", (PyCFunction) Editor_get_indent_prefs, METH_VARARGS },
    { "set_indent_type", (PyCFunction) Editor_set_indent_type, METH_VARARGS },
    { "get_scintilla", (PyCFunction) Editor_get_scintilla, METH_VARARGS },
	{ NULL }
};


static PyTypeObject EditorType = {
	PyObject_HEAD_INIT(NULL)
    0,                          /*ob_size*/
    "geany.editor.Editor",      /*tp_name*/
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
    "Wrapper class around Geany's `GeanyEditor` structure.  This class "
    "should not be directly initialized, instead retrieve instances of it "
    "using through the `Document.get_editor()` method.", /* tp_doc */
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
Editor__get_default_eol_char(PyObject *module)
{
    const gchar *eol_char = editor_get_eol_char(NULL);
    if (eol_char != NULL)
        return Py_BuildValue("s", eol_char);
    Py_RETURN_NONE;
}


static PyObject *
Editor__get_default_eol_char_len(PyObject *module)
{
    return Py_BuildValue("i", editor_get_eol_char_len(NULL));
}


static PyObject *
Editor__get_default_eol_char_mode(PyObject *module)
{
    return Py_BuildValue("i", editor_get_eol_char_mode(NULL));
}


static PyObject *
Editor__get_default_eol_char_name(PyObject *module)
{
    const gchar *eol_char_name = editor_get_eol_char_name(NULL);
    if (eol_char_name != NULL)
        return Py_BuildValue("s", eol_char_name);
    Py_RETURN_NONE;
}


static PyObject *
Editor__get_default_indent_prefs(PyObject *module, PyObject *args)
{
    PyObject *mod, *cls, *inst, *pylong;
    const GeanyIndentPrefs *ip;

    ip = editor_get_indent_prefs(NULL);
    if (ip == NULL)
        Py_RETURN_NONE;

    if (mod = PyImport_ImportModule("indentprefs"))
    {
        cls = PyObject_GetAttrString(mod, "IndentPrefs");
        Py_DECREF(mod);
        if (cls && PyCallable_Check(cls))
        {
            pylong = PyLong_FromVoidPtr((GeanyIndentPrefs *) ip);
            inst = PyObject_CallObject(cls, pylong);
            Py_DECREF(cls);
            Py_DECREF(pylong);
            return inst;
        }
        else if (PyErr_Occurred())
            PyErr_Print();
    }
    else if (PyErr_Occurred())
        PyErr_Print();

    Py_RETURN_NONE;
}



static
PyMethodDef EditorModule_methods[] = {
    { "find_snippet", (PyCFunction) Editor__find_snippet, METH_VARARGS },
    { "get_default_eol_char", (PyCFunction) Editor__get_default_eol_char, METH_NOARGS },
    { "get_default_eol_char_len", (PyCFunction) Editor__get_default_eol_char_len, METH_NOARGS },
    { "get_default_eol_char_mode", (PyCFunction) Editor__get_default_eol_char_mode, METH_NOARGS },
    { "get_default_eol_char_name", (PyCFunction) Editor__get_default_eol_char_name, METH_NOARGS },
    { "get_default_indent_prefs", (PyCFunction) Editor__get_default_indent_prefs, METH_NOARGS },
    { NULL }
};


PyMODINIT_FUNC
initeditor(void)
{
    PyObject *m;

    EditorPrefsType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&EditorPrefsType) < 0)
        return;

    IndentPrefsType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&IndentPrefsType) < 0)
        return;

    EditorType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&EditorType) < 0)
        return;

    m = Py_InitModule3("editor", EditorModule_methods,
            "The :mod:`editor` module provides a functions working with "
            ":class:`Editor` objects.");

    Py_INCREF(&EditorPrefsType);
    PyModule_AddOjbect(m, "EditorPrefs", (PyObject *)&EditorPrefsType);

    Py_INCREF(&IndentPrefsType);
    PyModule_AddObject(m, "IndentPrefs", (PyObject *)&IndentPrefsType);

    Py_INCREF(&EditorType);
    PyModule_AddObject(m, "Editor", (PyObject *)&EditorType);
}
