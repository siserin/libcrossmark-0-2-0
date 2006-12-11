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

#include <glib.h>
#include <glib/gstdio.h>
#include "cm-scanner.hh"

/*

# Yeah, this is flawed, but I'm trying to treat markup stuff 
# as a single character (by using lookahead).
{charset}    := {UTF-8} \ {" *", "* ", " /", "/ ", " `", "` ", " _", "_ "}

# Initial scanner grammar
# 0 means start of file
token      := paragraph | style | text | eof
paragraph  := '\n' '\n' '\n'*
style      := " *" | "* " | " /" | "/ " | " `" | "` " | " _" | "_ "
text       := {charset}*

*/

using namespace crossmark;

Scanner::Scanner (std::string &file)
  : _c1 (0)
{
	_istream = fopen (file.c_str (), "r");
}

Scanner::~Scanner ()
{
	if (_istream) {
		fclose (_istream);
		_istream = NULL;
	}
}

tokens::Token *
Scanner::fetchToken ()
{
	gchar c2;

	g_assert (_istream);

	if (!_c1) {
		_c1 = getc (_istream);
		if (_c1 == EOF) {
			return scanEof ();
		}
	}

	while (TRUE) {

		c2 = getc (_istream);
		if (c2 == EOF) {
			tokens::Token *t = scanText (_c1);
			_c1 = EOF;
			return t;
		}

		if (_c1 == ' ' && c2 == '*') {
			return scanStyle (tokens::Style::ASTERISK, tokens::Style::LEFT);
		} else if (_c1 == '*' && c2 == ' ') {
			return scanStyle (tokens::Style::ASTERISK, tokens::Style::RIGHT);
		} else if (_c1 == ' ' && c2 == '/') {
			return scanStyle (tokens::Style::SLASH, tokens::Style::LEFT);
		} else if (_c1 == '/' && c2 == ' ') {
			return scanStyle (tokens::Style::SLASH, tokens::Style::RIGHT);
		} else if (_c1 == ' ' && c2 == '`') {
			return scanStyle (tokens::Style::BACKTICK, tokens::Style::LEFT);
		} else if (_c1 == '`' && c2 == ' ') {
			return scanStyle (tokens::Style::BACKTICK, tokens::Style::RIGHT);
		} else if (_c1 == ' ' && c2 == '_') {
			return scanStyle (tokens::Style::UNDERSCORE, tokens::Style::LEFT);
		} else if (_c1 == '_' && c2 == ' ') {
			return scanStyle (tokens::Style::UNDERSCORE, tokens::Style::RIGHT);
		}


		if (c2 == EOF) {
			// TODO
		}


	}
}

tokens::Token * 
Scanner::scanEof ()
{
	return new tokens::Eof ();
}

tokens::Token * 
Scanner::scanParagraph ()
{

}

tokens::Token * 
Scanner::scanStyle (tokens::Style::Type type, 
		    tokens::Style::Pos  pos)
{
	_c1 = 0;
	return new tokens::Style (type, pos);
}

// TODO have only a c2 version since c1 is a member

tokens::Token * 
Scanner::scanText (gchar c1)
{

}

tokens::Token * 
Scanner::scanText (gchar c1, 
		   gchar c2)
{

}
