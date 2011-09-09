#include "geanypy.h"


typedef struct
{
	PyObject_HEAD
	GeanyPrefs *prefs;
} Prefs;


static void
Prefs_dealloc(Prefs *self)
{
	g_return_if_fail(self != NULL);
	self->ob_type->tp_free((PyObject *) self);
}


static int
Prefs_init(Prefs *self)
{
	g_return_val_if_fail(self != NULL, -1);
	self->prefs = geany_data->prefs;
	return 0;
}


static PyObject *
Prefs_get_property(Prefs *self, const gchar *prop_name)
{
	g_return_val_if_fail(self != NULL, NULL);
	g_return_val_if_fail(prop_name != NULL, NULL);

	if (g_str_equal(prop_name, "default_open_path") && self->prefs->default_open_path)
		return PyString_FromString(self->prefs->default_open_path);
	Py_RETURN_FALSE;
}
GEANYPY_PROPS_READONLY(Prefs);


static PyGetSetDef Prefs_getseters[] = {
	GEANYPY_GETSETDEF(Prefs, "default_open_path",
		"Default path to look for files when no other path is appropriate."),
	{ NULL }
};


static PyTypeObject PrefsType = {
	PyObject_HEAD_INIT(NULL)
	0,											/* ob_size */
	"geany.prefs.Prefs",						/* tp_name */
	sizeof(Prefs),								/* tp_basicsize */
	0,											/* tp_itemsize */
	(destructor)Prefs_dealloc,					/* tp_dealloc */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* tp_print - tp_as_buffer */
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,	/* tp_flags */
	"Wrapper around a GeanyPrefs structure.",	/* tp_doc  */
	0, 0, 0, 0, 0, 0, 0, 0,						/* tp_traverse - tp_members */
	Prefs_getseters,							/* tp_getset */
	0, 0, 0, 0, 0,								/* tp_base - tp_dictoffset */
	(initproc)Prefs_init,						/* tp_init */
	0, 0,										/* tp_alloc - tp_new */
};


static PyMethodDef PrefsModule_methods[] = { { NULL } };


PyMODINIT_FUNC initprefs(void)
{
	PyObject *m;

	PrefsType.tp_new = PyType_GenericNew;
	if (PyType_Ready(&PrefsType) < 0)
		return;

	m = Py_InitModule3("prefs", PrefsModule_methods,
			"General preferences dialog settings");

	Py_INCREF(&PrefsType);
	PyModule_AddObject(m, "Prefs", (PyObject *)&PrefsType);
}
