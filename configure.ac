AC_PREREQ([2.67])

# Initialize build system tools
AC_INIT([geanypy], [0.01], [mbrush@codebrainz.ca])
AM_INIT_AUTOMAKE([foreign silent-rules])
LT_INIT([disable-static])

# Configure build system and tools
AM_SILENT_RULES([yes])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([src/plugin.c])

# Check for utilities
AC_PROG_CC
AC_PROG_SED

# Check for Python to pre-compile python sources
AC_PATH_PROG([PYTHON], [python[$PYTHON_VERSION]], [no])
AC_SUBST([PYTHON])
if [[ x"$PYTHON" = "xno" ]]; then
	AC_MSG_ERROR([Python not found, is python$PYTHON_VERSION installed?])
fi

# Check for headers, libraries and packages
AC_CHECK_HEADERS([stdio.h string.h dlfcn.h])
PKG_CHECK_MODULES([geany], [geany])
PKG_CHECK_MODULES([pygtk], [pygtk-2.0])
AX_PYTHON_DEVEL([>= '2.6'])

# Generate build files
AC_OUTPUT([
    Makefile
	src/Makefile
	geany/Makefile
])
