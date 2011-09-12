import os
import imp
from collections import namedtuple
import geany
import gobject

PluginInfo = namedtuple('PluginInfo', 'filename, name, version, description, author, cls')


class PluginLoader(gobject.GObject):

	__gsignals__ = {
		'plugin-activate':		(gobject.SIGNAL_RUN_LAST, gobject.TYPE_NONE,
									(gobject.TYPE_PYOBJECT,)),
		'plugin-deactivate':	(gobject.SIGNAL_RUN_LAST, gobject.TYPE_NONE,
									(gobject.TYPE_PYOBJECT,)),
	}

	plugins = {}

	def __init__(self, plugin_dirs):

		self.__gobject_init__()

		self.plugin_dirs = plugin_dirs

		self.available_plugins = []
		for plugin in self.iter_plugin_info():
			self.available_plugins.append(plugin)

		self.restore_loaded_plugins()


	def restore_loaded_plugins(self):

		for path in self.plugin_dirs:
			if os.path.isdir(path):
				state_file = os.path.join(path, '.loaded_plugins')
				if os.path.isfile(state_file):
					loaded_plugins = list(f.strip() for f in open(state_file) if f.strip())
					for filename in loaded_plugins:
						self.load_plugin(filename)


	def load_all_plugins(self):

		for plugin_info in self.iter_plugin_info():
			if plugin_filename.endswith('test.py'): # hack for testing
				continue
			plug = self.load_plugin(plugin_info.filename)
			if plug:
				print("Loaded plugin: %s" % plugin_info.filename)
				print("  Name: %s v%s" % (plug.name, plug.version))
				print("  Desc: %s" % plug.description)
				print("  Author: %s" % plug.author)


	def unload_all_plugins(self):

		for plugin in self.plugins:
			self.unload_plugin(plugin)


	def reload_all_plugins(self):

		self.unload_all_plugins()
		self.load_all_plugins()


	def iter_plugin_info(self):

		for d in self.plugin_dirs:
			if os.path.isdir(d):
				for f in os.listdir(d):
					if f.endswith('.py'):
						filename = os.path.abspath(os.path.join(d, f))
						if filename.endswith("test.py"):
							continue
						text = open(filename).read()
						module_name = os.path.basename(filename)[:-3]
						try:
							module = imp.load_source(module_name, filename)
						except ImportError:
							continue
						for k, v in module.__dict__.iteritems():
							if k == geany.Plugin.__name__:
								continue
							try:
								if issubclass(v, geany.Plugin):
									inf = PluginInfo(
											filename,
											getattr(v, '__plugin_name__'),
											getattr(v, '__plugin_version__', ''),
											getattr(v, '__plugin_description__', ''),
											getattr(v, '__plugin_author__', ''),
											v)
									yield inf
							except TypeError:
								continue


	def load_plugin(self, filename):

		for avail in self.available_plugins:
			if avail.filename == filename:
				inst = avail.cls()
				self.plugins[filename] = inst
				self.emit('plugin-activate', inst)
				return inst


	def unload_plugin(self, filename):

		try:
			plugin = self.plugins[filename]
			self.emit('plugin-deactivate', plugin)
			plugin.cleanup()
			del self.plugins[filename]
		except KeyError:
			print("Unable to unload plugin '%s': it's not loaded" % filename)


	def reload_plugin(self, filename):

		if filename in self.plugins:
			self.unload_plugin(filename)
		self.load_plugin(filename)


	def plugin_has_help(self, filename):

		for plugin_info in self.iter_plugin_info():
			if plugin_info.filename == filename:
				return hasattr(plugin_info.cls, 'show_help')


	def plugin_has_configure(self, filename):

		try:
			return hasattr(self.plugins[filename], 'show_configure')
		except KeyError:
			#print "** no configure: %s" % filename
			return None


gobject.type_register(PluginLoader)
