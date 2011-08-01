geanyplugin_LTLIBRARIES		=	geanypy.la
geanyplugindir				=	$(libdir)/geany

geanypy_defines				=	-DGEANYPY_PYTHONDIR="\"$(PYTHON_SITE_PKG)\"" \
								-DGEANYPY_PYTHON_LIBRARY="\"$(PYTHON_LIBRARY)\""

geanypy_la_LDFLAGS			=	-module -avoid-version -Wl,--export-dynamic
geanypy_la_CPPFLAGS			=	@GEANY_CFLAGS@ @PYGTK_CFLAGS@ @PYTHON_CPPFLAGS@ \
								$(geanypy_defines)
geanypy_la_LIBADD			=	@GEANY_LIBS@ @PYGTK_LIBS@ @PYTHON_LDFLAGS@ \
								@PYTHON_EXTRA_LIBS@ @PYTHON_EXTRA_LDFLAGS@
geanypy_la_SOURCES			=	plugin.c \
								plugin.h \
								dialogs.c \
								document.c \
								filetype.c \
								editorprefs.c \
								indentprefs.c \
								editor.c \
								mainwidgets.c \
								project.c \
								app.c \
								fileprefs.c \
								scintilla.c
