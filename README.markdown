GeanyPy
=======

Write Geany plugins in Python!

*Please note:* since GeanyPy is under heavy development, the API it exposes
may change quite drastically.  If you start using GeanyPy to write plugins,
drop me a line and I'll do my best to not break your code.

*Update:* There's now some documentation available, visit
http://codebrainz.github.com/geanypy to read it.

How it works
------------

In the `src/` directory is a normal Geany plugin (`plugin.c`) which loads the
Python interpreter.  It then loads some C Python modules (`dialogs.c`,
`documents.c`, etc.) that interface with Geany's C API.  After that, it loads
the `geany` Python module which is just glue/sugar-coating to make the C
module more "Pythonic".

To write a plugin, inherit from the `geany.Plugin` class and implmenent the
required members (see `geany/plugin.py` documentation comments).  Then put the
plugin in a searched plugin path.  Currently two locations are search for
plugins.  The first is `PREFIX/share/geany/geanypy/plugins` and the recommended
location is under your personal Geany directory (usually
`~/.config/geany/plugins/geanypy/plugins`).  To load or unload plugins, click
the Python Plugin Manager item under the Tools menu which will appear when you
activate GeanyPy through Geany's regular plugin manager.

When GeanyPy plugin is loaded, it will add a new tab to the notebook in the
message window area that contains an interactive Python shell with the `geany`
module pre-imported.  You can tinker around with API with this console, for
example:


```python
import geany # called automatically in the built-in interactive shell

doc = geany.document.open("/some/file/here")

print(doc.file_name)
print(doc.file_type.display_name)

geany.dialogs.show_msgbox("Hello World")

geany.main_widgets.window.set_title("Hello Window")

def on_document_open(doc):
    print("Document '%s' was opened" % doc.file_name)

geany.signals.connect('document-open', on_document_open)

```

Dependencies
------------

To build GeanyPy you need the following dependencies:

* Python 2.6+ and development files (I don't think Python 3 will work).
* Geany 0.21+ and development files (from SVN)
* PyGTK 2.0 and development files

On Debian/Ubuntu, the (non-Geany) dependencies can be installed like this:

    $ apt-get install python python-dev python-gtk2 python-gtk2-dev

See Geany's documentation/website for information on compiling it from the
Subversion or Git repositories.

Installation
------------

First you need to know where Geany is installed:

    $ GEANY_PREFIX=`pkg-config --variable=prefix geany`

The you can install as normal:

    $ ./autogen.sh
    $ ./configure --prefix=$GEANY_PREFIX
    $ make
    # make install

You should be able to force a specific version of Python to be used, by using
the `PYTHON_VERSION` environment variable, but I've only tested with 2.6.6 so
far.  I imagine 2.7 series will also work fine.

Running on Windows
------------------

Good luck!  Let me know how it goes :)

(Really, it should be possible with some tweaking)
