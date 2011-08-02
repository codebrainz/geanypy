"""
All plugins need to import this module and inherit from the Plugin class.  A
basic plugin could look something like this:

    import geany
    from geany.plugin import Plugin

    class OpenNewDocumentPlugin(Plugin):

        __plugin_name__ = "Open new document plugin"

        _doc = None

        def __init__(self):
            Plugin.__init__(self)
            print "About to open a new document"
            self.open_document('/path/to/some/file')

        def open_document(self, filename):
            self._doc = geany.document.open(filename)

        def close_document(self):
            self._doc.close()
            self._doc = None

        def cleanup(self):
            if self._doc is not None:
                self._doc.close()

The guts of the API are exposed to plugins through the `geany` package and
its modules.

Plugins should be placed in either the system plugin directory (something
like /usr/local/share/geany/geanypy/plugins) or in their personal plugin
directory (something like ~/.config/geany/plugins/geanypy/plugins).  Only
files with a `.py` extension will be loaded.
"""

# TODO: Remove the event stuff from here and either bind the C API or do
#       this in another way/place.

class Plugin(object):
    """
    Base class for all plugins.  All plugins must inherit from this in order
    to be properly detected.
    """

    # Child classes should implement these (at least __plugin__name__)
    #__plugin_name__ = None
    #__plugin_description__ = None
    #__plugin_version__ = None
    #__plugin_author__ = None


    _events = {
        "document-open": [],
        # TODO: add more events here
    }


    def __init__(self):
        """
        When the plugin is loaded its __init__() function will be called
        so that's a good place to put plugin initialization code.
        """
        pass


    def cleanup(self):
        """
        When the plugin is unloaded the cleanup() function will be called, so
        it's a good place to put and clean-up/tear-down code.
        """
        pass


    def plugin_connect(self, event, func, *args, **kwargs):
        """
        Add a callback function to the list that will be called when the
        event occurs.  The function should have a prototype like this:

            some_callback_handler(*args, **kwargs)

        Where the args and kwargs passed to plugin_connect() will be passed
        the the callback function, possibly including addition data.

        Plugins should be sure not to override this function.
        """
        try:
            self._events[event].append((func, args, kwargs))
        except KeyError:
            print("Unable to connecto to event '%s': unknown event" % event)


    def plugin_disconnect(self, event, func):
        """
        Remove a callback function from list that will be called when the
        event occurrs.  Only the first matching func will be removed.

        Plugins should be sure not to override this function.
        """
        try:
            for callback_info in self._events[event]:
                if callback_info[0] == func:
                    self._events[event].remove(callback_info)
                    break
            else:
                print("Unable to disconnect function from event '%s': " +
                        "function not connected" % event)
        except KeyError:
            print("Unable to disconnect function from event '%s': " +
                    "unknown event" % event)


    def plugin_emit(self, event, *args, **kwargs):
        """
        Fires an event, causing any callback functions registered for the
        event will be called.  The args and kwargs passed in will be prepended
        to args associated with the callback function and kwargs will be
        merged with the kwargs associated with the callback function.

        Plugins should be sure not to override this function.
        """
        try:
            for callback_info in self._events[event]:
                try:
                    new_args = args + callback_info[1]
                    new_kwargs = {}
                    for kw, arg in callback_info[2].items() + kwargs.items():
                        new_args[kw] = arg
                    callback_info[0](*new_args, **new_kwargs)
                except IndexError:
                    print("Unable to call event handler for event '%s': " +
                            "corrupt callback information" % event)
        except KeyError:
            print("Unable to emit event '%s': unknown event" % event)


    @property
    def __plugin_name__(self):
        """
        Plugins must implement a __plugin_name__ attribute that returns the
        string name of the plugin.
        """
        raise NotImplementedError(
            "Plugin's must implement the __plugin_name__ attribute.")


    @property
    def name(self):
        """
        Get plugin's name.
        """
        return self.__plugin_name__


    @property
    def description(self):
        """
        Get plugin's description.
        """
        if hasattr(self, '__plugin_description__'):
            return self.__plugin_description__
        else:
            return ""


    @property
    def version(self):
        """
        Get plugin's version.
        """
        if hasattr(self, '__plugin_version__'):
            return self.__plugin_version__
        else:
            return ""


    @property
    def author(self):
        """
        Get plugin's author.
        """
        if hasattr(self, '__plugin_author__'):
            return self.__plugin_author__
        else:
            return ""
