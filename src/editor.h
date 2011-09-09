#ifndef GEANYPY_EDITOR_H__
#define GEANYPY_EDITOR_H__

typedef struct
{
	PyObject_HEAD
	GeanyEditor *editor;
} Editor;

Editor *Editor_create_new_from_geany_editor(GeanyEditor *editor);

#endif /* GEANYPY_EDITOR_H__ */
