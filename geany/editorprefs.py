import _geany_editor_prefs


class EditorPrefs(object):

    _editor_prefs = None

    def __init__(self):
        self._editor_prefs = _geany_editor_prefs.EditorPrefs()

    @property
    def indentation(self):
        return self._editor_prefs._get_indentation()

    @property
    def show_white_space(self):
        return self._editor_prefs._get_show_white_space()

    @property
    def show_indent_guide(self):
        return self._editor_prefs._get_show_indent_guide()

    @property
    def show_line_endings(self):
        return self._editor_prefs._get_show_line_endings()

    @property
    def long_line_type(self):
        return self._editor_prefs._get_long_line_type()

    @property
    def long_line_column(self):
        return self._editor_prefs._get_long_line_column()

    @property
    def long_line_color(self):
        return self._editor_prefs._get_long_line_color()

    @property
    def show_markers_margin(self):
        return self._editor_prefs._get_show_markers_margin()

    @property
    def show_linenumber_margin(self):
        return self._editor_prefs._get_show_linenumber_margin()

    @property
    def show_scrollbars(self):
        return self._editor_prefs._get_show_scrollbars()

    @property
    def scroll_stop_at_last_line(self):
        return self._editor_prefs._get_scroll_stop_at_last_line()

    @property
    def line_wrapping(self):
        return self._editor_prefs._get_line_wrapping()

    @property
    def use_indicators(self):
        return self._editor_prefs._get_use_indicators()

    @property
    def folding(self):
        return self._editor_prefs._get_folding()

    @property
    def unfold_all_children(self):
        return self._editor_prefs._get_unfold_all_children()

    @property
    def disable_dnd(self):
        return self._editor_prefs._get_disable_dnd()

    @property
    def use_tab_to_indent(self):
        return self._editor_prefs._get_use_tab_to_indent()

    @property
    def smart_home_key(self):
        return self._editor_prefs._get_smart_home_key()

    @property
    def newline_strip(self):
        return self._editor_prefs._get_newline_strip()

    @property
    def auto_complete_symbols(self):
        return self._editor_prefs._get_auto_complete_symbols()

    @property
    def auto_close_xml_tags(self):
        return self._editor_prefs._get_auto_close_xml_tags()

    @property
    def complete_snippets(self):
        return self._editor_prefs._get_complete_snippets()

    @property
    def symbolcompletion_min_chars(self):
        return self._editor_prefs._get_symbolcompletion_min_chars()

    @property
    def symbolcompletion_max_height(self):
        return self._editor_prefs._get_symbolcompletion_max_height()

    @property
    def brace_match_ltgt(self):
        return self._editor_prefs._get_brace_match_ltgt()

    @property
    def use_gtk_word_boundaries(self):
        return self._editor_prefs._get_use_gtk_word_boundaries()

    @property
    def complete_snippets_whilst_editing(self):
        return self._editor_prefs._get_complete_snippets_whilst_editing()

    @property
    def line_break_column(self):
        return self._editor_prefs._get_line_break_column()

    @property
    def auto_continue_multiline(self):
        return self._editor_prefs._get_auto_continue_multiline()

    @property
    def comment_toggle_mark(self):
        return self._editor_prefs._get_comment_toggle_mark()

    @property
    def autocompletion_max_entries(self):
        return self._editor_prefs._get_autocompletion_max_entries()

    @property
    def autoclose_chars(self):
        return self._editor_prefs._get_autoclose_chars()

    @property
    def autocomplete_doc_words(self):
        return self._editor_prefs._get_autocomplete_doc_words()

    @property
    def completion_drops_rest_of_word(self):
        return self._editor_prefs._get_completion_drops_rest_of_word()

    @property
    def color_scheme(self):
        return self._editor_prefs._get_color_scheme()

    @property
    def show_virtual_space(self):
        return self._editor_prefs._get_show_virtual_space()

    @property
    def long_line_enabled(self):
        return self._editor_prefs._get_long_line_enabled()

    @property
    def autocompletion_update_freq(self):
        return self._editor_prefs._get_autocompletion_update_freq()
