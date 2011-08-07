import geany
import document
import editor
import filetypes
import project
import scintilla
import _geany_document
import _geany_editor
import _geany_filetypes
import _geany_project
import _geany_scintilla_notification
from collections import namedtuple


#: Simple object to hold a callback callable and the arguments to pass to it.
Callback = namedtuple('Callback', 'func args')


class SignalManager(object):
    """
    Manages callback functions for events emitted by Geany's internal GObject.
    """

    #: Stores registered callbacks and arguments for each event.
    callbacks = {
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


    def connect(self, event, func, *args):
        """
        Connect C{func} to the C{event} specified.  When the C{event} is
        emitted, C{func} will be called with the C{event}s arguments and the
        C{args} appended.  C{args} can be used for any type of user data to
        pass when the signal is emitted.

        @type event: string
        @param event: name of one of the events, see C{SignalManager.callbacks}.
        @type func: callable
        @param func: a callable object that will be called when the signal
            specified by C{event} is emitted.
        @type args: tuple
        @param args: any user-supplied arguments to pass to C{func} when
            calling it.

        @raise ValueError: when C{event} is not one of the known events.
        """

        if event not in self.callbacks:
            raise ValueError("Cannot connect to unknown event '%s'" % event)
        self.callbacks[event].append(Callback(func, args))


    def disconnect(self, event, func):
        """
        Disconnect C{func} from being called when C{event} is emitted.  Any
        callback functions registered which are equal to C{func} will be
        removed.

        @type event: string
        @param event: name of the event to remove callable C{func} from being
            called for.
        @type func: callable
        @param func: the callable object to search for in the registered
            callbacks for C{event} and remove.

        @raise ValueError: when C{event} is not one of the known events.
        """

        if event not in self.callbacks:
            raise ValueError("Cannot disconnect from unknown event '%s'" % event)
        for callback in self.callbacks[event]:
            if callback.func == func:
                self.callbacks[event].remove(callback)


    def emit_signal(self, event, *args):
        """
        For each callback registered for C{event}, call it with the specifed
        C{args} prefixed to the registered callback arguments passed when
        it was connected with C{connect}.

        @type event: string
        @param event: name of the event to emit
        @type args: tuple
        @param args: extra arguments to prefix to the registered arguments
            passed to the callback.

        @raise ValueError: when C{event} is not one of the known events.
        """

        if event not in self.callbacks:
            raise ValueError("Cannot emit unknown event '%s'" % event)

        new_args = []
        for arg in args:
            if isinstance(arg, _geany_document.Document):
                doc = document.Document()
                doc._doc = arg
                new_args.append(doc)
            elif isinstance(arg, _geany_filetypes.Filetype):
                ft = filetypes.Filetype()
                ft._ft = arg
                new_args.append(ft)
            elif isinstance(arg, _geany_editor.Editor):
                ed = editor.Editor()
                ed._editor = arg
                new_args.append(ed)
            elif isinstance(arg, _geany_project.Project):
                proj = project.Project()
                project._project = arg
                new_args.append(project)
            elif isinstance(arg, _geany_scintilla_notification.Notification):
                notif = scintilla.Notification()
                notif._notif = arg
                new_args.append(notif)
            else:
                new_args.append(arg)

        for callback in self.callbacks[event]:
            args_ = list(new_args[:])
            args_.extend(list(callback.args))
            callback.func(*tuple(args_))
