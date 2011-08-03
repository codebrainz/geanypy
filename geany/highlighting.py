import _geany_highlighting


def get_style(ft_id, style_id):
    _ls = _geany_highlighting.get_style(ft_id, style_id)
    if _ls is not None:
        ls = LexerStyle()
        ls._lexer_style = _ls
        return ls

def is_code_style(lexer, style):
    return _geany_highlighting.is_code_style(lexer, style)

def is_comment_style(lexer, style):
    return _geany_highlighting.is_comment_style(lexer, style)

def is_string_style(lexer, style):
    return _geany_highlighting.is_string_style(lexer, style)

def set_style(sci, ft):
    _geany_highlighting.set_style(sci, ft)


class LexerStyle(object):

    _lexer_style = None

    def __init__(self, lexer_style=None):
        if lexer_style is not None:
            self._lexer_style = lexer_style

    @property
    def background(self):
        return self._lexer_style._get_background()

    @property
    def foreground(self):
        return self._lexer_style._get_foreground()

    @property
    def bold(self):
        return self._lexer_style._get_bold()

    @property
    def italic(self):
        return self._lexer_style._get_italic()
