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

GeanyPlugin		*geany_plugin;
GeanyData		*geany_data;
GeanyFunctions	*geany_functions;


PLUGIN_VERSION_CHECK(211)

PLUGIN_SET_INFO(_("GeanyPy"),
				_("Python plugins support"),
				"1.0",
				"Matthew Brush <mbrush@codebrainz.ca>")


static GtkWidget *loader_item = NULL;
static PyObject *manager = NULL;
static gchar *plugin_dir = NULL;
static SignalManager *signal_manager = NULL;


static void
GeanyPy_start_interpreter(void)
{
    gchar *init_code;

    /* This prevents a crash in the dynload thingy */
	if (dlopen(GEANYPY_PYTHON_LIBRARY, RTLD_LAZY | RTLD_GLOBAL) == NULL)
    {
        g_warning(_("Unable to pre-load Python library."));
        return;
    }

    Py_Initialize();

    /* Import the C modules */
    init_geany_main();
    init_geany_main_widgets();
    init_geany_msgwin();
    init_geany_navqueue();
    init_geany_scintilla();
    init_geany_scintilla_notification();

    initapp();
    initdialogs();
    initdocument();
    initeditor();
    initencoding();
    initfiletypes();
    inithighlighting();
    //initindentprefs();
    initprefs();
    initproject();

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
GeanyPy_install_console(void)
{
    PyObject *module, *console, *console_inst, *args, *kwargs;
    PyGObject *console_gobject;
    GtkWidget *console_widget, *scroll;
    PangoFontDescription *pfd;

    module = PyImport_ImportModule("geany.console");
    if (module == NULL)
    {
        g_warning(_("Failed to import console module"));
        return;
    }

    console = PyObject_GetAttrString(module, "Console");
    Py_DECREF(module);

    if (console == NULL)
    {
        g_warning(_("Failed to retrieve Console from console module"));
        return;
    }

    args = Py_BuildValue("()");
    kwargs = Py_BuildValue("{s:s, s:s}",
                "banner", _("Geany Python Console"),
                "start_script", "import geany\n");

    console_inst = PyObject_Call(console, args, kwargs);
    Py_DECREF(console);
    Py_DECREF(args);
    Py_DECREF(kwargs);

    if (console_inst == NULL)
    {
        g_warning(_("Unable to instantiate new Console"));
        return;
    }

    console_gobject = (PyGObject *) console_inst;
    console_widget = GTK_WIDGET(console_gobject->obj);
    Py_DECREF(console_inst);

    if (console_widget == NULL)
    {
        g_warning(_("Failed to get GtkWidget for Console"));
        return;
    }

    pfd = pango_font_description_from_string("Monospace 9");
    gtk_widget_modify_font(console_widget, pfd);
    pango_font_description_free(pfd);

    scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll),
        GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(scroll), console_widget);

    gtk_notebook_append_page(
        GTK_NOTEBOOK(geany->main_widgets->message_window_notebook),
        scroll,
        gtk_label_new("Python"));

    gtk_widget_show_all(scroll);
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
    if (PyErr_Occurred)
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


void plugin_init(GeanyData *data)
{
    GeanyPy_start_interpreter();
    signal_manager = signal_manager_new(geany_plugin);
    GeanyPy_install_console();

    plugin_dir = g_build_filename(geany->app->configdir,
                    "plugins", "geanypy", "plugins", NULL);

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


void plugin_cleanup(void)
{
    signal_manager_free(signal_manager);
    Py_XDECREF(manager);
	GeanyPy_stop_interpreter();
    gtk_widget_destroy(loader_item);
    g_free(plugin_dir);
}
