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
								console.py
geanypy_objects				=	$(geanypy_sources:.py=.pyc)
geanypydir					=	$(PYTHON_SITE_PKG)/geany
geanypy_DATA				=	$(geanypy_sources)  $(geanypy_objects)

CLEANFILES					=	$(geanypy_objects)

%.pyc: %.py
	$(AM_V_GEN)$(PYTHON) -c "import py_compile; py_compile.compile('$<')"
