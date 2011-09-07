import _document as c_document
import filetypes
import editor


def find_by_filename(filename):
	return c_document.find_by_filename(filename)

def find_by_real_path(realname):
	_doc = c_document.find_by_real_path(filename)
	if _doc is not None:
		return Document(_doc)

def get_current():
	_doc = c_document.get_current()
	if _doc is not None:
		return Document(_doc)

def get_from_page(page_num):
	_doc = c_document.get_from_page(page_num)
	if _doc is not None:
		return Document(_doc)

def get_from_index(index):
	_doc = c_document.get_from_index(index)
	if _doc is not None:
		return Document(_doc)

def new(filename=None, filetype=None, text=None):
	if filetype is not None:
		filetype = filetype._ft
	_doc = c_document.new_file(filename, filetype, text)
	if _doc is not None:
		return Document(_doc)

def open(filename, read_only=False, filetype=None, forced_enc=None):
	if filetype is not None:
		filetype = filetype._ft
	_doc = c_document.open_file(filename, read_only, filetype, forced_enc)
	if _doc is not None:
		return Document(_doc)

def open_files(filenames, read_only=False, filetype="", forced_enc=""):
	if filetype is not None:
		filetype = filetype._ft
	for filename in filenames:
		Document.open_file(filename, read_only, filetype, forced_enc)

def remove_page(page_num):
	return c_document.remove_page(page_num)

def get_documents_list():
	doc_list = []
	for _doc in c_document.get_documents_list():
		doc_list.append(Document(_doc))
	return doc_list

class Document(object):

	_doc = None

	def __init__(self, doc=None):
		self._doc = doc

	@property
	def basename_for_display(self):
		return self._doc.get_basename_for_display()

	@property
	def notebook_page(self):
		return self._doc.get_notebook_page()

	@property
	def status_color(self):
		return self._doc.get_status_color()

	@property
	def encoding(self):
		return self._doc.get_encoding()

	@encoding.setter
	def encoding(self, value):
		self._doc.set_encoding(value)

	@property
	def file_type(self):
		_ft = self._doc.get_file_type()
		if _ft is not None:
			ft = filetype.Filetype()
			ft._ft = _ft
			return ft

	@file_type.setter
	def file_type(self, value):
		self._doc.set_file_type(value._ft)

	@property
	def text_changed(self):
		return self._doc.get_text_changed()

	@text_changed.setter
	def text_changed(self, value):
		self._doc.set_text_changed(value)

	@property
	def file_name(self):
		return self.doc._get_file_name()

	@property
	def has_bom(self):
		return self._doc.get_has_bom()

	@property
	def has_tags(self):
		return self._doc.get_has_tags()

	@property
	def index(self):
		return self._doc.get_index()

	@property
	def is_valid(self):
		return self._doc.get_is_valid()

	@property
	def read_only(self):
		return self._doc.get_read_only()

	@property
	def real_path(self):
		return self._doc.get_real_path()

	@property
	def editor(self):
		_ed = self._doc.get_editor()
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
