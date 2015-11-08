#include "geanypy.h"

static void PluginBase_dealloc(GeanyPyPluginBase *self)
{
}

static gboolean call_key(gpointer *unused, guint key_id, gpointer data)
{
	PyObject *callback = data;
	PyObject *args;

	args = Py_BuildValue("(i)", key_id);
	PyObject_CallObject(callback, args);
	Py_DECREF(args);
}

static PyObject *
PluginBase_set_kb_group(GeanyPyPluginBase *self, PyObject *args, PyObject *kwargs)
{
	static gchar *kwlist[] = { "section_name", "count", "callback", NULL };
	int count = 0;
	const gchar *section_name = NULL;
	GeanyKeyGroup *group = NULL;
	PyObject *py_callback = NULL;
	if (PyArg_ParseTupleAndKeywords(args, kwargs, "si|O", kwlist, &section_name, &count, &py_callback))
	{
		if (PyCallable_Check(py_callback))
		{
			Py_INCREF(py_callback);
			group = plugin_set_key_group_full(self->plugin, section_name, count,
			                                  (GeanyKeyGroupFunc) call_key, py_callback, Py_DecRef);
		}
		else
			group = plugin_set_key_group(self->plugin, section_name, count, NULL);
	}

	if (group)
	{
		GObject *wrapper;
		PyObject *ret;
		wrapper = g_object_new(G_TYPE_OBJECT, NULL);
		g_object_set_data(wrapper, "pointer", group);
		ret = pygobject_new(wrapper);
		g_object_unref(wrapper);
		return ret;
	}
	Py_RETURN_NONE;
}

static PyObject *
PluginBase_set_kb_item(GeanyPyPluginBase *self, PyObject *args, PyObject *kwargs)
{
	static gchar *kwlist[] = { "key_group", "key_id", "key", "mod", "name", "label", "menu_item", "callback", NULL };
	int id = -1;
	int key;
	int mod;
	PyObject *py_group;
	const gchar *name = NULL, *label = NULL;
	PyObject *py_menu_item = NULL;
	PyObject *py_callback  = NULL;

	if (PyArg_ParseTupleAndKeywords(args, kwargs, "Oiiiss|OO", kwlist,
		&py_group, &id, &key, &mod, &name, &label, &py_menu_item, &py_callback) && id >= 0)
	{
		GObject   *group_wrapper = G_OBJECT(pygobject_get(py_group));
		GtkWidget *menu_item = (py_menu_item == NULL || py_menu_item == Py_None)
									? NULL : GTK_WIDGET(pygobject_get(py_menu_item));
		GeanyKeyGroup *group = g_object_get_data(group_wrapper, "pointer");
		if (PyCallable_Check(py_callback))
		{
			Py_INCREF(py_callback);
			keybindings_set_item_full(group, id, (guint) key, (GdkModifierType) mod, name, label,
									  menu_item, (GeanyKeyBindingFunc) call_key, py_callback, Py_DecRef);
		}
		else
			keybindings_set_item(group, id, NULL, (guint) key, (GdkModifierType) mod, name, label,
									  menu_item);
	}
	Py_RETURN_NONE;
}

static PyMethodDef
PluginBase_methods[] = {
	{ "set_key_group",				(PyCFunction)PluginBase_set_kb_group,	METH_KEYWORDS,
		"Sets up a GeanyKeybindingGroup for this plugin." },
	{ "set_key_item",				(PyCFunction)PluginBase_set_kb_item,	METH_KEYWORDS,
		"Adds an action to one of the plugin's key groups" },
	{ NULL }
};

static PyMethodDef
PluginModule_methods[] = {
	{ NULL }
};

static PyGetSetDef
PluginBase_getseters[] = {
	{ NULL },
};

static int
PluginBase_init(GeanyPyPluginBase *self, PyObject *args, PyObject *kwargs)
{
	GeanyPyPluginBase *py_context;

	if (PyArg_ParseTuple(args, "O", (PyObject **) &py_context) && (PyObject *)py_context != Py_None)
		self->plugin = py_context->plugin;

	return 0;
}

PyTypeObject PluginBaseType = {
	PyObject_HEAD_INIT(NULL)
	0,											/* ob_size */
	"geany.pluginbase.PluginBase",					/* tp_name */
	sizeof(GeanyPyPluginBase),								/* tp_basicsize */
	0,											/* tp_itemsize */
	(destructor) PluginBase_dealloc,				/* tp_dealloc */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* tp_print - tp_as_buffer */
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,	/* tp_flags */
	"Wrapper around a GeanyPlugin structure."	,/* tp_doc */
	0, 0, 0, 0, 0, 0,							/* tp_traverse - tp_iternext */
	PluginBase_methods,							/* tp_methods */
	0,											/* tp_members */
	PluginBase_getseters,							/* tp_getset */
	0, 0, 0, 0, 0,								/* tp_base - tp_dictoffset */
	(initproc) PluginBase_init,						/* tp_init */
	0, 0,										/* tp_alloc - tp_new */
};

PyMODINIT_FUNC initpluginbase(void)
{
	PyObject *m;

	PluginBaseType.tp_new = PyType_GenericNew;
	if (PyType_Ready(&PluginBaseType) < 0)
		return;

	m = Py_InitModule3("geany.pluginbase", PluginModule_methods,
			"Plugin management.");

	Py_INCREF(&PluginBaseType);
	PyModule_AddObject(m, "PluginBase", (PyObject *)&PluginBaseType);
}
