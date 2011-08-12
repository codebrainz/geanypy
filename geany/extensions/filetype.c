#include <Python.h>
#include <structmember.h>
#include <pygtk/pygtk.h>
#include <geanyplugin.h>

#include "modules-common.h"


typedef struct
{
	PyObject_HEAD
	GeanyFiletype *ft;
} Filetype;


static PyTypeObject FiletypeType;


/*
 * Create a new PyObject filetype from a GeanyFiletype pointer.
 */
static PyObject *
Filetype_new(GeanyFiletype *ft)
{
    PyObject *l, *p;
    if (l = PyLong_FromVoidPtr((void *) ft))
    {
        p = PyObject_CallObject((PyObject *) &FiletypeType, l);
        Py_DECREF(l);
        return p;
    }
    Py_RETURN_NONE;
}


static void
Filetype_dealloc(Filetype *self)
{
	self->ob_type->tp_free((PyObject *) self);
}


static int
Filetype_init(Filetype *self, PyObject *args)
{
    PyObject *pylong;

    self->ft = NULL;

    if (PyArg_ParseTuple(args, "l", &pylong))
    {
        self->ft = GET_POINTER(pylong, GeanyFiletype);
        return 0;
    }

	return -1;
}


/*
 * Get the internal GeanyFiletype pointer as a PyLong.  This is the value
 * used by the class's methods to get data from.
 */
static PyObject *
Filetype_get_pointer(Filetype *self)
{
    if (self->ft == NULL)
        Py_RETURN_NONE;
    return _StructPointer_ToPyObject(self->ft);
}


/*
 * Set the internal GeanyFiletype pointer from a PyLong.  This is the value
 * used by the class's methods to get data from.
 */
static PyObject *
Filetype_set_pointer(Filetype *self, PyObject *ptr)
{
    if (ptr != NULL && ptr != Py_None)
        self->ft = GET_POINTER(ptr, GeanyFiletype);
    Py_RETURN_NONE;
}


/*
 * Returns a new PyString copied from the value returned by the
 * filetypes_get_display_name() function.
 */
static PyObject *
Filetype_get_display_name(Filetype *self)
{
    const gchar *ft_name = NULL;

    if (self->ft != NULL)
    {
        ft_name = filetypes_get_display_name(self->ft);
        if (ft_name != NULL)
            return PyString_FromString(ft_name);
    }

    Py_RETURN_NONE;
}


/*
 * Get the default extension used for new files or Py_None.
 */
static PyObject *
Filetype_get_extension(Filetype *self)
{
    if (self->ft->extension != NULL)
        return PyString_FromString(self->ft->extension);
    Py_RETURN_NONE;
}


/*
 * Gets the index into Geany's documents_array.
 */
static PyObject *
Filetype_get_index(Filetype *self)
{
    PyObject *res;

    res = Py_BuildValue("i", self->ft->id);
    if (res)
        return res;
    Py_RETURN_NONE;
}


static PyObject *
Filetype_get_lang_id(Filetype *self)
{
    PyObject *res;

    res = Py_BuildValue("i", self->ft->lang);
    if (res)
        return res;
    Py_RETURN_NONE;
}


static PyObject *
Filetype_get_name(Filetype *self)
{
    if (self->ft->name != NULL)
        return PyString_FromString(self->ft->name);
    Py_RETURN_NONE;
}


static PyObject *
Filetype_get_patterns(Filetype *self)
{
    gchar **patterns;
    gint i, len;
    PyObject *list;

    if (self->ft->pattern != NULL)
    {
        list = PyList_New(0);
        patterns = self->ft->pattern;
        len = g_strv_length(patterns);
        for (i = 0; i < len; i++)
        {
            if (patterns[i] != NULL)
                PyList_Append(list, PyString_FromString(patterns[i]));
        }
        return list;
    }
    Py_RETURN_NONE;
}


static PyObject *
Filetype_get_title(Filetype *self)
{
    if (self->ft->title != NULL)
        return PyString_FromString(self->ft->title);
    Py_RETURN_NONE;
}


static PyMethodDef Filetype_methods[] = {
    /* Private methods */
    { "_get_pointer", (PyCFunction) Filetype_get_pointer, METH_NOARGS },
    { "_set_pointer", (PyCFunction) Filetype_set_pointer, METH_O },

    /* Public methods */
    { "get_display_name", (PyCFunction)Filetype_get_display_name, METH_NOARGS },
    { "get_extension", (PyCFunction)Filetype_get_extension, METH_NOARGS },
    { "get_index", (PyCFunction)Filetype_get_index, METH_NOARGS },
    { "get_lang_id", (PyCFunction)Filetype_get_lang_id, METH_NOARGS },
    { "get_name", (PyCFunction)Filetype_get_name, METH_NOARGS },
    { "get_patterns", (PyCFunction)Filetype_get_patterns, METH_NOARGS },
    { "get_title", (PyCFunction)Filetype_get_title, METH_NOARGS },
	{ NULL }
};


static PyMemberDef Filetype_members[] = { { NULL } };


static PyTypeObject FiletypeType = {
	PyObject_HEAD_INIT(NULL)
    0,                          /*ob_size */
    "geany.filetype.Filetype",  /*tp_name */
    sizeof(Filetype),           /*tp_basicsize */
    0,                          /*tp_itemsize */
    (destructor)Filetype_dealloc, /*tp_dealloc */
    0,                          /* tp_print */
    0,                          /* tp_getattr */
    0,                          /* tp_setattr */
    0,                          /* tp_compare */
    0,                          /* tp_repr */
    0,                          /* tp_as_number */
    0,                          /* tp_as_sequence */
    0,                          /* tp_as_mapping */
    0,                          /* tp_hash */
    0,                          /* tp_call */
    0,                          /* tp_str */
    0,                          /* tp_getattro */
    0,                          /* tp_setattro */
    0,                          /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    "Wrapper class around Geany's :c:type:`GeanyFiletype` structure.  This "
    "class should not be directly initialized, instead retrieve instances of it "
    "using the module level functions (ex. :func:`filetype.detect_from_file()`) "
    "or from an accessor method on a :class:`Document` instance.", /* tp_doc */
    0,		                    /* tp_traverse */
    0,		               	    /* tp_clear */
    0,		                    /* tp_richcompare */
    0,		                    /* tp_weaklistoffset */
    0,		                    /* tp_iter */
    0,		                    /* tp_iternext */
    Filetype_methods,           /* tp_methods */
    Filetype_members,           /* tp_members */
    0,                          /* tp_getset */
    0,                          /* tp_base */
    0,                          /* tp_dict */
    0,                          /* tp_descr_get */
    0,                          /* tp_descr_set */
    0,                          /* tp_dictoffset */
    (initproc)Filetype_init,    /* tp_init */
    0,                          /* tp_alloc */
    0,                          /* tp_new */

};


static PyObject *
Filetype_detect_from_file(PyObject *self, PyObject *args, PyObject *kwargs)
{
    GeanyFiletype *ft;
    gchar *filename = NULL;
    static gchar *kwlist[] = { "filename", NULL };

    if (PyArg_ParseTupleAndKeywords(args, kwargs, "s", kwlist, &filename))
    {
        if (filename == NULL)
            Py_RETURN_NONE;

        if ((ft = filetypes_detect_from_file(filename)) != NULL)
            return Filetype_new(ft);
    }

    Py_RETURN_NONE;
}


static PyObject *
Filetype_get_from_index(PyObject *self, PyObject *args, PyObject *kwargs)
{
    GeanyFiletype *ft;
    gint idx = -1;
    static gchar *kwlist[] = { "index", NULL };

    if (PyArg_ParseTupleAndKeywords(args, kwargs, "I", kwlist,  &idx))
    {
        if ((ft = filetypes_index(idx)) != NULL)
            return Filetype_new(ft);
    }

    Py_RETURN_NONE;
}


static PyObject *
Filetype_get_from_name(PyObject *self, PyObject *args, PyObject *kwargs)
{
    GeanyFiletype *ft;
    gchar *filetype = NULL;
    static gchar *kwlist[] = { "filetype_name", NULL };

    if (PyArg_ParseTupleAndKeywords(args, kwargs, "s", kwlist, &filetype))
    {
        if (filetype == NULL)
            Py_RETURN_NONE;

        if ((ft = filetypes_lookup_by_name(filetype)) != NULL)
            return Filetype_new(ft);
    }

    Py_RETURN_NONE;
}


static PyObject *
Filetype_get_filetypes_list(PyObject *self)
{
    const GSList *glist, *iter;
    PyObject *list;

    if (!(list = PyList_New(0)))
        Py_RETURN_NONE;

    glist = filetypes_get_sorted_by_name();
    if (glist == NULL)
        Py_RETURN_NONE;

    for (iter = glist; iter != NULL; iter = g_slist_next(iter))
        PyList_Append(list, Filetype_new(iter->data));

    return list;
}


static
PyMethodDef FiletypeModule_methods[] = {
    { "detect_from_file", (PyCFunction) Filetype_detect_from_file, METH_VARARGS | METH_KEYWORDS },
    { "get_from_index", (PyCFunction) Filetype_get_from_index, METH_VARARGS | METH_KEYWORDS },
    { "get_from_name", (PyCFunction) Filetype_get_from_name, METH_VARARGS | METH_KEYWORDS },
    { "get_filetypes_list", (PyCFunction) Filetype_get_filetypes_list, METH_NOARGS },
    { NULL }
};


PyMODINIT_FUNC
initfiletype(void)
{
    PyObject *m;

    FiletypeType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&FiletypeType) < 0)
        return;

    m = Py_InitModule3("filetype", FiletypeModule_methods,
            "The `filetype` module provides a functions for working with and "
            "getting instances of `Filetype` objects.");

    Py_INCREF(&FiletypeType);
    PyModule_AddObject(m, "Filetype", (PyObject *)&FiletypeType);
}
