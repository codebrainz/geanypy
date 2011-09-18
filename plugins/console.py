import os
from ConfigParser import SafeConfigParser
import geany
import geany.console
import gobject
import gtk
import gtk.gdk as gdk
import pango


WIDGET_STATES = [ gtk.STATE_NORMAL, gtk.STATE_ACTIVE, gtk.STATE_PRELIGHT,
	gtk.STATE_SELECTED, gtk.STATE_INSENSITIVE ]


class ConsolePlugin(geany.Plugin):

	__plugin_name__ = "Python Console"
	__plugin_description__ = ("Installs a Python console that allows you " +
		"to use the Geany API.")
	__plugin_version__ = "0.01"
	__plugin_author__ = "Matthew Brush <mbrush@codebrainz.ca>"

	_font = "Monospace 9"
	_bg = "#000000"
	_fg = "#FFFFFF"
	_banner = ("Geany Python Console\n You can access the Geany Python " +
				"API by importing the `geany' module.\n")
	_use_rl_completer = True

	def __init__(self):
		geany.Plugin.__init__(self)
		self.load_config()
		self.install_console()


	def cleanup(self):
		#self.save_config()
		self.on_save_config_timeout() # do it now
		self.uninstall_console()


	def load_config(self):
		self.cfg_path = os.path.join(geany.app.configdir, "plugins", "pyconsole.conf")
		self.cfg = SafeConfigParser()
		self.cfg.read(self.cfg_path)


	def save_config(self):
		gobject.idle_add(self.on_save_config_timeout)


	def on_save_config_timeout(self, data=None):
		self.cfg.write(open(self.cfg_path, 'w'))


	def install_console(self):

		# load general settings
		self.banner = self.banner
		self.use_rl_completer = self.use_rl_completer

		self.swin = gtk.ScrolledWindow()
		self.swin.set_policy(gtk.POLICY_AUTOMATIC, gtk.POLICY_ALWAYS)
		self.console = geany.console.Console(banner = self.banner,
							use_rlcompleter = self.use_rl_completer)

		# apply appearance settings
		self.font = self.font
		self.bg = self.bg
		self.fg = self.fg

		self.swin.add(self.console)
		geany.main_widgets.message_window_notebook.append_page(self.swin,
			gtk.Label("Python"))

		self.swin.show_all()
		self.save_config()

	def uninstall_console(self):
		self.swin.destroy()


	def _get_font(self):
		if self.cfg.has_section('appearances'):
			if self.cfg.has_option('appearances', 'font'):
				return self.cfg.get('appearances', 'font')
		return self._font
	def _set_font(self, font):
		self._font = font
		font_desc = pango.FontDescription(font)
		self.console.modify_font(font_desc)
		if not self.cfg.has_section('appearances'):
			self.cfg.add_section('appearances')
		self.cfg.set('appearances', 'font', self._font)
		self.save_config()
	font = property(_get_font, _set_font)


	def _get_bg(self):
		if self.cfg.has_section('appearances'):
			if self.cfg.has_option('appearances', 'bg_color'):
				return self.cfg.get('appearances', 'bg_color')
		return self._bg
	def _set_bg(self, bg):
		self._bg = bg
		color = gdk.color_parse(self._bg)
		for state in WIDGET_STATES:
			self.console.modify_bg(state, color)
		if not self.cfg.has_section('appearances'):
			self.cfg.add_section('appearances')
		self.cfg.set('appearances', 'bg_color', self._bg)
		self.save_config()
	bg = property(_get_bg, _set_bg)


	def _get_fg(self):
		if self.cfg.has_section('appearances'):
			if self.cfg.has_option('appearances', 'fg_color'):
				return self.cfg.get('appearances', 'fg_color')
		return self._fg
	def _set_fg(self, fg):
		self._fg = fg
		color = gdk.color_parse(self._fg)
		for state in WIDGET_STATES:
			self.console.modify_fg(state, color)
		if not self.cfg.has_section('appearances'):
			self.cfg.add_section('appearances')
		self.cfg.set('appearances', 'fg_color', self._fg)
		self.save_config()
	fg = property(_get_fg, _set_fg)


	def _get_banner(self):
		if self.cfg.has_section('general'):
				if self.cfg.has_option('general', 'banner'):
					return self.cfg.get('general', 'banner')
		return self._banner
	def _set_banner(self, banner):
		self._banner = banner
		if not self.cfg.has_section('general'):
			self.cfg.add_section('general')
		self.cfg.set('general', 'banner', self._banner)
		self.save_config()
	banner = property(_get_banner, _set_banner)


	def _get_use_rl_completer(self):
		if self.cfg.has_section('general'):
			if self.cfg.has_option('general', 'use_rl_completer'):
				return self.cfg.getboolean('general', 'use_rl_completer')
		return self._use_rl_completer
	def _set_use_rl_completer(self, use_rl_completer):
		self._use_rl_completer = use_rl_completer
		if not self.cfg.has_section('general'):
			self.cfg.add_section('general')
		self.cfg.set('general', 'use_rl_completer',
			str(self._use_rl_completer).lower())
		self.save_config()
	use_rl_completer = property(_get_use_rl_completer, _set_use_rl_completer)














