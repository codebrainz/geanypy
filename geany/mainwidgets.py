import _geany_main_widgets


class MainWidgets(object):

    _main_widgets = None

    def __init__(self):
        self._main_widgets = _geany_main_widgets.MainWidgets()

    @property
    def editor_menu(self):
        return self._main_widgets._get_editor_menu()

    @property
    def message_window_notebook(self):
        return self._main_widgets._get_message_window_notebook()

    @property
    def notebook(self):
        return self._main_widgets._get_notebook()

    @property
    def progressbar(self):
        return self._main_widgets._get_progressbar()

    @property
    def project_menu(self):
        return self._main_widgets._get_project_menu()

    @property
    def sidebar_notebook(self):
        return self._main_widgets._get_sidebar_notebook()

    @property
    def toolbar(self):
        return self._main_widgets._get_toolbar()

    @property
    def tools_menu(self):
        return self._main_widgets._get_tools_menu()

    @property
    def window(self):
        return self._main_widgets._get_window()
