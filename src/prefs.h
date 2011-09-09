#ifndef GEANYPY_PREFS_H__
#define GEANYPY_PREFS_H__
#ifdef __cplusplus
extern "C" {
#endif


typedef struct
{
	PyObject_HEAD
	GeanyPrefs *prefs;
} Prefs;


PyMODINIT_FUNC initprefs(void);


#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* GEANYPY_PREFS_H__ */
