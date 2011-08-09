The :mod:`editor` module
*********************

.. module:: editor
    :synopsis: Geany's editors

This modules contains a class to wrap the
`GeanyEditor <http://www.geany.org/manual/reference/structGeanyEditor.html>`_
structure and functions as well as function to query default editor data.

:mod:`editor` Constants
=======================

Word characters
---------------

.. data:: WORDCHARS

    Default character set to define which characters should be treated as part
    of a word.

.. data:: INDENT_TYPE_SPACES
.. data:: INDENT_TYPE_TABS
.. data:: INDENT_TYPE_BOTH

    These 3 constants can be used with :meth:`Editor.set_indent_type` to control
    which indenting mode the :class:`Editor` instance uses.

.. data:: INDICATOR_ERROR

    Indicator to highlight errors in the document text with a red squiggly underline.

.. data:: INDICATOR_SEARCH

    Indicator used to highlight search results in the document.

:mod:`editor` Functions
=======================

.. function:: find_snippet(snippet)

    Gets a snippet by name from the default set of snippets.

    :param snippet: The name of the snippet to get.

    :return: The snippet or :data:`None` if it was not found.

.. function:: get_default_eol_char()
.. function:: get_default_eol_char_len()
.. function:: get_default_eol_char_mode()
.. function:: get_default_eol_char_name()

    Access information about the default end-of-line usage.

.. function:: get_default_indent_prefs()

    Get the default indentation preferences as a :class:`IndentPrefs` instance
    for new :class:`Editor` instances.

:class:`editor.Editor` Objects
=======================

.. class:: Editor

    Wraps the `GeanyEditor` structure and functions.

.. method:: Editor.get_eol_char()
.. method:: Editor.get_eol_char_len()
.. method:: Editor.get_eol_char_module()
.. method:: Editor.get_eol_char_name()

    Access information about the :class:`Editor` end-of-line usage.

.. method:: Editor.get_indent_prefs()

    Gets the indentation preferences for the editor as a :class:`IndentPrefs`
    instance.

.. method:: Editor.set_indent_type(indent_type)

    Sets the indent type for the :class:`Editor` instance to `indent_type`.

.. method:: Editor.get_scintilla()

    Return the :class:`geany.scintilla.Scintilla` instance used by this
    editor.

.. method:: Editor.find_snippet(snippet)

    Gets a snippet by name from this :class:`Editor`.

    :param snippet: The name of the snippet to get.

    :return: The snippet or :data:`None` if it was not found.

.. method:: Editor.get_word_at_position(pos[, wordchars=None])

    Finds the word at the position specified by `pos`.  If any word is found,
    it is returned.  Otherwise :data:`None` is returned.  Additional
    `wordchars` can be specified to define what to consider as a word.

    :param pos: The position to get the word at.
    :param wordchars: The wordchars to sepatrate words.  `wordchars` means all
        characters to count as part of a word.  If :data:`None` then use the
        default wordchars, :data:`editor.WORDCHARS`.

    :return: The word at the specified position or :data:`None` if no word was
        found.

.. method:: Editor.goto_pos(pos[, mark=True])

    Moves the position to `pos`, switching to the document if necessary, setting
    a marker if mark is :data:`True`.

    :param pos: The position to move to.
    :param mark: Whether to mark the line with a line marker.

    :return: :data:`True` if the action has been performed, :data:`False`
        otherwise.

.. method:: Editor.indicator_clear(indic)

    Delete all currently set indicators matching `indic` in the editor window.

    :param indic: The indicator to delete.

.. method:: Editor.indicator_set_on_line(indic, line)

    Sets the indicator `indic` on `line`.  White space at the start and end
    if the line is not marked.

    :param indic: The indicator to set.
    :param line: The line to set the indicator on.

.. method:: Editor.indicator_set_on_range(indic, start, end)

    Sets the indicator `indic` on a specific range of text specified by
    `start` and `end`.

    :param indic: The indicator to set.
    :param start: The start position to mark.
    :param end: The end position to mark.

.. method:: Editor.insert_snippet(snippet[, pos=-1])

    Replaces all special sequences in `snippet` and inserts it at `pos`.

    :param snippet: The name of the snippet to insert.
    :param pos: The position to insert the snippet at or -1 for the current position.

.. method:: Editor.insert_text_block(text, insert_pos[, cursor_index=-1[, newline_indent_size=-1[, replace_newlines=False]]])

    Inserts text, replacing `\\t` characters (`0x9`) and `\\n` characters (`0xA`)
    according for the document.

    * Leading tabs are replaced with the correct indentation.
    * Non-leading tabs are replaced with spaces (except when using :data:`INDENT_TYPE_TABS`)
    * Newline characters are replaced with the correct line ending string. This is very useful for inserting code without having to handle the indent type yourself (:data:`INDENT_TYPE_TABS` and :data:`INDENT_TYPE_SPACES` modes can be tricky).

    :param text: The text block to insert.
    :param insert_pos: The position to insert the text at.
    :param cursor_index: If greater than zero, the index into `text` to place the
        cursor at.
    :param newline_indent_size: Indentation size (in spaces) to insert for each
        newline; use -1 to read the indent size from the line with `insert_pos`
        on it.
    :param replace_newlines: Whether to replace newlines.  If newlines have been
        replaced already, this should be :data:`False` to avoid errors
        (e.g. on Windows).

    .. warning:: Make sure all `\\t` characters in `text` are intended as indent
        widths or alignment, not hard tabs, as those won't be preserved.

    .. note:: This doesn't scroll the cursor into view afterwards.

