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
  : _next (NULL),
    _c1 (0)
{
	_istream = fopen (file.c_str (), "r");
	g_assert (_istream);
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
	tokens::Token 	*t;
	gchar 		 c2;

	g_assert (_istream);

	if (_next) {
		t = _next;
		_next = NULL;
		return t;
	}

	while (TRUE) {

		// look ahead
		if (_c1 == 0) {
			_c1 = getc (_istream);
		}

		// look further
		c2 = getc (_istream);

		if (t = scanEof (c2)) {
			return t;
		}
		else if (t = scanParagraph (c2)) {
			return t;
		}
		else if (t = scanStyle (c2)) {
			return t;
		}
		else {
			return scanText (c2);
		}
	}
}

tokens::Token * 
Scanner::scanEof (gchar c2)
{
	if (c2 == EOF) {
		_next = new tokens::Eof ();
		return new tokens::Text (g_strdup_printf ("%c", _c1));
	} else if (_c1 == EOF) {
		return new tokens::Eof ();
	}
	return NULL;
}

tokens::Token * 
Scanner::scanParagraph (gchar c2)
{
	if (_c1 == '\n' && c2 == '\n') {
		do {
			_c1 = getc (_istream);
		} while (_c1 == '\n');
		return new tokens::Paragraph ();
	}
	return NULL;
}

tokens::Token * 
Scanner::scanStyle (gchar c2)
{
	if (_c1 == ' ' && c2 == '*') {
		_c1 = 0;
		return new tokens::Style (tokens::Style::ASTERISK, 
					  tokens::Style::LEFT);
	} else if (_c1 == '*' && c2 == ' ') {
		_c1 = 0;
		return new tokens::Style (tokens::Style::ASTERISK, 
					  tokens::Style::RIGHT);
	} else if (_c1 == ' ' && c2 == '/') {
		_c1 = 0;
		return new tokens::Style (tokens::Style::SLASH, 
					  tokens::Style::LEFT);
	} else if (_c1 == '/' && c2 == ' ') {
		_c1 = 0;
		return new tokens::Style (tokens::Style::SLASH, 
					  tokens::Style::RIGHT);
	} else if (_c1 == ' ' && c2 == '`') {
		_c1 = 0;
		return new tokens::Style (tokens::Style::BACKTICK, 
					  tokens::Style::LEFT);
	} else if (_c1 == '`' && c2 == ' ') {
		_c1 = 0;
		return new tokens::Style (tokens::Style::BACKTICK, 
					  tokens::Style::RIGHT);
	} else if (_c1 == ' ' && c2 == '_') {
		_c1 = 0;
		return new tokens::Style (tokens::Style::UNDERSCORE, 
					  tokens::Style::LEFT);
	} else if (_c1 == '_' && c2 == ' ') {
		_c1 = 0;
		return new tokens::Style (tokens::Style::UNDERSCORE, 
					  tokens::Style::RIGHT);
	}
	return NULL;
}

tokens::Token * 
Scanner::scanText (gchar c2)
{
	tokens::Text 	*t;
	std::string 	 s;

	while (TRUE) {

		if (_next = scanEof (c2)) {
			return t;
		}
		else if (_next = scanParagraph (c2)) {
			return t;
		}
		else if (_next = scanStyle (c2)) {
			return t;
		}
		else {
			t->append (_c1);
			_c1 = c2;
			c2 = getc (_istream);
		}
	} 
}
