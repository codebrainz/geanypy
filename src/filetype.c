#include <Python.h>
#include <structmember.h>
#include <gtk/gtk.h>
#include <geanyplugin.h>
#include "plugin.h"


extern GeanyPlugin		*geany_plugin;
extern GeanyData		*geany_data;
extern GeanyFunctions	*geany_functions;


static void
Filetype_dealloc(Filetype *self)
{
	self->ob_type->tp_free((PyObject *) self);
}


static PyObject *
Filetype_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	Filetype *self;
	self = (Filetype *)type->tp_alloc(type, 0);
	if (self != NULL)
	{
		self->ft = NULL;
	}
    g_debug("Filetype_new() called");
	return (PyObject *) self;
}


static int
Filetype_init(Filetype *self, PyObject *args, PyObject *kwds)
{
    self->ft = NULL;

	return 0;
}


static PyObject *
Filetype__get_display_name(Filetype *self, PyObject *args)
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
Filetype__get_extension(Filetype *self, PyObject *args)
{
    if (self->ft->extension != NULL)
        return PyString_FromString(self->ft->extension);
    Py_RETURN_NONE;
}


static PyObject *
Filetype__get_index(Filetype *self, PyObject *args)
{
    PyObject *res;

    res = Py_BuildValue("i", self->ft->id);
    if (res)
        return res;
    Py_RETURN_NONE;
}


static PyObject *
Filetype__get_lang_id(Filetype *self, PyObject *args)
{
    PyObject *res;

    res = Py_BuildValue("i", self->ft->lang);
    if (res)
        return res;
    Py_RETURN_NONE;
}

static PyObject *
Filetype__get_name(Filetype *self, PyObject *args)
{
    if (self->ft->name != NULL)
        return PyString_FromString(self->ft->name);
    Py_RETURN_NONE;
}

static PyObject *
Filetype__get_patterns(Filetype *self, PyObject *args)
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
Filetype__get_title(Filetype *self, PyObject *args)
{
    if (self->ft->title != NULL)
        return PyString_FromString(self->ft->title);
    Py_RETURN_NONE;
}


static PyMethodDef Filetype_methods[] = {
    { "_get_display_name", (PyCFunction)Filetype__get_display_name, METH_VARARGS },
    { "_get_extension", (PyCFunction)Filetype__get_extension, METH_VARARGS },
    { "_get_index", (PyCFunction)Filetype__get_index, METH_VARARGS },
    { "_get_lang_id", (PyCFunction)Filetype__get_lang_id, METH_VARARGS },
    { "_get_name", (PyCFunction)Filetype__get_name, METH_VARARGS },
    { "_get_patterns", (PyCFunction)Filetype__get_patterns, METH_VARARGS },
    { "_get_title", (PyCFunction)Filetype__get_title, METH_VARARGS },
	{ NULL }
};


static PyMemberDef Filetype_members[] = {
	//{"active_profile", T_STRING, offsetof(Document, active_profile), 0,
		//"The profile used by Zen Coding when performing actions."},
	//{"context", T_LONG, offsetof(Document, context), 0,
		//"The current editor context used by Zen Coding when performing actions."},
	{ NULL }
};


static PyTypeObject FiletypeType = {
	PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "_geany_filetype.Filetype",  /*tp_name*/
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
    "Geany filetype",          /* tp_doc */
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
    Filetype_new,       /* tp_new */

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
    gint index = -1;

    if (PyArg_ParseTuple(args, "i", &index))
    {
        if (index == -1)
            Py_RETURN_NONE;

        if ((ft = filetypes_index(index)) != NULL)
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
    GSList *iter;
    Filetype *py_ft;
    PyObject *list;

    if (!(list = PyList_New(0)))
        Py_RETURN_NONE;

    for (iter = (GSList*)filetypes_index; iter->data != NULL; iter = g_slist_next(iter))
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
init_geany_filetype(void)
{
    PyObject *m;

    FiletypeType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&FiletypeType) < 0)
        return;

    m = Py_InitModule("_geany_filetype", FiletypeModule_methods);

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
