/*
 * plugin.c
 *
 * Copyright 2011 Matthew Brush <mbrush@codebrainz.ca>
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

#include "geanypy.h"

G_MODULE_EXPORT GeanyPlugin		*geany_plugin;
G_MODULE_EXPORT GeanyData		*geany_data;
G_MODULE_EXPORT GeanyFunctions	*geany_functions;


G_MODULE_EXPORT PLUGIN_VERSION_CHECK(211)

G_MODULE_EXPORT PLUGIN_SET_INFO(
	_("GeanyPy"),
	_("Python plugins support"),
	"1.0",
	"Matthew Brush <mbrush@codebrainz.ca>")


static GtkWidget *loader_item = NULL;
static PyObject *manager = NULL;
static gchar *plugin_dir = NULL;
static SignalManager *signal_manager = NULL;


/* Forward declarations to prevent compiler warnings. */
PyMODINIT_FUNC initapp(void);
PyMODINIT_FUNC initdialogs(void);
PyMODINIT_FUNC initdocument(void);
PyMODINIT_FUNC initeditor(void);
PyMODINIT_FUNC initencoding(void);
PyMODINIT_FUNC initfiletypes(void);
PyMODINIT_FUNC inithighlighting(void);
PyMODINIT_FUNC initmain(void);
PyMODINIT_FUNC initmsgwin(void);
PyMODINIT_FUNC initnavqueue(void);
PyMODINIT_FUNC initprefs(void);
PyMODINIT_FUNC initproject(void);
PyMODINIT_FUNC initscintilla(void);
PyMODINIT_FUNC initsearch(void);
PyMODINIT_FUNC inittemplates(void);
PyMODINIT_FUNC initui_utils(void);


static void
GeanyPy_start_interpreter(void)
{
    gchar *init_code;

#if 0
    /* This prevents a crash in the dynload thingy */
	if (dlopen(GEANYPY_PYTHON_LIBRARY, RTLD_LAZY | RTLD_GLOBAL) == NULL)
    {
        g_warning(_("Unable to pre-load Python library."));
        return;
    }
#endif
#if 1
	{
		GModule *mod = g_module_open(GEANYPY_PYTHON_LIBRARY, G_MODULE_BIND_LAZY);
		if (!mod) {
			g_warning(_("Unable to pre-load Python library."));
			return;
		}
		g_module_close(mod);
	}
#endif

    Py_Initialize();

    /* Import the C modules */
    initapp();
    initdialogs();
    initdocument();
    initeditor();
    initencoding();
    initfiletypes();
    inithighlighting();
    initmain();
    initmsgwin();
    initnavqueue();
    initprefs();
    initproject();
    initscintilla();
    initsearch();
    inittemplates();
    initui_utils();

    /* Adjust Python path to find wrapper package (geany) */
    init_code = g_strdup_printf(
        "import os, sys\n"
        "path = '%s'.replace('~', os.path.expanduser('~'))\n"
        "sys.path.append(path)\n"
        "import geany",
        GEANYPY_PYTHON_DIR);
    PyRun_SimpleString(init_code);
    g_free(init_code);

}

static void
GeanyPy_stop_interpreter(void)
{
    if (Py_IsInitialized())
        Py_Finalize();
}


static void
GeanyPy_init_manager(const gchar *dir)
{
    PyObject *module, *man, *args;

    g_return_if_fail(dir != NULL);

    module = PyImport_ImportModule("geany.manager");
    if (module == NULL)
    {
        g_warning(_("Failed to import manager module"));
        return;
    }

    man = PyObject_GetAttrString(module, "PluginManager");
    Py_DECREF(module);

    if (man == NULL)
    {
        g_warning(_("Failed to retrieve PluginManager from manager module"));
        return;
    }

    args = Py_BuildValue("([s, s])", GEANYPY_PLUGIN_DIR, dir);
    manager = PyObject_CallObject(man, args);
    if (PyErr_Occurred())
		PyErr_Print();
    Py_DECREF(man);
    Py_DECREF(args);

    if (manager == NULL)
    {
        g_warning(_("Unable to instantiate new PluginManager"));
        return;
    }
}


static void
GeanyPy_show_manager(void)
{
    PyObject *show_method;

    g_return_if_fail(manager != NULL);

    show_method = PyObject_GetAttrString(manager, "show_all");
    if (show_method == NULL)
    {
        g_warning(_("Unable to get show_all() method on plugin manager"));
        return;
    }
    PyObject_CallObject(show_method, NULL);
    Py_DECREF(show_method);
}


static void
on_python_plugin_loader_activate(GtkMenuItem *item, gpointer user_data)
{
    GeanyPy_show_manager();
}


G_MODULE_EXPORT void
plugin_init(GeanyData *data)
{
    GeanyPy_start_interpreter();
    signal_manager = signal_manager_new(geany_plugin);

#ifndef GEANYPY_WINDOWS
	/* On *nix, use user's configuration directory. */
    plugin_dir = g_build_filename(geany->app->configdir,
		"plugins", "geanypy", "plugins", NULL);
#else
	/* On Windows, use Geany prefix's plugin directory, ie.
	 *   `C:\Program Files\Geany\lib` */
	plugin_dir = g_strdup(GEANYPY_PLUGIN_DIR);
#endif

    if (!g_file_test(plugin_dir, G_FILE_TEST_IS_DIR))
    {
        if (g_mkdir_with_parents(plugin_dir, 0755) == -1)
        {
            g_warning(_("Unable to create Python plugins directory: %s: %s"),
                plugin_dir,
                strerror(errno));
            g_free(plugin_dir);
            plugin_dir = NULL;
        }
    }

    if (plugin_dir != NULL)
        GeanyPy_init_manager(plugin_dir);

    loader_item = gtk_menu_item_new_with_label(_("Python Plugin Manager"));
	gtk_widget_set_sensitive(loader_item, plugin_dir != NULL);
	gtk_menu_append(GTK_MENU(geany->main_widgets->tools_menu), loader_item);
	gtk_widget_show(loader_item);
	g_signal_connect(loader_item, "activate",
		G_CALLBACK(on_python_plugin_loader_activate), NULL);
}


G_MODULE_EXPORT void plugin_cleanup(void)
{
    signal_manager_free(signal_manager);
    Py_XDECREF(manager);
	GeanyPy_stop_interpreter();
    gtk_widget_destroy(loader_item);
    g_free(plugin_dir);
}
