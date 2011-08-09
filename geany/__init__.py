"""
Package file that exposes some of Geany's guts as its own attibutes.  Any
objects where it only makes sense to have one instance of are initialed here
and set as attributes.

You can sort of think of this file as the GeanyData struct from the C API,
though no real attempt is made to mimic that structure here.
"""

import console
import dialogs
import document
import editor
import encodings
import filetype
import highlighting
import indentprefs
import loader
import main
import manager
import msgwindow
import navqueue
import project
import scintilla

from app import App
from editorprefs import EditorPrefs
from fileprefs import FilePrefs
from mainwidgets import MainWidgets
from signalmanager import SignalManager


__all__ = [ "Plugin",
            "app",
            "editor_prefs",
            "file_prefs",
            "main_widgets",
            "signals" ]

# Geany's application data fields
app = App()

# Settings that will by used by Editors
editor_prefs = EditorPrefs()

# Settings relating to how files are handled
file_prefs = FilePrefs()

# Import GTK+ widgets that are part of Geany's UI
main_widgets = MainWidgets()

# Allow plugins to connect/disconnect from Geany signals
signals = SignalManager()

import plugin
from plugin import Plugin
