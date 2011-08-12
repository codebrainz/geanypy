#include <Python.h>
#include <structmember.h>
#include <pygobject.h>
#include <pygtk/pygtk.h>
#include <geanyplugin.h>

#ifndef GTK
#  define GTK
#endif
#include <Scintilla.h>
#include <ScintillaWidget.h>

#include "modules-common.h"


typedef struct
{
    PyObject_HEAD
    SCNotification *notif;
} Notification;


typedef struct
{
	PyObject_HEAD
	ScintillaObject *sci;
} Scintilla;


static void
Notification_dealloc(Notification *self)
{
	self->ob_type->tp_free((PyObject *) self);
}


static int
Notification_init(Notification *self)
{
    self->notif = NULL;
	return 0;
}


static PyObject *
Notification_get_header_hwnd_from(Notification *self)
{
    return PyLong_FromVoidPtr(self->notif->nmhdr.hwndFrom);
}


static PyObject *
Notification_get_header_id_from(Notification *self)
{
    return PyLong_FromLong(self->notif->nmhdr.idFrom);
}


static PyObject *
Notification_get_header_code(Notification *self)
{
    return Py_BuildValue("I", self->notif->nmhdr.code);
}


static PyObject *
Notification_get_position(Notification *self)
{
    return Py_BuildValue("i", self->notif->position);
}


static PyObject *
Notification_get_ch(Notification *self)
{
    return Py_BuildValue("c", self->notif->ch);
}

static PyObject *
Notification_get_modifiers(Notification *self)
{
    return Py_BuildValue("i", self->notif->modifiers);
}


static PyObject *
Notification_get_modification_type(Notification *self)
{
    return Py_BuildValue("i", self->notif->modificationType);
}


static PyObject *
Notification_get_text(Notification *self)
{
    return Py_BuildValue("s", self->notif->text);
}


static PyObject *
Notification_get_length(Notification *self)
{
    return Py_BuildValue("i", self->notif->length);
}


static PyObject *
Notification_get_lines_added(Notification *self)
{
    return Py_BuildValue("i", self->notif->linesAdded);
}


static PyObject *
Notification_get_message(Notification *self)
{
    return Py_BuildValue("i", self->notif->message);
}


static PyObject *
Notification_get_w_param(Notification *self)
{
    return Py_BuildValue("l", self->notif->wParam);
}


static PyObject *
Notification_get_l_param(Notification *self)
{
    return Py_BuildValue("l", self->notif->lParam);
}


static PyObject *
Notification_get_line(Notification *self)
{
    return Py_BuildValue("i", self->notif->line);
}


static PyObject *
Notification_get_fold_level_now(Notification *self)
{
    return Py_BuildValue("i", self->notif->foldLevelNow);
}



static PyObject *
Notification_get_fold_level_prev(Notification *self)
{
    return Py_BuildValue("i", self->notif->foldLevelPrev);
}


static PyObject *
Notification_get_margin(Notification *self)
{
    return Py_BuildValue("i", self->notif->margin);
}


static PyObject *
Notification_get_list_type(Notification *self)
{
    return Py_BuildValue("i", self->notif->listType);
}


static PyObject *
Notification_get_x(Notification *self)
{
    return Py_BuildValue("i", self->notif->x);
}


static PyObject *
Notification_get_y(Notification *self)
{
    return Py_BuildValue("i", self->notif->y);
}


static PyObject *
Notification_get_token(Notification *self)
{
    return Py_BuildValue("i", self->notif->token);
}


static PyObject *
Notification_get_annotation_lines_added(Notification *self)
{
    return Py_BuildValue("i", self->notif->annotationLinesAdded);
}


static PyObject *
Notification_get_updated(Notification *self)
{
    return Py_BuildValue("i", self->notif->updated);
}


static PyMethodDef Notification_methods[] = {
    { "get_header_hwnd_from", (PyCFunction) Notification_get_header_hwnd_from, METH_NOARGS },
    { "get_header_id_from", (PyCFunction) Notification_get_header_id_from, METH_NOARGS },
    { "get_header_code", (PyCFunction) Notification_get_header_code, METH_NOARGS },
    { "get_position", (PyCFunction) Notification_get_position, METH_NOARGS },
    { "get_ch", (PyCFunction) Notification_get_ch, METH_NOARGS },
    { "get_modifiers", (PyCFunction) Notification_get_modifiers, METH_NOARGS },
    { "get_modification_type", (PyCFunction) Notification_get_modification_type, METH_NOARGS },
    { "get_text", (PyCFunction) Notification_get_text, METH_NOARGS },
    { "get_length", (PyCFunction) Notification_get_length, METH_NOARGS },
    { "get_lines_added", (PyCFunction) Notification_get_lines_added, METH_NOARGS },
    { "get_message", (PyCFunction) Notification_get_message, METH_NOARGS },
    { "get_w_param", (PyCFunction) Notification_get_w_param, METH_NOARGS },
    { "get_l_param", (PyCFunction) Notification_get_l_param, METH_NOARGS },
    { "get_line", (PyCFunction) Notification_get_line, METH_NOARGS },
    { "get_fold_level_now", (PyCFunction) Notification_get_fold_level_now, METH_NOARGS },
    { "get_fold_level_prev", (PyCFunction) Notification_get_fold_level_prev, METH_NOARGS },
	{ "get_margin", (PyCFunction) Notification_get_margin, METH_NOARGS },
    { "get_list_type", (PyCFunction) Notification_get_list_type, METH_NOARGS },
    { "get_x", (PyCFunction) Notification_get_x, METH_NOARGS },
    { "get_y", (PyCFunction) Notification_get_y, METH_NOARGS },
    { "get_token", (PyCFunction) Notification_get_token, METH_NOARGS },
    { "get_annotation_lines_added", (PyCFunction) Notification_get_annotation_lines_added, METH_NOARGS },
    { "get_updated", (PyCFunction) Notification_get_updated, METH_NOARGS },
    { NULL }
};


static PyMemberDef Notification_members[] = {
	{ NULL }
};



static PyTypeObject NotificationType = {
	PyObject_HEAD_INIT(NULL)
    0,                          /*ob_size*/
    "geany.scintilla.Notification",  /*tp_name*/
    sizeof(Scintilla),          /*tp_basicsize*/
    0,                          /*tp_itemsize*/
    (destructor)Notification_dealloc, /*tp_dealloc*/
    0,                          /*tp_print*/
    0,                          /*tp_getattr*/
    0,                          /*tp_setattr*/
    0,                          /*tp_compare*/
    0,                          /*tp_repr*/
    0,                          /*tp_as_number*/
    0,                          /*tp_as_sequence*/
    0,                          /*tp_as_mapping*/
    0,                          /*tp_hash */
    0,                          /*tp_call*/
    0,                          /*tp_str*/
    0,                          /*tp_getattro*/
    0,                          /*tp_setattro*/
    0,                          /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    "Geany scintilla notification",          /* tp_doc */
    0,		                    /* tp_traverse */
    0,		               	    /* tp_clear */
    0,		                    /* tp_richcompare */
    0,		                    /* tp_weaklistoffset */
    0,		                    /* tp_iter */
    0,		                    /* tp_iternext */
    Notification_methods,          /* tp_methods */
    Notification_members,          /* tp_members */
    0,                          /* tp_getset */
    0,                          /* tp_base */
    0,                          /* tp_dict */
    0,                          /* tp_descr_get */
    0,                          /* tp_descr_set */
    0,                          /* tp_dictoffset */
    (initproc)Notification_init,   /* tp_init */
    0,                          /* tp_alloc */
    0,                          /* tp_new */

};


static void
Scintilla_dealloc(Scintilla *self)
{
	self->ob_type->tp_free((PyObject *) self);
}


static int
Scintilla_init(Scintilla *self, PyObject *ptr)
{
    if (ptr != NULL && ptr != Py_None)
        self->sci = GET_POINTER(ptr, ScintillaObject);

	return 0;
}


static PyObject *
Scintilla_get_pointer(Scintilla *self)
{
    if (self->sci == NULL)
        Py_RETURN_NONE;
    return _StructPointer_ToPyObject(self->sci);
}


static PyObject *
Scintilla_set_pointer(Scintilla *self, PyObject *ptr)
{
    if (ptr != NULL && ptr != Py_None)
        self->sci = GET_POINTER(ptr, ScintillaObject);
    Py_RETURN_NONE;
}


static PyObject *
Scintilla_get_widget(Scintilla *self)
{
    return (PyObject *) pygobject_new(G_OBJECT(self->sci));
}


static PyObject *
Scintilla_delete_marker_at_line(Scintilla *self, PyObject *args)
{
    gint line_num, marker;
    if (self->sci != NULL)
    {
        if (PyArg_ParseTuple(args, "ii", &line_num, &marker))
            sci_delete_marker_at_line(self->sci, line_num, marker);
    }
    Py_RETURN_NONE;
}


static PyObject *
Scintilla_end_undo_action(Scintilla *self, PyObject *args)
{
    if (self->sci != NULL)
        sci_end_undo_action(self->sci);
    Py_RETURN_NONE;
}


static PyObject *
Scintilla_ensure_line_is_visible(Scintilla *self, PyObject *args)
{
    if (self->sci != NULL)
        sci_end_undo_action(self->sci);
    Py_RETURN_NONE;
}


static PyObject *
Scintilla_find_matching_brace(Scintilla *self, PyObject *args)
{
    gint pos, match_pos;
    if (self->sci != NULL)
    {
        if (PyArg_ParseTuple(args, "i", &pos))
        {
            match_pos = sci_find_matching_brace(self->sci, pos);
            return Py_BuildValue("i", match_pos);
        }
    }
    Py_RETURN_NONE;
}


static PyObject *
Scintilla_find_text(Scintilla *self, PyObject *args)
{
    gint pos = -1, flags = 0;
    glong start_chr = 0, end_chr = 0;
    gchar *search_text;
    struct Sci_TextToFind ttf = { 0 };

    if (self->sci != NULL)
    {
        if (PyArg_ParseTuple(args, "s|ill", &search_text, &flags, &start_chr, &end_chr))
        {
            ttf.chrg.cpMin = start_chr;
            ttf.chrg.cpMax = end_chr;
            ttf.lpstrText = search_text;
            pos = sci_find_text(self->sci, flags, &ttf);
            if (pos > -1)
                return Py_BuildValue("ll", ttf.chrgText.cpMin, ttf.chrgText.cpMax);
        }
    }

    Py_RETURN_NONE;
}


static PyObject *
Scintilla_get_char_at(Scintilla *self, PyObject *args)
{
    gint pos;
    gchar chr;
    if (self->sci != NULL)
    {
        if (PyArg_ParseTuple(args, "i", &pos))
        {
            chr = sci_get_char_at(self->sci, pos);
            return PyString_FromFormat("%c", chr);
        }
    }
    Py_RETURN_NONE;
}


static PyObject *
Scintilla_get_col_from_position(Scintilla *self, PyObject *args)
{
    gint pos, col;
    if (self->sci != NULL)
    {
        if (PyArg_ParseTuple(args, "i", &pos))
        {
            col = sci_get_col_from_position(self->sci, pos);
            return Py_BuildValue("i", col);
        }
    }
    Py_RETURN_NONE;
}


static PyObject *
Scintilla_get_contents(Scintilla *self, PyObject *args)
{
    gint len = -1;
    gchar *text;
    PyObject *py_text;
    if (self->sci != NULL)
    {
        if (PyArg_ParseTuple(args, "|i", &len))
        {
            if (len == -1)
                len = sci_get_length(self->sci) + 1;
            text = sci_get_contents(self->sci, len);
            if (text == NULL)
                Py_RETURN_NONE;
            py_text = PyString_FromString(text);
            g_free(text);
            return py_text;
        }
    }
    Py_RETURN_NONE;
}


static PyObject *
Scintilla_get_contents_range(Scintilla *self, PyObject *args)
{
    gint start = -1, end = -1;
    gchar *text;
    PyObject *py_text;
    if (self->sci != NULL)
    {
        if (PyArg_ParseTuple(args, "|ii", &start, &end))
        {
            if (start == -1)
                start = 0;
            if (end == -1)
                end = sci_get_length(self->sci) + 1;
            text = sci_get_contents_range(self->sci, start, end);
            if (text == NULL)
                Py_RETURN_NONE;
            py_text = PyString_FromString(text);
            g_free(text);
            return py_text;
        }
    }
    Py_RETURN_NONE;
}


static PyObject *
Scintilla_get_current_line(Scintilla *self, PyObject *args)
{
    gint line;
    if (self->sci != NULL)
    {
        line = sci_get_current_line(self->sci);
        return Py_BuildValue("i", line);
    }
    Py_RETURN_NONE;
}

static PyObject *
Scintilla_get_current_position(Scintilla *self, PyObject *args)
{
    gint pos;
    if (self->sci != NULL)
    {
        pos = sci_get_current_position(self->sci);
        return Py_BuildValue("i", pos);
    }
    Py_RETURN_NONE;
}


static PyObject *
Scintilla_get_length(Scintilla *self, PyObject *args)
{
    gint len;
    if (self->sci != NULL)
    {
        len = sci_get_length(self->sci);
        return Py_BuildValue("i", len);
    }
    Py_RETURN_NONE;
}


static PyObject *
Scintilla_get_line(Scintilla *self, PyObject *args)
{
    gint line_num = -1;
    gchar *text;
    PyObject *py_text;
    if (self->sci != NULL)
    {
        if (PyArg_ParseTuple(args, "|i", &line_num))
        {
            if (line_num == -1)
                line_num = sci_get_current_line(self->sci);
            text = sci_get_line(self->sci, line_num);
            if (text == NULL)
                Py_RETURN_NONE;
            py_text = PyString_FromString(text);
            g_free(text);
            return py_text;
        }
    }
    Py_RETURN_NONE;
}


static PyObject *
Scintilla_get_line_count(Scintilla *self, PyObject *args)
{
    gint line_count;
    if (self->sci != NULL)
    {
        line_count = sci_get_line_count(self->sci);
        return Py_BuildValue("i", line_count);
    }
    Py_RETURN_NONE;
}


static PyObject *
Scintilla_get_line_end_position(Scintilla *self, PyObject *args)
{
    gint line = -1, line_end_pos;
    if (self->sci != NULL)
    {
        if (PyArg_ParseTuple(args, "|i", &line))
        {
            if (line == -1)
                line = sci_get_current_line(self->sci);
            line_end_pos = sci_get_line_end_position(self->sci, line);
            return Py_BuildValue("i", line_end_pos);
        }
    }
    Py_RETURN_NONE;
}


static PyObject *
Scintilla_get_line_from_position(Scintilla *self, PyObject *args)
{
    gint line, pos;
    if (self->sci != NULL)
    {
        if (PyArg_ParseTuple(args, "|i", &pos))
        {
            if (pos == -1)
                pos = sci_get_current_position(self->sci);
            line = sci_get_line_from_position(self->sci, pos);
            return Py_BuildValue("i", line);
        }
    }
    Py_RETURN_NONE;
}


static PyObject *
Scintilla_get_line_indentation(Scintilla *self, PyObject *args)
{
    gint line = -1, width;
    if (self->sci != NULL)
    {
        if (PyArg_ParseTuple(args, "|i", &line))
        {
            if (line == -1)
                line = sci_get_current_line(self->sci);
            width = sci_get_line_indentation(self->sci, line);
            return Py_BuildValue("i", width);
        }
    }
    Py_RETURN_NONE;
}


static PyObject *
Scintilla_get_line_is_visible(Scintilla *self, PyObject *args)
{
    gint line = -1;
    gboolean visible;
    if (self->sci != NULL)
    {
        if (PyArg_ParseTuple(args, "|i", &line))
        {
            if (line == -1)
                line = sci_get_current_line(self->sci);
            visible = sci_get_line_is_visible(self->sci, line);
            if (visible)
                Py_RETURN_TRUE;
            else
                Py_RETURN_FALSE;
        }
    }
    Py_RETURN_NONE;
}


static PyObject *
Scintilla_get_line_length(Scintilla *self, PyObject *args)
{
    gint line = -1, length;
    if (self->sci != NULL)
    {
        if (PyArg_ParseTuple(args, "|i", &line))
        {
            if (line == -1)
                line = sci_get_current_line(self->sci);
            length = sci_get_line_length(self->sci, line);
            return Py_BuildValue("i", length);
        }
    }
    Py_RETURN_NONE;
}



static PyObject *
Scintilla_get_position_from_line(Scintilla *self, PyObject *args)
{
    gint line = -1, pos;
    if (self->sci != NULL)
    {
        if (PyArg_ParseTuple(args, "|i", &line))
        {
            if (line == -1)
                line = sci_get_current_line(self->sci);
            pos = sci_get_position_from_line(self->sci, line);
            return Py_BuildValue("i", pos);
        }
    }
    Py_RETURN_NONE;
}


static PyObject *
Scintilla_get_selected_text_length(Scintilla *self, PyObject *args)
{
    gint len;
    if (self->sci != NULL)
    {
        len = sci_get_selected_text_length(self->sci);
        return Py_BuildValue("i", len);
    }
    Py_RETURN_NONE;
}


static PyObject *
Scintilla_get_selection_contents(Scintilla *self, PyObject *args)
{
    gchar *text;
    PyObject *py_text;
    if (self->sci != NULL)
    {
        text = sci_get_selection_contents(self->sci);
        if (text == NULL)
            Py_RETURN_NONE;
        py_text = PyString_FromString(text);
        g_free(text);
        return py_text;
    }
    Py_RETURN_NONE;
}


static PyObject *
Scintilla_get_selection_end(Scintilla *self, PyObject *args)
{
    gint pos;
    if (self->sci != NULL)
    {
        pos = sci_get_selection_end(self->sci);
        return Py_BuildValue("i", pos);
    }
    Py_RETURN_NONE;
}


static PyObject *
Scintilla_get_selection_mode(Scintilla *self, PyObject *args)
{
    gint mode;
    if (self->sci != NULL)
    {
        mode = sci_get_selection_mode(self->sci);
        return Py_BuildValue("i", mode);
    }
    Py_RETURN_NONE;
}


static PyObject *
Scintilla_get_selection_start(Scintilla *self, PyObject *args)
{
    gint pos;
    if (self->sci != NULL)
    {
        pos = sci_get_selection_start(self->sci);
        return Py_BuildValue("i", pos);
    }
    Py_RETURN_NONE;
}


static PyObject *
Scintilla_get_style_at(Scintilla *self, PyObject *args)
{
    gint pos = -1, style;
    if (self->sci != NULL)
    {
        if (PyArg_ParseTuple(args, "|i", &pos))
        {
            if (pos == -1)
                pos = sci_get_current_position(self->sci);
            style = sci_get_style_at(self->sci, pos);
            return Py_BuildValue("i", style);
        }
    }
    Py_RETURN_NONE;
}


static PyObject *
Scintilla_get_tab_width(Scintilla *self, PyObject *args)
{
    gint width;
    if (self->sci != NULL)
    {
        width = sci_get_tab_width(self->sci);
        return Py_BuildValue("i", width);
    }
    Py_RETURN_NONE;
}


static PyObject *
Scintilla_goto_line(Scintilla *self, PyObject *args)
{
    gint line, unfold;
    if (self->sci != NULL)
    {
        if (PyArg_ParseTuple(args, "ii", &line, &unfold))
            sci_goto_line(self->sci, line, (gboolean) unfold);
    }
    Py_RETURN_NONE;
}


static PyObject *
Scintilla_has_selection(Scintilla *self, PyObject *args)
{
    if (self->sci != NULL)
    {
        if (sci_has_selection(self->sci))
            Py_RETURN_TRUE;
        else
            Py_RETURN_FALSE;
    }
    Py_RETURN_NONE;
}


static PyObject *
Scintilla_indicator_clear(Scintilla *self, PyObject *args)
{
    gint pos, len;
    if (self->sci != NULL)
    {
        if (PyArg_ParseTuple(args, "ii", &pos, &len))
            sci_indicator_clear(self->sci, pos, len);
    }
    Py_RETURN_NONE;
}


static PyObject *
Scintilla_indicator_set(Scintilla *self, PyObject *args)
{
    gint indic;
    if (self->sci != NULL)
    {
        if (PyArg_ParseTuple(args, "i", &indic))
            sci_indicator_set(self->sci, indic);
    }
    Py_RETURN_NONE;
}


static PyObject *
Scintilla_insert_text(Scintilla *self, PyObject *args)
{
    gint pos = -1;
    gchar *text;
    if (self->sci != NULL)
    {
        if (PyArg_ParseTuple(args, "s|i", &text, &pos))
        {
            if (pos == -1)
                pos = sci_get_current_position(self->sci);
            if (text != NULL)
                sci_insert_text(self->sci, pos, text);
        }
    }
    Py_RETURN_NONE;
}


static PyObject *
Scintilla_is_marker_set_at_line(Scintilla *self, PyObject *args)
{
    gboolean result;
    gint line, marker;
    if (self->sci != NULL)
    {
        if (PyArg_ParseTuple(args, "ii", &line, &marker))
        {
            result = sci_is_marker_set_at_line(self->sci, line, marker);
            if (result)
                Py_RETURN_TRUE;
            else
                Py_RETURN_FALSE;
        }
    }
    Py_RETURN_NONE;
}


static PyObject *
Scintilla_replace_sel(Scintilla *self, PyObject *args)
{
    gchar *text;
    if (self->sci != NULL)
    {
        if (PyArg_ParseTuple(args, "s", &text))
            sci_replace_sel(self->sci, text);
    }
    Py_RETURN_NONE;
}


static PyObject *
Scintilla_scroll_caret(Scintilla *self, PyObject *args)
{
    if (self->sci != NULL)
        sci_scroll_caret(self->sci);
    Py_RETURN_NONE;
}


static PyObject *
Scintilla_send_command(Scintilla *self, PyObject *args)
{
    gint cmd;
    if (self->sci != NULL)
    {
        if (PyArg_ParseTuple(args, "i", &cmd))
            sci_send_command(self->sci, cmd);
    }
    Py_RETURN_NONE;
}


static PyObject *
Scintilla_set_current_position(Scintilla *self, PyObject *args)
{
    gint pos, stc = FALSE;
    if (self->sci != NULL)
    {
        if (PyArg_ParseTuple(args, "i|i", &pos, &stc))
            sci_set_current_position(self->sci, pos, stc);
    }
    Py_RETURN_NONE;
}


static PyObject *
Scintilla_set_font(Scintilla *self, PyObject *args)
{
    gint style, size;
    gchar *font;
    if (self->sci != NULL)
    {
        if (PyArg_ParseTuple(args, "isi", &style, &font, &size))
            sci_set_font(self->sci, style, font, size);
    }
    Py_RETURN_NONE;
}


static PyObject *
Scintilla_set_line_indentation(Scintilla *self, PyObject *args)
{
    gint line, indent;
    if (self->sci != NULL)
    {
        if (PyArg_ParseTuple(args, "ii", &line, &indent))
            sci_set_line_indentation(self->sci, line, indent);
    }
    Py_RETURN_NONE;
}


static PyObject *
Scintilla_set_marker_at_line(Scintilla *self, PyObject *args)
{
    gint line, marker;
    if (self->sci != NULL)
    {
        if (PyArg_ParseTuple(args, "ii", &line, &marker))
            sci_set_marker_at_line(self->sci, line, marker);
    }
    Py_RETURN_NONE;
}


static PyObject *
Scintilla_set_selection_end(Scintilla *self, PyObject *args)
{
    gint pos;
    if (self->sci != NULL)
    {
        if (PyArg_ParseTuple(args, "i", &pos))
            sci_set_selection_end(self->sci, pos);
    }
    Py_RETURN_NONE;
}


static PyObject *
Scintilla_set_selection_mode(Scintilla *self, PyObject *args)
{
    gint mode;
    if (self->sci != NULL)
    {
        if (PyArg_ParseTuple(args, "i", &mode))
            sci_set_selection_mode(self->sci, mode);
    }
    Py_RETURN_NONE;
}


static PyObject *
Scintilla_set_selection_start(Scintilla *self, PyObject *args)
{
    gint pos;
    if (self->sci != NULL)
    {
        if (PyArg_ParseTuple(args, "i", &pos))
            sci_set_selection_start(self->sci, pos);
    }
    Py_RETURN_NONE;
}


static PyObject *
Scintilla_set_text(Scintilla *self, PyObject *args)
{
    gchar *text;
    if (self->sci != NULL)
    {
        if (PyArg_ParseTuple(args, "s", &text))
            sci_set_text(self->sci, text);
    }
    Py_RETURN_NONE;
}


static PyObject *
Scintilla_start_undo_action(Scintilla *self, PyObject *args)
{
    if (self->sci != NULL)
        sci_start_undo_action(self->sci);
    Py_RETURN_NONE;
}


static PyObject *
Scintilla_send_message(Scintilla *self, PyObject *args)
{
    guint msg;
    glong uptr = 0, sptr = 0, ret;
    if (self->sci != NULL)
    {
        if (PyArg_ParseTuple(args, "I|ll", &msg, &uptr, &sptr))
        {
            ret = scintilla_send_message(self->sci, msg, uptr, sptr);
            return Py_BuildValue("l", ret);
        }
    }
    Py_RETURN_NONE;
}


static PyMethodDef Scintilla_methods[] = {
    /* Private methods */
    { "_get_pointer", (PyCFunction) Scintilla_get_pointer, METH_NOARGS },
    { "_set_pointer", (PyCFunction) Scintilla_set_pointer, METH_O },


    /* Public methods */
    { "get_widget", (PyCFunction) Scintilla_get_widget, METH_NOARGS },
    { "delete_marker_at_line", (PyCFunction) Scintilla_delete_marker_at_line, METH_VARARGS },
    { "end_undo_action", (PyCFunction) Scintilla_end_undo_action, METH_VARARGS },
    { "ensure_line_is_visible", (PyCFunction) Scintilla_ensure_line_is_visible, METH_VARARGS },
    { "find_matching_brace", (PyCFunction) Scintilla_find_matching_brace, METH_VARARGS },
    { "find_text", (PyCFunction) Scintilla_find_text, METH_VARARGS },
    { "get_char_at", (PyCFunction) Scintilla_get_char_at, METH_VARARGS },
    { "get_col_from_position", (PyCFunction) Scintilla_get_col_from_position, METH_VARARGS },
    { "get_contents", (PyCFunction) Scintilla_get_contents, METH_VARARGS },
    { "get_contents_range", (PyCFunction) Scintilla_get_contents_range, METH_VARARGS },
    { "get_current_line", (PyCFunction) Scintilla_get_current_line, METH_VARARGS },
    { "get_current_position", (PyCFunction) Scintilla_get_current_position, METH_VARARGS },
    { "get_length", (PyCFunction) Scintilla_get_length, METH_VARARGS },
    { "get_line", (PyCFunction) Scintilla_get_line, METH_VARARGS },
    { "get_line_count", (PyCFunction) Scintilla_get_line_count, METH_VARARGS },
    { "get_line_end_position", (PyCFunction) Scintilla_get_line_end_position, METH_VARARGS },
    { "get_line_from_position", (PyCFunction) Scintilla_get_line_from_position, METH_VARARGS },
    { "get_line_indentation", (PyCFunction) Scintilla_get_line_indentation, METH_VARARGS },
    { "get_line_is_visible", (PyCFunction) Scintilla_get_line_is_visible, METH_VARARGS },
    { "get_line_length", (PyCFunction) Scintilla_get_line_length, METH_VARARGS },
    { "get_position_from_line", (PyCFunction) Scintilla_get_position_from_line, METH_VARARGS },
    { "get_selected_text_length", (PyCFunction) Scintilla_get_selected_text_length, METH_VARARGS },
    { "get_selection_contents", (PyCFunction) Scintilla_get_selection_contents, METH_VARARGS },
    { "get_selection_end", (PyCFunction) Scintilla_get_selection_end, METH_VARARGS },
    { "get_selection_mode", (PyCFunction) Scintilla_get_selection_mode, METH_VARARGS },
    { "get_selection_start", (PyCFunction) Scintilla_get_selection_start, METH_VARARGS },
    { "get_style_at", (PyCFunction) Scintilla_get_style_at, METH_VARARGS },
    { "get_tab_width", (PyCFunction) Scintilla_get_tab_width, METH_VARARGS },
    { "goto_line", (PyCFunction) Scintilla_goto_line, METH_VARARGS },
    { "has_selection", (PyCFunction) Scintilla_has_selection, METH_VARARGS },
    { "indicator_clear", (PyCFunction) Scintilla_indicator_clear, METH_VARARGS },
    { "indicator_set", (PyCFunction) Scintilla_indicator_set, METH_VARARGS },
    { "insert_text", (PyCFunction) Scintilla_insert_text, METH_VARARGS },
    { "is_marker_set_at_line", (PyCFunction) Scintilla_is_marker_set_at_line, METH_VARARGS },
    { "replace_sel", (PyCFunction) Scintilla_replace_sel, METH_VARARGS },
    { "scroll_caret", (PyCFunction) Scintilla_scroll_caret, METH_VARARGS },
    { "send_command", (PyCFunction) Scintilla_send_command, METH_VARARGS },
    { "set_current_position", (PyCFunction) Scintilla_set_current_position, METH_VARARGS },
    { "set_font", (PyCFunction) Scintilla_set_font, METH_VARARGS },
    { "set_line_indentation", (PyCFunction) Scintilla_set_line_indentation, METH_VARARGS },
    { "set_marker_at_line", (PyCFunction) Scintilla_set_marker_at_line, METH_VARARGS },
    { "set_selection_end", (PyCFunction) Scintilla_set_selection_end, METH_VARARGS },
    { "set_selection_mode", (PyCFunction) Scintilla_set_selection_mode, METH_VARARGS },
    { "set_selection_start", (PyCFunction) Scintilla_set_selection_start, METH_VARARGS },
    { "set_text", (PyCFunction) Scintilla_set_text, METH_VARARGS },
    { "start_undo_action", (PyCFunction) Scintilla_start_undo_action, METH_VARARGS },
    { "send_message", (PyCFunction) Scintilla_send_message, METH_VARARGS },
	{ NULL }
};


static PyMemberDef Scintilla_members[] = {
	{ NULL }
};


static PyTypeObject ScintillaType = {
	PyObject_HEAD_INIT(NULL)
    0,                          /*ob_size*/
    "geany.scintilla.Scintilla",  /*tp_name*/
    sizeof(Scintilla),          /*tp_basicsize*/
    0,                          /*tp_itemsize*/
    (destructor)Scintilla_dealloc, /*tp_dealloc*/
    0,                          /*tp_print*/
    0,                          /*tp_getattr*/
    0,                          /*tp_setattr*/
    0,                          /*tp_compare*/
    0,                          /*tp_repr*/
    0,                          /*tp_as_number*/
    0,                          /*tp_as_sequence*/
    0,                          /*tp_as_mapping*/
    0,                          /*tp_hash */
    0,                          /*tp_call*/
    0,                          /*tp_str*/
    0,                          /*tp_getattro*/
    0,                          /*tp_setattro*/
    0,                          /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    "Wrapper class around Scintilla's :c:type:`ScintillaObject` structure. "
    "This class should not be directly initialized, instead retrieve instances "
    "of it using through the :func:`Editor.get_scintilla()` method.", /* tp_doc */
    0,		                    /* tp_traverse */
    0,		               	    /* tp_clear */
    0,		                    /* tp_richcompare */
    0,		                    /* tp_weaklistoffset */
    0,		                    /* tp_iter */
    0,		                    /* tp_iternext */
    Scintilla_methods,          /* tp_methods */
    Scintilla_members,          /* tp_members */
    0,                          /* tp_getset */
    0,                          /* tp_base */
    0,                          /* tp_dict */
    0,                          /* tp_descr_get */
    0,                          /* tp_descr_set */
    0,                          /* tp_dictoffset */
    (initproc)Scintilla_init,   /* tp_init */
    0,                          /* tp_alloc */
    0,                          /* tp_new */

};


static PyMethodDef ScintillaModule_methods[] = { { NULL } };


PyMODINIT_FUNC
initscintilla(void)
{
    PyObject *m;

    NotificationType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&NotificationType) < 0)
        return;

    ScintillaType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&ScintillaType) < 0)
        return;

    m = Py_InitModule3("scintilla", ScintillaModule_methods,
            "The :mod:`scintilla` module provides a functions working with "
            ":class:`Scintilla` objects.");

    Py_INCREF(&NotificationType);
    PyModule_AddObject(m, "Notification", (PyObject *)&NotificationType);

    Py_INCREF(&ScintillaType);
    PyModule_AddObject(m, "Scintilla", (PyObject *)&ScintillaType);
}
