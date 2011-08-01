import _geany_scintilla


FLAG_WHOLEWORD = 2
FLAG_MATCHCASE = 4
FLAG_WORDSTART = 0x00100000
FLAG_REGEXP = 0x00200000
FLAG_POSIX = 0x00400000


class Scintilla(object):

    _sci = None

    def __init__(self, scintilla=None):
        if scintilla is not None:
            self._sci = scintilla

    def delete_marker_at_line(self, line, marker):
        return self._sci.delete_marker_at_line(line, marker)

    def end_undo_action(self):
        return self._sci.end_undo_action()

    def ensure_line_is_visible(self, line=-1):
        return self._sci.ensure_line_is_visible(line)

    def find_matching_brace(self, pos=-1):
        return self._sci.find_matching_brace(pos)

    def find_text(self, text, start=-1, end=-1, flags=0):
        return self._sci.find_text(text, flags, start, end)

    def get_char_at(self, pos):
        return self._sci.get_char_at(pos)

    def get_col_from_position(self, pos):
        return self._sci.get_col_from_position(pos)

    def get_contents(self, length=-1):
        return self._sci.get_contents(length)

    def get_contents_range(self, start=-1, end=-1):
        return self._sci.get_contents_range(start, end)

    def get_current_line(self):
        return self._sci.get_current_line()

    def get_current_position(self):
        return self._sci.get_current_position()

    def get_length(self):
        return self._sci.get_length()

    def get_line(self, line=-1):
        return self._sci.get_line(line)

    def get_line_count(self):
        return self._sci.get_line_count()

    def get_line_end_position(self, line=-1):
        return self._sci.get_line_end_position(line)

    def get_line_from_position(self, pos=-1):
        return self._sci.get_line_from_position(pos)

    def get_line_indentation(self, line=-1):
        return self._sci.get_line_indentation(line)

    def get_line_is_visible(self, line=-1):
        return self._sci.get_line_is_visible(line)

    def get_line_length(self, line=-1):
        return self._sci.get_line_length(line)

    def get_position_from_line(self, line=-1):
        return self._sci.get_position_from_line(line)

    def get_selected_text_length(self):
        return self._sci.get_selected_text_length()

    def get_selection_contents(self):
        return self._sci.get_selection_contents()

    def get_selection_end(self):
        return self._sci.get_selection_end()

    def get_selection_mode(self):
        return self._sci.get_selection_mode()

    def get_selection_start(self):
        return self._sci.get_selection_start()

    def get_style_at(self, pos=-1):
        return self._sci.get_style_at(pos)

    def get_tab_width(self):
        return self._sci.get_tab_width()

    def goto_line(self, line):
        return self._sci.goto_line(line)

    def has_selection(self):
        return self._sci.has_selection()

    def indicator_clear(self, pos, length):
        return self._sci.indicator_clear(pos, length)

    def indicator_set(self, indic):
        return self._sci.indicator_set(indic)

    def insert_text(self, text, pos=-1):
        return self._sci.insert_text(text, pos)

    def is_marker_set_at_line(self, line, marker):
        return self._sci.is_marker_set_at_line(line, marker)

    def replace_sel(self, text):
        return self._sci.replace_sel(text)

    def scroll_caret(self):
        return self._sci.scroll_caret()

    def send_command(self, cmd):
        return self._sci.send_command(cmd)

    def set_current_position(self, position, scroll_caret=True):
        return self._sci.set_current_position(position, scroll_caret)

    def set_font(self, style, font, size):
        return self._sci.set_font(style, font, size)

    def set_line_indentation(self, line, indent):
        return self._sci.set_line_indentation(line, indent)

    def set_marker_at_line(self, line, marker):
        return self._sci.set_marker_at_line(line, marker)

    def set_selection_end(self, pos):
        return self._sci.set_selection_end(pos)

    def set_selection_mode(self, mode):
        return self._sci.set_selection_mode(mode)

    def set_selection_start(self, pos):
        return self._sci.set_selection_start(pos)

    def set_text(self, text):
        return self._sci.set_text(text)

    def start_undo_action(self):
        return self._sci.start_undo_action()

    def send_message(self, msg, uptr, sptr):
        return self._sci.send_message(msg, uptr, sptr)
