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


struct _SubPlugin
{
	GeanyPlugin *sub;

	gchar *name;
	gchar *description;
	gchar *version;
	gchar *author;

	GeanyKeyGroup *group;
	gchar *kg_name;
	gint n_items;
	GSList *kb_list;
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


SubPlugin *subplugin_new(const PluginInfo *info)
{
	SubPlugin *plugin;

	g_return_val_if_fail(geany_plugin != NULL, NULL);
	g_return_val_if_fail(info != NULL, NULL);
	g_return_val_if_fail(info->name != NULL, NULL);

	plugin = g_new0(SubPlugin, 1);
	plugin->n_items = 0;
	plugin->kb_list = NULL;

	plugin->sub = g_new0(GeanyPlugin, 1);
	plugin->sub->info = g_new0(PluginInfo, 1);
	plugin->sub->priv = g_new0(struct GeanyPluginPrivate, 1);

	plugin->name = strdup(info->name);
	plugin->sub->info->name = (const gchar *) plugin->name;

	if (info->description)
	{
		plugin->description = strdup(info->description);
		plugin->sub->info->description = (const gchar *) plugin->description;
	}
	if (info->version)
	{
		plugin->version = strdup(info->version);
		plugin->sub->info->version = (const gchar *) plugin->version;
	}
	if (info->author)
	{
		plugin->author = strdup(info->author);
		plugin->sub->info->author = (const gchar *) plugin->author;
	}

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

























