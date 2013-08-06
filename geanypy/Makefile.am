wrapper_sources = __init__.py doc.py

wrapper_objects = $(wrapper_sources:.py=.pyc)
wrapperdir      = $(libdir)/geany/geanypy/geanypy
wrapper_DATA    = $(wrapper_sources) $(wrapper_objects)

EXTRA_DIST = $(wrapper_sources)
CLEANFILES = $(wrapper_objects)

.SUFFIXES = .pyc .py

.py.pyc:
	$(AM_V_GEN) \
	$(PYTHON) -c "from py_compile import compile; compile('$<', '$@')"
