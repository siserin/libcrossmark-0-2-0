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
#include <string>
#include "cm-scanner.hh"
#include "cm-stdio-stream.hh"
// debug
#include <iostream>

using namespace crossmark;

Scanner::Scanner (const std::string &file)
  : _istream (* new streams::StdInput (file)),
    _ownStream (FALSE),
    _next (NULL),
    _c1 (0)
{
	_next = new tokens::Start ();
}

Scanner::Scanner (streams::Input &istream)
  : _istream (istream),
    _ownStream (TRUE),
    _next (NULL),
    _c1 (0)
{
	_next = new tokens::Start ();
}

Scanner::~Scanner ()
{
	if (_ownStream) {
		delete &_istream;
	}
}

/*!
 * Fetch the next token from the input file.
 */
tokens::Token *
Scanner::fetchToken ()
{
	tokens::Token *token;

	if (_next) {
		token = _next;
		_next = NULL;
		return token;
	}

	tokens::Text 	*text = NULL;
	gboolean	 restart;
	gunichar 	 c2;
	gunichar 	 tail;
	while (TRUE) {

		restart = FALSE;

		// look ahead
		if (_c1 == 0) {
			_c1 = _istream.getChar ();
		}

		if ((_next = scanEnd ()) ||
		    (_next = scanNewline (restart)) ||
		    (_next = scanIndent ())) {
			if (text) {
				return text;
			} else {
				token = _next;
				_next = NULL;
				return token;
			}
		}

		if (restart) {
			continue;
		}

		// look further
		c2 = _istream.getChar ();

		//std::cout << "-- '" << _c1 << "' '" << c2 << "'" << std::endl;

		if ((_next = scanEnd (c2))) {
			if (text) {
				text->append (_c1);
			} else {
				text = new tokens::Text (g_strdup_printf ("%c", c2));
			}
			_c1 = c2;
			return text;
		} else if (_next = scanStyle (c2, tail)) {
			if (text && tail) {
				text->append (tail);
				token = text;
			} else if (text) {
				token = text;
			} else if (tail) {
				text = new tokens::Text (g_strdup_printf ("%c", tail));
				token = text;
			} else {
				token = _next;
				_next = NULL;
			}
			return token;
		} else {
			if (text) {
				text->append (_c1);
			} else {
				text = new tokens::Text (g_strdup_printf ("%c", _c1));
			}
			_c1 = c2;
		}
	}
}

tokens::Token * 
Scanner::scanEnd ()
{
	if (_c1 == EOF) {
		return new tokens::End ();
	}
	return NULL;
}

tokens::Token * 
Scanner::scanEnd (gunichar c)
{
	if (c == EOF) {
		return new tokens::End ();
	}
	return NULL;
}

/*!
 * \todo Not eat the newline if an indentation follows, 
 * 	 need to recognise that for blockquote, lists.
 */
tokens::Token * 
Scanner::scanNewline (gboolean &restart)
{
	gboolean isParagraph;

	restart = FALSE;
	isParagraph = FALSE;
	if (_c1 == '\n') {
		do {
			_c1 = _istream.getChar ();
			if (_c1 == '\n') { 
				isParagraph = TRUE;
			}
		} while (_c1 == '\n');
		if (isParagraph) {
			return new tokens::Paragraph ();
		} else {
			return new tokens::Newline ();
			restart = TRUE;
		}
	}
	return NULL;
}

/*!
 * \todo For now only tab indentation is supported.
 */
tokens::Token * 
Scanner::scanIndent ()
{
	if (_c1 == '\t') {
		_c1 = 0;
		return new tokens::Indent ();
	}
	return NULL;
}

tokens::Token * 
Scanner::scanStyle (gunichar c2, gunichar &tail)
{
	tail = 0;

	if (_c1 == ' ' && c2 == '*') {
		tail = _c1;
		_c1 = 0;
		return new tokens::Style (tokens::Style::ASTERISK, 
					  tokens::Style::LEFT);
	} else if (_c1 == '*' && c2 == ' ') {
		_c1 = c2;
		return new tokens::Style (tokens::Style::ASTERISK, 
					  tokens::Style::RIGHT);
	} else if (_c1 == ' ' && c2 == '/') {
		tail = _c1;
		_c1 = 0;
		return new tokens::Style (tokens::Style::SLASH, 
					  tokens::Style::LEFT);
	} else if (_c1 == '/' && c2 == ' ') {
		_c1 = c2;
		return new tokens::Style (tokens::Style::SLASH, 
					  tokens::Style::RIGHT);
	} else if (_c1 == ' ' && c2 == '`') {
		tail = _c1;
		_c1 = 0;
		return new tokens::Style (tokens::Style::BACKTICK, 
					  tokens::Style::LEFT);
	} else if (_c1 == '`' && c2 == ' ') {
		_c1 = c2;
		return new tokens::Style (tokens::Style::BACKTICK, 
					  tokens::Style::RIGHT);
	} else if (_c1 == ' ' && c2 == '_') {
		tail = _c1;
		_c1 = 0;
		return new tokens::Style (tokens::Style::UNDERSCORE, 
					  tokens::Style::LEFT);
	} else if (_c1 == '_' && c2 == ' ') {
		_c1 = c2;
		return new tokens::Style (tokens::Style::UNDERSCORE, 
					  tokens::Style::RIGHT);
	}
	return NULL;
}
