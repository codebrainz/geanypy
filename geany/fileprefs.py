import _geany_file_prefs


class FilePrefs(object):

    _file_prefs = None

    def __init__(self):
        self._file_prefs = _geany_file_prefs.FilePrefs()

    @property
    def default_new_encoding(self):
        return self._file_prefs._get_default_new_encoding()

    @property
    def default_open_encoding(self):
        return self._file_prefs._get_default_open_encoding()

    @property
    def final_new_line(self):
        return self._file_prefs._get_final_new_line()

    @property
    def strip_trailing_spaces(self):
        return self._file_prefs._get_strip_trailing_spaces()

    @property
    def replace_tabs(self):
        return self._file_prefs._get_replace_tabs()

    @property
    def tab_order_ltr(self):
        return self._file_prefs._get_tab_order_ltr()

    @property
    def tab_order_beside(self):
        return self._file_prefs._get_tab_order_beside()

    @property
    def show_tab_cross(self):
        return self._file_prefs._get_show_tab_cross()

    @property
    def mru_length(self):
        return self._file_prefs._get_mru_length()

    @property
    def default_eol_char(self):
        return self._file_prefs._get_default_eol_char()

    @property
    def disk_check_timeout(self):
        return self._file_prefs._get_disk_check_timeout()

    @property
    def cmdline_new_files(self):
        return self._file_prefs._get_cmdline_new_files()

    @property
    def use_safe_file_saving(self):
        return self._file_prefs._get_use_safe_file_saving()

    @property
    def ensure_convert_new_lines(self):
        return self._file_prefs._get_ensure_convert_new_lines()

    @property
    def gio_unsafe_save_backup(self):
        return self._file_prefs._get_gio_unsafe_save_backup()

    @property
    def use_gio_unsafe_file_saving(self):
        return self._file_prefs._get_use_gio_unsafe_file_saving()
