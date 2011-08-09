#include <Python.h>
#include <structmember.h>
#include <geanyplugin.h>
#include "plugin.h"


static void
Filetype_dealloc(Filetype *self)
{
	self->ob_type->tp_free((PyObject *) self);
}


static int
Filetype_init(Filetype *self, PyObject *args, PyObject *kwds)
{
    self->ft = NULL;

	return 0;
}


static PyObject *
Filetype_get_display_name(Filetype *self, PyObject *args)
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


static PyObject *
Filetype_get_extension(Filetype *self, PyObject *args)
{
    if (self->ft->extension != NULL)
        return PyString_FromString(self->ft->extension);
    Py_RETURN_NONE;
}


static PyObject *
Filetype_get_index(Filetype *self, PyObject *args)
{
    PyObject *res;

    res = Py_BuildValue("i", self->ft->id);
    if (res)
        return res;
    Py_RETURN_NONE;
}


static PyObject *
Filetype_get_lang_id(Filetype *self, PyObject *args)
{
    PyObject *res;

    res = Py_BuildValue("i", self->ft->lang);
    if (res)
        return res;
    Py_RETURN_NONE;
}

static PyObject *
Filetype_get_name(Filetype *self, PyObject *args)
{
    if (self->ft->name != NULL)
        return PyString_FromString(self->ft->name);
    Py_RETURN_NONE;
}

static PyObject *
Filetype_get_patterns(Filetype *self, PyObject *args)
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
Filetype_get_title(Filetype *self, PyObject *args)
{
    if (self->ft->title != NULL)
        return PyString_FromString(self->ft->title);
    Py_RETURN_NONE;
}


static PyMethodDef Filetype_methods[] = {
    { "get_display_name", (PyCFunction)Filetype_get_display_name, METH_VARARGS },
    { "get_extension", (PyCFunction)Filetype_get_extension, METH_VARARGS },
    { "get_index", (PyCFunction)Filetype_get_index, METH_VARARGS },
    { "get_lang_id", (PyCFunction)Filetype_get_lang_id, METH_VARARGS },
    { "get_name", (PyCFunction)Filetype_get_name, METH_VARARGS },
    { "get_patterns", (PyCFunction)Filetype_get_patterns, METH_VARARGS },
    { "get_title", (PyCFunction)Filetype_get_title, METH_VARARGS },
	{ NULL }
};


static PyMemberDef Filetype_members[] = { { NULL } };


static PyTypeObject FiletypeType = {
	PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "geany.filetype.Filetype",  /*tp_name*/
    sizeof(Filetype),         /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)Filetype_dealloc, /*tp_dealloc*/
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
    "Wrapper class around Geany's `GeanyFiletype` structure.  This class "
    "should not be directly initialized, instead retrieve instances of it "
    "using the module level functions (ex. `filetype.detect_from_file()`) "
    "or from an accessor method on a `Document` object.", /* tp_doc */
    0,		                   /* tp_traverse */
    0,		               	   /* tp_clear */
    0,		                   /* tp_richcompare */
    0,		                   /* tp_weaklistoffset */
    0,		                   /* tp_iter */
    0,		                   /* tp_iternext */
    Filetype_methods,   /* tp_methods */
    Filetype_members,   /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)Filetype_init,  /* tp_init */
    0,                         /* tp_alloc */
    0,       /* tp_new */

};


static PyObject *
Filetype_detect_from_file(PyObject *self, PyObject *args)
{
    Filetype *py_ft;
    GeanyFiletype *ft;
    gchar *filename = NULL;

    if (PyArg_ParseTuple(args, "s", &filename))
    {
        if (filename == NULL)
            Py_RETURN_NONE;

        if ((ft = filetypes_detect_from_file(filename)) != NULL)
        {
            py_ft = (Filetype *) PyObject_CallObject((PyObject *) &FiletypeType, NULL);
            py_ft->ft = ft;
            return (PyObject *) py_ft;
        }
    }

    Py_RETURN_NONE;
}


static PyObject *
Filetype_index(PyObject *self, PyObject *args)
{
    Filetype *py_ft;
    GeanyFiletype *ft;
    gint idx = -1;

    if (PyArg_ParseTuple(args, "i", &idx))
    {
        if (idx == -1)
            Py_RETURN_NONE;

        if ((ft = filetypes_index(idx)) != NULL)
        {
            py_ft = (Filetype *) PyObject_CallObject((PyObject *) &FiletypeType, NULL);
            py_ft->ft = ft;
            return (PyObject *) py_ft;
        }
    }

    Py_RETURN_NONE;
}


static PyObject *
Filetype_lookup_by_name(PyObject *self, PyObject *args)
{
    Filetype *py_ft;
    GeanyFiletype *ft;
    gchar *filetype = NULL;

    if (PyArg_ParseTuple(args, "s", &filetype))
    {
        if (filetype == NULL)
            Py_RETURN_NONE;

        if ((ft = filetypes_lookup_by_name(filetype)) != NULL)
        {
            py_ft = (Filetype *) PyObject_CallObject((PyObject *) &FiletypeType, NULL);
            py_ft->ft = ft;
            return (PyObject *) py_ft;
        }
    }

    Py_RETURN_NONE;
}


static PyObject *
Filetype_filetypes_by_name(PyObject *self, PyObject *args)
{
    const GSList *glist, *iter;
    Filetype *py_ft;
    PyObject *list;

    if (!(list = PyList_New(0)))
        Py_RETURN_NONE;

    glist = filetypes_get_sorted_by_name();
    if (glist == NULL)
        Py_RETURN_NONE;

    for (iter = glist; iter != NULL; iter = g_slist_next(iter))
    {
        py_ft = (Filetype *) PyObject_CallObject((PyObject *) &FiletypeType, NULL);
        py_ft->ft = (GeanyFiletype *) iter->data;

        PyList_Append(list, (PyObject *) py_ft);
    }

    return list;
}


static
PyMethodDef FiletypeModule_methods[] = {
    { "detect_from_file", (PyCFunction)Filetype_detect_from_file, METH_VARARGS },
    { "index", (PyCFunction)Filetype_index, METH_VARARGS },
    { "lookup_by_name", (PyCFunction)Filetype_lookup_by_name, METH_VARARGS },
    { "get_filetypes_by_name", (PyCFunction)Filetype_filetypes_by_name, METH_VARARGS },
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


Filetype *Filetype_create_new_from_geany_filetype(GeanyFiletype *ft)
{
    Filetype *self;
    self = (Filetype *) PyObject_CallObject((PyObject *) &FiletypeType, NULL);
    self->ft = ft;
    return self;
}
