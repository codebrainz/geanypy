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
			self._doc = geany.document.open_file(filename)

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

import gobject
import gtk


class Plugin(gobject.GObject):
	"""
	Base class for all plugins.  All plugins must inherit from this in order
	to be properly detected.
	"""

	# Child classes should implement these (at least __plugin__name__)
	#__plugin_name__ = None
	#__plugin_description__ = None
	#__plugin_version__ = None
	#__plugin_author__ = None

	__gsignals__ = {
		# These two signals notify GeanyPy that a keybinding was added or
		# removed.
		'keybinding-added':		(gobject.SIGNAL_RUN_LAST, gobject.TYPE_NONE,
									(gobject.TYPE_STRING, gobject.TYPE_PYOBJECT)),
		'keybinding-removed':	(gobject.SIGNAL_RUN_LAST, gobject.TYPE_NONE,
									(gobject.TYPE_STRING, gobject.TYPE_PYOBJECT)),
	}


	def _normalize_accel_string(self, accel_string):
		"""
		Converts an accel_string to keyval and modifiers and back to a string
		to filter it into the same string even accel_string is slightly
		different each time.
		"""
		kv, km = gtk.accelerator_parse(accel_string)
		if kv and km:
			return gtk.accelerator_name(kv, km)
		else:
			return False


	def add_keybinding(self, accel_string, callback):
		"""
		Registers a new keybinding.  Returns True if the callback was
		registered or False if there was a problem and it wasn't.
		"""
		# normalize the accel_string
		accel_string = self._normalize_accel_string(accel_string)
		if not accel_string:
			return False
		if accel_string in self._keybindings:
			self._keybindings[accel_string].append(callback)
		else:
			self._keybindings[accel_string] = [callback]
		self.emit('keybinding-added', accel_string, callback)
		return True


	def remove_keybinding(self, accel_string, callback, remove_all=False):
		"""
		Unregisters an existing keybinding.  Returns True if the callback
		was unregistered or False if there was a problem and it wasn't. If
		remove_all is True, all instances of callback that were registered
		for accel_string will be removed, otherwise only the first one found
		is removed.
		"""
		accel_string = self._normalize_accel_string(accel_string)
		if not accel_string:
			return False
		if accel_string not in self._keybindings:
			return False
		else:
			rmed = False
			for cb in self._keybindings[accel_string]:
				if cb == callback:
					self._keybindings[accel_string].remove(cb)
					rmed = True
					self.emit('keybinding-removed', accel_string, callback)
					if not remove_all:
						return True
			return rmed


	def __init__(self):
		"""
		When the plugin is loaded its __init__() function will be called
		so that's a good place to put plugin initialization code.
		"""
		self.__gobject_init__()
		self._keybindings = {} # used internally to track registered keybindings


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


gobject.type_register(Plugin)
