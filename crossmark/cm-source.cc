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

#include "crossmark.hh"
#include "cm-scanner.hh"

#if 0
//#ifdef DEBUG
#define TRACE(M) fprintf(stderr, M);fprintf(stderr, "\n");
#else
#define TRACE(M)
#endif

using namespace crossmark;

Source::Source (const std::string &file, 
		Reader &reader)
  : _reader (reader),
    _scanner (* new Scanner (file))
{

}

Source::Source (streams::Input &istream, 
		Reader &reader)
  : _reader (reader),
    _scanner (* new Scanner (istream))
{

}

Source::~Source ()
{
	delete &_scanner;
}

/*
 * Scanner tokens:
 * start end pbreak newline indent text 
 * style-lb style-rb style-li style-ri style-lm style-rm style-lu style-ru

# Some quickie top-down grammar, flaws:
# + way incomplete
# + arbitrary nesting of styling allowed
# + ...

document   := start newline* ( pbreak | paragraph | blockquote )* end
paragraph  := line* pbreak
blockquote := indent line ( newline indent line )* pbreak
line       := ( text | markup )*
markup     := bold | italic | monospace | underline
bold       := style-lb ( text | markup )* style-rb
italic     := style-li ( text | markup )* style-ri
monospace  := style-lm ( text | markup )* style-rm
underline  := style-lu ( text | markup )* style-ru
*/
gboolean 
Source::sputter ()
{
	parseDocument ();
}

void 
Source::parseDocument ()
{
	TRACE (__FUNCTION__);

	// document   := start newline* ( paragraph | blockquote )* end

	_reader.pushDocument ();

	tokens::Token *token = _scanner.fetchToken ();
	g_assert (token->getClass () == tokens::Token::START);
	delete token;

	token = _scanner.fetchToken ();
	while (token->getClass () == tokens::Token::NEWLINE) {
		delete token;
		token = _scanner.fetchToken ();
	}
	while (token->getClass () == tokens::Token::PARAGRAPH) {
		delete token;
		// ignore empty paragraph at beginning?
		// _reader.pushStructure (document::Structure::PARAGRAPH);
		// _reader.popStructure ();
		token = _scanner.fetchToken ();
	}

	tokens::Token *next;
	while (token->getClass () != tokens::Token::END) {

		next = NULL;

		if (token->getClass () == tokens::Token::INDENT) {
			next = parseBlockquote (token);
		} else {
			next = parseParagraph (token);
		}

		if (token != next) {
			delete token;
		}
		token = next;
	}
	delete token;

	_reader.popDocument ();
}

tokens::Token * 
Source::parseParagraph (const tokens::Token *first)
{
	TRACE (__FUNCTION__);

	if (first->getClass () == tokens::Token::END) {
		// ok, because the token is owned by the caller
		return const_cast<tokens::Token *> (first);
	}

	// paragraph  := line* pbreak
	_reader.pushStructure (document::Structure::PARAGRAPH);

	tokens::Token *token = parseLine (first);

	tokens::Token *next;
	while (token->getClass () != tokens::Token::PARAGRAPH && 
	       token->getClass () != tokens::Token::END) {

		next = parseLine (token);

		if (token != next) {
			delete token;
		}
		token = next;
	}
	
	_reader.popStructure ();

	if (token->getClass () == tokens::Token::PARAGRAPH) {
		delete token;
		token = _scanner.fetchToken ();
	}
	return token;
}

tokens::Token * 
Source::parseBlockquote (const tokens::Token *first)
{
	TRACE (__FUNCTION__);

	if (first->getClass () == tokens::Token::PARAGRAPH ||
	    first->getClass () == tokens::Token::END) {
		// ok, because the token is owned by the caller
		return const_cast<tokens::Token *> (first);
	}

	// blockquote := indent line ( newline indent line )* pbreak
	_reader.pushStructure (document::Structure::BLOCKQUOTE);

	g_assert (first->getClass () == tokens::Token::INDENT);

	tokens::Token *token = parseLine (_scanner.fetchToken ());
	if (token->getClass () == tokens::Token::END) {
		return token;
	}	
	
	while (token->getClass () != tokens::Token::PARAGRAPH && 
	       token->getClass () != tokens::Token::END) {

		g_assert (token->getClass () == tokens::Token::NEWLINE);
		delete token;

		token = _scanner.fetchToken ();
		g_assert (token->getClass () == tokens::Token::INDENT);
		delete token;		

		token = parseLine (_scanner.fetchToken ());		
	}

	_reader.popStructure ();

	if (token->getClass () == tokens::Token::PARAGRAPH) {
		delete token;
		token = _scanner.fetchToken ();
	}
	return token;
}

/*!
 * \todo What to do with tabs in running text?
 */
tokens::Token * 
Source::parseLine (const tokens::Token *first)
{
	TRACE (__FUNCTION__);

	if (first->getClass () == tokens::Token::PARAGRAPH ||
	    first->getClass () == tokens::Token::END) {
		// ok, because the token is owned by the caller
		return const_cast<tokens::Token *> (first);
	}

	// line       := ( text | markup )*

	tokens::Token *token;
	if (first->getClass () == tokens::Token::STYLE) {
		token = parseMarkup (first);
	} else {
		token = parseText (first);
	}

	tokens::Token *next;
	while (token->getClass () != tokens::Token::NEWLINE &&
	       token->getClass () != tokens::Token::PARAGRAPH &&  
	       token->getClass () != tokens::Token::END) {

		if (token->getClass () == tokens::Token::STYLE) {
			next = parseMarkup (token);
		} else if (token->getClass () == tokens::Token::INDENT) {
			_reader.text ("\t");
			next = _scanner.fetchToken ();
		} else {
			next = parseText (token);
		}

		if (token != next) {
			delete token;
		}
		token = next;
	}

	if (token->getClass () == tokens::Token::NEWLINE) {
		delete token;
		token = _scanner.fetchToken ();
	}
	return token;
}

tokens::Token * 
Source::parseMarkup (const tokens::Token *first)
{
	TRACE (__FUNCTION__);

	if (first->getClass () == tokens::Token::PARAGRAPH ||
	    first->getClass () == tokens::Token::END) {
		// ok, because the token is owned by the caller
		return const_cast<tokens::Token *> (first);
	}

	// markup     := bold | italic | monospace | underline

	const tokens::Style *style;
	tokens::Token 	    *next;

	style = dynamic_cast<const tokens::Style *> (first);
	g_assert (style && style->getPos () == tokens::Style::LEFT);
	next = parseStyle (style, style->getType ());
	return next;
}

/*!
 * This is a generalisation of parseBold(), parseItalic(), parseMonospace(), parseUnderline().
 * \todo Abort at newline?
 */
tokens::Token * 
Source::parseStyle (const tokens::Token *first, tokens::Style::Type type)
{
	TRACE (__FUNCTION__);

	if (first->getClass () == tokens::Token::PARAGRAPH ||
	    first->getClass () == tokens::Token::END) {
		// ok, because the token is owned by the caller
		return const_cast<tokens::Token *> (first);
	}

	// bold       := style-lb ( text | markup )* style-rb
	// italic     := style-li ( text | markup )* style-ri
	// monospace  := style-lm ( text | markup )* style-rm
	// underline  := style-lu ( text | markup )* style-ru

	g_assert (type == tokens::Style::ASTERISK || 
		  type == tokens::Style::SLASH || 
		  type == tokens::Style::BACKTICK || 
		  type == tokens::Style::UNDERSCORE);

	const tokens::Style *style;
	style = dynamic_cast<const tokens::Style *> (first);
	g_assert (style && style->getType () == type);

	_reader.pushStyle ((document::Style::Type) type);

	tokens::Token *token;
	tokens::Token *next;

	token = _scanner.fetchToken ();
	while (!((style = dynamic_cast<const tokens::Style *> (token)) != NULL &&
	         style->getType () == type && 
		 style->getPos () == tokens::Style::RIGHT) && 
	       token->getClass () != tokens::Token::PARAGRAPH &&
	       token->getClass () != tokens::Token::END) {

		if (token->getClass () == tokens::Token::STYLE) {
			next = parseMarkup (token);
		} else {
			next = parseText (token);
		}

		if (token != next) {
			delete token;
		}
		token = next;
	}	

	_reader.popStyle ();

	if (token->getClass () == tokens::Token::STYLE) {
		delete token;
		token = _scanner.fetchToken ();
	}
	return token;
}

tokens::Token * 
Source::parseText (const tokens::Token *first)
{
	TRACE (__FUNCTION__);

	if (first->getClass () == tokens::Token::PARAGRAPH ||
	    first->getClass () == tokens::Token::END) {
		// ok, because the token is owned by the caller
		return const_cast<tokens::Token *> (first);
	}

	g_assert (first->getClass () == tokens::Token::TEXT);
	const gchar *text = first->toString ();
	g_assert (text);
	g_return_val_if_fail (text, _scanner.fetchToken ());
	_reader.text (text);

	return _scanner.fetchToken ();
}