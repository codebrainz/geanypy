#ifndef MODULES_COMMON_H
#define MODULES_COMMON_H
#ifdef __cplusplus
extern "C" {
#endif


#include <string.h>


#define _StructPointer_ToPyObject(ptr) ((PyObject *) PyLong_FromVoidPtr((void *)(ptr)))
//#define _StructPointer_FromPyObject(p,tp) ((tp *) PyLong_AsVoidPtr(p))


/* The GeanyPy plugin will define this for us. */
extern GeanyPlugin		*geany_plugin;
extern GeanyData		*geany_data;
extern GeanyFunctions	*geany_functions;


/*
 * If `obj` is a PyLong or one of its subtypes, just return it's value as a
 * void pointer, otherwise, see if it has a `_get_pointer` method, in which
 * case call that to get the PyLong and the result as a void pointer.  If any
 * error occurs, returns NULL.
 *
 * Note: Use the `GET_POINTER` macro below to have the resulting void pointer
 * casted to a pointer of the specified type.
 */
inline void *_PyObject_GetPointer(PyObject *obj)
{
    PyObject *ptr_func = NULL, *py_long = NULL;
    void *ptr = NULL;

    if (PyLong_Check(obj))
        return PyLong_AsVoidPtr(obj);
    else if (PyObject_HasAttrString(obj, "_get_pointer"))
    {
        ptr_func = PyObject_GetAttrString(obj, "_get_pointer");

        if (!ptr_func)
            return NULL;
        if (!PyCallable_Check(ptr_func))
        {
            Py_DECREF(ptr_func);
            return NULL;
        }

        py_long = PyObject_CallObject(ptr_func, NULL);
        Py_DECREF(ptr_func);

        if (!py_long || py_long == Py_None)
            return NULL;
        if (!PyLong_Check(py_long))
        {
            Py_DECREF(py_long);
            return NULL;
        }

        ptr = PyLong_AsVoidPtr(py_long);
        Py_DECREF(py_long);

        return ptr;
    }

    return NULL;
}
/*
 * Retrieve a pointer of type `type` from the PyObject `py_obj`.
 *
 * See: `_PyObject_GetPointer` for more info.
 */
#define GET_POINTER(py_obj, type) ((type *) _PyObject_GetPointer(py_obj))


/*
 * Calls the `_set_pointer` function on `inst` with a PyLong converted from
 * `ptr` as the first/only argument.  Returns True if the function called the
 * `set_pointer` function, otherwise False.
 */
inline PyObject *_PyObject_SetPointer(PyObject *inst, void *ptr)
{
    PyObject *py_long = NULL, *func = NULL;

    if (!(py_long = PyLong_FromVoidPtr(ptr)))
        goto bail_out;

    if (!PyObject_HasAttrString(inst, "_set_pointer"))
        goto bail_out;

    func = PyObject_GetAttrString(inst, "_set_pointer");

    if (!func)
        goto bail_out;
    else if (!PyCallable_Check(func))
        goto bail_out;

    PyObject_CallObject(func, py_long);

    Py_DECREF(py_long);
    Py_DECREF(func);
    Py_RETURN_TRUE;

bail_out:
    Py_XDECREF(py_long);
    Py_XDECREF(func);
    Py_RETURN_FALSE;
}
/*
 * Set the pointer `ptr` on the `py_obj`.
 *
 * See: `_PyObject_SetPointer` for more info.
 */
#define SET_POINTER(py_obj, ptr) (_PyObject_SetPointer((py_obj), (void *) (ptr)))


/*
 * Initializes a new PyObject whose package.module.class is specified in `fqn`.
 * The initialization function is called with `ptr` converted to a PyLong as
 * the first/only argument.  Returns the new PyObject on success or None in all
 * other cases.
 */
inline PyObject *_PyObject_NewFromVoidPtr(const char *fqn, void *ptr)
{
    char *mod_name = NULL, *cls_name = NULL;
    PyObject *mod = NULL, *cls = NULL, *py_long = NULL, *result = NULL;

    if (strlen(fqn) == 0 || fqn[0] == '\0')
        goto bail_out;

    if (!(py_long = PyLong_FromVoidPtr(ptr)))
        goto bail_out;

    if (!PyLong_Check(py_long))
        goto bail_out;

    mod_name = strdup(fqn);

    if ((cls_name = strrchr(mod_name, '.')) == NULL)
        goto bail_out;

    *cls_name = '\0';
    cls_name++;

    if (!(mod = PyImport_ImportModule(mod_name)))
        goto bail_out;

    if (!PyObject_HasAttrString(mod, cls_name))
        goto bail_out;

    cls = PyObject_GetAttrString(mod, cls_name);

    if (!cls)
        goto bail_out;

    if (!PyCallable_Check(cls))
        goto bail_out;

    if (!(result = PyObject_CallObject(cls, py_long)))
        goto bail_out;

    free(mod_name);
    Py_DECREF(py_long);
    Py_DECREF(mod);
    Py_DECREF(cls);

    return result;

bail_out:
    free(mod_name);
    Py_XDECREF(py_long);
    Py_XDECREF(mod);
    Py_XDECREF(cls);
    Py_RETURN_NONE;
}
/*
 * Initializes a new PyObject with `fqn` and using `ptr` to create the argument.
 *
 * See: _PyObject_NewFromVoidPtr for more info.
 */
#define NEW_FROM_POINTER(fqn, ptr) (_PyObject_NewFromVoidPtr((fqn), (void *) (ptr)))


#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* MODULES_COMMON_H */
