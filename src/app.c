#include "geanypy.h"


typedef struct
{
	PyObject_HEAD
	GeanyApp *app;
} App;


static void
App_dealloc(App *self)
{
	self->ob_type->tp_free((PyObject *) self);
}


static int
App_init(App *self)
{
	self->app = geany_data->app;
	return 0;
}


static PyObject *
App_get_property(App *self, void *prop_name)
{
	if (!self || !self->app || !prop_name)
		Py_RETURN_NONE;

	if (g_str_equal(prop_name, "configdir") && self->app->configdir)
		return PyString_FromString(self->app->configdir);
#if ENABLE_PRIVATE
	else if (g_str_equal(prop_name, "datadir") && self->app->datadir)
		return PyString_FromString(self->app->datadir);
	else if (g_str_equal(prop_name, "docdir") && self->app->docdir)
		return PyString_FromString(self->app->docdir);
#endif
	else if (g_str_equal(prop_name, "debug_mode") && self->app->debug_mode)
	{
		if (self->app->debug_mode)
			Py_RETURN_TRUE;
		else
			Py_RETURN_FALSE;
	}
	else if (g_str_equal(prop_name, "project") && self->app->project)
		return (PyObject *) GEANYPY_NEW(Project);

	Py_RETURN_NONE;
}


static int
App_set_property(App *self, PyObject *value, void *closure)
{
	PyErr_SetString(PyExc_AttributeError, "can't set attribute");
		return -1;
}


static PyGetSetDef App_getseters[] = {
	{ "configdir", (getter) App_get_property, (setter) App_set_property,
		"User configuration directory, usually ~/.config/geany. ",
		"configdir" },
#ifdef ENABLE_PRIVATE
	{ "datadir", (getter) App_get_property, (setter) App_set_property,
		"Geany's data directory.",
		"datadir" },
	{ "docdir", (getter) App_get_property, (setter) App_set_property,
		"Geany's documentation directory.",
		"docdir" },
#endif
	{ "debug_mode", (getter) App_get_property, (setter) App_set_property,
		"True if debug messages should be printed.",
		"debug_mode" },
	{ "project", (getter) App_get_property, (setter) App_set_property,
		"Currently active project or None if none is open.",
		"project" },
	{ NULL }
};


static PyTypeObject AppType = {
	PyObject_HEAD_INIT(NULL)
	0,											/* ob_size */
	"geany.app.App",							/* tp_name */
	sizeof(App),								/* tp_basicsize */
	0,											/* tp_itemsize */
	(destructor)App_dealloc,					/* tp_dealloc */
	0,											/* tp_print */
	0,											/* tp_getattr */
	0,											/* tp_setattr */
	0,											/* tp_compare */
	0,											/* tp_repr */
	0,											/* tp_as_number */
	0,											/* tp_as_sequence */
	0,											/* tp_as_mapping */
	0,											/* tp_hash  */
	0,											/* tp_call */
	0,											/* tp_str */
	0,											/* tp_getattro */
	0,											/* tp_setattro */
	0,											/* tp_as_buffer */
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,	/* tp_flags */
	"Wrapper around a GeanyApp structure.",		/* tp_doc  */
	0,											/* tp_traverse  */
	0,											/* tp_clear  */
	0,											/* tp_richcompare */
	0,											/* tp_weaklistoffset */
	0,											/* tp_iter */
	0,											/* tp_iternext */
	0,											/* tp_methods */
	0,											/* tp_members */
	App_getseters,								/* tp_getset */
	0,											/* tp_base */
	0,											/* tp_dict */
	0,											/* tp_descr_get */
	0,											/* tp_descr_set */
	0,											/* tp_dictoffset */
	(initproc)App_init,							/* tp_init */
	0,											/* tp_alloc */
	0,											/* tp_new */
};


static PyMethodDef AppModule_methods[] = { { NULL } };


PyMODINIT_FUNC initapp(void)
{
	PyObject *m;

	AppType.tp_new = PyType_GenericNew;
	if (PyType_Ready(&AppType) < 0)
		return;

	m = Py_InitModule("app", AppModule_methods);

	Py_INCREF(&AppType);
	PyModule_AddObject(m, "App", (PyObject *)&AppType);
}
