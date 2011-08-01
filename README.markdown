GeanyPy
=======

*Warning:* This is a work in-progress, and it doesn't do too much yet.

How it works
------------

In the `src/` directory is a normal Geany plugin (`plugin.c`) which loads the
Python interpreter.  It then loads some C Python modules (`dialogs.c`,
`documents.c`, etc.) that interface with Geany's C API.  After that, it loads
the `geany` Python module which is just glue/sugar-coating to make the C
module more "Pythonic".

There will be a Python plugin loader soon, which will find Python plugins and
import them into Geany's process.  These plugins can import the `geany` module,
giving them access to the plugin API.

Currently, a Python interactive interpreter is run when the plugin is loaded.
If you want to play around with the bindings, here's an example.


```python
import geany # called automatically in the built-in interactive shell

doc = geany.document.open("/some/file/here")

print(doc.file_name)
print(doc.file_type.display_name)

geany.dialogs.show_msgbox("Hello World")

geany.main_widgets.window.set_title("Hello Window")

```

Dependencies
------------

To build GeanyPy you need the following dependencies:

* Python 2.6+ and development files (I don't think Python 3 will work).
* Geany 0.21+ and development files (ex, from SVN)
* PyGTK 2.0 and development files

On Debian/Ubuntu, the (non-Geany) dependencies can be installed like this:

    $ apt-get install python python-dev python-gtk2 python-gtk2-dev

Installation
------------

First you need to know where Geany is installed:

    $ GEANY_PREFIX=`pkg-config --variable=prefix geany`

The you can install as normal:

    $ ./autogen.sh
    $ ./configure --prefix=$GEANY_PREFIX
    $ make
    # make install

*Note:* That even if `GEANY_PREFIX` is in your home directory, you still need
root access since the Python modules will go into your system-wide
`site-packages` directory.

You should be able to force a specific version of Python to be used, by using
the `PYTHON_VERSION` environment variable.

Running on Windows
------------------

Good luck!  Let me know how it goes :)

(Really, it should be possible with some tweaking)
