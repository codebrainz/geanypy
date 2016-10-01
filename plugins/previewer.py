# © Copyright 2016  xiota «xiota•mentalfossa•com»
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.

__PLUGIN_NAME__ = 'Mark-Up/Down Previewer'
__PLUGIN_DESC__ = 'Semi-real-time preview of multiple light-weight markup languages.'
__PLUGIN_VERS__ = '20161001.095420'
__PLUGIN_AUTH__ = 'xiota «xiota•mentalfossa•com»'

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

import geany
import gtk, gobject
import webkit, email

import time, subprocess
import os, tempfile
import cgi

from ConfigParser import SafeConfigParser

# Import find-executable engine
try:
	from shutil import which
except ImportError:
	from distutils.spawn import find_executable
	which = find_executable

# Path to the executables
PANDOC_PATH = which('pandoc')
ASCIIDOC_PATH = which('asciidoc')

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class PreviewerPlugin(geany.Plugin):
	__plugin_name__        = __PLUGIN_NAME__
	__plugin_description__ = __PLUGIN_DESC__
	__plugin_version__     = __PLUGIN_VERS__
	__plugin_author__      = __PLUGIN_AUTH__

	# default settings
	_update_interval = '1.0'
	_preview_position = 'sidebar'
	_message_position = 'message_tab'

	def __init__(self):
		geany.Plugin.__init__(self)

		self.hadj = None
		self.vadj = None
		self.hadj_save = 0
		self.vadj_save = 0

		self.load_config()

		# limit update frequency to improve performance
		# future: add option to change
		self.update_time = 0

		self.install_previewer()

		geany.signals.connect('editor-notify', self.on_editor_notify)
		geany.signals.connect('document-open', self.on_document_activity)
		geany.signals.connect('document-new', self.on_document_activity)
		geany.signals.connect('document-save', self.on_document_activity)
		geany.signals.connect('document-activate', self.on_document_activity)
		geany.signals.connect('document-reload', self.on_document_activity)
		geany.signals.connect('document-filetype-set', self.on_document_activity)

		geany.signals.connect('geany-startup-complete', self.on_document_activity)
		geany.signals.connect('document-close', self.on_document_close)

	def cleanup(self):
		self.on_save_config_timeout()
		self.uninstall_previewer()

	def load_config(self):
		self.cfg_path = os.path.join(geany.app.configdir, "plugins", "previewer.conf")
		self.cfg = SafeConfigParser()
		self.cfg.read(self.cfg_path)

	def save_config(self):
		gobject.idle_add(self.on_save_config_timeout)

	def on_save_config_timeout(self, data=None):
		self.cfg.write(open(self.cfg_path, 'w'))
		return False


	def install_previewer(self):
		# load general settings
		self.update_interval = self.update_interval

		# load appearance settings
		self.preview_position = self.preview_position
		self.message_position = self.message_position

		# create preview window
		self.box1 = gtk.VBox()
		self.box2 = gtk.HBox()

		self.addressbar = gtk.Entry()
		self.addressbar.connect('activate', self.on_address_action)

		self.gobutton = gtk.Button('Go')
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

		if self.preview_position == 'sidebar':
			geany.main_widgets.sidebar_notebook.append_page(self.box1, gtk.Label('Preview'))
		else:
			geany.main_widgets.message_window_notebook.append_page(self.box1, gtk.Label('Preview'))

		self.box1.show_all()

		# set scrollbar objects for position save/restore
		self.hadj = self.swin.get_hadjustment()
		self.vadj = self.swin.get_vadjustment()

		# hack to process first document when geany first started
		self.unchanged = True

	def uninstall_previewer(self):
		self.box1.destroy()

	def on_address_action(self, widget):
		address = self.addressbar.get_text()

		if address == '':
			self.on_document_activity()
		else:
			if not (('://' in address) or address.startswith('/')):
				address = 'http://' + address
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

		text = ''
		file_type = ''
		file_name = ''
		file_ext = ''

		if doc != None:
			file_type = doc.file_type.name
			text = doc.editor.scintilla.get_contents()

			if doc.file_name != None:
				file_name, file_ext = os.path.splitext(doc.file_name)

		self.preview_update(text, file_type, file_name, file_ext)

	def on_document_close(self, obj = None, doc = None):
		text = ''
		file_type = ''
		file_name = ''
		file_ext = ''

		self.preview_update(text, file_type, file_name, file_ext)

	def on_editor_notify(self, obj, editor, notification):
		if ( (notification.modification_type & geany.scintilla.MODIFIED)
					or (notification.modification_type & geany.scintilla.MOD_INSERT_TEXT)
					or (notification.modification_type & geany.scintilla.MOD_DELETE_TEXT)
				) and ((time.time() - self.update_time) > float(self.update_interval)):
			self.on_document_activity()
		elif self.unchanged:
			self.unchanged = False
			self.on_document_activity()

	def preview_update(self, text, file_type, file_name='', file_ext=''):
		address = self.addressbar.get_text()
		if address != '':
			self.update_time = time.time()
			return False

		mimetype = 'text/html'
		encoding = 'utf-8'
		uri = ''
		headers = ''
		body = None

		# detect RFC-822-like email messages
		eml = email.message_from_string(text)

		if (eml['from'] != None):
			headers2 = []
			for header in eml._headers:
				headers2.append(': '.join(header))

			headers = "\n".join(headers2)
			headers = '<pre>' + headers + '</pre>'

			if 'html' in eml.get_content_type().lower():
				file_type = 'HTML'
			elif 'markdown' in eml.get_content_type().lower():
				file_type = 'Markdown'
			else:  # plain/text
				file_type = None

			text = eml._payload

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

			with os.popen(cmd) as fin:
				msg = fin.read()

			if msg:
				geany_message("Asciidoc message: {}".format(msg), self.message_position)

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
			#mimetype = 'text/plain'
			body = '<pre>' + cgi.escape(text) + '</pre>'

		# generate html if not already generated above
		if body == None:
			temp_file = make_temp_file(text)
			subprocess_arguments = [PANDOC_PATH, '-f {}'.format(pandoc_type), '-t html',
				temp_file.name, '-o {}.html'.format(temp_file.name)]
			cmd = ' '.join(subprocess_arguments)

			with os.popen(cmd) as fin:
				msg = fin.read()

			if msg:
				geany_message("Pandoc message: {}".format(msg), self.message_position)

			with open('{}.html'.format(temp_file.name), 'r') as f:
				body = f.read()

			os.remove(temp_file.name)
			os.remove('{}.html'.format(temp_file.name))

		content = headers + body

		# display html
		self.web.load_string(content, mimetype, encoding, uri)

		self.update_time = time.time()

	def save_scrollbar_position(self):
		self.hadj_save = self.hadj.value
		self.vadj_save = self.vadj.value

	def restore_scrollbar_position(self):
		self.hadj.value = self.hadj_save
		self.vadj.value = self.vadj_save


	def on_update_interval_changed(self, text_buf, data=None):
		self.update_interval = text_buf.get_text()

	def _get_update_interval(self):
		if self.cfg.has_section('general'):
				if self.cfg.has_option('general', 'update_interval'):
					return self.cfg.get('general', 'update_interval')
		return self._update_interval

	def _set_update_interval(self, update_interval):
		self._update_interval = str(update_interval)
		if not self.cfg.has_section('general'):
			self.cfg.add_section('general')
		self.cfg.set('general', 'update_interval', self._update_interval)
		self.save_config()

	update_interval = property(_get_update_interval, _set_update_interval)


	def on_preview_position_changed(self, widget, data=None):
		print data
		self.preview_position = data

	def _get_preview_position(self):
		if self.cfg.has_section('appearance'):
				if self.cfg.has_option('appearance', 'preview_position'):
					return self.cfg.get('appearance', 'preview_position')
		return self._preview_position

	def _set_preview_position(self, preview_position):
		self._preview_position = preview_position
		if not self.cfg.has_section('appearance'):
			self.cfg.add_section('appearance')
		self.cfg.set('appearance', 'preview_position', self._preview_position)
		self.save_config()

	preview_position = property(_get_preview_position, _set_preview_position)


	def on_message_position_changed(self, widget, data=None):
		self.message_position = data

	def _get_message_position(self):
		if self.cfg.has_section('appearance'):
				if self.cfg.has_option('appearance', 'message_position'):
					return self.cfg.get('appearance', 'message_position')
		return self._message_position

	def _set_message_position(self, message_position):
		self._message_position = message_position
		if not self.cfg.has_section('appearance'):
			self.cfg.add_section('appearance')
		self.cfg.set('appearance', 'message_position', self._message_position)
		self.save_config()

	message_position = property(_get_message_position, _set_message_position)


	def configure(self, dialog):
		vbox1 = gtk.VBox(spacing = 6)
		vbox1.set_border_width(6)

		# General Settings
		lbl = gtk.Label()
		lbl.set_use_markup(True)
		lbl.set_markup("<b>General</b>")

		fra_general = gtk.Frame("")
		fra_general.set_shadow_type(gtk.SHADOW_NONE)
		fra_general.set_label_widget(lbl)

		al_general = gtk.Alignment(0.0, 0.0, 1.0, 1.0)
		al_general.set_padding(0, 0, 12, 0)

		tbl = gtk.Table(1, 2, False)
		tbl.set_row_spacings(6)
		tbl.set_col_spacings(6)
		tbl.set_border_width(6)

		al_general.add(tbl)
		fra_general.add(al_general)

		lbl = gtk.Label("Update Interval (seconds)")
		lbl.set_alignment(0.0, 0.0)

		txt = gtk.Entry()
		txt.connect('changed', self.on_update_interval_changed)
		txt.set_text(self.update_interval)

		tbl.attach(lbl, 0, 1, 0, 1, gtk.FILL | gtk.EXPAND, gtk.FILL, 0, 0)
		tbl.attach(txt, 1, 2, 0, 1, gtk.FILL | gtk.EXPAND, gtk.FILL, 0, 0)

		# Appearance Settings
		lbl = gtk.Label()
		lbl.set_use_markup(True)
		lbl.set_markup("<b>Appearance</b>")

		fra_appearances = gtk.Frame("")
		fra_appearances.set_shadow_type(gtk.SHADOW_NONE)
		fra_appearances.set_label_widget(lbl)

		al_appearances = gtk.Alignment(0.0, 0.0, 1.0, 1.0)
		al_appearances.set_padding(0, 0, 12, 0)

		tbl = gtk.Table(6, 2, False)
		tbl.set_row_spacings(6)
		tbl.set_col_spacings(6)
		tbl.set_border_width(6)

		al_appearances.add(tbl)
		fra_appearances.add(al_appearances)

		lbl = gtk.Label('Preview Position:')
		lbl.set_alignment(0.0, 0.0)

		vbox2 = gtk.VBox()
		button = gtk.RadioButton(None, 'Sidebar')
		button.connect("toggled", self.on_preview_position_changed, 'sidebar')
		vbox2.pack_start(button)

		if self.preview_position == 'sidebar':
			button.set_active(True)
		else:
			button.set_active(False)

		button = gtk.RadioButton(button, 'Message Window')
		button.connect("toggled", self.on_preview_position_changed, 'message_window')
		vbox2.pack_start(button)

		if self.preview_position == 'message_window':
			button.set_active(True)
		else:
			button.set_active(False)

		tbl.attach(lbl, 0, 1, 0, 1, gtk.FILL | gtk.EXPAND, gtk.FILL, 0, 0)
		tbl.attach(vbox2, 1, 2, 0, 1, gtk.FILL | gtk.EXPAND, gtk.FILL, 0, 0)


		lbl = gtk.Label('Output Position:')
		lbl.set_alignment(0.0, 0.0)

		vbox3 = gtk.VBox()
		button = gtk.RadioButton(None, 'Status Tab')
		button.connect("toggled", self.on_message_position_changed, 'status_tab')
		vbox3.pack_start(button)

		if self.message_position == 'status_tab':
			button.set_active(True)
		else:
			button.set_active(False)

		button = gtk.RadioButton(button, 'Compiler Tab')
		button.connect("toggled", self.on_message_position_changed, 'compiler_tab')
		vbox3.pack_start(button)

		if self.message_position == 'compiler_tab':
			button.set_active(True)
		else:
			button.set_active(False)

		button = gtk.RadioButton(button, 'Messages Tab')
		button.connect("toggled", self.on_message_position_changed, 'message_tab')
		vbox3.pack_start(button)

		if self.message_position == 'message_tab':
			button.set_active(True)
		else:
			button.set_active(False)

		button = gtk.RadioButton(button, 'Standard Output')
		button.connect("toggled", self.on_message_position_changed, 'stdout')
		vbox3.pack_start(button)

		if self.message_position == 'stdout':
			button.set_active(True)
		else:
			button.set_active(False)

		tbl.attach(lbl, 0, 1, 1, 2, gtk.FILL | gtk.EXPAND, gtk.FILL, 0, 0)
		tbl.attach(vbox3, 1, 2, 1, 2, gtk.FILL | gtk.EXPAND, gtk.FILL, 0, 0)

		#lbl = gtk.Label('Header:')
		#lbl.set_alignment(0.0, 0.0)
		#txt = gtk.Entry()
		#txt.set_text('not ready yet')

		#tbl.attach(lbl, 0, 1, 2, 3, gtk.FILL | gtk.EXPAND, gtk.FILL, 0, 0)
		#tbl.attach(txt, 1, 2, 2, 3, gtk.FILL | gtk.EXPAND, gtk.FILL, 0, 0)

		#lbl = gtk.Label('Style:')
		#lbl.set_alignment(0.0, 0.0)
		#txt = gtk.Entry()
		#txt.set_text('not ready yet')

		#tbl.attach(lbl, 0, 1, 3, 4, gtk.FILL | gtk.EXPAND, gtk.FILL, 0, 0)
		#tbl.attach(txt, 1, 2, 3, 4, gtk.FILL | gtk.EXPAND, gtk.FILL, 0, 0)

		#lbl = gtk.Label('Footer:')
		#lbl.set_alignment(0.0, 0.0)
		#txt = gtk.Entry()
		#txt.set_text('not ready yet')

		#tbl.attach(lbl, 0, 1, 4, 5, gtk.FILL | gtk.EXPAND, gtk.FILL, 0, 0)
		#tbl.attach(txt, 1, 2, 4, 5, gtk.FILL | gtk.EXPAND, gtk.FILL, 0, 0)

		vbox1.pack_start(fra_general)
		vbox1.pack_start(fra_appearances)

		return vbox1

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

def geany_message(message, position='message_tab'):
	if position == 'message_tab':
		geany.msgwindow.msg_add(message)
	elif position == 'compiler_tab':
		geany.msgwindow.compiler_add(message)
	elif position == 'status_tab':
		geany.msgwindow.status_add(message)
	else:
		print message

def make_temp_file(text):
	temp_file = tempfile.NamedTemporaryFile(mode='w', prefix='geany-preview-', delete=False)
	temp_file.write(text)
	temp_file.close()

	return temp_file
