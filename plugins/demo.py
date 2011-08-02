import geany


class DemoPlugin(geany.Plugin):

    __plugin_name__ = "Demo Plugin"
    __plugin_version__ = "0.01"
    __plugin_description__ = "A Sample plugin"
    __plugin_author__ = "Matthew Brush <mbrush@codebrainz.ca>"

    def __init__(self):
        print("Demo plugin initializing")

    def cleanup(self):
        print("Demo plugin cleaning up")
