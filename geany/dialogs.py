import _geany_dialogs
import gtk

def show_input(title=None, parent=None, label_text=None, default_text=None):
    return _geany_dialogs.show_input(title, parent, label_text, default_text)

def show_input_numeric(title=None, label_text=None, value=0.0, minimum=0.0, maximum=100.0, step=1.0):
    return _geany_dialogs.show_input_numeric(title, label_text, value, minimum, maximum, step)

def show_msgbox(text, msgtype=gtk.MESSAGE_INFO):
    _geany_dialogs.show_msgbox(str(text), int(msgtype))

def show_question(text):
    return _geany_dialogs.show_question(str(text))

def show_save_as():
    return _geany_dialogs.show_save_as()
