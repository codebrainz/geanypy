import _geany_scintilla
import _geany_scintilla_notification


FLAG_WHOLEWORD = 2
FLAG_MATCHCASE = 4
FLAG_WORDSTART = 0x00100000
FLAG_REGEXP = 0x00200000
FLAG_POSIX = 0x00400000


UPDATE_CONTENT = 0x01 	#: Contents, styling or markers have been changed.
UPDATE_SELECTION = 0x02 	#: Selection has been changed.
UPDATE_V_SCROLL = 0x04 	#: Scrolled vertically.
UPDATE_H_SCROLL = 0x08 	#: Scrolled horizontally.


MOD_INSERTTEXT = 0x01 	#: Text has been inserted into the document. 	position, length, text, linesAdded
MOD_DELETETEXT = 0x02 	#: Text has been removed from the document. 	position, length, text, linesAdded
MOD_CHANGESTYLE = 0x04 	#: A style change has occurred. 	position, length
MOD_CHANGEFOLD = 0x08 	#: A folding change has occurred. 	line, foldLevelNow, foldLevelPrev
PERFORMED_USER = 0x10 	#: Information: the operation was done by the user. 	None
PERFORMED_UNDO = 0x20 	#: Information: this was the result of an Undo. 	None
PERFORMED_REDO = 0x40 	#: Information: this was the result of a Redo. 	None
MULTISTEPUNDOREDO = 0x80 	#: This is part of a multi-step Undo or Redo transaction. 	None
LASTSTEPINUNDOREDO = 0x100 	#: This is the final step in an Undo or Redo transaction. 	None
MOD_CHANGEMARKER = 0x200 	#: One or more markers has changed in a line. 	line
MOD_BEFOREINSERT = 0x400 	#: Text is about to be inserted into the document.
MOD_BEFOREDELETE = 0x800 	#: Text is about to be deleted from the document. 	position, length
MOD_CHANGEINDICATOR = 0x4000 	#: An indicator has been added or removed from a range of text. 	position, length
MOD_CHANGELINESTATE = 0x8000 	#: A line state has changed because SCI_SETLINESTATE was called. 	line
MOD_LEXERSTATE = 0x80000 	#: The internal state of a lexer has changed over a range. 	position, length
MOD_CHANGEMARGIN = 0x10000 	#: A text margin has changed. 	line
MOD_CHANGEANNOTATION = 0x20000 	#: An annotation has changed. 	line
MULTILINEUNDOREDO = 0x1000 	#: This is part of an Undo or Redo with multi-line changes. 	None
STARTACTION = 0x2000 	#: This is set on a PERFORMED_USER action when it is the first or only step in an undo transaction. This can be used to integrate the Scintilla undo stack with an undo stack in the container application by adding a Scintilla action to the container's stack for the currently opened container transaction or to open a new container transaction if there is no open container transaction. 	None
MOD_CONTAINER = 0x40000 	#: This is set on for actions that the container stored into the undo stack with SCI_ADDUNDOACTION. 	token
MODEVENTMASKALL = 0x7FFFF 	#: This is a mask for all valid flags. This is the default mask state set by SCI_SETMODEVENTMASK.


STYLENEEDED = 2000
CHARADDED = 2001
SAVEPOINTREACHED = 2002
SAVEPOINTLEFT = 2003
MODIFYATTEMPTRO = 2004
KEY = 2005
DOUBLECLICK = 2006
UPDATEUI = 2007
MODIFIED = 2008
MACRORECORD = 2009
MARGINCLICK = 2010
NEEDSHOWN = 2011
PAINTED = 2013
USERLISTSELECTION = 2014
URIDROPPED = 2015
DWELLSTART = 2016
DWELLEND = 2017
ZOOM = 2018
HOTSPOTCLICK = 2019
HOTSPOTDOUBLECLICK = 2020
CALLTIPCLICK = 2021
AUTOCSELECTION = 2022
INDICATORCLICK = 2023
INDICATORRELEASE = 2024
AUTOCCANCELLED = 2025
AUTOCCHARDELETED = 2026
HOTSPOTRELEASECLICK = 2027


class NotifyHeader(object):

    _notif = None

    def __init__(self, notif=None):
        if notif is not None:
            self._notif = notif

    @property
    def hwnd_from(self):
        return self._notif._get_header_hwnd_from()

    @property
    def id_from(self):
        return self._notif._get_header_id_from()

    @property
    def code(self):
        return self._notif._get_header_code()


class Notification(object):

    _notif = None
    def __init__(self, notif=None):
        if notif is not None:
            self._notif = notif

    @property
    def nmhdr(self):
        return NotifyHeader(self._notif)

    @property
    def position(self):
        return self._notif._get_position()

    @property
    def ch(self):
        return self._notif._get_ch()

    @property
    def modifiers(self):
        return self._notif._get_modifiers()

    @property
    def modification_type(self):
        return self._notif._get_modification_type()

    @property
    def text(self):
        return self._notif._get_text()

    @property
    def length(self):
        return self._notif._get_length()

    @property
    def lines_added(self):
        return self._notif._get_lines_added()

    @property
    def message(self):
        return self._notif._get_message()

    @property
    def w_param(self):
        return self._notif._get_w_param()

    @property
    def l_param(self):
        return self._notif._get_l_param()

    @property
    def line(self):
        return self._notif._get_line()

    @property
    def fold_level_now(self):
        return self._notif._get_fold_level_now()

    @property
    def fold_level_prev(self):
        return self._notif._get_fold_level_prev()

    @property
    def margin(self):
        return self._notif._get_margin()

    @property
    def list_type(self):
        return self._notif._get_list_type()

    @property
    def x(self):
        return self._notif._get_x()

    @property
    def y(self):
        return self._notif._get_y()

    @property
    def token(self):
        return self._notif._get_token()

    @property
    def annotation_lines_added(self):
        return self._notif._get_annotation_lines_added()

    @property
    def updated(self):
        return self._notif._get_updated()


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
