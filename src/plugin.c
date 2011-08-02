#include <Python.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <dlfcn.h>
#include <glib.h>
#include <gtk/gtk.h>
#include <pygobject.h>
#include <pygtk/pygtk.h>
#include <geanyplugin.h>

#include "plugin-config.h"
#include "plugin.h"


GeanyPlugin		*geany_plugin;
GeanyData		*geany_data;
GeanyFunctions	*geany_functions;


PLUGIN_VERSION_CHECK(211)

PLUGIN_SET_INFO(_("GeanyPy"),
				_("Python plugins support"),
				"1.0",
				"Matthew Brush <mbrush@codebrainz.ca>");


static void
GeanyPy_start_interpreter(void)
{
    gchar *init_code;

    /* This prevents a crash in the dynload thingy */
	if (dlopen(GEANYPY_PYTHON_LIBRARY, RTLD_LAZY | RTLD_GLOBAL) == NULL)
    {
        g_warning("Unable to pre-load Python library.");
        return;
    }

    Py_Initialize();

    /* Import the C modules */
    init_geany_dialogs();
    init_geany_filetype();
    init_geany_document();
    init_geany_indent_prefs();
    init_geany_editor_prefs();
    init_geany_editor();
    init_geany_project();
    init_geany_app();
    init_geany_file_prefs();
    init_geany_main_widgets();
    init_geany_scintilla();

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
        g_warning("Failed to import console module");
        return;
    }

    console = PyObject_GetAttrString(module, "Console");
    Py_DECREF(module);

    if (console == NULL)
    {
        g_warning("Failed to retrieve Console from console module");
        return;
    }

    args = Py_BuildValue("()");
    kwargs = Py_BuildValue("{s:s, s:s}",
                "banner", "Geany Python Console",
                "start_script", "import geany\n");

    console_inst = PyObject_Call(console, args, kwargs);
    Py_DECREF(console);
    Py_DECREF(args);
    Py_DECREF(kwargs);

    if (console_inst == NULL)
    {
        g_warning("Unable to instantiate new Console");
        return;
    }

    console_gobject = (PyGObject *) console_inst;
    console_widget = GTK_WIDGET(console_gobject->obj);
    Py_DECREF(console_inst);

    if (console_widget == NULL)
    {
        g_warning("Failed to get GtkWidget for Console");
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
on_geany_startup_complete(GObject unused, gpointer user_data)
{
    GeanyPy_install_console();
}


void plugin_init(GeanyData *data)
{
	GeanyPy_start_interpreter();

    plugin_signal_connect(geany_plugin, NULL, "geany-startup-complete",
        TRUE, G_CALLBACK(on_geany_startup_complete), NULL);
}


void plugin_cleanup(void)
{
	GeanyPy_stop_interpreter();
}
