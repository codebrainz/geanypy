#!/usr/bin/env python

import geany
import gtk, webkit
import email

import subprocess
from os.path import exists
from tempfile import NamedTemporaryFile
import os

# Import find executable engine
try:
	from shutil import which
except ImportError:
	from distutils.spawn import find_executable

	which = find_executable

# Path to the executable
PANDOC_PATH = which('pandoc')
ASCIIDOC_PATH = which('asciidoc')

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class ConsolePlugin(geany.Plugin):
	__plugin_name__ = "Mark-Up/Down Previewer"
	__plugin_description__ = ("Displays preview of multiple light-weight markup languages in the sidebar.  Uses pandoc and asciidoc to generate html.")
	__plugin_version__ = "20161001.001010"
	__plugin_author__ = "xiota <xiota@mentalfossa.com>"

	def __init__(self):
		self.hadj = None
		self.vadj = None
		self.hadj_save = 1	# something non-zero
		self.vadj_save = 1	# something non-zero

		self.install_preview()
		geany.signals.connect('editor-notify', self.on_editor_notify)
		geany.signals.connect('document-open', self.on_document_activity)
		geany.signals.connect('document-new', self.on_document_activity)
		geany.signals.connect('document-save', self.on_document_activity)
		geany.signals.connect('document-activate', self.on_document_activity)
		geany.signals.connect('document-filetype-set', self.on_document_activity)

	def cleanup(self):
		self.uninstall_preview()

	def install_preview(self):
		self.box1 = gtk.VBox()
		self.box2 = gtk.HBox()

		self.addressbar = gtk.Entry()
		self.addressbar.connect('activate', self.on_address_action)

		self.gobutton = gtk.Button("Go")
		self.gobutton.connect('clicked', self.on_address_action)

		self.swin = gtk.ScrolledWindow()

		self.web = webkit.WebView()
		self.web.connect('load-committed', self.on_load_committed)
		self.web.connect('load-started', self.on_load_started)
		self.web.connect('load-finished', self.on_load_finished)

		self.box1.pack_start(self.box2, False)
		self.box1.pack_start(self.swin)

		self.box2.pack_start(self.addressbar)
		self.box2.pack_start(self.gobutton, False)

		self.swin.add(self.web)

		# future option to choose: message_window_notebook, sidebar_notebook
		geany.main_widgets.sidebar_notebook.append_page(self.box1, gtk.Label("Preview"))

		self.box1.show_all()

		# set scrollbar objects for position save/restore
		self.hadj = self.swin.get_hadjustment()
		self.vadj = self.swin.get_vadjustment()

	def uninstall_preview(self):
		self.box1.destroy()

	def on_address_action(self, widget):
		address = self.addressbar.get_text()

		if not (address.startswith("http://") or address.startswith("/")):
			address = "http://" + address
			self.addressbar.set_text(address)

		self.web.open(address)

	def on_load_committed(self, view, frame):
		uri = frame.get_uri()
		if uri == None:
			uri = ''

		self.addressbar.set_text(uri)

	def on_load_started(self, view, frame):
		self.save_scrollbar_position()

	def on_load_finished(self, view, frame):
		self.restore_scrollbar_position()

	def on_document_activity(self, obj = None, doc = None, filetype_old = None):
		doc = geany.document.get_current()

		if doc == None:
			text = ''
			file_type = ''
			file_name = ''
			file_ext = ''
		else:
			file_type = doc.file_type.name
			text = doc.editor.scintilla.get_contents()
			file_name, file_ext = os.path.splitext(doc.file_name)

		self.preview_update(text, file_type, file_name, file_ext)

	def on_editor_notify(self, obj, editor, notification):
		if ((notification.modification_type & geany.scintilla.MODIFIED)
				or (notification.modification_type & geany.scintilla.MOD_INSERT_TEXT)
				or (notification.modification_type & geany.scintilla.MOD_DELETE_TEXT)):

			self.on_document_activity()

	def preview_update(self, text, file_type, file_name='', file_ext=''):
		mimetype = 'text/html'
		encoding = 'utf-8'
		uri = ''
		headers = ''
		body = None

		# detect other types by extension
		email_ext = ('.eml', '.msg')

		eml = email.message_from_string(text)

		if ((eml._headers != []) or (file_ext in email_ext)):
			headers2 = []
			for header in eml._headers:
				headers2.append(": ".join(header))

			headers = "</br>\n".join(headers2)
			text = eml._payload

			if "html" in eml.get_content_type().lower():
				file_type = "HTML"
			elif "markdown" in eml.get_content_type().lower():
				file_type = "Markdown"
			else:
				file_type = None


		# process text into html
		if file_type == 'Markdown':
			pandoc_type = 'markdown'
			# html = markdown.markdown(text)
		elif file_type == 'HTML':
			body = text
		elif file_type == 'Asciidoc':
			temp_file = make_temp_file(text)
			subprocess_arguments = [ASCIIDOC_PATH, '--backend=html5',
				'-o {}.html'.format(temp_file.name), temp_file.name]
			cmd = ' '.join(subprocess_arguments)

			fin = os.popen(cmd)
			msg = fin.read()
			fin.close()

			if msg:
				geany_message("Asciidoc message: {}".format(msg))

			with open('{}.html'.format(temp_file.name), 'r') as f:
				body = f.read()

			os.remove(temp_file.name)
			os.remove('{}.html'.format(temp_file.name))

		elif file_type == 'reStructuredText':
			pandoc_type = 'rst'
		elif file_type == 'Txt2tags':
			pandoc_type = 't2t'
		elif file_type == 'LaTeX':
			pandoc_type = 'latex'
		elif file_type == 'Docbook':
			pandoc_type = 'docbook'
		else:
			#print file_type
			body = text
			mimetype = 'text/plain'

		# generate html if not already generated above
		if body == None:
			temp_file = make_temp_file(text)
			subprocess_arguments = [PANDOC_PATH, '-f {}'.format(pandoc_type), '-t html',
				temp_file.name, '-o {}.html'.format(temp_file.name)]
			cmd = ' '.join(subprocess_arguments)

			fin = os.popen(cmd)
			msg = fin.read()
			fin.close()

			if msg:
				geany_message("Pandoc message: {}".format(msg))

			with open('{}.html'.format(temp_file.name), 'r') as f:
				body = f.read()

			os.remove(temp_file.name)
			os.remove('{}.html'.format(temp_file.name))

		if headers != '':
			html = '<div style="white-space: nowrap;">' + headers + '</div>' + body
		else:
			html = body

		# display html
		self.web.load_string(html, mimetype, encoding, uri)
		#self.web.load_html_string(content = html, base_uri='')

	def save_scrollbar_position(self):
		self.hadj_save = self.hadj.value
		self.vadj_save = self.vadj.value

	def restore_scrollbar_position(self):
		self.hadj.value = self.hadj_save
		self.vadj.value = self.vadj_save

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

def geany_message(message):
	# future add option for which window to use
	geany.msgwindow.msg_add(message)
	#geany.msgwindow.compiler_add('test')
	#geany.msgwindow.status_add('test')

def make_temp_file(text):
	temp_file = NamedTemporaryFile(mode='w', prefix='geany-preview-', delete=False)
	temp_file.write(text)
	temp_file.close()

	return temp_file
