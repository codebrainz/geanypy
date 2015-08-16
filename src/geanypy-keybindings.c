/*
 * geanypy-keybindings.c
 *
 * Copyright 2011 Matthew Brush <mbrush@codebrainz.ca>
 * Copyright 2015 Fairview 5 Engineering, LLC <developers@fairview5.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#if defined(HAVE_CONFIG_H) && !defined(GEANYPY_WINDOWS)
# include "config.h"
#endif

#include "geanypy.h"
#include "geanypy-keybindings.h"

#define KB_MAX 256
#define KB_LABEL_MAX 255

static GHashTable *key_cmd_hash = NULL;
static GHashTable *key_binding_by_name_hash = NULL;
static GeanyKeyGroup *plugin_group = NULL;

typedef struct {
	gchar key_name[KB_LABEL_MAX];
	gchar label[KB_LABEL_MAX];
	gchar pg_name[KB_LABEL_MAX];
	GeanyKeyBinding *binding;
	PyObject *callback;
	PyObject *user_data;
} key_binding;

static key_binding *key_bindings[KB_MAX] = { NULL };

static gchar *name_to_key(const gchar *name)
{
	gchar *ptr, *ret = g_strdup(name);

	for (ptr = ret; *ptr != '\0'; ptr++) {
		if (!isalnum(*ptr)) {
			*ptr = '_';
		}
	}
	if (strlen(ret) && !isalpha(ret[0])) {
		ret[0] = '_';
	}

	return ret;
}

static uint next_key_id(gboolean *found)
{
	uint i;

	for (i = 0; i < KB_MAX; i++) {
		if (!key_bindings[i]) {
			if (found) {
				*found = TRUE;
			}
			return i;
		}
	}
	if (found) {
		*found = FALSE;
	}

	return 0;
}

static void on_keybinding_activate(guint key_id)
{
	PyObject *args;
	key_binding *kb;

	g_return_if_fail(key_id < KB_MAX);

	kb = key_bindings[key_id];
	if (!kb) {
		return;
	}

	g_return_if_fail(kb->callback);
	g_return_if_fail(PyCallable_Check(kb->callback));

	if (kb->user_data) {
		args = Py_BuildValue("(iO)", key_id, kb->user_data);
	} else {
		args = Py_BuildValue("(i)", key_id);
	}

	if (!args) {
		g_warning("Unable to build Python arguments.");
		if (PyErr_Occurred()) {
			PyErr_Print();
		}
		return;
	}

	(void) PyObject_CallObject(kb->callback, args);
	Py_DECREF(args);
}

PyObject * kb_manager_init(PyObject *module, PyObject *args, PyObject *kwargs)
{
	gchar *pgn;
	gchar *plugin_group_name;
	const gchar *temp_name;

	plugin_group_name = name_to_key("GeanyPy");

	if (!plugin_group) {
		plugin_group = plugin_set_key_group(geany_plugin, plugin_group_name,
		KB_MAX, (GeanyKeyGroupCallback) on_keybinding_activate);
	}

	if (plugin_group) {
		Py_RETURN_TRUE;
	} else {
		Py_RETURN_FALSE;
	}
}

PyObject * kb_manager_register_binding(PyObject *module, PyObject *args, PyObject *kwargs)
{
	guint id;
	gboolean found_id;
	gchar *key_name;
	gchar *key_key, *new_label;
	const gchar *label, *plugin_group_name;
	PyObject *pCallback, *pData = NULL;
	key_binding *kb;

	static gchar *kwlist[] = { "plugin_group_name", "key_name", "key_label",
		"callback", "data", NULL };

	if (!PyArg_ParseTupleAndKeywords(args, kwargs, "sssO|O", kwlist,
		&plugin_group_name, &key_name, &label, &pCallback, &pData)) {
		PyErr_SetString(PyExc_ValueError, "unable to parse arguments");
		return NULL;
	}

	key_key = g_strdup_printf("%s_%s", plugin_group_name, key_name);
	kb = g_hash_table_lookup(key_binding_by_name_hash, key_key);

	if (kb) {
		PyErr_SetString(PyExc_RuntimeError, "this key is already bound");
		g_free(key_key);
		return NULL;
	}

	found_id = FALSE;
	id = next_key_id(&found_id);
	if (!found_id) {
		PyErr_SetString(PyExc_RuntimeError, "no free keybindings left");
		g_free(key_key);
		return NULL;
	}

	if (!PyCallable_Check(pCallback)) {
		PyErr_SetString(PyExc_ValueError, "callback function is not callable");
		g_free(key_key);
		return NULL;
	}

	new_label = g_strdup_printf("%s: %s", plugin_group_name, key_name);
	kb = g_new0(key_binding, 1);
	strcpy(kb->key_name, key_name);
	strcpy(kb->pg_name, plugin_group_name);
	strcpy(kb->label, new_label);
	kb->callback = pCallback;
	kb->user_data = pData;

	kb->binding = keybindings_set_item(plugin_group, id, NULL, 0, 0, key_key,
		new_label, NULL);

	if (!kb->binding) {
		PyErr_SetString(PyExc_RuntimeError, "unable to register keybinding");
		g_free(key_key);
		g_free(new_label);
		return NULL;
	}

	Py_INCREF(pCallback);
	Py_XINCREF(pData);

	key_bindings[id] = kb;

	if (!g_hash_table_insert(key_binding_by_name_hash, key_key, kb)) {
		PyErr_SetString(PyExc_RuntimeError,
			"unable to insert to key_binding_by_name_hash");
		g_free(key_key);
		g_free(new_label);
		return NULL;
	}
	g_free(new_label);

	return PyInt_FromLong((long) id);
}

PyObject * kb_manager_unregister_binding(PyObject *module, PyObject *args,
	PyObject *kwargs)
{
	guint id;
	gchar *key_key;
	key_binding *kb;

	static gchar *kwlist[] = { "key_id", NULL };

	if (!PyArg_ParseTupleAndKeywords(args, kwargs, "I", kwlist, &id)) {
		PyErr_SetString(PyExc_ValueError, "expecting a single int argument");
		return NULL;
	}

	if (id >= KB_MAX) {
		gchar *msg = g_strdup_printf("id is out of range (0-%d)", KB_MAX);
		PyErr_SetString(PyExc_IndexError, msg);
		g_free(msg);
		return NULL;
	}

	if (!key_bindings[id]) {
		PyErr_SetString(PyExc_IndexError, "id is not registered");
		return NULL;
	}

	kb = key_bindings[id];
	Py_XDECREF(kb->callback);
	Py_XDECREF(kb->user_data);

	key_bindings[id] = NULL;

	key_key = g_strdup_printf("%s_%s", kb->pg_name, kb->key_name);
	g_hash_table_remove(key_binding_by_name_hash, key_key);
	g_free(key_key);

	Py_RETURN_NONE;
}

static void init_tables(void)
{
	gint i;

	key_cmd_hash = g_hash_table_new(g_str_hash, g_str_equal);

	for (i = 0; key_cmd_hash_entries[i].name; i++) {
		g_hash_table_insert(key_cmd_hash, (gpointer) key_cmd_hash_entries[i].name,
			&key_cmd_hash_entries[i]);
	}

	key_binding_by_name_hash = g_hash_table_new_full(g_str_hash, g_str_equal,
		g_free, NULL);

	for (i = 0; i < KB_MAX; i++) {
		key_bindings[i] = NULL;
	}
}

static void free_tables(void)
{
	gint i;

	if (key_cmd_hash) {
		g_hash_table_destroy(key_cmd_hash);
		key_cmd_hash = NULL;
	}

	for (i = 0; i < KB_MAX; i++) {
		if (key_bindings[i]) {
			g_free(key_bindings[i]);
			key_bindings[i] = NULL;
		}
	}

	if (key_binding_by_name_hash) {
		g_hash_table_destroy(key_binding_by_name_hash);
		key_binding_by_name_hash = NULL;
	}
}

#define lookup_key_cmd_str(cmd) g_hash_table_lookup(key_cmd_hash,cmd);

static PyObject *kb_manager_send_command(PyObject *module, PyObject *args,
	PyObject *kwargs)
{
	gchar *key;
	static gchar *kwlist[] = { "key", NULL };

	if (PyArg_ParseTupleAndKeywords(args, kwargs, "s", kwlist, &key)) {
		if (key) {
			KeyCmdHashEntry*he = NULL;
			gchar cmdbuf[64];
			gchar *cmdname;
			gint i;
			memset(cmdbuf, '\0', sizeof(cmdbuf));
			strncpy(cmdbuf, key, sizeof(cmdbuf) - 1);
			for (i = 0; cmdbuf[i]; i++) {
				cmdbuf[i] = g_ascii_toupper(cmdbuf[i]);
			}
			cmdname = cmdbuf;
			if (strncmp(cmdname, "GEANY_", 6) == 0) {
				cmdname += 6;
				if (strncmp(cmdname, "KEYS_", 5) == 0) {
					cmdname += 5;
				}
			}
			he = lookup_key_cmd_str(cmdname)
			;
			keybindings_send_command(he->group, he->key_id);
		}
	}

	Py_RETURN_NONE;
}

static PyMethodDef KeybindingsModule_methods[] = {
	{ "send_command", (PyCFunction) kb_manager_send_command, METH_KEYWORDS,
		"Sends a built-in keyboard command." },
	{ "init", (PyCFunction) kb_manager_init, METH_NOARGS,
		"Initialize the keybindings manager." },
	{ "register_binding", (PyCFunction) kb_manager_register_binding, METH_KEYWORDS,
		"Register a callback function for a keybinding event." },
	{ "unregister_binding", (PyCFunction) kb_manager_unregister_binding, METH_KEYWORDS,
		"Unregister a callback function by id." },
	{ NULL } };

PyMODINIT_FUNC initkeybindings(void)
{
	init_tables();
	Py_InitModule3("keybindings", KeybindingsModule_methods,
		"Keybindings related functions.");
}

PyMODINIT_FUNC cleanupkeybindings(void)
{
	free_tables();
}
