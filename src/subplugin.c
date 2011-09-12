#include "geanypy.h"
#include <string.h>
#include <ctype.h>


/* This is how many items will be in our keygroups, pre-reserved to avoid
 * adding them later. */
#define KEYBINDINGS_MAX 1024


/* This is stupid, I know */
typedef struct GeanyPluginPrivate // from pluginprivate.h
{
	GModule 		*module;
	gchar			*filename;
	PluginInfo		info;
	GeanyPlugin		public;

	void		(*init) (GeanyData *data);
	GtkWidget*	(*configure) (GtkDialog *dialog);
	void		(*configure_single) (GtkWidget *parent);
	void		(*help) (void);
	void		(*cleanup) (void);

	PluginFields	fields;
	GeanyKeyGroup	*key_group;
	GeanyAutoSeparator	toolbar_separator;
	GArray			*signal_ids;
	GList			*sources;
}
GeanyPluginPrivate;


/* Yes, yes, I know */
struct GeanyKeyGroup	// from plugindata.h
{
	const gchar *name; /* We'll set this ourselves, I guess */
	const gchar *label; /* We need to set this for whatever reason,
							otherwise the group name is not visible in
							the preferences dialog keybindings tab. */
	GeanyKeyGroupCallback callback;
	gboolean plugin;  /* We use this flag, I guess */
	GPtrArray *key_items;
	gsize plugin_key_count;
	GeanyKeyBinding *plugin_keys;
};


/* Converts, for ex. `4Foo -Bar12!` to `_foo__bar12_` or more typically,
 * `Foo Bar` to `foo_bar`. */
static gchar *name_to_identifier(const gchar *reg_name)
{
	gchar *ptr, *ret;

	g_return_val_if_fail(reg_name != NULL, NULL);
	g_return_val_if_fail(strlen(reg_name) > 0, NULL);

	ret = strdup(reg_name);

	for (ptr = ret; *ptr != '\0'; ptr++)
	{
		if (*ptr == ' ') {
			*ptr = '_';
			continue;
		}
		if (!isalnum(*ptr)) {
			*ptr = '_';
			continue;
		}
		*ptr = tolower(*ptr);
	}

	if (strlen(ret) > 0 && !isalpha(ret[0]) && ret[0] != '_')
		ret[0] = '_';

	return ret;
}


static void subplugin_set_info(SubPlugin *sub, PyObject *plugin)
{
	PyObject *str;

	g_return_if_fail(sub != NULL);
	g_return_if_fail(sub->sub != NULL);
	g_return_if_fail(sub->sub->info != NULL);
	g_return_if_fail(plugin != NULL);
	g_return_if_fail(plugin != Py_None);

	if (PyObject_HasAttrString(plugin, "__plugin_name__"))
	{
		str = PyObject_GetAttrString(plugin, "__plugin_name__");
		if (str && str != Py_None)
		{
			if (PyString_Check(str))
			{
				sub->name = g_strdup(PyString_AsString(str));
				sub->sub->info->name = (const gchar *) sub->name;
			}
			else
				sub->name = NULL;
			Py_DECREF(str);
		}
	}

	if (PyObject_HasAttrString(plugin, "__plugin_description__"))
	{
		str = PyObject_GetAttrString(plugin, "__plugin_description__");
		if (str && str != Py_None)
		{
			if (PyString_Check(str))
			{
				sub->description = g_strdup(PyString_AsString(str));
				sub->sub->info->description = (const gchar *) sub->description;
			}
			else
				sub->description = NULL;
			Py_DECREF(str);
		}
	}

	if (PyObject_HasAttrString(plugin, "__plugin_version__"))
	{
		str = PyObject_GetAttrString(plugin, "__plugin_version__");
		if (str && str != Py_None)
		{
			if (PyString_Check(str))
			{
				sub->version = g_strdup(PyString_AsString(str));
				sub->sub->info->version = (const gchar *) sub->version;
			}
			else
				sub->version = NULL;
			Py_DECREF(str);
		}
	}

	if (PyObject_HasAttrString(plugin, "__plugin_author__"))
	{
		str = PyObject_GetAttrString(plugin, "__plugin_author__");
		if (str && str != Py_None)
		{
			if (PyString_Check(str))
			{
				sub->author = g_strdup(PyString_AsString(str));
				sub->sub->info->author = (const gchar *) sub->author;
			}
			else
				sub->author = NULL;
			Py_DECREF(str);
		}
	}
}


static void on_subplugin_keybinding_added(GObject *source,
	const gchar *accel_string, PyObject *callback_func, SubPlugin *self)
{
	guint keyid;
	GdkModifierType mods;

	g_return_if_fail(self != NULL);

	g_debug("Plugin added '%s' keybinding.", accel_string);
	//gtk_accelerator_parse(accel_string, &keyid, &mods);
	//g_debug("Plugin added '%s' keybinding (%u, %d).", accel_string,
	//	keyid, (gint) mods);
	//subplugin_keybindings_set_item(self, 0, NULL, keyid, mods, accel_string, NULL);
}

static void on_subplugin_keybinding_removed(GObject *source,
	const gchar *accel_string, PyObject *callback_func, SubPlugin *self)
{
	g_debug("Plugin removed '%s' keybinding.", accel_string);
}


SubPlugin *subplugin_new(PyObject *py_plugin)
{
	SubPlugin *plugin;

	g_return_val_if_fail(geany_plugin != NULL, NULL);
	g_return_val_if_fail(py_plugin != NULL, NULL);
	g_return_val_if_fail(py_plugin != Py_None, NULL);

	plugin = g_new0(SubPlugin, 1);
	plugin->n_items = 0;
	plugin->kb_list = NULL;

	plugin->sub = g_new0(GeanyPlugin, 1);
	plugin->sub->info = g_new0(PluginInfo, 1);
	plugin->sub->priv = g_new0(struct GeanyPluginPrivate, 1);

	plugin->plugin = py_plugin;
	plugin->pygobj = pygobject_get(py_plugin);
	subplugin_set_info(plugin, py_plugin);

	g_signal_connect(plugin->pygobj, "keybinding-added",
		G_CALLBACK(on_subplugin_keybinding_added), plugin);
	g_signal_connect(plugin->pygobj, "keybinding-removed",
		G_CALLBACK(on_subplugin_keybinding_removed), plugin);

	plugin->kg_name = name_to_identifier(plugin->sub->info->name);
	plugin->group = plugin_set_key_group(plugin->sub, plugin->kg_name,
						KEYBINDINGS_MAX, NULL);

	if (plugin->group)
	{
		plugin->group->name = (const gchar *) plugin->kg_name;
		plugin->group->label = (const gchar *) plugin->name;
		plugin->group->plugin = TRUE;
	}

	return plugin;
}

void subplugin_free(SubPlugin *plugin)
{
	g_free(plugin->name);
	g_free(plugin->description);
	g_free(plugin->version);
	g_free(plugin->author);

	g_free(plugin->sub->info);
	g_free(plugin->sub->priv);
	g_free(plugin->sub);

	g_free(plugin->kg_name);
	g_slist_free(plugin->kb_list);

	g_free(plugin);
}


void subplugin_keybindings_set_item(SubPlugin *plugin, gsize key_id,
	GeanyKeyCallback cb, guint key, GdkModifierType mod, const gchar *label,
	GtkWidget *menu_item)
{
	GeanyKeyBinding *kb;
	gchar *keyfile_name;

	g_return_if_fail(plugin != NULL);
	g_return_if_fail(label != NULL);
	g_return_if_fail(key_id < KEYBINDINGS_MAX);

	keyfile_name = name_to_identifier(label);
	kb = keybindings_set_item(plugin->group, key_id, cb, key, mod,
			keyfile_name, label, menu_item);
	plugin->kb_list = g_slist_append(plugin->kb_list, kb);
	g_free(keyfile_name);
}

























