geanyplugin_LTLIBRARIES		=	geanypy.la
geanyplugindir				=	$(libdir)/geany

geanypy_la_LDFLAGS			=	-module -avoid-version -Wl,--export-dynamic
geanypy_la_CPPFLAGS			=	@GEANY_CFLAGS@ @PYGTK_CFLAGS@ @PYTHON_CPPFLAGS@ \
								-DGEANYPY_PYTHON_DIR="\"$(libdir)/geany/geanypy\"" \
								-DGEANYPY_PLUGIN_DIR="\"$(datadir)/geany/geanypy/plugins\"" \
								-UHAVE_CONFIG_H
geanypy_la_LIBADD			=	@GEANY_LIBS@ @PYGTK_LIBS@ @PYTHON_LDFLAGS@ \
								@PYTHON_EXTRA_LIBS@ @PYTHON_EXTRA_LDFLAGS@
geanypy_la_SOURCES			=	app.c \
								dialogs.c \
								document.c \
								editor.c \
								encodings.c \
								filetypes.c \
								highlighting.c \
								indentprefs.c \
								main.c \
								mainwidgets.c \
								msgwindow.c \
								navqueue.c \
								plugin.c plugin.h \
								prefs.c \
								project.c \
								scinotification.c \
								scintilla.c \
								signalmanager.c \
								signalmanager.h
