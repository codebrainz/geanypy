import glib
import gtk
import geany
from doc import Document

__all__ = [ "Document" ]

def status_message(msg, log=False):
    """ Place a message in the status bar, and logged in the status
        tab if log is True. """
    geany.ui_utils.set_statusbar(msg, log)

def error_message(msg):
    " Present an error dialog to the user. "
    geany.dialogs.show_msgbox(msg, gtk.MESSAGE_ERROR)

def info_message(msg):
    " Present an informational dialog to the user. "
    geany.dialogs.show_msgbox(msg, gtk.MESSAGE_INFO)

def question_dialog(msg):
    """ Present the user with a question dialog and return True if the
        answer is positive (ie. they cliked "Yes"). """
    return geany.dialogs.show_question(msg)

def user_input(label, initial=None):
    """ Presents the user with an input dialog. Label is the text
        show in the dialog and initial is the initial value in the
        input box. """
    return geany.dialogs.show_input(label_text = label,
            default_text = '' if initial is None else initial)

def add_idle(callback):
    """ Connect a callback function to be called when the main loop
        is idle. If the callback returns True it will continue to
        be called, otherwise it will only be called once. If the
        callback always returns True, the plugin MUST use the
        disconnect() function with the return value of this function
        to remove the function from being called. """
    return glib.idle_add(callback)

def add_timeout(interval_ms, callback):
    """ Same as add_idle() except that the function is called after
        a timeout (in milliseconds) instead of when the main loop
        is idle. If the callback returns True it will continue to
        be called, otherwise it will only be called once. If the
        callback always returns True, the plugin MUST use the
        disconnect() function with the return value of this function
        to remove the function from being called. """
    return glib.timeout_add(interval_ms, callback)

def disconnect(callback_id):
    """ Disconnects the callback id as returned by add_idle() or
        add_timeout() functions, causing the callback to longer be
        called. This has the same effect as the callback function
        not returning True. """
    glib.source_remove(callback_id)

def documents():
    " Returns a list of open documents. An alias for Document.list_documentS(). "
    return Document.list_documents()
