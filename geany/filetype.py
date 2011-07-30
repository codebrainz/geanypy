import _geany_filetype


def detect_from_file(filename):
    _ft = _geany_filetype.detect_from_file(filename)
    if _ft is not None:
        ft = Filetype()
        ft._ft = _ft
        return ft

def get_from_index(index):
    _ft = _geany_filetype.index(index)
    if _ft is not None:
        ft = Filetype()
        ft._ft = _ft
        return ft

def iter_filetypes():
    lst = _geany_filetype.get_filetypes_by_name();
    if lst is None:
        return
    for itm in lst:
        if itm is not None:
            ft = Filetype()
            ft._ft = itm
            yield ft

def lookup_by_name(filetype_name):
    _ft = _geany_filetype.lookup_by_name(filetype_name)
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
