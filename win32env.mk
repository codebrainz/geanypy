# Adjust the following paths to where you have stuff installed
# ============================================================
# Note: don't quote the paths here, it's done below where needed

# Where Mingw was installed
MINGW_PREFIX = C:/Mingw

# Where GTK+ bundle was installed
GTK_PREFIX = C:/Gtk

# Where Python 2.x was installed
PYTHON_PREFIX = C:/Python27

# Where Geany was installed
GEANY_PREFIX = C:/Program Files/Geany

# Where Geany's source code is (for includes, not distributed on win32)
GEANY_SRCDIR = C:/Documents and Settings/User/My Documents/Projects/Geany/geany

# Where PyGObject and PyGTK source code is (for includes, not distributed on win32)
PYGOBJECT_SRCDIR = C:/Gtk/Python/pygobject-2.28.6
PYGTK_SRCDIR = C:/Gtk/Python/pygtk-2.24.0


# Shouldn't need to change anything below here
# ============================================

CC = $(MINGW_PREFIX)/bin/gcc.exe
MAKE = $(MINGW_PREFIX)/bin/mingw32-make.exe
RM_F = -del /f /q

PY_LIB = "python27.dll"

GEANY_PLUGINS_DIR = "$(GEANY_PREFIX)/lib"
GEANYPY_PYDIR = "$(GEANY_PREFIX)/lib/geanypy"
GEANYPY_PLUGDIR = "$(GEANY_PREFIX)/lib/geanypy/plugins"
GEANYPY_DEFINES = -DGEANYPY_PYTHON_DIR=\"$(GEANYPY_PYDIR)\" \
	-DGEANYPY_PLUGIN_DIR=\"$(GEANYPY_PLUGDIR)\" \
	-DGEANYPY_PYTHON_LIBRARY=\""$(PY_LIB)"\"

GEANY_INCLUDES = \
	-I"$(GEANY_SRCDIR)/plugins" \
	-I"$(GEANY_SRCDIR)/src" \
	-I"$(GEANY_SRCDIR)/tagmanager/src" \
	-I"$(GEANY_SRCDIR)/scintilla/include"

GTK_INCLUDES = \
	-I"$(GTK_PREFIX)/include" \
	-I"$(GTK_PREFIX)/include/gtk-2.0" \
	-I"$(GTK_PREFIX)/include/glib-2.0" \
	-I"$(GTK_PREFIX)/include/cairo" \
	-I"$(GTK_PREFIX)/include/pango-1.0" \
	-I"$(GTK_PREFIX)/include/gdk-pixbuf-2.0" \
	-I"$(GTK_PREFIX)/include/atk-1.0" \
	-I"$(GTK_PREFIX)/lib/glib-2.0/include" \
	-I"$(GTK_PREFIX)/lib/gtk-2.0/include"

GTK_LIBS = -L"$(GTK_PREFIX)/lib" \
	-lgtk-win32-2.0 -lgdk-win32-2.0 -latk-1.0 -lgio-2.0 -lpangoft2-1.0 \
	-lpangocairo-1.0 -lgdk_pixbuf-2.0 -lcairo -lpango-1.0 -lfreetype \
	-lfontconfig -lgobject-2.0 -lglib-2.0 -lgmodule-2.0

PYGOBJECT_INCLUDES = \
	-I"$(PYGOBJECT_SRCDIR)/gio" \
	-I"$(PYGOBJECT_SRCDIR)/glib" \
	-I"$(PYGOBJECT_SRCDIR)/gobject"

PYGTK_INCLUDES = -I"$(PYGTK_SRCDIR)/gtk"

_CFLAGS = $(CFLAGS) -Wall -Werror -g -I. \
	-I"$(PYTHON_PREFIX)/include" \
	$(GTK_INCLUDES) \
	$(PYGOBJECT_INCLUDES) \
	$(PYGTK_INCLUDES) \
	$(GEANY_INCLUDES) \
	$(GEANYPY_DEFINES) \
	-I../src \
	-DGEANYPY_WINDOWS_BUILD=1 -DGTK

_LDFLAGS = $(LDFLAGS) -L"$(PYTHON_PREFIX)/libs" -lpython27 $(GTK_LIBS)
