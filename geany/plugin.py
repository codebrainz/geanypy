import geany

class Plugin(object):


    _events = {
        "document-open": [],
        # TODO: add more events here
    }


    def __init__(self):
        pass


    def cleanup(self):
        pass


    def plugin_connect(self, event, func, *args, **kwargs):
        try:
            self._events[event].append((func, args, kwargs))
        except KeyError:
            print("Unable to connecto to event '%s': unknown event" % event)


    def plugin_disconnect(self, event, func):
        try:
            for callback_info in self._events[event]:
                if callback_info[0] == func:
                    self._events[event].remove(callback_info)
                    break
            else:
                print("Unable to disconnect function from event '%s': " +
                        "function not connected" % event)
        except KeyError:
            print("Unable to disconnect function from event '%s': " +
                    "unknown event" % event)


    def plugin_emit(self, event, *args, **kwargs):
        try:
            for callback_info in self._events[event]:
                try:
                    new_args = args + callback_info[1]
                    new_kwargs = {}
                    for kw, arg in callback_info[2].items() + kwargs.items():
                        new_args[kw] = arg
                    callback_info[0](*new_args, **new_kwargs)
                except IndexError:
                    print("Unable to call event handler for event '%s': " +
                            "corrupt callback information" % event)
        except KeyError:
            print("Unable to emit event '%s': unknown event" % event)


    @property
    def name(self):
        return self.__geany_plugin_info__.get('name', None)

    @property
    def description(self):
        return self.__geany_plugin_info__.get('description', None)

    @property
    def version(self):
        return self.__geany_plugin_info__.get('version', None)

    @property
    def author(self):
        return self.__geany_plugin_info__.get('author', None)
