#include <Python.h>
#include <structmember.h>
#include <gtk/gtk.h>
#include <geanyplugin.h>
#include "plugin.h"


extern GeanyPlugin		*geany_plugin;
extern GeanyData		*geany_data;
extern GeanyFunctions	*geany_functions;


static void
FilePrefs_dealloc(FilePrefs *self)
{
	self->ob_type->tp_free((PyObject *) self);
}


static int
FilePrefs_init(FilePrefs *self, PyObject *args, PyObject *kwds)
{
    self->file_prefs = geany_data->file_prefs;
	return 0;
}


static PyObject *
FilePrefs__get_default_new_encoding(FilePrefs *self, PyObject *args)
{
    if (self->file_prefs != NULL)
        return Py_BuildValue("i", self->file_prefs->default_new_encoding);
    Py_RETURN_NONE;
}


static PyObject *
FilePrefs__get_default_open_encoding(FilePrefs *self, PyObject *args)
{
    if (self->file_prefs != NULL)
        return Py_BuildValue("i", self->file_prefs->default_open_encoding);
    Py_RETURN_NONE;
}


static PyObject *
FilePrefs__get_final_new_line(FilePrefs *self, PyObject *args)
{
    if (self->file_prefs != NULL)
    {
        if (self->file_prefs->final_new_line)
            Py_RETURN_TRUE;
        else
            Py_RETURN_FALSE;
    }
    Py_RETURN_NONE;
}


static PyObject *
FilePrefs__get_strip_trailing_spaces(FilePrefs *self, PyObject *args)
{
    if (self->file_prefs != NULL)
    {
        if (self->file_prefs->strip_trailing_spaces)
            Py_RETURN_TRUE;
        else
            Py_RETURN_FALSE;
    }
    Py_RETURN_NONE;
}


static PyObject *
FilePrefs__get_replace_tabs(FilePrefs *self, PyObject *args)
{
    if (self->file_prefs != NULL)
    {
        if (self->file_prefs->replace_tabs)
            Py_RETURN_TRUE;
        else
            Py_RETURN_FALSE;
    }
    Py_RETURN_NONE;
}


static PyObject *
FilePrefs__get_tab_order_ltr(FilePrefs *self, PyObject *args)
{
    if (self->file_prefs != NULL)
    {
        if (self->file_prefs->tab_order_ltr)
            Py_RETURN_TRUE;
        else
            Py_RETURN_FALSE;
    }
    Py_RETURN_NONE;
}


static PyObject *
FilePrefs__get_tab_order_beside(FilePrefs *self, PyObject *args)
{
    if (self->file_prefs != NULL)
    {
        if (self->file_prefs->tab_order_beside)
            Py_RETURN_TRUE;
        else
            Py_RETURN_FALSE;
    }
    Py_RETURN_NONE;
}


static PyObject *
FilePrefs__get_show_tab_cross(FilePrefs *self, PyObject *args)
{
    if (self->file_prefs != NULL)
    {
        if (self->file_prefs->show_tab_cross)
            Py_RETURN_TRUE;
        else
            Py_RETURN_FALSE;
    }
    Py_RETURN_NONE;
}


static PyObject *
FilePrefs__get_mru_length(FilePrefs *self, PyObject *args)
{
    if (self->file_prefs != NULL)
        return Py_BuildValue("I", self->file_prefs->mru_length);
    Py_RETURN_NONE;
}


static PyObject *
FilePrefs__get_default_eol_character(FilePrefs *self, PyObject *args)
{
    if (self->file_prefs != NULL)
        return Py_BuildValue("i", self->file_prefs->default_eol_character);
    Py_RETURN_NONE;
}


static PyObject *
FilePrefs__get_disk_check_timeout(FilePrefs *self, PyObject *args)
{
    if (self->file_prefs != NULL)
        return Py_BuildValue("i", self->file_prefs->disk_check_timeout);
    Py_RETURN_NONE;
}


static PyObject *
FilePrefs__get_cmdline_new_files(FilePrefs *self, PyObject *args)
{
    if (self->file_prefs != NULL)
    {
        if (self->file_prefs->cmdline_new_files)
            Py_RETURN_TRUE;
        else
            Py_RETURN_FALSE;
    }
    Py_RETURN_NONE;
}


static PyObject *
FilePrefs__get_use_safe_file_saving(FilePrefs *self, PyObject *args)
{
    if (self->file_prefs != NULL)
    {
        if (self->file_prefs->use_safe_file_saving)
            Py_RETURN_TRUE;
        else
            Py_RETURN_FALSE;
    }
    Py_RETURN_NONE;
}


static PyObject *
FilePrefs__get_ensure_convert_new_lines(FilePrefs *self, PyObject *args)
{
    if (self->file_prefs != NULL)
    {
        if (self->file_prefs->ensure_convert_new_lines)
            Py_RETURN_TRUE;
        else
            Py_RETURN_FALSE;
    }
    Py_RETURN_NONE;
}


static PyObject *
FilePrefs__get_gio_unsafe_save_backup(FilePrefs *self, PyObject *args)
{
    if (self->file_prefs != NULL)
    {
        if (self->file_prefs->gio_unsafe_save_backup)
            Py_RETURN_TRUE;
        else
            Py_RETURN_FALSE;
    }
    Py_RETURN_NONE;
}


static PyObject *
FilePrefs__get_use_gio_unsafe_file_saving(FilePrefs *self, PyObject *args)
{
    if (self->file_prefs != NULL)
    {
        if (self->file_prefs->use_gio_unsafe_file_saving)
            Py_RETURN_TRUE;
        else
            Py_RETURN_FALSE;
    }
    Py_RETURN_NONE;
}


static PyMethodDef FilePrefs_methods[] = {
    { "_get_default_new_encoding", (PyCFunction) FilePrefs__get_default_new_encoding, METH_VARARGS },
    { "_get_default_open_encoding", (PyCFunction) FilePrefs__get_default_open_encoding, METH_VARARGS },
	{ "_get_final_new_line", (PyCFunction) FilePrefs__get_final_new_line, METH_VARARGS },
    { "_get_strip_trailing_spaces", (PyCFunction) FilePrefs__get_strip_trailing_spaces, METH_VARARGS },
    { "_get_replace_tabs", (PyCFunction) FilePrefs__get_replace_tabs, METH_VARARGS },
    { "_get_tab_order_ltr", (PyCFunction) FilePrefs__get_tab_order_ltr, METH_VARARGS },
    { "_get_tab_order_beside", (PyCFunction) FilePrefs__get_tab_order_beside, METH_VARARGS },
    { "_get_show_tab_cross", (PyCFunction) FilePrefs__get_show_tab_cross, METH_VARARGS },
    { "_get_mru_length", (PyCFunction) FilePrefs__get_mru_length, METH_VARARGS },
    { "_get_default_eol_char", (PyCFunction) FilePrefs__get_default_eol_character, METH_VARARGS },
    { "_get_disk_check_timeout", (PyCFunction) FilePrefs__get_disk_check_timeout, METH_VARARGS },
    { "_get_cmdline_new_files", (PyCFunction) FilePrefs__get_cmdline_new_files, METH_VARARGS },
    { "_get_use_safe_file_saving", (PyCFunction) FilePrefs__get_use_safe_file_saving, METH_VARARGS },
    { "_get_ensure_convert_new_lines", (PyCFunction) FilePrefs__get_ensure_convert_new_lines, METH_VARARGS },
    { "_get_gio_unsafe_save_backup", (PyCFunction) FilePrefs__get_gio_unsafe_save_backup, METH_VARARGS },
    { "_get_use_gio_unsafe_file_saving", (PyCFunction) FilePrefs__get_use_gio_unsafe_file_saving, METH_VARARGS },
    { NULL }
};


static PyMemberDef FilePrefs_members[] = {
	{ NULL }
};


static PyTypeObject FilePrefsType = {
	PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "_geany_file_prefs.FilePrefs",  /*tp_name*/
    sizeof(FilePrefs),         /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)FilePrefs_dealloc, /*tp_dealloc*/
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
    "Geany file_prefs",          /* tp_doc */
    0,		                   /* tp_traverse */
    0,		               	   /* tp_clear */
    0,		                   /* tp_richcompare */
    0,		                   /* tp_weaklistoffset */
    0,		                   /* tp_iter */
    0,		                   /* tp_iternext */
    FilePrefs_methods,   /* tp_methods */
    FilePrefs_members,   /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)FilePrefs_init,  /* tp_init */
    0,                         /* tp_alloc */
    0,       /* tp_new */

};

static
PyMethodDef FilePrefsModule_methods[] = {
    { NULL }
};


PyMODINIT_FUNC
init_geany_file_prefs(void)
{
    PyObject *m;

    FilePrefsType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&FilePrefsType) < 0)
        return;

    m = Py_InitModule("_geany_file_prefs", FilePrefsModule_methods);

    Py_INCREF(&FilePrefsType);
    PyModule_AddObject(m, "FilePrefs", (PyObject *)&FilePrefsType);
}
