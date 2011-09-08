#include "geanypy.h"


static void
LexerStyle_dealloc(LexerStyle *self)
{
	self->ob_type->tp_free((PyObject *) self);
}


static int
LexerStyle_init(LexerStyle *self, PyObject *args, PyObject *kwds)
{
    self->lexer_style = NULL;
	return 0;
}


static PyObject *
LexerStyle__get_background(LexerStyle *self, PyObject *args)
{
    if (self->lexer_style != NULL)
        return Py_BuildValue("i", self->lexer_style->background);
    Py_RETURN_NONE;
}


static PyObject *
LexerStyle__get_bold(LexerStyle *self, PyObject *args)
{
    if (self->lexer_style != NULL)
    {
        if (self->lexer_style->bold)
            Py_RETURN_TRUE;
        else
            Py_RETURN_FALSE;
    }
    Py_RETURN_NONE;
}


static PyObject *
LexerStyle__get_foreground(LexerStyle *self, PyObject *args)
{
    if (self->lexer_style != NULL)
        return Py_BuildValue("i", self->lexer_style->foreground);
    Py_RETURN_NONE;
}


static PyObject *
LexerStyle__get_italic(LexerStyle *self, PyObject *args)
{
    if (self->lexer_style != NULL)
    {
        if (self->lexer_style->italic)
            Py_RETURN_TRUE;
        else
            Py_RETURN_FALSE;
    }
    Py_RETURN_NONE;
}


static PyMethodDef LexerStyle_methods[] = {
    { "_get_background", (PyCFunction) LexerStyle__get_background, METH_VARARGS },
    { "_get_bold", (PyCFunction) LexerStyle__get_bold, METH_VARARGS },
    { "_get_foreground", (PyCFunction) LexerStyle__get_foreground, METH_VARARGS },
    { "_get_italic", (PyCFunction) LexerStyle__get_italic, METH_VARARGS },
	{ NULL }
};


static PyTypeObject LexerStyleType = {
	PyObject_HEAD_INIT(NULL)
    0,                          /*ob_size*/
    "_geany_highlighting.LexerStyle",     /*tp_name*/
    sizeof(Editor),             /*tp_basicsize*/
    0,                          /*tp_itemsize*/
    (destructor)LexerStyle_dealloc, /*tp_dealloc*/
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
    "Geany lexer style",             /* tp_doc */
    0,		                    /* tp_traverse */
    0,		               	    /* tp_clear */
    0,		                    /* tp_richcompare */
    0,		                    /* tp_weaklistoffset */
    0,		                    /* tp_iter */
    0,		                    /* tp_iternext */
    LexerStyle_methods,             /* tp_methods */
    0,                          /* tp_members */
    0,                          /* tp_getset */
    0,                          /* tp_base */
    0,                          /* tp_dict */
    0,                          /* tp_descr_get */
    0,                          /* tp_descr_set */
    0,                          /* tp_dictoffset */
    (initproc)LexerStyle_init,      /* tp_init */
    0,                          /* tp_alloc */
    0,                          /* tp_new */

};


static PyObject *
Highlighting_get_style(PyObject *module, PyObject *args)
{
    gint ft_id, style_id;
    LexerStyle *lexer_style;
    const GeanyLexerStyle *ls;

    if (PyArg_ParseTuple(args, "ii", &ft_id, &style_id))
    {
        ls = highlighting_get_style(ft_id, style_id);
        if (ls != NULL)
        {
            lexer_style = (LexerStyle *) PyObject_CallObject((PyObject *) &LexerStyleType, NULL);
            lexer_style->lexer_style = ls;
            return (PyObject *) lexer_style;
        }
    }

    Py_RETURN_NONE;
}


static PyObject *
Highlighting_is_code_style(PyObject *module, PyObject *args)
{
    gint lexer, style;

    if (PyArg_ParseTuple(args, "ii", &lexer, &style))
    {
        if (highlighting_is_code_style(lexer, style))
            Py_RETURN_TRUE;
        else
            Py_RETURN_FALSE;
    }

    Py_RETURN_NONE;
}


static PyObject *
Highlighting_is_comment_style(PyObject *module, PyObject *args)
{
    gint lexer, style;

    if (PyArg_ParseTuple(args, "ii", &lexer, &style))
    {
        if (highlighting_is_comment_style(lexer, style))
            Py_RETURN_TRUE;
        else
            Py_RETURN_FALSE;
    }

    Py_RETURN_NONE;
}


static PyObject *
Highlighting_is_string_style(PyObject *module, PyObject *args)
{
    gint lexer, style;

    if (PyArg_ParseTuple(args, "ii", &lexer, &style))
    {
        if (highlighting_is_string_style(lexer, style))
            Py_RETURN_TRUE;
        else
            Py_RETURN_FALSE;
    }

    Py_RETURN_NONE;
}


static PyObject *
Highlighting_set_styles(PyObject *module, PyObject *args)
{
    PyObject *py_sci, *py_ft;
    Scintilla *sci;
    Filetype *ft;

    if (PyArg_ParseTuple(args, "OO", &py_sci, &py_ft))
    {
        if (py_sci != Py_None && py_ft != Py_None)
        {
            sci = (Scintilla *) py_sci;
            ft = (Filetype *) py_ft;
            highlighting_set_styles(sci->sci, ft->ft);
        }
    }

    Py_RETURN_NONE;
}


static
PyMethodDef EditorModule_methods[] = {
    { "get_style", (PyCFunction) Highlighting_get_style, METH_VARARGS },
    { "is_code_style", (PyCFunction) Highlighting_is_code_style, METH_VARARGS },
    { "is_comment_style", (PyCFunction) Highlighting_is_comment_style, METH_VARARGS },
    { "is_string_style", (PyCFunction) Highlighting_is_string_style, METH_VARARGS },
    { "set_styles", (PyCFunction) Highlighting_set_styles, METH_VARARGS },
    { NULL }
};


PyMODINIT_FUNC
init_geany_highlighting(void)
{
    PyObject *m;

    LexerStyleType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&LexerStyleType) < 0)
        return;

    m = Py_InitModule("_geany_highlighting", EditorModule_methods);

    Py_INCREF(&LexerStyleType);
    PyModule_AddObject(m, "LexerStyle", (PyObject *)&LexerStyleType);
}
