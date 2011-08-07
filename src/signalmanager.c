#include <Python.h>
#include <gtk/gtk.h>
#include <pygobject.h>
#include <pygtk/pygtk.h>
#include <geanyplugin.h>
#include <Scintilla.h>
#include <ScintillaWidget.h>
#include "plugin.h"
#include "signalmanager.h"

extern GeanyPlugin      *geany_plugin;
extern GeanyData        *geany_data;
extern GeanyFunctions   *geany_functions;


static void signal_manager_connect_signals(SignalManager *man);

static void on_build_start(GObject *geany_object, SignalManager *man);
static void on_document_activate(GObject *geany_object, GeanyDocument *doc, SignalManager *man);
static void on_document_before_save(GObject *geany_object, GeanyDocument *doc, SignalManager *man);
static void on_document_close(GObject *geany_object, GeanyDocument *doc, SignalManager *man);
static void on_document_filetype_set(GObject *geany_object, GeanyDocument *doc, GeanyFiletype *filetype_old, SignalManager *man);
static void on_document_new(GObject *geany_object, GeanyDocument *doc, SignalManager *man);
static void on_document_open(GObject *geany_object, GeanyDocument *doc, SignalManager *man);
static void on_document_reload(GObject *geany_object, GeanyDocument *doc, SignalManager *man);
static void on_document_save(GObject *geany_object, GeanyDocument *doc, SignalManager *man);
static gboolean on_editor_notify(GObject *geany_object, GeanyEditor *editor, SCNotification *nt, SignalManager *man);
static void on_geany_startup_complete(GObject *geany_object, SignalManager *man);
static void on_project_close(GObject *geany_object, SignalManager *man);
static void on_project_dialog_confirmed(GObject *geany_object, GtkWidget *notebook, SignalManager *man);
static void on_project_dialog_create(GObject *geany_object, GtkWidget *notebook, SignalManager *man);
static void on_project_open(GObject *geany_object, GKeyFile *config, SignalManager *man);
static void on_project_save(GObject *geany_object, GKeyFile *config, SignalManager *man);
static void on_update_editor_menu(GObject *geany_object, const gchar *word, gint pos, GeanyDocument *doc, SignalManager *man);


SignalManager *signal_manager_new(GeanyPlugin *geany_plugin)
{
    SignalManager *man;
    PyObject *module;

    man = g_new0(SignalManager, 1);
    man->geany_plugin = geany_plugin;

    module = PyImport_ImportModule("geany");
    if (!module)
    {
        if (PyErr_Occurred())
            PyErr_Print();
        g_warning("Unable to import 'geany' module");
        return NULL;
    }

    man->py_manager = PyObject_GetAttrString(module, "signal_manager");
    Py_DECREF(module);
    if (!man->py_manager)
    {
        if (PyErr_Occurred())
            PyErr_Print();
        g_warning("Unable to get 'signal_manager' from 'geany' module.");
        return NULL;
    }

    man->py_emit_func = PyObject_GetAttrString(man->py_manager, "emit_signal");
    if (!man->py_emit_func)
    {
        if (PyErr_Occurred())
            PyErr_Print();
        g_warning("Unable to get 'emit_signal' function from 'signal_manager' instance.");
        return NULL;
    }

    signal_manager_connect_signals(man);

    return man;
}


void signal_manager_free(SignalManager *man)
{
    Py_XDECREF(man->py_manager);
    Py_XDECREF(man->py_emit_func);
    g_free(man);
}


static void signal_manager_connect_signals(SignalManager *man)
{
    plugin_signal_connect(geany_plugin, NULL, "build-start", TRUE, G_CALLBACK(on_build_start), man);
    plugin_signal_connect(geany_plugin, NULL, "document-activate", TRUE, G_CALLBACK(on_document_activate), man);
    plugin_signal_connect(geany_plugin, NULL, "document-before-save", TRUE, G_CALLBACK(on_document_before_save), man);
    plugin_signal_connect(geany_plugin, NULL, "document-close", TRUE, G_CALLBACK(on_document_close), man);
    plugin_signal_connect(geany_plugin, NULL, "document-filetype-set", TRUE, G_CALLBACK(on_document_filetype_set), man);
    plugin_signal_connect(geany_plugin, NULL, "document-new", TRUE, G_CALLBACK(on_document_new), man);
    plugin_signal_connect(geany_plugin, NULL, "document-open", TRUE, G_CALLBACK(on_document_open), man);
    plugin_signal_connect(geany_plugin, NULL, "document-reload", TRUE, G_CALLBACK(on_document_reload), man);
    plugin_signal_connect(geany_plugin, NULL, "document-save", TRUE, G_CALLBACK(on_document_save), man);
    plugin_signal_connect(geany_plugin, NULL, "editor-notify", TRUE, G_CALLBACK(on_editor_notify), man);
    plugin_signal_connect(geany_plugin, NULL, "genay-startup-complete", TRUE, G_CALLBACK(on_geany_startup_complete), man);
    plugin_signal_connect(geany_plugin, NULL, "project-close", TRUE, G_CALLBACK(on_project_close), man);
    plugin_signal_connect(geany_plugin, NULL, "project-dialog-confirmed", TRUE, G_CALLBACK(on_project_dialog_confirmed), man);
    plugin_signal_connect(geany_plugin, NULL, "project-dialog-create", TRUE, G_CALLBACK(on_project_dialog_create), man);
    plugin_signal_connect(geany_plugin, NULL, "project-open", TRUE, G_CALLBACK(on_project_open), man);
    plugin_signal_connect(geany_plugin, NULL, "project-save", TRUE, G_CALLBACK(on_project_save), man);
    plugin_signal_connect(geany_plugin, NULL, "update-editor-menu", TRUE, G_CALLBACK(on_update_editor_menu), man);
    return;
}

static void on_build_start(GObject *geany_object, SignalManager *man)
{
    PyObject *args;
    args = Py_BuildValue("(s)", "build-start");
    PyObject_CallObject(man->py_emit_func, args);
}


static void on_document_event(GObject *geany_object, GeanyDocument *doc, SignalManager *man, const gchar *signal_name)
{
    PyObject *args, *py_doc;
    py_doc = (PyObject *) Document_create_new_from_geany_document(doc);
    args = Py_BuildValue("(sO)", signal_name, py_doc);
    PyObject_CallObject(man->py_emit_func, args);
}


static void on_document_activate(GObject *geany_object, GeanyDocument *doc, SignalManager *man)
{
    on_document_event(geany_object, doc, man, "document-activate");
}


static void on_document_before_save(GObject *geany_object, GeanyDocument *doc, SignalManager *man)
{
    on_document_event(geany_object, doc, man, "document-before-save");
}


static void on_document_close(GObject *geany_object, GeanyDocument *doc, SignalManager *man)
{
    on_document_event(geany_object, doc, man, "document-close");
}


static void on_document_filetype_set(GObject *geany_object, GeanyDocument *doc, GeanyFiletype *filetype_old, SignalManager *man)
{
    PyObject *args, *py_doc, *py_ft;
    py_doc = (PyObject *) Document_create_new_from_geany_document(doc);
    py_ft = (PyObject *) Filetype_create_new_from_geany_filetype(filetype_old);
    args = Py_BuildValue("(sOO)", "document-filetype-set",py_doc, py_ft);
    PyObject_CallObject(man->py_emit_func, args);
}


static void on_document_new(GObject *geany_object, GeanyDocument *doc, SignalManager *man)
{
    on_document_event(geany_object, doc, man, "document-new");
}


static void on_document_open(GObject *geany_object, GeanyDocument *doc, SignalManager *man)
{
    on_document_event(geany_object, doc, man, "document-open");
}


static void on_document_reload(GObject *geany_object, GeanyDocument *doc, SignalManager *man)
{
    on_document_event(geany_object, doc, man, "document-reload");
}


static void on_document_save(GObject *geany_object, GeanyDocument *doc, SignalManager *man)
{
    on_document_event(geany_object, doc, man, "document-save");
}


static gboolean on_editor_notify(GObject *geany_object, GeanyEditor *editor, SCNotification *nt, SignalManager *man)
{
    /* TODO */
	return FALSE;
}


static void on_geany_startup_complete(GObject *geany_object, SignalManager *man)
{
    PyObject *args;
    args = Py_BuildValue("(s)", "geany-startup-complete");
    PyObject_CallObject(man->py_emit_func, args);
}


static void on_project_close(GObject *geany_object, SignalManager *man)
{
    PyObject *args;
    args = Py_BuildValue("(s)", "project-close");
    PyObject_CallObject(man->py_emit_func, args);
}


static void on_project_dialog_confirmed(GObject *geany_object, GtkWidget *notebook, SignalManager *man)
{
    PyObject *gob, *args;
    gob = (PyObject *) pygobject_new(G_OBJECT(notebook));
    args = Py_BuildValue("(sO)", "project-dialog-confirmed", gob);
    PyObject_CallObject(man->py_emit_func, args);
}


static void on_project_dialog_create(GObject *geany_object, GtkWidget *notebook, SignalManager *man)
{
    PyObject *gob, *args;
    gob = (PyObject *) pygobject_new(G_OBJECT(notebook));
    args = Py_BuildValue("(sO)", "project-dialog-confirmed", gob);
    PyObject_CallObject(man->py_emit_func, args);
}


static void on_project_open(GObject *geany_object, GKeyFile *config, SignalManager *man)
{
    g_debug("'on_project_open' signal is not implemented in GeanyPy");
    return;
    //PyObject *gob, *args;
    //gob = (PyObject *) pygobject_new(G_OBJECT(config)); /* Is GKeyFile a GObject? */
    //args = Py_BuildValue("(sO)", "project-open", gob);
    //PyObject_CallObject(man->py_emit_func, args);
}


static void on_project_save(GObject *geany_object, GKeyFile *config, SignalManager *man)
{
    g_debug("'on_project_save' signal is not implmented in GeanyPy");
    return;
    //PyObject *gob, *args;
    //gob = (PyObject *) pygobject_new(G_OBJECT(config)); /* Is GKeyFile a GObject? */
    //args = Py_BuildValue("(sO)", "project-open", gob);
    //PyObject_CallObject(man->py_emit_func, args);
}


static void on_update_editor_menu(GObject *geany_object, const gchar *word, gint pos, GeanyDocument *doc, SignalManager *man)
{
    PyObject *args, *py_doc;
    py_doc = (PyObject *) Document_create_new_from_geany_document(doc);
    args = Py_BuildValue("(ssiO)", "update-editor-menu", word, pos, py_doc);
    PyObject_CallObject(man->py_emit_func, args);
}


