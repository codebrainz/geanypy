AC_PREREQ([2.62])

# Initialize build system tools
AC_INIT([geanypy], [0.01], [mbrush@codebrainz.ca])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([src/geanypy-plugin.c])
AC_CONFIG_HEADERS([src/plugin-config.h])
AM_INIT_AUTOMAKE([1.11.1 -Wall foreign])

# Configure build system and tools
AM_SILENT_RULES([yes])

# fix for automake >= 1.12
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])

# Check for utilities
AC_PROG_CC
AC_PROG_SED
LT_INIT([disable-static])

# Check for headers, libraries and packages
AC_CHECK_HEADERS([stdio.h string.h dlfcn.h])
PKG_CHECK_MODULES([GEANY], [geany >= 0.21])
GP_CHECK_GTK3([PKG_CHECK_MODULES([PYGOBJECT], [pygobject-3.0])], [PKG_CHECK_MODULES([PYGTK], [pygtk-2.0])])
AX_PYTHON_DEVEL([>= '2.6'])
AX_PYTHON_LIBRARY(,[AC_MSG_ERROR([Cannot determine location of the Python DSO])])
AC_SUBST([PYTHON])

# Puts these in the configuration header
AC_DEFINE_UNQUOTED(
    [GEANYPY_PYTHON_LIBRARY],
    ["$PYTHON_LIBRARY"],
    [Location of Python library to dlopen()])

# Generate build files
AC_CONFIG_FILES([
    Makefile
	src/Makefile
	geany/Makefile
    plugins/Makefile
])
AC_OUTPUT
