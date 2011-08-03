import _geany_main


def is_realized():
    return _geany_main.is_realized()

def locale_init(locale_dir, package):
    return _geany_main.locale_init(locale_dir, package)

def reload_configuration():
    return _geany_main.reload_configuration()
