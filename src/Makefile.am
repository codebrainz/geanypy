geanyplugin_LTLIBRARIES		=	geanypy.la
geanyplugindir				=	$(libdir)/geany

geanypy_defines				=	-DGEANYPY_PYTHONDIR="\"$(PYTHON_SITE_PKG)\"" \
								-DGEANYPY_PYTHON_LIBRARY="\"$(shell $(PYTHON) $(top_srcdir)/scripts/pylib.py)\""

geanypy_la_LDFLAGS			=	 -module -avoid-version -Wl,--export-dynamic
geanypy_la_CPPFLAGS			=	@geany_CFLAGS@ @pygtk_CFLAGS@ @PYTHON_CPPFLAGS@ \
								$(geanypy_defines)
geanypy_la_LIBADD			=	@geany_LIBS@ @pygtk_LIBS@ @PYTHON_LDFLAGS@ \
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
								app.c
