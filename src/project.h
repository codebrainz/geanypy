#ifndef GEANYPY_PROJECT_H__
#define GEANYPY_PROJECT_H__
#ifdef __cplusplus
extern "C" {
#endif

PyTypeObject ProjectType;

typedef struct
{
	PyObject_HEAD
	GeanyProject *project;
} Project;

PyMODINIT_FUNC initproject(void);


#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* GEANYPY_PROJECT_H__ */
