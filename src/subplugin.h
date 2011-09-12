#ifndef SUBPLUGIN_H__
#define SUBPLUGIN_H__


typedef struct _SubPlugin SubPlugin;


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

	PyObject *plugin;
	GObject *pygobj;
};


SubPlugin *subplugin_new(PyObject *py_plugin);
void subplugin_free(SubPlugin *plugin);
void subplugin_keybindings_set_item(SubPlugin *plugin, gsize key_id,
	GeanyKeyCallback cb, guint key, GdkModifierType mod, const gchar *label,
	GtkWidget *menu_item);


#endif /* SUBPLUGIN_H__ */
