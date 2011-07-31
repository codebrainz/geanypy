import _geany_indent_prefs


class IndentPrefs(object):

    _indent_prefs = None

    def __init__(self, indent_prefs=None):
        if indent_prefs is not None:
            self._indent_prefs = indent_prefs

    @property
    def width(self):
        return self._indent_prefs._get_width()

    @property
    def indent_type(self, value):
        return self._indent_prefs._get_type()

    @property
    def hard_tab_width(self):
        return self._indent_prefs._get_hard_tab_width()

    @property
    def auto_indent_mode(self):
        return self._indent_prefs._get_auto_indent_mode()

    @property
    def detect_type(self):
        return self._indent_prefs._get_detect_type()

    @property
    def detect_width(self):
        return self._indent_prefs._get_detect_width()
