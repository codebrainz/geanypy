#include <Python.h>
#include <structmember.h>
#include <gtk/gtk.h>
#include <geanyplugin.h>
#include <Scintilla.h>
#include <ScintillaWidget.h>
#include "plugin.h"


static void
ScintillaNotification_dealloc(ScintillaNotification *self)
{
	self->ob_type->tp_free((PyObject *) self);
}


static int
ScintillaNotification_init(ScintillaNotification *self, PyObject *args, PyObject *kwds)
{
    self->notif = NULL;
	return 0;
}


static PyObject *
ScintillaNotification__get_header_hwnd_from(ScintillaNotification *self, PyObject *args)
{
    return PyLong_FromVoidPtr(self->notif->nmhdr.hwndFrom);
}


static PyObject *
ScintillaNotification__get_header_id_from(ScintillaNotification *self, PyObject *args)
{
    return PyLong_FromLong(self->notif->nmhdr.idFrom);
}


static PyObject *
ScintillaNotification__get_header_code(ScintillaNotification *self, PyObject *args)
{
    return Py_BuildValue("I", self->notif->nmhdr.code);
}


static PyObject *
ScintillaNotification__get_position(ScintillaNotification *self, PyObject *args)
{
    return Py_BuildValue("i", self->notif->position);
}


static PyObject *
ScintillaNotification__get_ch(ScintillaNotification *self, PyObject *args)
{
    return Py_BuildValue("c", self->notif->ch);
}

static PyObject *
ScintillaNotification__get_modifiers(ScintillaNotification *self, PyObject *args)
{
    return Py_BuildValue("i", self->notif->modifiers);
}


static PyObject *
ScintillaNotification__get_modification_type(ScintillaNotification *self, PyObject *args)
{
    return Py_BuildValue("i", self->notif->modificationType);
}


static PyObject *
ScintillaNotification__get_text(ScintillaNotification *self, PyObject *args)
{
    return Py_BuildValue("s", self->notif->text);
}


static PyObject *
ScintillaNotification__get_length(ScintillaNotification *self, PyObject *args)
{
    return Py_BuildValue("i", self->notif->length);
}


static PyObject *
ScintillaNotification__get_lines_added(ScintillaNotification *self, PyObject *args)
{
    return Py_BuildValue("i", self->notif->linesAdded);
}


static PyObject *
ScintillaNotification__get_message(ScintillaNotification *self, PyObject *args)
{
    return Py_BuildValue("i", self->notif->message);
}


static PyObject *
ScintillaNotification__get_w_param(ScintillaNotification *self, PyObject *args)
{
    return Py_BuildValue("l", self->notif->wParam);
}


static PyObject *
ScintillaNotification__get_l_param(ScintillaNotification *self, PyObject *args)
{
    return Py_BuildValue("l", self->notif->lParam);
}


static PyObject *
ScintillaNotification__get_line(ScintillaNotification *self, PyObject *args)
{
    return Py_BuildValue("i", self->notif->line);
}


static PyObject *
ScintillaNotification__get_fold_level_now(ScintillaNotification *self, PyObject *args)
{
    return Py_BuildValue("i", self->notif->foldLevelNow);
}



static PyObject *
ScintillaNotification__get_fold_level_prev(ScintillaNotification *self, PyObject *args)
{
    return Py_BuildValue("i", self->notif->foldLevelPrev);
}


static PyObject *
ScintillaNotification__get_margin(ScintillaNotification *self, PyObject *args)
{
    return Py_BuildValue("i", self->notif->margin);
}


static PyObject *
ScintillaNotification__get_list_type(ScintillaNotification *self, PyObject *args)
{
    return Py_BuildValue("i", self->notif->listType);
}


static PyObject *
ScintillaNotification__get_x(ScintillaNotification *self, PyObject *args)
{
    return Py_BuildValue("i", self->notif->x);
}


static PyObject *
ScintillaNotification__get_y(ScintillaNotification *self, PyObject *args)
{
    return Py_BuildValue("i", self->notif->y);
}


static PyObject *
ScintillaNotification__get_token(ScintillaNotification *self, PyObject *args)
{
    return Py_BuildValue("i", self->notif->token);
}


static PyObject *
ScintillaNotification__get_annotation_lines_added(ScintillaNotification *self, PyObject *args)
{
    return Py_BuildValue("i", self->notif->annotationLinesAdded);
}


static PyObject *
ScintillaNotification__get_updated(ScintillaNotification *self, PyObject *args)
{
    return Py_BuildValue("i", self->notif->updated);
}


static PyMethodDef ScintillaNotification_methods[] = {
    { "_get_header_hwnd_from", (PyCFunction) ScintillaNotification__get_header_hwnd_from, METH_VARARGS },
    { "_get_header_id_from", (PyCFunction) ScintillaNotification__get_header_id_from, METH_VARARGS },
    { "_get_header_code", (PyCFunction) ScintillaNotification__get_header_code, METH_VARARGS },
    { "_get_position", (PyCFunction) ScintillaNotification__get_position, METH_VARARGS },
    { "_get_ch", (PyCFunction) ScintillaNotification__get_ch, METH_VARARGS },
    { "_get_modifiers", (PyCFunction) ScintillaNotification__get_modifiers, METH_VARARGS },
    { "_get_modification_type", (PyCFunction) ScintillaNotification__get_modification_type, METH_VARARGS },
    { "_get_text", (PyCFunction) ScintillaNotification__get_text, METH_VARARGS },
    { "_get_length", (PyCFunction) ScintillaNotification__get_length, METH_VARARGS },
    { "_get_lines_added", (PyCFunction) ScintillaNotification__get_lines_added, METH_VARARGS },
    { "_get_message", (PyCFunction) ScintillaNotification__get_message, METH_VARARGS },
    { "_get_w_param", (PyCFunction) ScintillaNotification__get_w_param, METH_VARARGS },
    { "_get_l_param", (PyCFunction) ScintillaNotification__get_l_param, METH_VARARGS },
    { "_get_line", (PyCFunction) ScintillaNotification__get_line, METH_VARARGS },
    { "_get_fold_level_now", (PyCFunction) ScintillaNotification__get_fold_level_now, METH_VARARGS },
    { "_get_fold_level_prev", (PyCFunction) ScintillaNotification__get_fold_level_prev, METH_VARARGS },
	{ "_get_margin", (PyCFunction) ScintillaNotification__get_margin, METH_VARARGS },
    { "_get_list_type", (PyCFunction) ScintillaNotification__get_list_type, METH_VARARGS },
    { "_get_x", (PyCFunction) ScintillaNotification__get_x, METH_VARARGS },
    { "_get_y", (PyCFunction) ScintillaNotification__get_y, METH_VARARGS },
    { "_get_token", (PyCFunction) ScintillaNotification__get_token, METH_VARARGS },
    { "_get_annotation_lines_added", (PyCFunction) ScintillaNotification__get_annotation_lines_added, METH_VARARGS },
    { "_get_updated", (PyCFunction) ScintillaNotification__get_updated, METH_VARARGS },
    { NULL }
};


static PyMemberDef ScintillaNotification_members[] = {
	{ NULL }
};



static PyTypeObject ScintillaNotificationType = {
	PyObject_HEAD_INIT(NULL)
    0,                          /*ob_size*/
    "_geany_scintilla_notification.Notification",  /*tp_name*/
    sizeof(Scintilla),          /*tp_basicsize*/
    0,                          /*tp_itemsize*/
    (destructor)ScintillaNotification_dealloc, /*tp_dealloc*/
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
    "Geany scintilla notification",          /* tp_doc */
    0,		                    /* tp_traverse */
    0,		               	    /* tp_clear */
    0,		                    /* tp_richcompare */
    0,		                    /* tp_weaklistoffset */
    0,		                    /* tp_iter */
    0,		                    /* tp_iternext */
    ScintillaNotification_methods,          /* tp_methods */
    ScintillaNotification_members,          /* tp_members */
    0,                          /* tp_getset */
    0,                          /* tp_base */
    0,                          /* tp_dict */
    0,                          /* tp_descr_get */
    0,                          /* tp_descr_set */
    0,                          /* tp_dictoffset */
    (initproc)ScintillaNotification_init,   /* tp_init */
    0,                          /* tp_alloc */
    0,                          /* tp_new */

};


static PyMethodDef ScintillaNotificationModule_methods[] = { { NULL } };


PyMODINIT_FUNC
init_geany_scintilla_notification(void)
{
    PyObject *m;

    ScintillaNotificationType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&ScintillaNotificationType) < 0)
        return;

    m = Py_InitModule("_geany_scintilla_notification", ScintillaNotificationModule_methods);

    Py_INCREF(&ScintillaNotificationType);
    PyModule_AddObject(m, "Notification", (PyObject *)&ScintillaNotificationType);
}


ScintillaNotification *ScintillaNotification_create_new_from_scintilla_notification(SCNotification *notif)
{
    ScintillaNotification *self;
    self = (ScintillaNotification *) PyObject_CallObject((PyObject *) &ScintillaNotificationType, NULL);
    self->notif = notif;
    return self;
}
