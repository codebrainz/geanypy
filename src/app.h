#ifndef GEANYPY_APP_H__
#define GEANYPY_APP_H__
#ifdef __cplusplus
extern "C" {
#endif


PyTypeObject AppType;

typedef struct
{
	PyObject_HEAD
	GeanyApp *app;
} App;

PyMODINIT_FUNC initapp(void);


#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* GEANYPY_APP_H__ */
