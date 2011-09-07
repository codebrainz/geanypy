import _geany_app
import project


class App(object):
    _app = None

    def __init__(self):
        self._app = _geany_app.App()

    @property
    def configdir(self):
        return self._app.get_configdir()

    @property
    def debug_mode(self):
        return self._app.get_debug_mode()

    @property
    def project(self):
        _proj = self._app.get_project()
        if _proj is not None and _proj._get_is_open():
            proj = project.Project()
            proj._project = _proj
            return proj
