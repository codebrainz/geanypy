geanyplugin_LTLIBRARIES		=	geanypy.la
geanyplugindir				=	$(libdir)/geany

geanypy_la_LDFLAGS			=	-module -avoid-version -Wl,--export-dynamic
geanypy_la_CPPFLAGS			=	@GEANY_CFLAGS@ @PYGTK_CFLAGS@ @PYTHON_CPPFLAGS@ \
								-DGEANYPY_PYTHON_DIR="\"$(libdir)/geany/geanypy\"" \
								-DGEANYPY_PLUGIN_DIR="\"$(datadir)/geany/geanypy/plugins\"" \
								-UHAVE_CONFIG_H
geanypy_la_LIBADD			=	@GEANY_LIBS@ @PYGTK_LIBS@ @PYTHON_LDFLAGS@ \
								@PYTHON_EXTRA_LIBS@ @PYTHON_EXTRA_LDFLAGS@
geanypy_la_SOURCES			=	plugin.c \
								plugin.h \
								dialogs.c \
								document.c \
								filetypes.c \
								indentprefs.c \
								editor.c \
								mainwidgets.c \
								project.c \
								app.c \
								scintilla.c \
								scinotification.c \
								encodings.c \
								highlighting.c \
								main.c \
								msgwindow.c \
								navqueue.c \
								signalmanager.c \
								signalmanager.h
