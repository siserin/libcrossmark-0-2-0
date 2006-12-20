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
	g_assert (_istream);

	return getc (_istream);
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
	g_assert (_ostream);
	return EOF == fputc (c, _ostream);
}

gboolean 
StdOutput::write (const gchar *s)
{
	g_assert (_ostream);
	return EOF == fputs (s, _ostream);
}
