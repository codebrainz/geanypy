geanypy_sources				=	__init__.py \
								document.py \
								filetypes.py \
								indentprefs.py \
								editor.py \
								mainwidgets.py \
								scintilla.py \
								console.py \
								manager.py \
								loader.py \
								plugin.py \
								encodings.py \
								highlighting.py \
								main.py \
								msgwindow.py \
								navqueue.py \
								signalmanager.py
geanypy_objects				=	$(geanypy_sources:.py=.pyc)
geanypydir					=	$(libdir)/geany/geanypy/geany
geanypy_DATA				=	$(geanypy_sources)  $(geanypy_objects)

EXTRA_DIST					=	$(geanypy_sources)
CLEANFILES					=	$(geanypy_objects)

.SUFFIXES = .pyc .py

.py.pyc:
	$(AM_V_GEN) \
	$(PYTHON) -c "from py_compile import compile; compile('$<', '$@')"
