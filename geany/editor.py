import _geany_editor
import indentprefs
import editorprefs
import scintilla


INDENT_TYPE_SPACES = 0
INDENT_TYPE_TABS = 1
INDENT_TYPE_BOTH = 2

AUTOINDENT_NONE = 0
AUTOINDENT_BASIC = 1
AUTOINDENT_CURRENT_CHARS = 2
AUTOINDENT_MATCHBRACES = 3

VIRTUAL_SPACE_DISABLED = 0
VIRTUAL_SPACE_SELECTION = 1
VIRTUAL_SPACE_ALWAYS = 3

AUTOCLOSE_PARENTHESIS = 1
AUTOCLOSE_CBRACKET = 2
AUTOCLOSE_SBRACKET = 4
AUTOCLOSE_SQUOTE = 8
AUTOCLOSE_DQUOTE = 16

INDICATOR_ERROR = 0
INDICATOR_SEARCH = 8

WORDCHARS = "_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"


def find_snippet(name):
    _geany_editor.find_snippet(name)

def get_default_eol_char():
    _geany_editor.get_default_eol_char()

def get_default_eol_char_len():
    _geany_editor.get_default_eol_char_len()

def get_default_eol_char_mode():
    _geany_editor.get_default_eol_char_mode()

def get_default_eol_char_name():
    _geany_editor.get_default_eol_char_name()

def get_default_indent_prefs():
    _ip = _geany_editor.get_default_indent_prefs()
    if _ip is not None:
        ip = indentprefs.IndentPrefs()
        ip._indent_prefs = _ip
        return ip

def get_editor_prefs():
    _ep = editorprefs.EditorPrefs()


class Editor(object):

    _editor = None

    def __init__(self, editor=None):
        if editor is not None:
            self._editor = editor

    @property
    def eol_char(self):
        return self._editor._get_eol_char()

    @property
    def eol_char_len(self):
        return self._editor._get_eol_char_len()

    @property
    def eol_char_mode(self):
        return self._editor._get_eol_char_mode()

    @property
    def eol_char_name(self):
        return self._editor._get_eol_char_name()

    @property
    def indent_prefs(self):
        _ip = self._editor._get_indent_prefs()
        if _ip is not None:
            ip = indentprefs.IndentPrefs()
            ip._indent_prefs = _ip
            return ip

    @property
    def indent_type(self):
        return self.indent_prefs.indent_type

    @indent_type.setter
    def indent_type(self, value):
        self._editor._set_indent_type(value)

    @property
    def sci(self):
        _sci = self._editor._get_scintilla()
        if _sci is not None:
            sci = scintilla.Scintilla()
            sci._sci = _sci
            return sci

    def create_widget(self):
        return self._editor.create_widget()

    def find_snippet(self, name):
        return self._editor.find_snippet(name)

    def get_word_at_position(self, pos=-1, wordchars=None):
        return self._editor.get_word_at_position(pos, wordchars)

    def goto_position(self, pos, mark=False):
        return self._editor.goto_pos(pos, mark)

    def indicator_clear(self, indic):
        self._editor.indicator_clear(indic)

    def indicator_set_on_line(self, indic, line):
        self._editor.indicator_set_on_line(indic, line)

    def indicator_set_on_range(self, indic, start, end):
        self._editor.indicator_set_on_range(indic, start, end)

    def insert_snippet(self, pos, snippet):
        self._editor.insert_snippet(pos, snippet)

    def insert_text_block(self, text, insert_pos, cursor_index=-1, newline_indent_size=-1, replace_newlines=False):
        self._editor.insert_text_block(text, insert_pos, cursor_index, newline_indent_size, replace_newlines)
