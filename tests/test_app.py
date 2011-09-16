import os, sys
import unittest
import nose
from nose.tools import *
import geany


class TestApp(unittest.TestCase):

	def test_configdir_read(self):
		assert isinstance(geany.app.configdir, basestring)
		assert os.path.isdir(geany.app.configdir)

	def test_debug_mode_read(self):
			assert isinstance(geany.app.debug_mode, bool)

	def test_project_read(self):
			assert isinstance(geany.app.project, geany.project.Project) or \
				geany.app.project is None

	@raises(AttributeError)
	def test_configdir_write(self):
		geany.app.configdir = "/tmp"

	@raises(AttributeError)
	def test_debug_mode_write(self):
		geany.app.debug_mode = True

	@raises(AttributeError)
	def test_project_write(self):
		geany.app.project = geany.project.Project()


def run():
	args = [ 'GeanyPy' ]
	nose.run(argv = args)

if __name__ == '__main__': main()
