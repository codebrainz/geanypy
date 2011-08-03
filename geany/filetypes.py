import _geany_filetypes

NONE = 0,
PHP = 1
BASIC = 2
MATLAB = 3
RUBY = 4
LUA = 5
FERITE = 6
YAML = 7
C = 8
NSIS = 9
GLSL = 10
PO = 11
MAKE = 12
TCL = 13
XML = 14
CSS = 15
REST = 16
HASKELL = 17
JAVA = 18
CAML = 19
AS = 20
R = 21
DIFF = 22
HTML = 23
PYTHON = 24
CS = 25
PERL = 26
VALA = 27
PASCAL = 28
LATEX = 29
ASM = 30
CONF = 31
HAXE = 32
CPP = 33
SH = 34
FORTRAN = 35
SQL = 36
F77 = 37
DOCBOOK = 38
D = 39
JS = 40
VHDL = 41
ADA = 42
CMAKE = 43
MARKDOWN = 44
TXT2TAGS = 45
ABC = 46
VERILOG = 47
FORTH = 48
LISP = 49
ERLANG = 50
COBOL = 51


def detect_from_file(filename):
    _ft = _geany_filetypes.detect_from_file(filename)
    if _ft is not None:
        ft = Filetype()
        ft._ft = _ft
        return ft

def get_from_index(index):
    _ft = _geany_filetypes.index(index)
    if _ft is not None:
        ft = Filetype()
        ft._ft = _ft
        return ft

def iter_filetypes():
    lst = _geany_filetypes.get_filetypes_by_name()
    print "List: %s" % lst
    if lst is None:
        return
    for itm in lst:
        if itm is not None:
            ft = Filetype()
            ft._ft = itm
            yield ft

def lookup_by_name(filetype_name):
    _ft = _geany_filetypes.lookup_by_name(filetype_name)
    if _ft is not None:
        ft = Filetype()
        ft._ft = _ft
        return ft


class Filetype(object):

    _ft = None

    def __init__(self, filetype_name=None):
        if filetype_name is not None:
            ft = lookup_by_name(filetype_name)
            if ft is not None:
                self._ft = ft

    @property
    def display_name(self):
        return self._ft._get_display_name()

    @property
    def extension(self):
        return self._ft._get_extension()

    @property
    def index(self):
        return self._ft._get_index()

    @property
    def lang_id(self):
        return self._ft._get_lang_id()

    @property
    def name(self):
        return self._ft._get_name()

    @property
    def patterns(self):
        return self._ft._get_patterns()

    @property
    def title(self):
        return self._ft._get_title()
