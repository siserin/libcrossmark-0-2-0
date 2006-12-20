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

#include "cm-stdio-stream-private.hh"
#include "cm-stream-private.hh"

using namespace crossmark;
using namespace crossmark::stream;



/*!
 * Create input from a FILE*.
 */
Input * 
createStdInput (FILE *istream)
{
	return new StdInput (istream);
}

/*!
 * Create output from a FILE*.
 */
Output * 
createStdOutput (FILE *ostream)
{
	return new StdOutput (ostream);
}



/*!
 * Create input from file.
 *
 * \todo Use exceptions.
 */
StdInput::StdInput (const std::string &file)
  : _ownStream (TRUE)
{
	_istream = fopen (file.c_str (), "r");
	g_assert (_istream);
}

/*!
 * Create input from stream.
 *
 * \todo Use exceptions.
 */
StdInput::StdInput (FILE *istream)
  : _istream (istream),
    _ownStream (FALSE)
{
	g_assert (_istream);
}

/*!
 * Dtor.
 */
StdInput::~StdInput ()
{
	if (_ownStream && _istream) {
		fclose (_istream);
	}
}

/*!
 * \sa stream::Input::getChar()
 * \todo Use g_utf8_get_char_validated() ?
 */
gunichar
StdInput::getChar ()
{
	gchar        buf[] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
	int 	     mask;
	int          len;

	g_assert (_istream);

	buf[0] = getc (_istream);
	mask = 0;
	UTF8_COMPUTE (buf[0], mask, len);
	if (len > 1) {
		fread (buf + 1, sizeof (gchar), len - 1, _istream);
	}
	
	return g_utf8_get_char ((const gchar *) buf);
}



/*!
 * Create output to file.
 *
 * \todo Use exceptions.
 */
StdOutput::StdOutput (const std::string &file)
  : _ownStream (TRUE)
{
	_ostream = fopen (file.c_str (), "r");
	g_assert (_ostream);
}

/*!
 * Create output to stream.
 *
 * \todo Use exceptions.
 */
StdOutput::StdOutput (FILE *ostream)
  : _ostream (ostream),
    _ownStream (FALSE)
{
	g_assert (_ostream);
}

/*!
 * Dtor.
 */
StdOutput::~StdOutput ()
{
	if (_ownStream && _ostream) {
		fclose (_ostream);
		_ostream = NULL;
	}
}

/*!
 * \sa stream::Output::write(gunichar)
 */
gboolean 
StdOutput::write (gunichar c)
{
	gchar buf[] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
	gint  n_bytes;

	g_assert (_ostream);

	n_bytes = g_unichar_to_utf8 (c, buf);
	if (n_bytes) {
		return EOF == fputs (buf, _ostream);
	}
	return FALSE;
}

/*!
 * \sa stream::Output::write(const gchar *)
 */
gboolean 
StdOutput::write (const gchar *s)
{
	g_assert (_ostream);
	return EOF == fputs (s, _ostream);
}
