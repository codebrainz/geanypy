#include "geanypy.h"


#define KB_MAX 256
#define KB_LABEL_MAX 255

typedef struct
{
	gchar *kg_name;
	GeanyKeyGroup *group;
	GeanyKeyBinding *bindings[KB_MAX] ;
	PyObject *callbacks[KB_MAX];
	PyObject *user_data[KB_MAX];
	gchar labels[KB_MAX][KB_LABEL_MAX];
	gboolean slots[KB_MAX];
	gboolean registered[KB_MAX];
	gboolean initialized;
} KbManager;


static KbManager manager = { NULL };


static gchar *name_to_key(const gchar *name)
{
	gchar *ptr, *ret = g_strdup(name);
	for (ptr = ret; *ptr != '\0'; ptr++) {
		if (!isalnum(*ptr))
			*ptr = '_';
	}
	if (strlen(ret) && !isalpha(ret[0]))
		ret[0] = '_';
	return ret;
}


static uint next_key_id(gboolean *found)
{
	uint i;
	for (i = 0; i < KB_MAX; i++)
	{
		if (!manager.slots[i])
		{
			if (found)
				*found = TRUE;
			return i;
		}
	}
	if (found)
		*found = FALSE;
	return 0;
}


static void on_keybinding_activate(guint key_id)
{
	PyObject *args;

	g_return_if_fail(key_id < KB_MAX);

	if (manager.slots[key_id])
	{
		g_return_if_fail(manager.callbacks[key_id]);
		g_return_if_fail(PyCallable_Check(manager.callbacks[key_id]));

		if (manager.user_data[key_id])
			args = Py_BuildValue("(iO)", key_id, manager.user_data[key_id]);
		else
			args = Py_BuildValue("(i)", key_id);

		if (!args)
		{
			g_warning("Unable to build Python arguments.");
			if (PyErr_Occurred())
				PyErr_Print();
			return;
		}

		(void) PyObject_CallObject(manager.callbacks[key_id], args);
		Py_DECREF(args);
	}
}


PyObject * kb_manager_init(PyObject *module)
{

	if (manager.initialized)
		Py_RETURN_TRUE;

	manager.kg_name = name_to_key("GeanyPy" /* FIXME: use plugin name */);
	manager.group = plugin_set_key_group(geany_plugin, manager.kg_name, KB_MAX,
							(GeanyKeyGroupCallback) on_keybinding_activate);

	if (manager.group)
		Py_RETURN_TRUE;
	else
		Py_RETURN_FALSE;
}


#if 0
PyObject * kb_manager_finalize(PyObject *module)
{
	uint i;
	for (i = 0; i < KB_MAX; i++)
	{
		Py_XDECREF(manager.callbacks[i]);
		Py_XDECREF(manager.user_data[i]);
		g_free(manager.labels[i]);
	}
	g_free(manager.kg_name);
	Py_RETURN_NONE;
}
#endif


PyObject * kb_manager_register_binding(PyObject *module, PyObject *args)
{
	guint id;
	gboolean found_id;
	gchar *key_name;
	const gchar *name, *label;
	PyObject *pCallback, *pData = NULL;

	if (!PyArg_ParseTuple(args, "ssO|O", &name, &label, &pCallback, &pData))
	{
		PyErr_SetString(PyExc_ValueError, "unable to parse arguments");
		return NULL;
	}

	found_id = FALSE;
	id = next_key_id(&found_id);
	if (!found_id)
	{
		PyErr_SetString(PyExc_RuntimeError, "no free keybindings left");
		return NULL;
	}

	if (!PyCallable_Check(pCallback))
	{
		PyErr_SetString(PyExc_ValueError, "callback function is not callable");
		return NULL;
	}

	/* TODO: need to check name, label, callback? */

	snprintf(manager.labels[id], KB_LABEL_MAX, "%s: %s", name, label);

	if (!manager.registered[id])
	{
		key_name = g_strdup_printf("%s_%d", manager.kg_name, id);
		manager.bindings[id] = keybindings_set_item(manager.group,
													(gsize) id,
													NULL,
													0,
													0,
													key_name,
													manager.labels[id],
													NULL);
		g_free(key_name);

		if (manager.bindings[id])
			manager.registered[id] = TRUE;
		else
		{
			PyErr_SetString(PyExc_RuntimeError, "unable to register keybinding");
			return NULL;
		}
	}

	Py_INCREF(pCallback);
	manager.callbacks[id] = pCallback;

	Py_XINCREF(pData);
	manager.user_data[id] = pData;

	manager.slots[id] = TRUE;

	return PyInt_FromLong((long) id);
}


PyObject * kb_manager_unregister_binding(PyObject  *module, PyObject *args)
{
	guint id;

	if (!PyArg_ParseTuple(args, "I", &id))
	{
		PyErr_SetString(PyExc_ValueError, "expecting a single int argument");
		return NULL;
	}

	if (id >= KB_MAX)
	{
		gchar *msg = g_strdup_printf("id is out of range (0-%d)", KB_MAX);
		PyErr_SetString(PyExc_IndexError, msg);
		g_free(msg);
		return NULL;
	}

	if (!manager.slots[id])
	{
		PyErr_SetString(PyExc_IndexError, "id is not registered");
		return NULL;
	}

	Py_XDECREF(manager.callbacks[id]);
	Py_XDECREF(manager.user_data[id]);
	manager.callbacks[id] = NULL;
	manager.user_data[id] = NULL;
	manager.slots[id] = FALSE;
	manager.labels[id][0] = '\0';

	Py_RETURN_NONE;
}


static PyMethodDef KbManagerModule_methods[] = {
	{ "init", (PyCFunction) kb_manager_init, METH_NOARGS,
		"Initialize the keybindings manager." },
	{ "register_binding", (PyCFunction) kb_manager_register_binding,
		METH_VARARGS, "Register a callback function for a keybinding event." },
	{ "unregister_binding", (PyCFunction) kb_manager_unregister_binding,
		METH_VARARGS, "Unregister a callback function by id." },
	{ NULL }
};


PyMODINIT_FUNC initbindings(void)
{
	PyObject *m;

	m = Py_InitModule3("bindings", KbManagerModule_methods,
			"Keybindings management.");
}