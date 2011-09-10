#ifndef GEANYPY_INDENTPREFS_H__
#define GEANYPY_INDENTPREFS_H__

typedef struct
{
	PyObject_HEAD
	GeanyIndentPrefs *indent_prefs;
} IndentPrefs;

IndentPrefs *IndentPrefs_create_new_from_geany_indent_prefs(GeanyIndentPrefs *indent_prefs);

#endif /* GEANYPY_INDENTPREFS_H__ */
