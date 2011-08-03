geanyplugin_LTLIBRARIES		=	geanypy.la
geanyplugindir				=	$(libdir)/geany

geanypy_la_LDFLAGS			=	-module -avoid-version -Wl,--export-dynamic
# FIXME: How to stop AC_CONFIG_HEADERS from defining -DHAVE_CONFIG_H on it's
#        own?  I undef'd it here for now since it causes geany.h:30 to fail.
geanypy_la_CPPFLAGS			=	@GEANY_CFLAGS@ @PYGTK_CFLAGS@ @PYTHON_CPPFLAGS@ \
								-UHAVE_CONFIG_H
geanypy_la_LIBADD			=	@GEANY_LIBS@ @PYGTK_LIBS@ @PYTHON_LDFLAGS@ \
								@PYTHON_EXTRA_LIBS@ @PYTHON_EXTRA_LDFLAGS@
geanypy_la_SOURCES			=	plugin.c \
								plugin.h \
								dialogs.c \
								document.c \
								filetypes.c \
								editorprefs.c \
								indentprefs.c \
								editor.c \
								mainwidgets.c \
								project.c \
								app.c \
								fileprefs.c \
								scintilla.c \
								encodings.c \
								highlighting.c \
								main.c \
								msgwindow.c \
								navqueue.c
