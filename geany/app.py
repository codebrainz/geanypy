"""
Simple wrapper around `_app` C extension module.
"""

import _app as c_app
import project


class App(object):

	"""
	Wrapper around `GeanyApp`.  No need to initialize this yourself,
	it's already set on `geany.app` since there's only 1 instance of
	this struct in Geany (`geany_data.app` in C).

	Example use:
		import geany
		print(geany.app.config_dir)
	"""

	_app = None

	def __init__(self, app=None):
		self._app = app if app is not None else c_app.App()

	@property
	def config_dir(self):
		"""
		User configuration directory, usually `~/.config/geany` on
		*nix-ish systems.

		For plugin configuration files, use a path such as this:
			import os
			cfg = os.path.join(geany.app.config_dir, "plugins", "pluginname")
		"""
		return self._app.get_config_dir()

	@property
	def debug_mode(self):
		"""
		Gets whether debug messages should be printed (ex. `-v` has been
		passed to Geany when run).  Returns `True` if debug messages
		should be printed or `False` if not.
		"""
		return self._app.get_debug_mode()

	@property
	def project(self):
		"""
		Gets the current active `Project` or `None` if no project is
		currently open.
		"""
		_proj = self._app.get_project()
		if _proj is not None and _proj._get_is_open():
			return  project.Project(_proj)
		return None
