geanyplugin_LTLIBRARIES		=	geanypy.la
geanyplugindir				=	$(libdir)/geany

geanypy_la_LDFLAGS			=	-module -avoid-version -Wl,--export-dynamic
# FIXME: How to stop AC_CONFIG_HEADERS from defining -DHAVE_CONFIG_H on it's
#        own?  I undef'd it here for now since it causes geany.h:30 to fail.
geanypy_la_CPPFLAGS			=	@GEANY_CFLAGS@ @PYGTK_CFLAGS@ @PYTHON_CPPFLAGS@ \
								-DGEANYPY_PYTHON_DIR="\"$(libdir)/geany/geanypy\"" \
								-DGEANYPY_PLUGIN_DIR="\"$(datadir)/geany/geanypy/plugins\"" \
								-UHAVE_CONFIG_H
geanypy_la_LIBADD			=	@GEANY_LIBS@ @PYGTK_LIBS@ @PYTHON_LDFLAGS@ \
								@PYTHON_EXTRA_LIBS@ @PYTHON_EXTRA_LDFLAGS@
geanypy_la_SOURCES			=	document.c \
								editor.c \
								editorprefs.c \
								encodings.c \
								fileprefs.c \
								filetype.c \
								highlighting.c \
								indentprefs.c \
								main.c \
								mainwidgets.c \
								msgwindow.c \
								plugin.c \
								plugin.h \
								project.c \
								scintilla.c \
								scinotification.c \
								navqueue.c \
								signalmanager.c \
								signalmanager.h
