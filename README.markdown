GeanyPy
=======

*Warning:* This is a work in-progress, and it doesn't do too much yet.

How it works:
-------------

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

