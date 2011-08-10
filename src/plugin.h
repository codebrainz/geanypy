/*
 * plugin.h
 *
 * Copyright 2011 Matthew Brush <mbrush@codebrainz.ca>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301  USA.
 *
 */

#ifndef PLUGIN_H
#define PLUGIN_H
#ifdef __cplusplus
extern "C" {
#endif


#ifndef PyMODINIT_FUNC
#define PyMODINIT_FUNC void
#endif


/* Set by Geany when plugin is loaded */
extern GeanyPlugin		*geany_plugin;
extern GeanyData		*geany_data;
extern GeanyFunctions	*geany_functions;


#include "app.h"
#include "project.h"





typedef struct
{
	PyObject_HEAD
	GeanyEditorPrefs *editor_prefs;
} EditorPrefs;




typedef struct
{
	PyObject_HEAD
	GeanyMainWidgets *main_widgets;
} MainWidgets;


typedef struct
{
	PyObject_HEAD
	GeanyFilePrefs *file_prefs;
} FilePrefs;



typedef struct
{
    PyObject_HEAD
    SCNotification *notif;
} ScintillaNotification;


typedef struct
{
	PyObject_HEAD
	const GeanyLexerStyle *lexer_style;
} LexerStyle;

PyMODINIT_FUNC initeditor(void);
PyMODINIT_FUNC initeditorprefs(void);
PyMODINIT_FUNC initencodings(void);
PyMODINIT_FUNC initfileprefs(void);
PyMODINIT_FUNC initfiletype(void);
PyMODINIT_FUNC inithighlighting(void);
PyMODINIT_FUNC initindentprefs(void);
PyMODINIT_FUNC initmain(void);
PyMODINIT_FUNC initmainwidgets(void);
PyMODINIT_FUNC initmsgwindow(void);


Document *Document_create_new_from_geany_document(GeanyDocument *doc);
Editor *Editor_create_new_from_geany_editor(GeanyEditor *editor);
Filetype *Filetype_create_new_from_geany_filetype(GeanyFiletype *ft);
IndentPrefs *IndentPrefs_create_new_from_geany_indent_prefs(GeanyIndentPrefs *indent_prefs);


PyMODINIT_FUNC init_geany_scintilla(void);
Scintilla *Scintilla_create_new_from_scintilla(ScintillaObject *sci);

PyMODINIT_FUNC init_geany_scintilla_notification(void);
ScintillaNotification *ScintillaNotification_create_new_from_scintilla_notification(SCNotification *notif);





PyMODINIT_FUNC init_geany_navqueue(void);


#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* PLUGIN_H */
