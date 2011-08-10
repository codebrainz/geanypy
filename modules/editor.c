#include <Python.h>
#include <structmember.h>
#include <geanyplugin.h>


#define _GeanyEditor_FromPyObject(ed) ((GeanyEditor *) PyLong_AsVoidPtr(ed))
#define _GeanyEditor_ToPyObject(ed) ((PyObject *) PyLong_FromVoidPtr((void *)(ed)))


extern GeanyPlugin		*geany_plugin;
extern GeanyData		*geany_data;
extern GeanyFunctions	*geany_functions;


typedef struct
{
	PyObject_HEAD
	GeanyEditor *editor;
} Editor;


static PyTypeObject EditorType;


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
	self->ob_type->tp_free((PyObject *) self);
}


static int
Editor_init(Editor *self, PyObject *ptr)
{
    self->editor = NULL;
    if (ptr != NULL && ptr != Py_None)
        self->editor = _GeanyEditor_FromPyObject(ptr);
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


/*
static PyObject *
Editor_get_indent_prefs(Editor *self)
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
*/


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


/*
static PyObject *
Editor_get_scintilla(Editor *self, PyObject *args)
{
    Scintilla *sci;
    if (self->editor != NULL)
    {
        sci = Scintilla_create_new_from_scintilla(self->editor->sci);
        return (PyObject *) sci;
    }
    Py_RETURN_NONE;
}
*/


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
    /*{ "get_indent_prefs", (PyCFunction) Editor_get_indent_prefs, METH_VARARGS },*/
    { "set_indent_type", (PyCFunction) Editor_set_indent_type, METH_VARARGS },
    /*{ "get_scintilla", (PyCFunction) Editor_get_scintilla, METH_VARARGS },*/
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


/*
static PyObject *
Editor__get_default_indent_prefs(PyObject *module, PyObject *args)
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
*/


static
PyMethodDef EditorModule_methods[] = {
    { "find_snippet", (PyCFunction) Editor__find_snippet, METH_VARARGS },
    { "get_default_eol_char", (PyCFunction) Editor__get_default_eol_char, METH_NOARGS },
    { "get_default_eol_char_len", (PyCFunction) Editor__get_default_eol_char_len, METH_NOARGS },
    { "get_default_eol_char_mode", (PyCFunction) Editor__get_default_eol_char_mode, METH_NOARGS },
    { "get_default_eol_char_name", (PyCFunction) Editor__get_default_eol_char_name, METH_NOARGS },
    /*{ "get_default_indent_prefs", (PyCFunction) Editor__get_default_indent_prefs, METH_NOARGS },*/
    { NULL }
};


PyMODINIT_FUNC
initeditor(void)
{
    PyObject *m;

    EditorType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&EditorType) < 0)
        return;

    m = Py_InitModule3("editor", EditorModule_methods,
            "The :mod:`editor` module provides a functions working with "
            ":class:`Editor` objects.");

    Py_INCREF(&EditorType);
    PyModule_AddObject(m, "Editor", (PyObject *)&EditorType);
}
