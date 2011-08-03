import _geany_msgwindow


RED = 0
DARK_RED = 1
BLACK = 2
BLUE = 3

STATUS = 0
COMPILER = 1
MESSAGE = 2
SCRATCH = 3
VTE = 4


def clear_tab(tab_num):
    _geany_msgwindow.clear_tab(tab_num)

def compiler_add(msg, msg_color=BLACK):
    _geany_msgwindow.compiler_add(msg, msg_color)

def msg_add(msg, msg_color=BLACK, line=-1, doc=None):
    _geany_msgwindow.msg_add(msg, msg_color, line, doc)

def set_messages_dir(msg_dir):
    _geany_msgwindow.set_messages_dir(msg_dir)

def status_add(msg):
    _geany_msgwindow.status_add(msg)

def switch_tab(tab_num, show=False):
    _geany_msgwindow.switch_tab(tab_num, show)

