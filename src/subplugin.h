#ifndef SUBPLUGIN_H__
#define SUBPLUGIN_H__


typedef struct _SubPlugin SubPlugin;


SubPlugin *subplugin_new(const PluginInfo *info);
void subplugin_free(SubPlugin *plugin);
void subplugin_keybindings_set_item(SubPlugin *plugin, gsize key_id,
	GeanyKeyCallback cb, guint key, GdkModifierType mod, const gchar *label,
	GtkWidget *menu_item);


#endif /* SUBPLUGIN_H__ */
