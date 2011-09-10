#include "geanypy.h"


static void
IndentPrefs_dealloc(IndentPrefs *self)
{
	self->ob_type->tp_free((PyObject *) self);
}


static int
IndentPrefs_init(IndentPrefs *self, PyObject *args, PyObject *kwds)
{
    self->indent_prefs = NULL;
	return 0;
}


static PyObject *
IndentPrefs_get_property(IndentPrefs *self, const gchar *prop_name)
{
	g_return_val_if_fail(self != NULL, NULL);
	g_return_val_if_fail(prop_name != NULL, NULL);

	if (!self->indent_prefs)
	{
		PyErr_SetString(PyExc_RuntimeError,
			"IndentPrefs instance not initialized properly");
		return NULL;
	}

	if (g_str_equal(prop_name, "width"))
		return PyInt_FromLong((glong) self->indent_prefs->width);
	else if (g_str_equal(prop_name, "type"))
		return PyInt_FromLong((glong) self->indent_prefs->type);
	else if (g_str_equal(prop_name, "hard_tab_width"))
		return PyInt_FromLong((glong) self->indent_prefs->hard_tab_width);

	Py_RETURN_NONE;
}
GEANYPY_PROPS_READONLY(IndentPrefs);


static PyGetSetDef IndentPrefs_getseters[] = {
	GEANYPY_GETSETDEF(IndentPrefs, "width", "Indent width in characters."),
	GEANYPY_GETSETDEF(IndentPrefs, "type",
		"Whether to use tabs, spaces, or both to indent."),
	GEANYPY_GETSETDEF(IndentPrefs, "hard_tab_width",
		"Width of a tab, but only when using INDENT_TYPE_BOTH."),
	{ NULL }
};


static PyTypeObject IndentPrefsType = {
	PyObject_HEAD_INIT(NULL)
    0,											/* ob_size */
    "geany.indentprefs.IndentPrefs",			/* tp_name */
    sizeof(IndentPrefs),						/* tp_basicsize */
    0,											/* tp_itemsize */
    (destructor) IndentPrefs_dealloc,			/* tp_dealloc */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* tp_print - tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,	/* tp_flags */
    "Wrapper around a GeanyApp structure.",		/* tp_doc */
	0, 0, 0, 0, 0, 0, 0, 0,						/* tp_traverse - tp_members */
	IndentPrefs_getseters,						/* tp_getset */
	0, 0, 0, 0, 0,								/* tp_base - tp_dictoffset */
	(initproc) IndentPrefs_init,				/* tp_init */
	0, 0,										/* tp_alloc - tp_new */
};


static PyMethodDef IndentPrefsModule_methods[] = { { NULL } };


PyMODINIT_FUNC
initindentprefs(void)
{
    PyObject *m;

    IndentPrefsType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&IndentPrefsType) < 0)
        return;

    m = Py_InitModule("indentprefs", IndentPrefsModule_methods);

    Py_INCREF(&IndentPrefsType);
    PyModule_AddObject(m, "IndentPrefs", (PyObject *)&IndentPrefsType);

    PyModule_AddIntConstant(m, "INDENT_TYPE_SPACES", (glong) GEANY_INDENT_TYPE_SPACES);
    PyModule_AddIntConstant(m, "INDENT_TYPE_TABS", (glong) GEANY_INDENT_TYPE_TABS);
    PyModule_AddIntConstant(m, "INDENT_TYPE_BOTH", (glong) GEANY_INDENT_TYPE_BOTH);
}


IndentPrefs *IndentPrefs_create_new_from_geany_indent_prefs(GeanyIndentPrefs *indent_prefs)
{
    IndentPrefs *self;
    self = (IndentPrefs *) PyObject_CallObject((PyObject *) &IndentPrefsType, NULL);
    self->indent_prefs = indent_prefs;
    return self;
}
