import dialogs
import filetype
import document
import editor
import mainwidgets
import editorprefs
import app
import fileprefs
from plugin import Plugin
from console import _make_window as show_console


__all__ = [ "show_console", "main_widgets", "editor_prefs", "app",
            "file_prefs", "Plugin" ]


main_widgets = mainwidgets.MainWidgets()
editor_prefs = editorprefs.EditorPrefs()
app = app.App()
file_prefs = fileprefs.FilePrefs()
