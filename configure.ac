AC_PREREQ([2.67])

# Initialize build system tools
AC_INIT([geanypy], [0.01], [mbrush@codebrainz.ca])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([src/plugin.c])
AC_CONFIG_HEADERS([src/plugin-config.h])
AM_INIT_AUTOMAKE([foreign silent-rules])
LT_INIT([disable-static])

# Configure build system and tools
AM_SILENT_RULES([yes])

# Check for utilities
AC_PROG_CC
AC_PROG_SED

# Check for headers, libraries and packages
AC_CHECK_HEADERS([stdio.h string.h dlfcn.h])
PKG_CHECK_MODULES([GEANY], [geany >= 0.21])
PKG_CHECK_MODULES([PYGTK], [pygtk-2.0])
AX_PYTHON_DEVEL([>= '2.6'])
AX_PYTHON_LIBRARY(,[AC_MSG_ERROR([Cannot determine location of the Python DSO])])
AC_SUBST([PYTHON])

# Puts these in the configuration header
AC_DEFINE_UNQUOTED(
    [GEANYPY_PYTHON_DIR],
    ["${prefix}/lib/geany/geanypy"],
    [Location to put GeanyPy Python package])

AC_DEFINE_UNQUOTED(
    [GEANYPY_PLUGIN_DIR],
    ["${prefix}/share/geany/geanypy/plugins"],
    [Location where pre-installed plugins go])

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
