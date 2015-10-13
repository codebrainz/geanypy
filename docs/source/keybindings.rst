The :mod:`keybindings` module
*****************************

.. module:: keybindings
    :synopsis: Utilities for binding keys and sending commands

This module contains some helper functions to create key bindings and to send pre-defined
menu commands from Python.

.. function:: init()

    Initializes the plugin key group interface.  This should be called before attempting
    to create a key binding and can be called multiple times without issue.

    :return: A boolean indicating success/failure.

.. function:: register_binding(plugin_group_name, key_name, key_label, callback[, data=None])

    Creates a key binding.  The binding will show up under Preferences/Keybindings/GeanyPy where it can be
    assigned a key by the user.

    :param plugin_group_name: This name will be prepended to the key label.
    	Your plugin should use the same name for all bindings.
    :param key_name: An internal name for the key.  I.E. "MY_KEY"
    :param key_label: The label which will be shown in Preferences/Keybindings.  I.E. "My Key"
    :param callback: The function to be called when the key is pressed.  It must take at least
    	1 argument which will be the key_id assigned to the key binding.
    :param data: Optional data to be passed to the callback.  If supplied, the callback must
    	take a second argument to receive it.
    	
    The final display label for the key will be "<plugin_group_name>: <key_label>". 

    :return: The unique id assigned to this key binding.  The callback will receive this id as its
    	first argument whenever the key is pressed.  You must also use this id to unregister the key binding.

.. function:: unregister_binding(key_id)

    Unregisters a key binding.

    :param key_id:  The id returned by register_binding.

.. function:: send_command(key)

    Sends a menu command as though the user clicked on it.

    :param key: A string containing the key to send from one of the GeanyKeyBindingIDs in keybindings.h.
    	I.E. "GEANY_KEYS_FILE_NEW" or just "FILE_NEW".  

Example::

    import geany

    class MyPlugin(geany.Plugin):

        __plugin_name__ = "MyPlugin"
        __plugin_version__ = "1.0"
        __plugin_description__ = "A Test Plugin"
        __plugin_author__ = "Someone"

        def onKey(self, key):
            print "Key pressed: %d" % key
            geany.keybindings.send_command("FILE_NEW")

        def __init__(self):
            geany.keybindings.init()
            geany.keybindings.register_binding("MyPlugin", "MyFileOpen", "My File Open", self.onKey)
