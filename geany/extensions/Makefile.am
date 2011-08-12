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
							encodings.la \
							fileprefs.la \
							filetype.la \
							highlighting.la \
							main.la \
							mainwidgets.la \
							msgwindow.la \
							navqueue.la \
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

encodings_la_CPPFLAGS	= $(PyPluginCflags)
encodings_la_LIBADD		= $(PyPluginLibs)
encodings_la_LDFLAGS	= $(PyModule)
encodings_la_SOURCES	= encodings.c

fileprefs_la_CPPFLAGS	= $(PyPluginCflags)
fileprefs_la_LIBADD		= $(PyPluginLibs)
fileprefs_la_LDFLAGS	= $(PyModule)
fileprefs_la_SOURCES	= fileprefs.c

highlighting_la_CPPFLAGS= $(PyPluginCflags)
highlighting_la_LIBADD	= $(PyPluginLibs)
highlighting_la_LDFLAGS	= $(PyModule)
highlighting_la_SOURCES	= highlighting.c

main_la_CPPFLAGS		= $(PyPluginCflags)
main_la_LIBADD			= $(PyPluginLibs)
main_la_LDFLAGS			= $(PyModule)
main_la_SOURCES			= main.c

mainwidgets_la_CPPFLAGS	= $(PyGtkPluginCflags)
mainwidgets_la_LIBADD	= $(PyGtkPluginLibs)
mainwidgets_la_LDFLAGS	= $(PyModule)
mainwidgets_la_SOURCES	= mainwidgets.c

msgwindow_la_CPPFLAGS	= $(PyPluginCflags)
msgwindow_la_LIBADD		= $(PyPluginLibs)
msgwindow_la_LDFLAGS	= $(PyModule)
msgwindow_la_SOURCES	= msgwindow.c

navqueue_la_CPPFLAGS	= $(PyPluginCflags)
navqueue_la_LIBADD		= $(PyPluginLibs)
navqueue_la_LDFLAGS		= $(PyModule)
navqueue_la_SOURCES		= navqueue.c


EXTRA_DIST				= $(PySources)
CLEANFILES				= $(PyObjects)


.SUFFIXES = .pyc .py

.py.pyc:
	$(AM_V_GEN) \
	$(PYTHON) -c "from py_compile import compile; compile('$<', '$@')"
