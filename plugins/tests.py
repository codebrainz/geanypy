import os, sys
import gtk
import geany

class HelloWorld(geany.Plugin):

	__plugin_name__ = "GeanyPy Testing Plugin"
	__plugin_version__ = "1.0"
	__plugin_description__ = "Runs unit tests on the GeanyPy API."
	__plugin_author__ = "Matthew Brush <mbrush@codebrainz.ca>"

	_last_test_dir = None

	def __init__(self):
		self.item = gtk.MenuItem('Run GeanyPy tests')
		self.item.connect('activate', self.run_tests)
		geany.main_widgets.tools_menu.add(self.item)
		self.item.show()

	def run_tests(self, widget, data=None):
		self.get_tests_path()
		print('Running app test')
		import test_app
		test_app.run()


	def get_tests_path(self):
		d = gtk.FileChooserDialog('Select tests dir',
				geany.main_widgets.window,
				gtk.FILE_CHOOSER_ACTION_SELECT_FOLDER,
				(gtk.STOCK_CANCEL, gtk.RESPONSE_REJECT,
					gtk.STOCK_OK, gtk.RESPONSE_ACCEPT))

		if self._last_test_dir:
			d.set_filename(self._last_test_dir)

		if d.run() == gtk.RESPONSE_ACCEPT:
			fn = d.get_filename()
			sys.path.append(fn)
			self._last_test_dir = fn

		d.destroy()


	def cleanup(self):
		self.item.destroy()
