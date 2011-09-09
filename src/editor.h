#ifndef GEANYPY_EDITOR_H__
#define GEANYPY_EDITOR_H__
#ifdef __cplusplus
extern "C" {
#endif


typedef struct
{
	PyObject_HEAD
	GeanyEditor *editor;
} Editor;


PyMODINIT_FUNC init_geany_editor(void);
Editor *Editor_create_new_from_geany_editor(GeanyEditor *editor);


#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* GEANYPY_EDITOR_H__ */
