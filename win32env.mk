# Adjust the following paths to where you have stuff installed
# ============================================================
# Note: don't quote the paths here, it's done below where needed

# Where Mingw was installed
MINGW_PREFIX = C:/Mingw

# Where GTK+ bundle was installed
GTK_PREFIX = C:/Gtk

# Python 2 digit version (ie. 25, 26, 27)
PYTHON_VERSION=27

# Where Python 2.x was installed
PYTHON_PREFIX = C:/Python$(PYTHON_VERSION)

# Where Geany was installed
GEANY_PREFIX = C:/Program Files/Geany

# Where Geany's source code is
# Note: official installer doesn't isntall headers, so need from source
GEANY_SRCDIR = C:/Documents and Settings/User/My Documents/Projects/Geany/geany

# Where PyGObject and PyGTK source code is
# Note: official installer doesn't install headers, so need from source
PYGOBJECT_SRCDIR = C:/Gtk/Python/pygobject-2.28.6
PYGTK_SRCDIR = C:/Gtk/Python/pygtk-2.24.0


# Shouldn't need to change anything below here
# ============================================

CC = $(MINGW_PREFIX)/bin/gcc.exe
MAKE = $(MINGW_PREFIX)/bin/mingw32-make.exe
RM_F = -del /f /q

GEANYPY_CFLAGS = \
	-DGEANYPY_WINDOWS_BUILD=1 \
	-DG_LOG_DOMAIN=\"GeanyPy\"

GEANY_CFLAGS = \
	-I"$(GEANY_SRCDIR)/plugins" \
	-I"$(GEANY_SRCDIR)/src" \
	-I"$(GEANY_SRCDIR)/tagmanager/src" \
	-I"$(GEANY_SRCDIR)/scintilla/include" \
	-DGTK

GTK_CFLAGS = \
	-I"$(GTK_PREFIX)/include" \
	-I"$(GTK_PREFIX)/include/gtk-2.0" \
	-I"$(GTK_PREFIX)/include/glib-2.0" \
	-I"$(GTK_PREFIX)/include/cairo" \
	-I"$(GTK_PREFIX)/include/pango-1.0" \
	-I"$(GTK_PREFIX)/include/gdk-pixbuf-2.0" \
	-I"$(GTK_PREFIX)/include/atk-1.0" \
	-I"$(GTK_PREFIX)/lib/glib-2.0/include" \
	-I"$(GTK_PREFIX)/lib/gtk-2.0/include"

GTK_LIBS = \
	-L"$(GTK_PREFIX)/lib" \
	-lgtk-win32-2.0 \
	-lgdk-win32-2.0 \
	-latk-1.0 -lgio-2.0 \
	-lpangoft2-1.0 \
	-lpangocairo-1.0 \
	-lgdk_pixbuf-2.0 \
	-lcairo \
	-lpango-1.0 \
	-lfreetype \
	-lfontconfig \
	-lgobject-2.0 \
	-lglib-2.0 \
	-lgmodule-2.0

PYTHON_CFLAGS = \
	-I"$(PYTHON_PREFIX)/include"

PYTHON_LIBS = \
	-L"$(PYTHON_PREFIX)/libs" \
	-lpython$(PYTHON_VERSION)

PYGOBJECT_CFLAGS = \
	-I"$(PYGOBJECT_SRCDIR)/gio" \
	-I"$(PYGOBJECT_SRCDIR)/glib" \
	-I"$(PYGOBJECT_SRCDIR)/gobject"

PYGTK_CFLAGS = \
	-I"$(PYGTK_SRCDIR)/gtk"

_CFLAGS = \
	$(CFLAGS) -Wall -Werror -g \
	$(PYTHON_CFLAGS) \
	$(GTK_CFLAGS) \
	$(PYGOBJECT_CFLAGS) \
	$(PYGTK_CFLAGS) \
	$(GEANY_CFLAGS) \
	$(GEANYPY_CFLAGS)

_LDFLAGS = \
	$(LDFLAGS) \
	$(PYTHON_LIBS) \
	$(GTK_LIBS)
