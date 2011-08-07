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

from signalmanager import geanysignal

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

    @geanysignal('build-start')
    def on_build_start():
        print("Signal 'build-start' emitted")

    @geanysignal('document-activate')
    def on_document_active(doc):
        print("Signal 'document-activate' emitted")

    @geanysignal('document-before-save')
    def on_document_before_save(doc):
        print("Signal 'document-before-save' emitted")

    @geanysignal('document-close')
    def on_document_close(doc):
        print("Signal 'document-close' emitted")

    @geanysignal('document-filetype-set')
    def on_document_filetype_set(doc, ft):
        print("Signal 'document-filetype-set' emitted")

    @geanysignal('document-new')
    def on_document_new(doc):
        print("Signal 'document-new' emitted")

    @geanysignal('document-open')
    def on_document_open(doc):
        print("Signal 'document-open' emitted")

    @geanysignal('document-reload')
    def on_document_reload(doc):
        print("Signal 'document-reload' emitted")

    @geanysignal('document-save')
    def on_document_save(doc):
        print("Signal 'document-save' emitted")

    @geanysignal('editor-notify')
    def on_editor_notify(editor, notif):
        print("Signal 'editor-notify' emitted")

    @geanysignal('geany-startup-complete')
    def on_geany_startup_complete():
        print("Signal 'geany-startup-complete' emitted")

    @geanysignal('project-close')
    def on_project_close():
        print("Signal 'project-close' emitted")

    @geanysignal('project-dialog-confirmed')
    def on_project_dialog_confirmed(notebook):
        print("Signal 'project-dialog-confirmed' emitted")

    @geanysignal('project-dialog-create')
    def on_project_dialog_create(notebook):
        print("Signal 'project-dialog-create' emitted")

    @geanysignal('project-open')
    def on_project_open(config):
        print("Signal 'project-open' emitted")

    @geanysignal('project-save')
    def on_project_save(config):
        print("Signal 'project-save' emitted")

    @geanysignal('update-editor-menu')
    def on_update_editor_menu(word, pos, doc):
        print("Signal 'update-editor-menu' emitted")
