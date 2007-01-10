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
#include "scanner-private.hh"
#include "stream/stream-private.hh"

using namespace crossmark;

/*!
 * Create scanner for file.
 */
Scanner::Scanner (gchar const *file)
  : _istream (* stream::Factory::instance().createInput (file)),
    _ownStream (FALSE),
    _next (NULL),
    _c1 (0)
{
	_next = token::Factory::instance().createToken (token::Token::START);
}

/*!
 * Create scanner for stream.
 */
Scanner::Scanner (stream::Input &istream)
  : _istream (istream),
    _ownStream (TRUE),
    _next (NULL),
    _c1 (0)
{
	_next = token::Factory::instance().createToken (token::Token::START);
}

/*!
 * Dtor.
 */
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
token::Token *
Scanner::fetchToken ()
{
	token::Token *token;

	if (_next) {
		token = _next;
		_next = NULL;
		return _return (token);
	}

	token::Text 	*text = NULL;
	gunichar 	 c2;
	gunichar 	 tail;
	while (TRUE) {

		// look ahead
		if (_c1 == 0) {
			_c1 = _istream.read ();
		}

		// only possible if not currently 
		// scanning text
		if (!text && 
		    (token = scanHeading ())) {
			return _return (token);
		}

		if ((_next = scanEnd ()) ||
		    (_next = scanNewline ()) ||
		    (_next = scanIndent ())) {
			if (text) {
				return text;
			} else {
				token = _next;
				_next = NULL;
				return _return (token);
			}
			continue;
		}

		// look further
		c2 = _istream.read ();

		tail = 0;
		if ((_next = scanEnd (c2))) {
			if (text) {
				text->append (_c1);
			} else {
				text = new token::Text (g_strdup_printf ("%c", c2));
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
				text = new token::Text (g_strdup_printf ("%c", tail));
				token = text;
			} else {
				token = _next;
				_next = NULL;
			}
			return _return (token);
		} else {
			if (tail) {
				// both, _c1 and c2 have been consumed
				if (text) {
					text->append (tail);
				} else {
					text = new token::Text (g_strdup_printf ("%c", tail));
				}

			} else {

				if (text) {
					text->append (_c1);
				} else {
					text = new token::Text (g_strdup_printf ("%c", _c1));
				}
				_c1 = c2;
			}
		}
	}
}

/*!
 * Test for EOF.
 */
token::Token * 
Scanner::scanEnd ()
{
	if (_c1 == (unsigned) EOF) {
		return new token::End ();
	}
	return NULL;
}

/*!
 * Test for EOF.
 */
token::Token * 
Scanner::scanEnd (gunichar c)
{
	if (c == (unsigned) EOF) {
		return new token::End ();
	}
	return NULL;
}

/*!
 * Test for heading.
 *
 * Must be called before scanNewline() because headings may result from
 * a special combination of characters over several lines.
 *
 * \todo Better error handling, especially for H1, H2.
 * \todo Fall back if H2, H3 not terminated correctly?
 */
token::Token * 
Scanner::scanHeading ()
{
	if (_prev == token::Token::NEWLINE) {
		// scan for H1 or H2
		if (_c1 == '=') {
			// H1
			do {
				_c1 = _istream.read ();
			} while (_c1 == '=');
			g_assert (_c1 == '\n');
			return new token::Heading (token::Heading::HEADING_1);
		} else if (_c1 == '-') {
			// H2
			do {
				_c1 = _istream.read ();
			} while (_c1 == '-');
			g_assert (_c1 == '\n');
			return new token::Heading (token::Heading::HEADING_2);
		}
	} else if ((_prev == token::Token::START ||
		    _prev == token::Token::PARAGRAPH) &&
		   _c1 == '=' ) {
		// scan for H3 or H4
		gint i = 0;
		do {
			++i;
			_c1 = _istream.read ();
		} while (_c1 == '=');
		// eat leading ' ' if there
		if (_c1 == ' ') {
			_c1 = _istream.read ();
		}
		if (i == 3) {
			return new token::Heading (token::Heading::HEADING_3);
		} else if (i == 4) {
			return new token::Heading (token::Heading::HEADING_4);
		}
		g_assert (FALSE);
	}
	return NULL;
}

/*!
 * Test for newline.
 *
 * \todo Not eat the newline if an indentation follows, 
 * 	 need to recognise that for blockquote, lists.
 * \todo Should a newline be replaced by a whitespace under certain 
 * 	 circumstances? Otherwise "foo\nbar" ends up as "foobar".
 */
token::Token * 
Scanner::scanNewline ()
{
	gboolean isParagraph;

	isParagraph = FALSE;
	if (_c1 == '\n') {
		do {
			_c1 = _istream.read ();
			if (_c1 == '\n') { 
				isParagraph = TRUE;
			}
		} while (_c1 == '\n');
		if (isParagraph) {
			return new token::Paragraph ();
		} else {
			return new token::Newline ();
		}
	}
	return NULL;
}

/*!
 * Test for indentation.
 *
 * \todo For now only tab indentation is supported.
 */
token::Token * 
Scanner::scanIndent ()
{
	if (_c1 == '\t') {
		_c1 = 0;
		return new token::Indent ();
	}
	return NULL;
}

/*!
 * Test for style markup.
 *
 * \todo Support all specified word boundaries (whitespace, punctuation, newline).
 */
token::Token * 
Scanner::scanStyle (gunichar c2, gunichar &tail)
{
	tail = 0;

	if (_c1 == ' ' && c2 == '*') {
		tail = _c1;
		_c1 = 0;
		return new token::Style (token::Style::ASTERISK, 
					  token::Style::LEFT);
	} else if (_c1 == '*' && c2 == ' ') {
		_c1 = c2;
		return new token::Style (token::Style::ASTERISK, 
					  token::Style::RIGHT);
	} else if (_c1 == ' ' && c2 == '/') {
		tail = _c1;
		_c1 = 0;
		return new token::Style (token::Style::SLASH, 
					  token::Style::LEFT);
	} else if (_c1 == '/' && c2 == ' ') {
		_c1 = c2;
		return new token::Style (token::Style::SLASH, 
					  token::Style::RIGHT);
	} else if (_c1 == ' ' && c2 == '`') {
		tail = _c1;
		_c1 = 0;
		return new token::Style (token::Style::BACKTICK, 
					  token::Style::LEFT);
	} else if (_c1 == '`' && c2 == ' ') {
		_c1 = c2;
		return new token::Style (token::Style::BACKTICK, 
					  token::Style::RIGHT);
	} else if (_c1 == ' ' && c2 == '_') {
		tail = _c1;
		_c1 = 0;
		return new token::Style (token::Style::UNDERSCORE, 
					  token::Style::LEFT);
	} else if (_c1 == '_' && c2 == ' ') {
		_c1 = c2;
		return new token::Style (token::Style::UNDERSCORE, 
					  token::Style::RIGHT);
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
		return new token::Style (token::Style::ASTERISK, 
					  token::Style::CENTER);	
	} else if (_c1 == '/' && c2 != ' ') {
		tail = _c1;
		_c1 = c2;
		return new token::Style (token::Style::SLASH, 
					  token::Style::CENTER);	
	} else if (_c1 == '`' && c2 != ' ') {
		tail = _c1;
		_c1 = c2;
		return new token::Style (token::Style::BACKTICK, 
					  token::Style::CENTER);
	} else if (_c1 == '_' && c2 != ' ') {
		tail = _c1;
		_c1 = c2;
		return new token::Style (token::Style::UNDERSCORE, 
					  token::Style::CENTER);
	}
	return NULL;
}

/*!
 * Token factory singleton getter.
 *
 * \note The token factory is not yet used ubuquituously.
 */
token::Factory &
token::Factory::instance ()
{
	static token::Factory *factory = NULL;

	if (!factory) {
		factory = new token::Factory ();
	}

	return *factory;

}

/*!
 * Token factory ctor.
 */
token::Factory::Factory ()
{
	_factories.push_back (this);
}

/*!
 * Hook in a token factory.
 */
void 
token::Factory::hook (const FactoryIface *factory)
{
	_factories.push_back (factory);
}

/*!
 * Unhook in a token factory.
 */
void 
token::Factory::unhook (const FactoryIface *factory)
{
	std::list<const token::FactoryIface *>::reverse_iterator iter;

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
 * Request a token from the factory.
 */
token::Token *
token::Factory::createToken (token::Token::Class klass) const
{
	std::list<const token::FactoryIface *>::const_reverse_iterator iter;
	token::Token *token;

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

/*!
 * Request a style token from the factory.
 */
token::Style * 
token::Factory::createStyleToken (token::Style::Type type, token::Style::Pos pos) const
{
	std::list<const token::FactoryIface *>::const_reverse_iterator iter;
	token::Style *token;

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

/*!
 * Request a text from the factory.
 */
token::Text * 
token::Factory::createTextToken (gchar const *text) const
{
	std::list<const token::FactoryIface *>::const_reverse_iterator iter;
	token::Text *token;

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

/*!
 * Default token creation impl.
 */
token::Token *
token::Factory::createTokenImpl (token::Token::Class klass) const
{
	switch (klass) {
	case token::Token::START: 
		return new token::Start ();
		break;
	case token::Token::END: 
		return new token::End ();
		break;
	case token::Token::INDENT: 
		return new token::Indent ();
		break;
	case token::Token::NEWLINE: 
		return new token::Newline ();
		break;
	case token::Token::PARAGRAPH: 
		return new token::Paragraph ();
		break;
	default:
		g_assert_not_reached ();
		return NULL;
	}
}

/*!
 * Default style token creation impl.
 */
token::Style * 
token::Factory::createStyleTokenImpl (token::Style::Type type, token::Style::Pos pos) const
{
	return new token::Style (type, pos);
}

/*!
 * Default text token creation impl.
 */
token::Text * 
token::Factory::createTextTokenImpl (gchar const *text) const
{
	return new token::Text (text);
}
