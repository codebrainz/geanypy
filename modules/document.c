#include <Python.h>
#include <structmember.h>
#include <geanyplugin.h>


extern GeanyPlugin		*geany_plugin;
extern GeanyData		*geany_data;
extern GeanyFunctions	*geany_functions;


#define _GeanyDocument_FromPyObject(ft) ((GeanyDocument *) PyLong_AsVoidPtr(ft))
#define _GeanyDocument_ToPyObject(ft) ((PyObject *) PyLong_FromVoidPtr((void *)(ft)))


typedef struct
{
	PyObject_HEAD
	GeanyDocument *doc;
    PyObject *ft;
    PyObject *editor;
} Document;


static PyTypeObject DocumentType;


static PyObject *
Document_new(GeanyDocument *doc)
{
    PyObject *l, *p;
    if (l = PyLong_FromVoidPtr((void *) doc))
    {
        p = PyObject_CallObject((PyObject *) &DocumentType, l);
        Py_DECREF(l);
        return p;
    }
    Py_RETURN_NONE;
}


static GeanyFiletype *
GeanyFiletype_FromPyFiletype(PyObject *obj)
{
    GeanyFiletype *ft = NULL;
    PyObject *ptr_func, *pylong;

    if (obj == NULL || obj == Py_None)
        return ft;

    ptr_func = PyObject_GetAttrString(obj, "_get_pointer");

    if (ptr_func != NULL)
    {
        if (PyCallable_Check(ptr_func))
        {
            pylong = PyObject_CallObject(ptr_func, NULL);
            if (pylong != NULL && pylong != Py_None)
            {
                ft = (GeanyFiletype *) PyLong_AsVoidPtr(pylong);
                Py_DECREF(pylong);
            }
        }
        Py_DECREF(ptr_func);
    }

    return ft;
}


static void
Document_dealloc(Document *self)
{
    Py_XDECREF(self->ft);
    Py_XDECREF(self->editor);
	self->ob_type->tp_free((PyObject *) self);
}


static int
Document_init(Document *self, PyObject *args)
{
    PyObject *pylong;

    self->doc = NULL;
    self->ft = NULL;
    self->editor = NULL;

    if (PyArg_ParseTuple(args, "l", &pylong))
    {
        self->doc = (GeanyDocument *) PyLong_AsVoidPtr(pylong);
        return 0;
    }

	return -1;
}


static PyObject *
Document_get_pointer(Document *self)
{
    if (self->doc == NULL)
        Py_RETURN_NONE;
    return PyLong_FromVoidPtr((void *) self->doc);
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
    return Py_BuildValue("i", document_get_notebook_page(self->doc));
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
    PyObject *ptr_func, *pylong;
    GeanyFiletype *gft;

    if (ft != Py_None && PyObject_HasAttrString(ft, "_get_pointer"))
    {
        ptr_func = PyObject_GetAttrString(ft, "_get_pointer");
        if (ptr_func && PyCallable_Check(ptr_func))
        {
            pylong = PyObject_CallObject(ptr_func, NULL);
            gft = (GeanyFiletype *) PyLong_AsVoidPtr(pylong);
            if (gft != NULL)
                document_set_filetype(self->doc, gft);
            Py_DECREF(pylong);
            Py_DECREF(ptr_func);
        }
    }

    Py_RETURN_NONE;
}


static PyObject*
Document_get_filetype(Document *self)
{
    PyObject *mod, *cls, *pylong, *ptr_func;

    if (!DOC_VALID(self->doc))
        Py_RETURN_NONE;

    if (self->ft == NULL)
    {
        if (mod = PyImport_ImportModule("filetype"))
        {
            cls = PyObject_GetAttrString(mod, "Filetype");
            Py_DECREF(mod);

            if (cls)
            {
                pylong = PyLong_FromVoidPtr((void *) self->doc->file_type);
                self->ft = PyObject_CallObject(cls, pylong);
                Py_DECREF(cls);
                Py_DECREF(pylong);
                return self->ft;
            }
        }
    }
    else
    {
        ptr_func = PyObject_GetAttrString(self->ft, "_set_pointer");
        if (ptr_func)
        {
            pylong = PyLong_FromVoidPtr((void *) self->doc->file_type);
            PyObject_CallObject(ptr_func, pylong);
            Py_DECREF(ptr_func);
            Py_DECREF(pylong);
            return self->ft;
        }
    }

    Py_RETURN_NONE;
}


static PyObject*
Document_set_text_changed(Document *self, PyObject *args)
{
    gint changed = 0;
    if (DOC_VALID(self->doc))
    {
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
    PyObject *pylong;

    if (!DOC_VALID(self->doc) || self->doc->editor == NULL)
        Py_RETURN_NONE;

    if (self->editor == NULL)
    {
        PyObject *mod, *cls;

        if (mod = PyImport_ImportModule("editor"))
        {
            cls = PyObject_GetAttrString(mod, "Editor");
            Py_DECREF(mod);
            if (cls && PyCallable_Check(cls))
            {
                pylong = PyLong_FromVoidPtr((GeanyEditor *) self->doc->editor);
                self->editor = PyObject_CallObject(cls, pylong);
                Py_DECREF(cls);
                Py_DECREF(pylong);
                return self->editor;
            }
            else if (PyErr_Occurred())
                PyErr_Print();
        }
        else if (PyErr_Occurred())
            PyErr_Print();

        Py_RETURN_NONE;
    }
    else
    {
        /* Update the GeanyEditor pointer in case it changed. */
        PyObject *ptr_func;
        pylong = PyLong_FromVoidPtr((GeanyEditor *) self->doc->editor);
        ptr_func = PyObject_GetAttrString(self->editor, "_set_pointer");
        if (ptr_func)
        {
            if (PyCallable_Check(ptr_func))
            {
                PyObject_CallObject(ptr_func, pylong);
                Py_DECREF(pylong);
                Py_DECREF(ptr_func);
            }
        }
        return self->editor;
    }
}


static PyMethodDef Document_methods[] = {
    { "_get_pointer", (PyCFunction) Document_get_pointer, METH_NOARGS },
    { "close", (PyCFunction)Document_close, METH_NOARGS },
    { "reload_file", (PyCFunction)Document_reload_file, METH_VARARGS | METH_KEYWORDS },
    { "rename_file", (PyCFunction)Document_rename_file, METH_VARARGS | METH_KEYWORDS },
    { "save_file", (PyCFunction)Document_save_file, METH_VARARGS | METH_KEYWORDS },
    { "save_file_as", (PyCFunction)Document_save_file_as, METH_VARARGS | METH_KEYWORDS },
    { "get_basename_for_display", (PyCFunction)Document_get_basename_for_display, METH_NOARGS },
    { "get_notebook_page", (PyCFunction)Document_get_notebook_page, METH_NOARGS },
    { "get_status_color", (PyCFunction)Document_get_status_color, METH_NOARGS },
    { "set_encoding", (PyCFunction)Document_set_encoding, METH_VARARGS },
    { "get_encoding", (PyCFunction)Document_get_encoding, METH_NOARGS },
    { "set_file_type", (PyCFunction)Document_set_filetype, METH_O },
    { "get_file_type", (PyCFunction)Document_get_filetype, METH_NOARGS },
    { "set_text_changed", (PyCFunction)Document_set_text_changed, METH_VARARGS },
    { "get_text_changed", (PyCFunction)Document_get_text_changed, METH_NOARGS },
    { "get_file_name", (PyCFunction)Document_get_file_name, METH_NOARGS },
    { "get_has_bom", (PyCFunction)Document_get_has_bom, METH_NOARGS },
    { "get_has_tags", (PyCFunction)Document_get_has_tags, METH_NOARGS },
    { "get_index", (PyCFunction)Document_get_index, METH_NOARGS },
    { "get_is_valid", (PyCFunction)Document_get_is_valid, METH_NOARGS },
    { "get_read_only", (PyCFunction)Document_get_readonly, METH_NOARGS },
    { "get_real_path", (PyCFunction)Document_get_real_path, METH_NOARGS },
    /*{ "get_editor", (PyCFunction)Document_get_editor, METH_NOARGS },*/
	{NULL}
};


static PyMemberDef Document_members[] = { { NULL } };


static PyTypeObject DocumentType = {
	PyObject_HEAD_INIT(NULL)
    0,                          /*ob_size*/
    "geany.document.Document",  /*tp_name*/
    sizeof(Document),           /*tp_basicsize*/
    0,                          /*tp_itemsize*/
    (destructor)Document_dealloc, /*tp_dealloc*/
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
    "Wrapper class around Geany's `GeanyDocument` structure.  This class "
    "should not be directly initialized, instead retrieve instances of it "
    "using the module level functions (ex. `document.get_current()`).", /* tp_doc */
    0,		                    /* tp_traverse */
    0,		               	    /* tp_clear */
    0,		                    /* tp_richcompare */
    0,		                    /* tp_weaklistoffset */
    0,		                    /* tp_iter */
    0,		                    /* tp_iternext */
    Document_methods,           /* tp_methods */
    Document_members,           /* tp_members */
    0,                          /* tp_getset */
    0,                          /* tp_base */
    0,                          /* tp_dict */
    0,                          /* tp_descr_get */
    0,                          /* tp_descr_set */
    0,                          /* tp_dictoffset */
    (initproc)Document_init,    /* tp_init */
    0,                          /* tp_alloc */
    0,                          /* tp_new */

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
Document_find_by_filename(PyObject *self, PyObject *args, PyObject *kwargs)
{
    gchar *fn;
    GeanyDocument *doc;
    static gchar *kwlist[] = { "filename", NULL };

    if (PyArg_ParseTupleAndKeywords(args, kwargs, "s", kwlist, &fn))
    {
        doc = document_find_by_filename(fn);
        if (DOC_VALID(doc))
            return Document_new(doc);
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
            return Document_new(doc);
    }

    Py_RETURN_NONE;
}


static PyObject*
Document_get_current(PyObject *self)
{
    GeanyDocument *doc;

    doc = document_get_current();
    if (DOC_VALID(doc))
        return Document_new(doc);

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
            return Document_new(doc);
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
            return Document_new(doc);
    }

    Py_RETURN_NONE;
}


static PyObject*
Document_new_file(PyObject *self, PyObject *args, PyObject *kwargs)
{
    gchar *filename = NULL;
    gchar *initial_text = NULL;
    PyObject *py_ft = NULL;
    GeanyDocument *doc;
    GeanyFiletype *ft = NULL;
    static gchar *kwlist[] = { "filename", "filetype", "initial_text", NULL };

    if (PyArg_ParseTupleAndKeywords(args, kwargs, "|zOz", kwlist,
        &filename, &py_ft, &initial_text))
    {
        ft = GeanyFiletype_FromPyFiletype(py_ft);
        doc = document_new_file(filename, ft, initial_text);
        if (DOC_VALID(doc))
            return Document_new(doc);
    }
    Py_RETURN_NONE;
}


static PyObject*
Document_open_file(PyObject *self, PyObject *args, PyObject *kwargs)
{
    gchar *filename = NULL, *forced_encoding = NULL;
    gint read_only = 0;
    PyObject *py_ft = NULL;
    GeanyDocument *doc;
    GeanyFiletype *ft = NULL;
    static gchar *kwlist[] = { "filename", "read_only", "filetype", "forced_enc", NULL };

    if (PyArg_ParseTupleAndKeywords(args, kwargs, "s|iOz", kwlist,
        &filename, &read_only, &py_ft, &forced_encoding))
    {
        ft = GeanyFiletype_FromPyFiletype(py_ft);
        doc = document_open_file(filename, read_only, ft, forced_encoding);
        if (DOC_VALID(doc))
            return Document_new(doc);
    }
    Py_RETURN_NONE;
}


static PyObject*
Document_remove_page(PyObject *self, PyObject *args, PyObject *kwargs)
{
    gboolean res;
    guint page_num;
    static gchar *kwlist[] = { "page_num", NULL };

    if (PyArg_ParseTupleAndKeywords(args, kwargs, "i", kwlist, &page_num))
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
Document_get_documents_list(PyObject *module)
{
    GeanyDocument *doc;
    guint i;
    PyObject *list;

    list = PyList_New(0);

    for (i = 0; i < geany_data->documents_array->len; i++)
    {
        doc = g_ptr_array_index(geany_data->documents_array, i);
        PyList_Append(list, (PyObject *) Document_new(doc));
    }

    return list;
}


static
PyMethodDef DocumentModule_methods[] = {
    { "compare_by_display_name", (PyCFunction)Document_compare_by_display_name, METH_VARARGS },
    { "compare_by_tab_order", (PyCFunction)Document_compare_by_tab_order, METH_VARARGS },
    { "find_by_filename", (PyCFunction)Document_find_by_filename, METH_VARARGS | METH_KEYWORDS },
    { "find_by_real_path", (PyCFunction)Document_find_by_real_path, METH_VARARGS | METH_KEYWORDS },
    { "get_current", (PyCFunction)Document_get_current, METH_VARARGS | METH_NOARGS },
    { "get_from_page", (PyCFunction)Document_get_from_page, METH_VARARGS | METH_KEYWORDS },
    { "get_from_index", (PyCFunction)Document_get_from_index, METH_VARARGS | METH_KEYWORDS },
    { "new_file", (PyCFunction)Document_new_file, METH_VARARGS | METH_KEYWORDS },
    { "open_file", (PyCFunction)Document_open_file, METH_VARARGS | METH_KEYWORDS },
/*    { "open_files", (PyCFunction)Document_open_files, METH_VARARGS | METH_KEYWORDS },*/
    { "remove_page", (PyCFunction)Document_remove_page, METH_VARARGS | METH_KEYWORDS },
    { "get_documents_list", (PyCFunction)Document_get_documents_list, METH_NOARGS },
    { NULL }
};


PyMODINIT_FUNC
initdocument(void)
{
    PyObject *m;

    DocumentType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&DocumentType) < 0)
        return;

    m = Py_InitModule3("document", DocumentModule_methods,
            "The `document` module provides a functions for opening, creating, "
            "and getting instances of `Document` objects.");

    Py_INCREF(&DocumentType);
    PyModule_AddObject(m, "Document", (PyObject *)&DocumentType);
}
