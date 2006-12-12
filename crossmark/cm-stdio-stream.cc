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

#include "cm-stdio-stream.hh"

using namespace crossmark;



streams::StdInput::StdInput (const std::string &file)
{
	_istream = fopen (file.c_str (), "r");
	g_assert (_istream);
}

streams::StdInput::~StdInput ()
{
	if (_istream) {
		fclose (_istream);
		_istream = NULL;
	}
}

gunichar
streams::StdInput::getChar ()
{
	g_assert (_istream);

	return getc (_istream);
}



streams::StdOutput::StdOutput (const std::string &file)
{
	_ostream = fopen (file.c_str (), "r");
	g_assert (_ostream);
}

streams::StdOutput::~StdOutput ()
{
	if (_ostream) {
		fclose (_ostream);
		_ostream = NULL;
	}
}

void 
streams::StdOutput::write (gunichar c)
{
	g_assert (_ostream);
	putc (c, _ostream);
}

void 
streams::StdOutput::write (const gchar *s)
{
	g_assert (_ostream);
	fputs (s, _ostream);
}
