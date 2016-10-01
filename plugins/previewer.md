# Mark-Up/Down Previewer

Semi-real-time preview of multiple light-weight markup languages.

## Features

 * The following formats are processed with `pandoc`:

	- Markdown
	- reStructuredText
	- Txt2tags
	- LaTeX
	- Docbook

 * Asciidoc is processed with `asciidoc` (as it is not supported by `pandoc`)

 * RFC-822-like email-messages are processed according to the Content-Type header.

 * HTML and plain text files minimally processed.

 * Other formats are treated as plain text.

 * Can be used as a rudimentary web browser.  Document processing and previewing is suspended until the location bar is cleared.

## Installation

 * Copy `previewer.py` to your geany plugins directory (`~/.config/geany/plugins/`).

 * Enable the `geanypy` plugin.  This plugin will appear as a sub-item, and can be enabled.

 * Once enabled, operation is automatic.  Settings may be changd, but they should already have reasonable defaults.

## Preferences

 * **update interval**.  Limit automatic reprocessing of files to no more than once during this time period (in seconds).  Does not prevent reprocessing when switching documents or saving.

 * **preview position**.  Where to display the document preview.

	- **sidebar** (default)
	- message window

 * **ouput position**.  Which tab in the message window to send informational messages.

	- status tab
	- compiler tab
	- **messages tab** (default)
	- standard output (or where ever the standard `print` command sends them)

## Known problems

 * Rapid reprocessing of files can make geany unusably slow.  Prevent by increasing the *update interval*.

 * High *update interval* settings causes the preview to lag behind the document contents.  Force immediate reprocessing by saving or switching documents.

## To Do

 * Save separate scrollbar positions for each document (in an array indexed according to the notebook tab)

 * Synchronize preview and document scrollbar positions

 * Add configuration options to change: Fonts, templates, style sheets.

 * Force file type of new documents.

 * Select default processing for unknown file types.

 * Detect other formats based on file extension

 * Detect other formats based on content (may be too slow)

 * Remove hard-coded HTML and magic numbers/strings

## Requirements

 * geany
 * gtk
 * webkit
 * `pandoc`
 * `asciidoc`

## License

This plugin is licensed under the GNU General Public License, version 2. For the full text of the license, please visit http://www.gnu.org/licenses/gpl-2.0.html.

## Authors

xiota «xiota•mentalfossa•com»
