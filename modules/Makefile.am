PyModule 				= -module -avoid-version -Wl,--export-dynamic
PyPluginCflags			= @GEANY_CFLAGS@ @PYTHON_CPPFLAGS@
PyGtkPluginCflags		= $(PyPluginCflags) @PYGTK_CFLAGS@
PyPluginLibs			= @GEANY_LIBS@ @PYTHON_LDFLAGS@
PyGtkPluginLibs			= $(PyPluginLibs)
PySources				= __init__.py
PyObjects				= $(PySources:.py=.pyc)


geanypydir				= $(libdir)/geany/geanypy/geany
geanypy_DATA			= $(PySources) $(PyObjects)
geanypy_LTLIBRARIES		= app.la \
							dialogs.la \
							document.la \
							editor.la \
							filetype.la \
							indentprefs.la \
							project.la \
							scintilla.la


app_la_CPPFLAGS			= $(PyPluginCflags)
app_la_LIBADD			= $(PyPluginLibs)
app_la_LDFLAGS			= $(PyModule)
app_la_SOURCES			= app.c

dialogs_la_CPPFLAGS		= $(PyGtkPluginCflags)
dialogs_la_LIBADD		= $(PyPluginLibs)
dialogs_la_LDFLAGS		= $(PyModule)
dialogs_la_SOURCES		= dialogs.c

document_la_CPPFLAGS	= $(PyPluginCflags)
document_la_LIBADD		= $(PyPluginLibs)
document_la_LDFLAGS		= $(PyModule)
document_la_SOURCES		= document.c

filetype_la_CPPFLAGS	= $(PyGtkPluginCflags)
filetype_la_LIBADD		= $(PyGtkPluginLibs)
filetype_la_LDFLAGS		= $(PyModule)
filetype_la_SOURCES		= filetype.c

project_la_CPPFLAGS		= $(PyPluginCflags)
project_la_LIBADD		= $(PyPluginLibs)
project_la_LDFLAGS		= $(PyModule)
project_la_SOURCES		= project.c

editor_la_CPPFLAGS		= $(PyGtkPluginCflags)
editor_la_LIBADD		= $(PyGtkPluginLibs)
editor_la_LDFLAGS		= $(PyModule)
editor_la_SOURCES		= editor.c

scintilla_la_CPPFLAGS	= $(PyGtkPluginCflags)
scintilla_la_LIBADD		= $(PyGtkPluginLibs)
scintilla_la_LDFLAGS	= $(PyModule)
scintilla_la_SOURCES	= scintilla.c

indentprefs_la_CPPFLAGS	= $(PyPluginCflags)
indentprefs_la_LIBADD	= $(PyPluginLibs)
indentprefs_la_LDFLAGS	= $(PyModule)
indentprefs_la_SOURCES	= indentprefs.c


EXTRA_DIST				= $(PySources)
CLEANFILES				= $(PyObjects)


.SUFFIXES = .pyc .py

.py.pyc:
	$(AM_V_GEN) \
	$(PYTHON) -c "from py_compile import compile; compile('$<', '$@')"
