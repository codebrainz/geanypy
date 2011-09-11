#include "geanypy.h"

static PyTypeObject *PyGObject_Type = NULL;

static PyMethodDef UiUtilsModule_methods[] = { { NULL } };


PyMODINIT_FUNC initui_utils(void)
{
	PyObject *m;

	init_pygobject();
	init_pygtk();
	m = PyImport_ImportModule("gobject");

	if (m)
	{
		PyGObject_Type = (PyTypeObject *) PyObject_GetAttrString(m, "GObject");
		Py_XDECREF(m);
	}

	InterfacePrefsType.tp_new = PyType_GenericNew;
	if (PyType_Ready(&InterfacePrefsType) < 0)
		return;

	MainWidgetsType.tp_new = PyType_GenericNew;
	if (PyType_Ready(&MainWidgetsType) < 0)
		return;

	m = Py_InitModule3("ui_utils", UiUtilsModule_methods,
			"User interface information and utilities.");

	Py_INCREF(&InterfacePrefsType);
	PyModule_AddObject(m, "InterfacePrefs", (PyObject *) &InterfacePrefsType);

	Py_INCREF(&MainWidgetsType);
	PyModule_AddObject(m, "MainWidgets", (PyObject *) &MainWidgetsType);
}
