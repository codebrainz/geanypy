import geany
import geany.signalmanager
#from geany import signal_manager

def geanysignal(event):
    def wrapper(func):
        geany.signal_manager.connect(event, func)
    return wrapper

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
        for callback in self._callbacks[event]:
            callback(*args)
