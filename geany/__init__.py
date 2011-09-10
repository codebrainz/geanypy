"""
Package file that exposes some of Geany's guts as its own attibutes.  Any
objects where it only makes sense to have one instance of are initialed here
and set as attributes.

You can sort of think of this file as the GeanyData struct from the C API,
though no real attempt is made to mimic that structure here.
"""

import app
import console
import dialogs
import document
import editor
import encoding
import filetypes
import highlighting
import indentprefs
import loader
import main
import mainwidgets
import manager
import msgwindow
import navqueue
import project
import scintilla
import prefs

from app import App
from prefs import Prefs
from mainwidgets import MainWidgets
from main import is_realized, locale_init, reload_configuration
from signalmanager import SignalManager


__all__ = [ "Plugin",
            "is_realized",
            "locale_init",
            "reload_configuration",
            "main_widgets",
            "app",
            "prefs",
            "signals" ]

# Geany's application data fields
app = App()

# Geany's preferences
prefs = Prefs()

# Import GTK+ widgets that are part of Geany's UI
main_widgets = MainWidgets()

# Allow plugins to connect/disconnect from Geany signals
signals = SignalManager()

import plugin
from plugin import Plugin
