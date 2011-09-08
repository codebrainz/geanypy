#include "geanypy.h"

static PyObject *
Encodings_convert_to_utf8(PyObject *module, PyObject *args)
{
    gchar *buffer = NULL, *used_encoding = NULL, *new_buffer = NULL;
    gssize size = -1; /* bug alert: this is gsize in Geany for some reason */
    PyObject *result;

    if (PyArg_ParseTuple(args, "s|l", &buffer, &size))
    {
        new_buffer = encodings_convert_to_utf8(buffer, size, &used_encoding);
        if (new_buffer != NULL)
        {
            result = Py_BuildValue("ss", new_buffer, used_encoding);
            g_free(new_buffer);
            g_free(used_encoding);
            return result;
        }
    }

    Py_RETURN_NONE;
}


static PyObject *
Encodings_convert_to_utf8_from_charset(PyObject *module, PyObject *args)
{
    gchar *buffer = NULL, *charset = NULL, *new_buffer = NULL;
    gssize size = -1;
    gint fast = 0;
    PyObject *result;

    if (PyArg_ParseTuple(args, "ss|li", &buffer, &charset, &size, &fast))
    {
        new_buffer = encodings_convert_to_utf8_from_charset(buffer, size, charset, fast);
        if (new_buffer != NULL)
        {
            result = Py_BuildValue("s", new_buffer);
            g_free(new_buffer);
            return result;
        }
    }

    Py_RETURN_NONE;
}


static PyObject *
Encodings_get_charset_from_index(PyObject *module, PyObject *args)
{
    gint idx = 0;
    const gchar *charset = NULL;

    if (PyArg_ParseTuple(args, "i", &idx))
    {
        charset = encodings_get_charset_from_index(idx);
        if (charset != NULL)
            return Py_BuildValue("s", charset);
    }

    Py_RETURN_NONE;
}


static
PyMethodDef EncodingsModule_methods[] = {
    { "convert_to_utf8", (PyCFunction)Encodings_convert_to_utf8, METH_VARARGS },
    { "convert_to_utf8_from_charset", (PyCFunction)Encodings_convert_to_utf8_from_charset, METH_VARARGS },
    { "get_charset_from_index", (PyCFunction)Encodings_get_charset_from_index, METH_VARARGS },
    { NULL }
};


PyMODINIT_FUNC
init_geany_encodings(void)
{
    PyObject *m;

    m = Py_InitModule("_geany_encodings", EncodingsModule_methods);
}
