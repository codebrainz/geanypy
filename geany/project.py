import _geany_project


class Project(object):

    _project = None

    def __init__(self):
        self._project = _geany_project.Project()

    @property
    def base_path(self):
        return self._project._get_base_path()

    @property
    def description(self):
        return self._project._get_description()

    @property
    def file_name(self):
        return self._project._get_file_name()

    @property
    def file_patterns(self):
        return self._project._get_file_patterns()

    @property
    def name(self):
        return self._project._get_name()

    @property
    def project_type(self):
        return self._project._get_type()
