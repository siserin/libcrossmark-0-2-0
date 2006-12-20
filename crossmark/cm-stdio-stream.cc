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



StdInput::StdInput (const std::string &file)
  : _ownStream (TRUE)
{
	_istream = fopen (file.c_str (), "r");
	g_assert (_istream);
}

StdInput::StdInput (FILE *istream)
  : _istream (istream),
    _ownStream (FALSE)
{
	g_assert (_istream);
}

StdInput::~StdInput ()
{
	if (_ownStream && _istream) {
		fclose (_istream);
	}
}

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



StdOutput::StdOutput (const std::string &file)
  : _ownStream (TRUE)
{
	_ostream = fopen (file.c_str (), "r");
	g_assert (_ostream);
}

StdOutput::StdOutput (FILE *ostream)
  : _ostream (ostream),
    _ownStream (FALSE)
{
	g_assert (_ostream);
}

StdOutput::~StdOutput ()
{
	if (_ownStream && _ostream) {
		fclose (_ostream);
		_ostream = NULL;
	}
}

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

gboolean 
StdOutput::write (const gchar *s)
{
	g_assert (_ostream);
	return EOF == fputs (s, _ostream);
}
