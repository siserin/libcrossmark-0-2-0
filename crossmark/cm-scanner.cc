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
#include "cm-scanner-private.hh"
#include "cm-stream-private.hh"
// debug
#include <iostream>

using namespace crossmark;

Scanner::Scanner (const std::string &file)
  : _istream (* stream::Factory::instance().createInput (file)),
    _ownStream (FALSE),
    _next (NULL),
    _c1 (0)
{
	_next = tokens::Factory::instance().createToken (tokens::Token::START);
}

Scanner::Scanner (stream::Input &istream)
  : _istream (istream),
    _ownStream (TRUE),
    _next (NULL),
    _c1 (0)
{
	_next = tokens::Factory::instance().createToken (tokens::Token::START);
}

Scanner::~Scanner ()
{
	if (_ownStream) {
		delete &_istream;
	}
}

/*!
 * Fetch the next token from the input file.
 * \todo Is g_strdup_printf() UTF-8 compliant?
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

		tail = 0;
		if ((_next = scanEnd (c2))) {
			if (text) {
				text->append (_c1);
			} else {
				text = new tokens::Text (g_strdup_printf ("%c", c2));
			}
			_c1 = c2;
			return text;
		} else if ((_next = scanStyle (c2, tail)) != NULL) {
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
			if (tail) {
				// both, _c1 and c2 have been consumed
				if (text) {
					text->append (tail);
				} else {
					text = new tokens::Text (g_strdup_printf ("%c", tail));
				}

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
}

tokens::Token * 
Scanner::scanEnd ()
{
	if (_c1 == (unsigned) EOF) {
		return new tokens::End ();
	}
	return NULL;
}

tokens::Token * 
Scanner::scanEnd (gunichar c)
{
	if (c == (unsigned) EOF) {
		return new tokens::End ();
	}
	return NULL;
}

/*!
 * \todo Not eat the newline if an indentation follows, 
 * 	 need to recognise that for blockquote, lists.
 * \todo Should a newline be replaced by a whitespace under certain circumstances?
 *	 Otherwise "foo\nbar" ends up as "foobar".
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

/*!
 * \todo Support all specified word boundaries (whitespace, punctuation, newline).
 */
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
	// handle escaped tokens as text
	else if (_c1 == '\\' && c2 == '*') {
		tail = c2;
		_c1 = 0;
		return NULL;
	} else if (_c1 == '\\' && c2 == '/') {
		tail = c2;
		_c1 = 0;
		return NULL;
	} else if (_c1 == '\\' && c2 == '`') {
		tail = c2;
		_c1 = 0;
		return NULL;
	} else if (_c1 == '\\' && c2 == '_') {
		tail = c2;
		_c1 = 0;
		return NULL;
	}
	// "centered" style tokens may cancel current style
	// scanning only for incorrect lead-out here because if
	// the lead in was valid it would have been consumed 
	// as style token already.
	else if (_c1 == '*' && c2 != ' ') {
		tail = _c1;
		_c1 = c2;
		return new tokens::Style (tokens::Style::ASTERISK, 
					  tokens::Style::CENTER);	
	} else if (_c1 == '/' && c2 != ' ') {
		tail = _c1;
		_c1 = c2;
		return new tokens::Style (tokens::Style::SLASH, 
					  tokens::Style::CENTER);	
	} else if (_c1 == '`' && c2 != ' ') {
		tail = _c1;
		_c1 = c2;
		return new tokens::Style (tokens::Style::BACKTICK, 
					  tokens::Style::CENTER);
	} else if (_c1 == '_' && c2 != ' ') {
		tail = _c1;
		_c1 = c2;
		return new tokens::Style (tokens::Style::UNDERSCORE, 
					  tokens::Style::CENTER);
	}
	return NULL;
}

tokens::Factory &
tokens::Factory::instance ()
{
	static tokens::Factory *factory = NULL;

	if (!factory) {
		factory = new tokens::Factory ();
	}

	return *factory;

}

tokens::Factory::Factory ()
{
	_factories.push_back (this);
}

void 
tokens::Factory::hook (const FactoryIface *factory)
{
	_factories.push_back (factory);
}

/*!
 * \todo Implement.
 */
void 
tokens::Factory::unhook (const FactoryIface *factory)
{
	std::list<const tokens::FactoryIface *>::reverse_iterator iter;

	iter = _factories.rbegin ();
	while (iter != _factories.rend ()) {
		if (*iter == factory) {
			_factories.erase (iter.base ());
			break;
		}
		iter++;
	}
}

/*!
 * \todo Implement.
 */
tokens::Token *
tokens::Factory::createToken (tokens::Token::Class klass) const
{
	std::list<const tokens::FactoryIface *>::const_reverse_iterator iter;
	tokens::Token *token;

	iter = _factories.rbegin ();
	while (iter != _factories.rend ()) {
		if ((token = (*iter)->createTokenImpl (klass)) != NULL) {
			return token;
		}
		iter++;
	}

	// the builtin factory which is tried last
	// must be able to handle all types
	g_assert_not_reached ();
}

tokens::Style * 
tokens::Factory::createStyleToken (tokens::Style::Type type, tokens::Style::Pos pos) const
{
	std::list<const tokens::FactoryIface *>::const_reverse_iterator iter;
	tokens::Style *token;

	iter = _factories.rbegin ();
	while (iter != _factories.rend ()) {
		if ((token = (*iter)->createStyleTokenImpl (type, pos)) != NULL) {
			return token;
		}
		iter++;
	}

	// the builtin factory which is tried last
	// must be able to handle all types
	g_assert_not_reached ();
}

tokens::Text * 
tokens::Factory::createTextToken (const gchar *text) const
{
	std::list<const tokens::FactoryIface *>::const_reverse_iterator iter;
	tokens::Text *token;

	iter = _factories.rbegin ();
	while (iter != _factories.rend ()) {
		if ((token = (*iter)->createTextTokenImpl (text)) != NULL) {
			return token;
		}
		iter++;
	}

	// the builtin factory which is tried last
	// must be able to handle all types
	g_assert_not_reached ();
}

tokens::Token *
tokens::Factory::createTokenImpl (tokens::Token::Class klass) const
{
	switch (klass) {
	case tokens::Token::START: 
		return new tokens::Start ();
		break;
	case tokens::Token::END: 
		return new tokens::End ();
		break;
	case tokens::Token::INDENT: 
		return new tokens::Indent ();
		break;
	case tokens::Token::NEWLINE: 
		return new tokens::Newline ();
		break;
	case tokens::Token::PARAGRAPH: 
		return new tokens::Paragraph ();
		break;
	default:
		g_assert_not_reached ();
		return NULL;
	}
}

tokens::Style * 
tokens::Factory::createStyleTokenImpl (tokens::Style::Type type, tokens::Style::Pos pos) const
{
	return new tokens::Style (type, pos);
}

tokens::Text * 
tokens::Factory::createTextTokenImpl (const gchar *text) const
{
	return new tokens::Text (text);
}
