import os, sys
import unittest
import nose
from nose.tools import *
import geany


class TestProject(unittest.TestCase):

	def test_base_path_read(self):
		if geany.app.project:
			assert isinstance(geany.app.project.base_path, basestring)

	def test_description_read(self):
		if geany.app.project:
			assert isinstance(geany.app.project.description, basestring)

	def test_file_name_read(self):
		if geany.app.project:
			assert isinstance(geany.app.project.file_name, basestring)
			assert os.path.isfile(geany.app.project.file_name)

	def test_file_patterns_read(self):
		if geany.app.project:
			print("****** TYPE: %s" % type(geany.app.project.file_patterns).__name__)
			assert isinstance(geany.app.project.file_patterns, frozenset)

	def test_name_read(self):
		if geany.app.project:
			assert isinstance(geany.app.project.name, basestring)

	def test_type_read(self):
		if geany.app.project:
			assert isinstance(geany.app.project.type, int)
			# TODO: test for expected value here

	@raises(AttributeError)
	def test_base_path_write(self):
		if geany.app.project:
			geany.app.project.base_path = "/tmp"

	@raises(AttributeError)
	def test_description_write(self):
		if geany.app.project:
			geany.app.project.description = "foo"

	@raises(AttributeError)
	def test_file_name_write(self):
		if geany.app.project:
			geany.app.project.file_name = "/tmp/tmp.proj"

	@raises(AttributeError)
	def test_file_patterns_write(self):
		if geany.app.project:
			geany.app.project.file_patterns = [ '*.foo' ]

	@raises(AttributeError)
	def test_name_write(self):
		if geany.app.project:
			geany.app.project.name = "foo"

	@raises(AttributeError)
	def test_type_write(self):
		if geany.app.project:
			geany.app.project.type = 1


def run():
	args = [ 'GeanyPy' ]
	nose.run(argv = args)

if __name__ == '__main__': main()
