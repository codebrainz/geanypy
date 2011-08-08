import _geany_document
import filetypes
import editor

def compare_by_display_name(doc_a, doc_b):
    return _geany_document.compare_by_display_name(doc_a, doc_b)

def compare_by_tab_order(doc_a, doc_b, reverse=False):
    return _geany_document.compare_by_tab_order(doc_a, doc_b, reverse)

def find_by_filename(filename):
    return _geany_document.find_by_filename(filename)

def find_by_real_path(realname):
    _doc = _geany_document.find_by_real_path(filename)
    if _doc is not None:
        doc = Document()
        doc._doc = _doc
        return doc

def get_current():
    _doc = _geany_document.get_current()
    if _doc is not None:
        doc = Document()
        doc._doc = _doc
        return doc

def get_from_page(page_num):
    _doc = _geany_document.get_from_page(page_num)
    if _doc is not None:
        doc = Document()
        doc._doc = _doc
        return doc

def get_from_index(index):
    _doc = _geany_document.index(index)
    if _doc is not None:
        doc = Document()
        doc._doc = _doc
        return doc

def new(filename=None, filetype=None, text=None):
    if filetype is not None:
        filetype = filetype._ft
    _doc = _geany_document.new_file(filename, filetype, text)
    if _doc is not None:
        doc = Document()
        doc._doc = _doc
        return doc

def open(filename, read_only=False, filetype=None, forced_enc=None):
    if filetype is not None:
        filetype = filetype._ft
    _doc = _geany_document.open_file(filename, read_only, filetype, forced_enc)
    if _doc is not None:
        doc = Document()
        doc._doc = _doc
        return doc

def open_files(filenames, read_only=False, filetype="", forced_enc=""):
    if filetype is not None:
        filetype = filetype._ft
    for filename in filenames:
        Document.open_file(filename, read_only, filetype, forced_enc)

def remove_page(page_num):
    return _geany_document.remove_page(page_num)

def get_documents_list():
    doc_list = []
    for _doc in _geany_document.get_documents_list():
        doc = Document()
        doc._doc = _doc
        doc_list.append(doc)
    return doc_list

class Document(object):

    _doc = None

    def __init__(self, filename=None):
        if filename is not None:
            doc = new(filename)
            if doc is not None:
                self._doc = doc

    @property
    def basename_for_display(self):
        return self._doc._get_basename_for_display()

    @property
    def notebook_page(self):
        return self._doc._get_notebook_page()

    @property
    def status_color(self):
        return self._doc._get_status_color()

    @property
    def encoding(self):
        return self._doc._get_encoding()

    @encoding.setter
    def encoding(self, value):
        self._doc._set_encoding(value)

    @property
    def file_type(self):
        _ft = self._doc._get_file_type()
        if _ft is not None:
            ft = filetype.Filetype()
            ft._ft = _ft
            return ft

    @file_type.setter
    def file_type(self, value):
        self._doc._set_file_type(value._ft)

    @property
    def text_changed(self):
        return self._doc._get_text_changed()

    @text_changed.setter
    def text_changed(self, value):
        self._doc._set_text_changed(value)

    @property
    def file_name(self):
        return self._doc._get_file_name()

    @property
    def has_bom(self):
        return self._doc._get_has_bom()

    @property
    def has_tags(self):
        return self._doc._get_has_tags()

    @property
    def index(self):
        return self._doc._get_index()

    @property
    def is_valid(self):
        return self._doc._get_is_valid()

    @property
    def read_only(self):
        return self._doc._get_read_only()

    @property
    def real_path(self):
        return self._doc._get_real_path()

    @property
    def editor(self):
        _ed = self._doc._get_editor()
        if _ed is not None:
            ed = editor.Editor()
            ed._editor = _ed
            return ed

    def close(self):
        return self._doc.close()

    def reload(self, forced_enc=None):
        return self._doc.reload_file(forced_enc)

    def rename(self, new_filename):
        return self._doc.rename_file(new_filename)

    def save(self, force=False):
        return self._doc.save_file(force)

    def save_as(self, new_filename):
        return self._doc.save_file_as(new_filename)
