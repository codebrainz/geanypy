import geany
import document
import editor
import filetypes
import _geany_document
import _geany_editor
import _geany_filetypes

class SignalManager(object):

    _callbacks = {
        'build-start': [],
        'document-activate': [],
        'document-before-save': [],
        'document-close': [],
        'document-filetype-set': [],
        'document-new': [],
        'document-open': [],
        'document-reload': [],
        'document-save': [],
        'editor-notify': [],
        'geany-startup-complete': [],
        'project-close': [],
        'project-dialog-confirmed': [],
        'project-dialog-create': [],
        'project-open': [],
        'project-save': [],
        'update-editor-menu': []
    }

    def __init__(self):
        pass

    def connect(self, event, func):
        if event not in self._callbacks:
            raise ValueError("Cannot connect to unknown event '%s'" % event)
        self._callbacks[event].append(func)

    def disconnect(self, event, func):
        if event not in self._callbacks:
            raise ValueError("Cannot disconnect from unknown event '%s'" % event)
        for callback in self._callbacks[event]:
            if callback == func:
                self._callbacks[event].remove(func)

    def emit_signal(self, event, *args):
        if event not in self._callbacks:
            raise ValueError("Cannot emit unknown event '%s'" % event)

        new_args = []
        for arg in args:
            if isinstance(arg, _geany_document.Document):
                doc = document.Document()
                doc._doc = arg
                new_args.append(doc)
            elif isinstance(arg, _geany_filetypes.Filetype):
                ft = document.Filetype()
                ft._ft = arg
                new_args.append(ft)
            elif isinstance(arg, _geany_editor.Editor):
                editor = editor.Editor()
                editor._editor = arg
                new_args.append(editor)
            else:
                new_args.append(arg)

        new_args = tuple(new_args)

        for callback in self._callbacks[event]:
            callback(*new_args)
