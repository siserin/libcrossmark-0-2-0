/* Crossmark processing library 
 * Copyright (C) 2006, Robert Staudinger <robert.staudinger@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include <gsf/gsf-input-stdio.h>
#include "config.h"
#include "cm-gsf-stream-private.hh"

using namespace crossmark;

// Stolen from glib's gutf8.c
#define UTF8_LENGTH(Char)              \
  ((Char) < 0x80 ? 1 :                 \
   ((Char) < 0x800 ? 2 :               \
    ((Char) < 0x10000 ? 3 :            \
     ((Char) < 0x200000 ? 4 :          \
      ((Char) < 0x4000000 ? 5 : 6)))))

/*!
 * \todo Use exceptions.
 */
streams::GsfInput::GsfInput (const std::string &file)
  : _line (NULL), 
    _iter (NULL)
{
	::GsfInput *input;
	GError	 *error;

	error = NULL;
	input = gsf_input_stdio_new (file.c_str (), &error);
	if (error) {
		g_warning (error->message);
		g_error_free (error);
		return;
	}

printf ("eof: %d\n", gsf_input_eof (input));
	_input = (GsfInputTextline *) gsf_input_textline_new (input);
printf ("eof: %d\n", gsf_input_eof ((::GsfInput *) _input));
	gboolean ret = gsf_input_seek ((::GsfInput *) _input, 0, G_SEEK_SET);
printf ("eof: %d (%d)\n", gsf_input_eof ((::GsfInput *) _input), ret);

	g_assert (_input);
	g_object_unref (G_OBJECT (input));
}

streams::GsfInput::~GsfInput ()
{
	if (_input) {
		g_object_unref (G_OBJECT (_input));
	}

	if (_line) {
		g_free (_line);
	}
}

/*!
 * \todo Use g_utf8_get_char_validated() ?
 */
gunichar
streams::GsfInput::getChar ()
{
	gunichar c;

	g_assert (_input);

	if (_line &&
	    _iter == _line + _len) {
		g_free (_line);
		_line = NULL;
	}

	if (!_line ||
	    _iter == _line + _len) {
		if (gsf_input_eof ((::GsfInput *) _input)) {
			return (unsigned) EOF;
		}
		_line = gsf_input_textline_utf8_gets (_input);
		_iter = 0;
		_len = strlen ((const gchar *) _line);
	}

	c = g_utf8_get_char ((const gchar *) _iter);
	_iter += UTF8_LENGTH (c);
	return c;
}


/*!
 * \todo Implement.
 */
streams::GsfOutput::GsfOutput (const std::string &file)
{
	g_assert (FALSE);
}

streams::GsfOutput::~GsfOutput ()
{
}

void 
streams::GsfOutput::write (gunichar c)
{

}

void 
streams::GsfOutput::write (const gchar *s)
{

}
