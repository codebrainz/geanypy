"""
A simple analog of `GeanyObject` in the C API, that is, an object to emit
all signals on.  The signals are emitted from the C code in signalmanager.c,
where the Geany types get wrapped in PyObject types.
"""
from gi.repository import GObject


class SignalManager(GObject.GObject):
    """
    Manages callback functions for events emitted by Geany's internal GObject.
    """
    __gsignals__ = {
        'build-start':                (GObject.SignalFlags.RUN_LAST, None,
                                        ()),
        'document-activate':        (GObject.SignalFlags.RUN_LAST, None,
                                        (object,)),
        'document-before-save':        (GObject.SignalFlags.RUN_LAST, None,
                                        (object,)),
        'document-close':            (GObject.SignalFlags.RUN_LAST, None,
                                        (object,)),
        'document-filetype-set':    (GObject.SignalFlags.RUN_LAST, None,
                                        (object, object)),
        'document-new':                (GObject.SignalFlags.RUN_LAST, None,
                                        (object,)),
        'document-open':            (GObject.SignalFlags.RUN_LAST, None,
                                        (object,)),
        'document-reload':            (GObject.SignalFlags.RUN_LAST, None,
                                        (object,)),
        'document-save':            (GObject.SignalFlags.RUN_LAST, None,
                                        (object,)),
        'editor-notify':            (GObject.SignalFlags.RUN_LAST, GObject.TYPE_BOOLEAN,
                                        (object, object)),
        'geany-startup-complete':    (GObject.SignalFlags.RUN_LAST, None,
                                        ()),
        'project-close':             (GObject.SignalFlags.RUN_LAST, None,
                                        ()),
        'project-dialog-confirmed':    (GObject.SignalFlags.RUN_LAST, None,
                                        (object,)),
        'project-dialog-open':        (GObject.SignalFlags.RUN_LAST, None,
                                        (object,)),
        'project-dialog-close':        (GObject.SignalFlags.RUN_LAST, None,
                                        (object,)),
        'project-open':                (GObject.SignalFlags.RUN_LAST, None,
                                        (object,)),
        'project-save':                (GObject.SignalFlags.RUN_LAST, None,
                                        (object,)),
        'update-editor-menu':        (GObject.SignalFlags.RUN_LAST, None,
                                        (GObject.TYPE_STRING, GObject.TYPE_INT,
                                        object)),
    } # __gsignals__

    def __init__(self):
        super(SignalManager, self).__init__()

GObject.type_register(SignalManager)