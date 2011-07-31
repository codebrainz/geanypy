import dialogs
import filetype
import document
import editor
import mainwidgets
import editorprefs
import app
from console import _make_window as show_console


__all__ = [ "show_console", "main_widgets", "editor_prefs", "app" ]


main_widgets = mainwidgets.MainWidgets()
editor_prefs = editorprefs.EditorPrefs()
app = app.App()
