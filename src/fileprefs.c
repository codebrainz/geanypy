#include <Python.h>
#include <structmember.h>
#include <geanyplugin.h>
#include "plugin.h"


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
FilePrefs_get_default_new_encoding(FilePrefs *self, PyObject *args)
{
    if (self->file_prefs != NULL)
        return Py_BuildValue("i", self->file_prefs->default_new_encoding);
    Py_RETURN_NONE;
}


static PyObject *
FilePrefs_get_default_open_encoding(FilePrefs *self, PyObject *args)
{
    if (self->file_prefs != NULL)
        return Py_BuildValue("i", self->file_prefs->default_open_encoding);
    Py_RETURN_NONE;
}


static PyObject *
FilePrefs_get_final_new_line(FilePrefs *self, PyObject *args)
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
FilePrefs_get_strip_trailing_spaces(FilePrefs *self, PyObject *args)
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
FilePrefs_get_replace_tabs(FilePrefs *self, PyObject *args)
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
FilePrefs_get_tab_order_ltr(FilePrefs *self, PyObject *args)
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
FilePrefs_get_tab_order_beside(FilePrefs *self, PyObject *args)
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
FilePrefs_get_show_tab_cross(FilePrefs *self, PyObject *args)
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
FilePrefs_get_mru_length(FilePrefs *self, PyObject *args)
{
    if (self->file_prefs != NULL)
        return Py_BuildValue("I", self->file_prefs->mru_length);
    Py_RETURN_NONE;
}


static PyObject *
FilePrefs_get_default_eol_character(FilePrefs *self, PyObject *args)
{
    if (self->file_prefs != NULL)
        return Py_BuildValue("i", self->file_prefs->default_eol_character);
    Py_RETURN_NONE;
}


static PyObject *
FilePrefs_get_disk_check_timeout(FilePrefs *self, PyObject *args)
{
    if (self->file_prefs != NULL)
        return Py_BuildValue("i", self->file_prefs->disk_check_timeout);
    Py_RETURN_NONE;
}


static PyObject *
FilePrefs_get_cmdline_new_files(FilePrefs *self, PyObject *args)
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
FilePrefs_get_use_safe_file_saving(FilePrefs *self, PyObject *args)
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
FilePrefs_get_ensure_convert_new_lines(FilePrefs *self, PyObject *args)
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
FilePrefs_get_gio_unsafe_save_backup(FilePrefs *self, PyObject *args)
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
FilePrefs_get_use_gio_unsafe_file_saving(FilePrefs *self, PyObject *args)
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
    { "get_default_new_encoding", (PyCFunction) FilePrefs_get_default_new_encoding, METH_VARARGS },
    { "get_default_open_encoding", (PyCFunction) FilePrefs_get_default_open_encoding, METH_VARARGS },
	{ "get_final_new_line", (PyCFunction) FilePrefs_get_final_new_line, METH_VARARGS },
    { "get_strip_trailing_spaces", (PyCFunction) FilePrefs_get_strip_trailing_spaces, METH_VARARGS },
    { "get_replace_tabs", (PyCFunction) FilePrefs_get_replace_tabs, METH_VARARGS },
    { "get_tab_order_ltr", (PyCFunction) FilePrefs_get_tab_order_ltr, METH_VARARGS },
    { "get_tab_order_beside", (PyCFunction) FilePrefs_get_tab_order_beside, METH_VARARGS },
    { "get_show_tab_cross", (PyCFunction) FilePrefs_get_show_tab_cross, METH_VARARGS },
    { "get_mru_length", (PyCFunction) FilePrefs_get_mru_length, METH_VARARGS },
    { "get_default_eol_char", (PyCFunction) FilePrefs_get_default_eol_character, METH_VARARGS },
    { "get_disk_check_timeout", (PyCFunction) FilePrefs_get_disk_check_timeout, METH_VARARGS },
    { "get_cmdline_new_files", (PyCFunction) FilePrefs_get_cmdline_new_files, METH_VARARGS },
    { "get_use_safe_file_saving", (PyCFunction) FilePrefs_get_use_safe_file_saving, METH_VARARGS },
    { "get_ensure_convert_new_lines", (PyCFunction) FilePrefs_get_ensure_convert_new_lines, METH_VARARGS },
    { "get_gio_unsafe_save_backup", (PyCFunction) FilePrefs_get_gio_unsafe_save_backup, METH_VARARGS },
    { "get_use_gio_unsafe_file_saving", (PyCFunction) FilePrefs_get_use_gio_unsafe_file_saving, METH_VARARGS },
    { NULL }
};


static PyMemberDef FilePrefs_members[] = {
	{ NULL }
};


static PyTypeObject FilePrefsType = {
	PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "geany.fileprefs.FilePrefs",  /*tp_name*/
    sizeof(FilePrefs),         /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)FilePrefs_dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tpgetattr*/
    0,                         /*tpsetattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tpgetattro*/
    0,                         /*tpsetattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    "Wrapper class around Geany's `GeanyFilePrefs` structure.  This class "
    "should not be directly initialized, instead access a single instance of "
    "it through `geany.file_prefs`.", /* tp_doc */
    0,		                   /* tp_traverse */
    0,		               	   /* tp_clear */
    0,		                   /* tp_richcompare */
    0,		                   /* tp_weaklistoffset */
    0,		                   /* tp_iter */
    0,		                   /* tp_iternext */
    FilePrefs_methods,   /* tp_methods */
    FilePrefs_members,   /* tp_members */
    0,                         /* tpgetset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descrget */
    0,                         /* tp_descrset */
    0,                         /* tp_dictoffset */
    (initproc)FilePrefs_init,  /* tp_init */
    0,                         /* tp_alloc */
    0,       /* tp_new */

};

static PyMethodDef FilePrefsModule_methods[] = { { NULL } };


PyMODINIT_FUNC
initfileprefs(void)
{
    PyObject *m;

    FilePrefsType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&FilePrefsType) < 0)
        return;

    m = Py_InitModule3("fileprefs", FilePrefsModule_methods,
            "The `fileprefs` module provides a single wrapper class around a  "
            "`GeanyFilePrefs` structure.");

    Py_INCREF(&FilePrefsType);
    PyModule_AddObject(m, "FilePrefs", (PyObject *)&FilePrefsType);
}
