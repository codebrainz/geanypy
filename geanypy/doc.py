import re
import geany

class Document(object):
    """
    This is a wrapper around geany.document.Document and its
    geany.editor.Editor and its geany.scintilla.Scintilla all combined
    into one object for convenience.
    """

    def __init__(self, doc_):
        self.doc_ = doc_
        if not self.doc_.is_valid:
            raise ArgumentError("Document wrapper requires a valid document")

    @property
    def notebook_page(self):
        " Returns the index of the document in the document notebook. "
        return self.doc_.notebook_page

    @property
    def display_name(self):
        " Returns a display-friendly name of the document. "
        return self.doc_.basename_for_display

    @property
    def filename(self):
        " Returns the actual filename of the document. "
        return self.doc_.file_name

    @property
    def filetype(self):
        " Returns the display name of the document's filetype. "
        return self.doc_.file_type.display_name

    @property
    def read_only(self):
        " Returns whether the document is read-only (locked for editing). "
        return self.doc_.read_only

    @property
    def is_dirty(self):
        " Returns whether the file has unsaved changes. "
        return self.doc_.text_changed

    @property
    def eol_char(self):
        " Returns the character(s) used to delimit lines in the document. "
        return self.doc_.editor.eol_char

    @property
    def scroll_percent(self):
        """ Returns the percentage of the total document that the view
            is scrolled to. """
        return self.doc_.editor.scroll_percent

    @scroll_percent.setter
    def scroll_percent(self, percent):
        """ Sets the percentage of the total document that the view is
            scrolled to. """
        self.doc_.editor.scroll_percent = percent

    @property
    def text(self):
        " Returns a copy of the document's text. "
        return self.sci.get_contents()

    @text.setter
    def text(self, txt=''):
        " Sets the document's text to a copy of txt. "
        self.sci.set_text(txt)

    @property
    def lines(self):
        " Returns the document's text, split into lines. "
        return self.text().split('\n')

    @property
    def num_lines(self):
        " Returns the total number of lines in the document. "
        return self.sci.get_line_count()

    @property
    def current_line(self):
        " Returns the line in the document that the caret is on. "
        return self.sci.get_current_line()

    @current_line.setter
    def current_line(self, line_num):
        " Places the caret at the start of the given line. "
        if line_num < 0 or line_num >= self.num_lines:
            raise ArgumentError("The given line number is out of range")
        self.sci.goto_line(line_num)

    @property
    def current_pos(self):
        " Returns the current position of the caret in the document. "
        return self.sci.get_current_position()

    @current_pos.setter
    def current_pos(self, pos):
        " Moves the caret to the requested position. "
        return self.sci.set_current_position(pos)

    @property
    def sci(self):
        " Returns the geany.scintilla.Scintilla object for the document. "
        return self.doc_.editor.scintilla

    @property
    def widget(self):
        " Returns the GtkWidget (ScintillaObject) for the document. "
        return self.doc_.editor.scintilla.widget

    @property
    def has_selection(self):
        " Returns whether there's any selection in the document. "
        return self.sci.has_selection()

    @property
    def selected_text(self):
        """ Returns the currently selected text or an empty string if
            there is no selection active. """
        if self.sci.get_selected_text_length() == 0:
            return ""
        return self.sci.get_selection_contents()

    @property
    def selection_start(self):
        """ Returns the start position of the selection or -1 if
            there's no selection active. """
        if self.sci.get_selected_text_length() == 0:
            return -1
        return self.sci.get_selection_start()

    @selection_start.setter
    def selection_start(self, pos):
        " Sets the starting position of the selection. "
        self.selection_range = (pos, self.selection_end)

    @property
    def selection_end(self):
        """ Returns the end position of the selection or -1 if
            there's no selection active. """
        if self.sci.get_selected_text_length() == 0:
            return -1
        return self.sci.get_selection_end()

    @selection_end.setter
    def selection_end(self, pos):
        " Sets the ending position of the selection. "
        self.selection_rage = (self.selection_start, pos)

    @property
    def selection_range(self):
        """ Returns a tuple containing the start and end of the
            selection or (-1, -1) if there's no selection. """
        return (self.selection_start, self.selection_end)

    @selection_range.setter
    def selection_range(self, range):
        """ Sets the starting and ending position of the selection from
            the given tuple range. """
        if len(range) < 2:
            raise ArgumentError("Range argument must contain two elements")
        if range[0] < 0 or range[0] >= len(self):
            raise IndexError("Selection start position is outside of document bounds")
        elif range[1] < 0 or range[1] >= len(self):
            raise IndexError("Selection end position is outside of document bounds")
        self.sci.set_selection_start(range[0])
        self.sci.set_selection_end(range[1])

    @property
    def selection_length(self):
        " Returns the number of characters selected. "
        return self.selection_end - self.selection_start

    def clear(self):
        " Erases all of the document's text. "
        self.text = ''

    def word_at_pos(self, pos):
        " Returns the word at the given position. "
        return self.doc_.editor.get_word_at_position(pos)

    def begin_undo_action(self):
        " Start grouping undo actions. "
        self.sci.begin_undo_action()

    def end_undo_action(self):
        " Stop grouping undo actions. "
        self.sci.end_undo_action()

    def show_line(self, line_num):
        """ Ensures the given line is visible, that is, scrolled to and
            and unfolded (if it was folded). """
        self.sci.ensure_line_is_visible(line_num)

    def is_line_visible(self, line_num):
        """ Returns whether or not the given line is visible, for example
            it may have been hidden by folding. """
        return self.sci.get_line_is_visible()

    def nth_line(self, n):
        " Returns the text of the given line number. "
        return self.sci.get_line(n)

    def line_from_pos(self, pos):
        " Returns the line number of the given position. "
        return self.sci.get_line_from_position(pos)

    def pos_from_line(self, line_num):
        " Returns the position of the start of the given line. "
        return self.sci.get_position_from_line(line_num)

    def col_from_pos(self, pos):
        """ Returns the column number (starting at 0) of the given
            position on its line. """
        return self.sci.get_col_from_position(pos)

    def clear_selection(self):
        " Remove any selections in the document. "
        self.sci.set_selection_start(0)
        self.sci.set_selection_end(0)

    def select_text(self, start, end):
        " Selects from the given start position to the given end position. "
        self.selection_range = (start, end)

    def replace_selection(self, text):
        " Replaces the current selection (if any) with the given text. "
        self.sci.replace_sel(text)

    def erase_selection(self, text):
        " Erases the current selection (if any). "
        self.replace_selection('')

    def finditer(self, pattern, flags=0):
        """ Return an iterator of re match objects for any matches found
            for the given pattern in the document's text. """
        return re.finditer(pattern, self.text, flags)

    def sub(self, pattern, replacement, flags=0):
        """ Substitute any occurrences of the re pattern in the document's
            text with replacement. """
        return re.sub(pattern, replacement, self.text, flags)

    def save(self, filename=None):
        " Save the file with it's current filename or a new one if given. "
        if filename is None:
            self.doc_.save_file()
        else:
            self.doc_.save_file_as(filename)

    def reload(self):
        " Reload the document's text from disk. "
        self.doc_.reload()

    def close(self):
        " Close the document. "
        geany.document.remove_page(self.doc_.notebook_page)

    def __str__(self):
        " Returns the document's text. "
        return self.text

    def __len__(self):
        return self.sci.get_length()

    def __getitem__(self, key):
        if isinstance(key, slice):
            indices = key.indices(len(self))
            return "".join(self[i] for i in range(*indices))
        elif key >= len(self):
            raise IndexError("Character position is outside of document bounds")
        elif key < 0:
            key = len(self) + key
        return self.sci.get_char_at(key)

    # TODO: support slices
    def __setitem__(self, key, value):
        if key >= len(self):
            raise IndexError("Character position is outside of document bounds")
        elif key < 0:
            key = len(self) + key
        if len(value) < 1:
            raise ArgumentError("Character to set must be a 1 character string")
        self.sci.set_char_at(key, value[0])

    def __delitem__(self, key):
        if isinstance(key, slice):
            indices = key.indices(len(self))
            for index in indices:
                del self[index]
        if key >= len(self):
            raise IndexError("Character position is outside of document bounds")
        elif key < 0:
            key = len(self) + key
        self.sci.del_char_at(key)

    @staticmethod
    def current():
        " Returns the current/active document. "
        return Document(geany.document.get_current())

    @staticmethod
    def list_documents():
        " Returns a list of documents, in no particular order. "
        raw_docs = geany.document.get_documents_list()
        return list( Document(d) for d in raw_docs )

    @staticmethod
    def open(filename=None):
        " Opens a new file or one with the supplied filename, if given. "
        if filename is None:
            return Document(geany.document.new_file())
        else:
            return Document(geany.document.open_file(filename))

    @staticmethod
    def new():
        return Document.open()
