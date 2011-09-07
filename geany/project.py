import _project as c_project


class Project(object):

    _project = None

    def __init__(self, project=None):
		if project is not None:
			self._project = project
		else:
			self._project = cproject.Project()

    @property
    def base_path(self):
        return self._project.get_base_path()

    @property
    def description(self):
        return self._project.get_description()

    @property
    def file_name(self):
        return self._project.get_file_name()

    @property
    def file_patterns(self):
        return self._project.get_file_patterns()

    @property
    def name(self):
        return self._project.get_name()

    @property
    def project_type(self):
        return self._project.get_type()
