geanypy_sources				=	__init__.py \
								dialogs.py \
								document.py \
								filetype.py \
								indentprefs.py \
								editorprefs.py \
								editor.py \
								mainwidgets.py \
								project.py \
								app.py \
								fileprefs.py \
								scintilla.py \
								console.py \
								manager.py \
								loader.py \
								plugin.py
geanypy_objects				=	$(geanypy_sources:.py=.pyc)
geanypydir					=	$(libdir)/geany/geanypy/geany
geanypy_DATA				=	$(geanypy_sources)  $(geanypy_objects)

EXTRA_DIST					=	$(geanypy_sources)
CLEANFILES					=	$(geanypy_objects)

.SUFFIXES = .pyc .py

.py.pyc:
	$(AM_V_GEN) \
	$(PYTHON) -c "from py_compile import compile; compile('$<', '$@')"
