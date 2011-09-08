"""
Simple wrapper around `_prefs` C extension module.
"""

import _prefs as c_prefs


class Prefs(object):

	"""
	Wrapper around `GeanyPrefs`.  No need to initialize this yourself,
	it's already set on `geany.prefs` since there's only 1 instance of
	this struct in Geany (`geany_data.prefs` in C).

	Example use:
		import geany
		print(geany.prefs.default_open_path)
	"""

	_prefs = None

	def __init__(self, prefs=None):
		self._prefs = prefs if prefs is not None else c_prefs.Prefs()

	@property
	def default_open_path(self):
		return self._prefs.get_default_open_path()
